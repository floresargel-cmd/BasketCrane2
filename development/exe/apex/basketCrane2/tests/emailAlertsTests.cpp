#include "../emailAlerts.h"

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <QSettings>
#include <QFile>
#include <QMutexLocker>
#include <QAtomicInt>
#include <cstdio>
#include <functional>
#include <stdexcept>

static void require(bool pass, const char* message)
{
    if (!pass) throw std::runtime_error(message);
}

static bool waitUntil(const std::function<bool()>& condition, int timeout = 35000)
{
    QElapsedTimer timer;
    timer.start();
    while (!condition() && timer.elapsed() < timeout) QThread::msleep(20);
    return condition();
}

// Loopback only: no production PLC, database or external mail server is used.
class SmtpFixture : public QThread
{
public:
    SmtpFixture() : port_(0), reject_(0), attempts_(0), connections_(0), stall_(false) { start(); }
    ~SmtpFixture() { requestInterruption(); wait(); }
    int port() { QMutexLocker l(&lock_); return port_; }
    int count() { QMutexLocker l(&lock_); return messages_.size(); }
    int attempts() { QMutexLocker l(&lock_); return attempts_; }
    int connections() { QMutexLocker l(&lock_); return connections_; }
    void stall(bool value) { QMutexLocker l(&lock_); stall_ = value; }
    QByteArray message(int index) { QMutexLocker l(&lock_); return messages_.at(index); }
    void rejectNext() { QMutexLocker l(&lock_); ++reject_; }
protected:
    void run() override {
        QTcpServer server;
        if (!server.listen(QHostAddress::LocalHost, 0)) return;
        { QMutexLocker l(&lock_); port_ = server.serverPort(); }
        while (!isInterruptionRequested()) {
            if (!server.waitForNewConnection(100)) continue;
            QTcpSocket* socket = server.nextPendingConnection();
            bool stall;
            { QMutexLocker l(&lock_); ++connections_; stall = stall_; }
            if (!stall) reply(socket, "220 localhost test SMTP\r\n");
            bool data = false;
            QByteArray body;
            while (!isInterruptionRequested() && socket->state() == QAbstractSocket::ConnectedState) {
                if (!socket->canReadLine() && !socket->waitForReadyRead(100)) continue;
                if (!socket->canReadLine()) continue;
                QByteArray line = socket->readLine();
                if (data) {
                    if (line == ".\r\n") {
                        bool reject;
                        { QMutexLocker l(&lock_); ++attempts_; reject = reject_ > 0;
                          if (reject) --reject_; else messages_.append(body); }
                        reply(socket, reject ? "451 try later\r\n" : "250 accepted\r\n");
                        data = false;
                    } else body += line;
                } else if (line.startsWith("EHLO") || line.startsWith("HELO")) reply(socket, "250 localhost\r\n");
                else if (line.startsWith("DATA")) { data = true; reply(socket, "354 send data\r\n"); }
                else if (line.startsWith("QUIT")) { reply(socket, "221 bye\r\n"); break; }
                else reply(socket, "250 OK\r\n");
            }
            delete socket;
        }
    }
private:
    static void reply(QTcpSocket* s, const QByteArray& text) { s->write(text); s->waitForBytesWritten(1000); }
    QMutex lock_;
    int port_, reject_, attempts_, connections_;
    bool stall_;
    QList<QByteArray> messages_;
};


static QByteArray decodedBody(const QByteArray& message) {
    return QByteArray::fromBase64(message.mid(message.indexOf("\r\n\r\n") + 4));
}
int main(int argc,char** argv) {
    QCoreApplication application(argc,argv);
    QCoreApplication::setApplicationVersion("test-version");
    try {
        QTemporaryDir directory; require(directory.isValid(),"Temporary settings");
        QSettings ini(directory.path()+"/email.ini",QSettings::IniFormat);
        basket::EmailAlertSettings settings; settings.Load(ini);
        require(!settings.Enabled(),"Existing INIs remain compatible");
        SmtpFixture smtp; require(waitUntil([&]{return smtp.port()!=0;}),"Loopback server");
        ini.setValue("EmailAlerts/EnableErrorEmailNotification",true);
        ini.setValue("EmailAlerts/SmtpHost","127.0.0.1");
        ini.setValue("EmailAlerts/SmtpPort",smtp.port());
        ini.setValue("EmailAlerts/SmtpEnableSsl",false);
        ini.setValue("EmailAlerts/SmtpFrom","BasketCrane2Application@lorvalcapital.ca");
        ini.setValue("EmailAlerts/SmtpTo","operator@example.test;maintenance@example.test");
        ini.setValue("EmailAlerts/SmtpSubjectPrefix","[Basket Crane 2 Error] : ");
        settings.Load(ini);
        require(settings.ValidationError().isEmpty() && settings.CooldownSeconds()==3600,"Default hourly interval");
        ini.setValue("EmailAlerts/ErrorEmailIntervalMinutes",1); settings.Load(ini);
        {
            EmailAlerts alerts(settings);
            QElapsedTimer logging; logging.start();
            EmailAlerts::Notify("DATABASE",QString::fromUtf8("first-error <&> $() ` literal \xC3\xA9"));
            require(logging.elapsed()<200,"Notifications never block UI on SMTP");
            require(waitUntil([&]{return smtp.count()==1;}),"First message delivered");
            require(smtp.message(0).contains("Subject: [Basket Crane 2 Error] : Application Error"),"INI subject prefix");
            require(smtp.message(0).contains("BasketCrane2Application@lorvalcapital.ca"),"INI sender");
            require(smtp.message(0).contains("operator@example.test") && smtp.message(0).contains("maintenance@example.test"),"INI recipients");
            require(decodedBody(smtp.message(0)).contains("first-error <&> $() ` literal \xC3\xA9"),"Literal and Unicode content");
            std::puts("PASS: SMTP sender, subject, recipients, Unicode and nonblocking delivery");std::fflush(stdout);
            EmailAlerts::Notify("ERROR","batch-one");EmailAlerts::Notify("ERROR","batch-two");
            QThread::msleep(200);require(smtp.count()==1,"Cooldown holds repeat errors");
            require(waitUntil([&]{return smtp.count()==2;},75000),"Batched delivery");
            require(decodedBody(smtp.message(1)).contains("Error events in this batch: 2"),"Batch event count");
            std::puts("PASS: interval throttling and grouped errors");std::fflush(stdout);
            smtp.rejectNext();EmailAlerts::Notify("ERROR","retry-me");
            require(waitUntil([&]{return smtp.attempts()>=3;},75000),"SMTP rejection");
            require(waitUntil([&]{return smtp.count()==3;},75000),"Retry retains failed batch");
            require(decodedBody(smtp.message(2)).contains("retry-me"),"Retained original error");
            std::puts("PASS: rejection retains errors and retries without recursive alerts");std::fflush(stdout);
            EmailAlerts::Notify("ERROR",QString(40000,'x'));alerts.stop();
            require(smtp.count()==4 && smtp.message(3).size()<30000,"Bounded shutdown batch");
        }
        ini.setValue("EmailAlerts/SmtpTimeoutSeconds",1);settings.Load(ini);smtp.stall(true);
        {
            EmailAlerts alerts(settings); QElapsedTimer deadline;deadline.start();
            EmailAlerts::Notify("ERROR","timeout");QThread::msleep(300);alerts.stop();
            require(deadline.elapsed()<9000,"Stalled SMTP has bounded shutdown");
        }
        smtp.stall(false);
        ini.setValue("EmailAlerts/SmtpEnableSsl",true);settings.Load(ini);
        { EmailAlerts alerts(settings);EmailAlerts::Notify("ERROR","TLS required");QThread::msleep(300);alerts.stop(); }
        require(smtp.count()==4,"No downgrade when SMTP does not offer STARTTLS");
        qputenv("BASKET_CRANE_EMAIL_TEST_SECRET","test-secret");
        ini.setValue("EmailAlerts/SmtpUsername","test");ini.setValue("EmailAlerts/SmtpPassword","${ENV:BASKET_CRANE_EMAIL_TEST_SECRET}");settings.Load(ini);
        require(settings.Password()=="test-secret" && settings.ValidationError().isEmpty(),"Environment credentials with TLS");
        ini.setValue("EmailAlerts/SmtpEnableSsl",false);settings.Load(ini);
        require(!settings.ValidationError().isEmpty(),"Credentials without TLS rejected");
        const int connections=smtp.connections();
        { EmailAlerts alerts(settings);EmailAlerts::Notify("ERROR","invalid configuration");QThread::msleep(200); }
        require(smtp.connections()==connections,"Invalid settings never connect");
        ini.setValue("EmailAlerts/EnableErrorEmailNotification",false);settings.Load(ini);
        { EmailAlerts alerts(settings);EmailAlerts::Notify("ERROR","disabled");QThread::msleep(200); }
        require(smtp.connections()==connections,"Disabled settings never connect");
        ini.setValue("EmailAlerts/EnableErrorEmailNotification",true);ini.setValue("EmailAlerts/SmtpEnableSsl",true);
        ini.setValue("EmailAlerts/SmtpSubjectPrefix","bad\r\nBcc: injected@example.test");settings.Load(ini);
        require(!settings.ValidationError().isEmpty(),"Header injection rejected");
        ini.setValue("EmailAlerts/SmtpSubjectPrefix","normal");ini.setValue("EmailAlerts/SmtpEnableSsl","invalid");settings.Load(ini);
        require(!settings.ValidationError().isEmpty(),"Malformed TLS boolean rejected");
        std::puts("PASS: bounded shutdown, TLS, disabled/invalid settings, credential expansion and header validation");
        return 0;
    } catch(const std::exception& error) {std::fprintf(stderr,"FAIL: %s\n",error.what());return 1;}
}
