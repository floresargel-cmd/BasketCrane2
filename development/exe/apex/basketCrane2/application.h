#pragma once
#include "version.h"
#include "emailAlerts.h"
#include "mW.h"
#include <QCommandLineParser>
#include <QScopedPointer>
#include <cstdio>
#include <QSharedMemory>
#include "tests/environmentSafety.h"
#include "tests/uiPreview.h"
#include "tests/operatorErrorTests.h"

namespace basket {
inline void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (type==QtCriticalMsg || type==QtFatalMsg) EmailAlerts::Notify("Qt application error",msg);
    std::fprintf(stderr,"%s\n",msg.toLocal8Bit().constData());
    if (type==QtFatalMsg) EmailAlerts::StopActive();
}

class BasketCraneApplication final
{
public:
    int Run(int argc, char* argv[])
    {
        // Validation mode uses QtCore only and never constructs the HMI.
        bool checkOnly = false; bool checkSafety = false; bool checkDatabases=false; bool checkHistory=false;
        for (int i = 1; i < argc; ++i) {
            if (QString::fromLocal8Bit(argv[i]) == "--check-config") checkOnly = true;
            if (QString::fromLocal8Bit(argv[i]) == "--check-safety") checkSafety = true;
            if (QString::fromLocal8Bit(argv[i]) == "--check-databases") checkDatabases=true;
            if (QString::fromLocal8Bit(argv[i]) == "--check-history") checkHistory=true;
        }
        QScopedPointer<QCoreApplication> app((checkOnly || checkSafety || checkDatabases || checkHistory)
            ? new QCoreApplication(argc, argv)
            : new OperatorApplication(argc, argv));
        QCoreApplication::setApplicationName("BasketCrane2");
        QCoreApplication::setOrganizationName("Apex");
        QCoreApplication::setApplicationVersion(BASKET_VERSION_STRING);
        QCommandLineParser parser;
        parser.addHelpOption();
        parser.addVersionOption();
        QCommandLineOption configOption("config", "INI file path", "path",
            QCoreApplication::applicationDirPath() + "/basketCrane2.ini");
        parser.addOption(configOption);
        parser.addOption(QCommandLineOption("check-config", "Validate INI settings without connecting to plant systems"));
        parser.addOption(QCommandLineOption("check-safety", "Run offline environment safety regression tests"));
        parser.addOption(QCommandLineOption("check-databases", "Test direct SQL Server connections using SELECT only; no HMI or PLC connections"));
        parser.addOption(QCommandLineOption("check-history", "Read controller movement history with basket audit recovery; no HMI or PLC connections"));
        parser.addOption(QCommandLineOption("check-errors", "Test operator error dialogs offline without plant connections"));
        QCommandLineOption previewOption("preview-ui", "Save an offline UI preview without plant connections", "png-path");
        parser.addOption(previewOption);
        parser.process(*app);
        if (parser.isSet("check-errors")) return basketOperatorErrorTests() ? 0 : 1;
        if (checkSafety) return basketEnvironmentSafetyTests() && basketNativeSqlServerTests() ? 0 : 1;
        QString configError;
        if (!basketCraneConfig().load(parser.value(configOption), configError)) {
            if (checkOnly || checkDatabases || checkHistory) std::fprintf(stderr, "%s\n", configError.toLocal8Bit().constData());
            else QMessageBox::critical(0, "Configuration error", configError);
            return 1;
        }
        if (checkOnly) {
            if (!basketCraneConfig().EmailSettings().ValidationError().isEmpty())
                std::fprintf(stderr,"%s\n",basketCraneConfig().EmailSettings().ValidationError().toLocal8Bit().constData());
            std::printf("Configuration is valid. Environment=%s; Database=%s; PLC=%s\n",
                basketCraneConfig().environment().toLatin1().constData(),
                basketCraneConfig().databaseWritesAllowed() ? "read/write" : "read-only",
                basketCraneConfig().liveControlAllowed() ? "live" : (basketCraneConfig().observer() ? "read-only telemetry" : "offline"));
            return 0;
        }
        if (checkDatabases) {
            bool allConnected=true;
            foreach (const QString& section,QStringList()<<"BasketDatabase"<<"EpicsDatabase"<<"Epics2Database") {
                const QString name="direct_check_"+section;
                {
                    QSqlDatabase database=QSqlDatabase::addDatabase(new basket::NativeSqlServerDriver(
                        basket::sqlServerConnectionString(basketCraneConfig(),section),basketCraneConfig().text(section+"/Password")),name);
                    if (!database.open()) {
                        std::fprintf(stderr,"%s: %s\n",section.toLatin1().constData(),database.lastError().text().toLocal8Bit().constData()); allConnected=false;
                    } else {
                        QSqlQuery query(database);
                        if (!query.exec("SELECT 1,DB_NAME()") || !query.next() || query.value(0).toInt()!=1
                            || query.value(1).toString()!=basketCraneConfig().text(section+"/Catalog")) {
                            std::fprintf(stderr,"%s: direct SQL Server read check failed: %s\n",section.toLatin1().constData(),query.lastError().text().toLocal8Bit().constData()); allConnected=false;
                        } else std::printf("PASS: %s direct SQL Server connection and catalog (SELECT only).\n",section.toLatin1().constData());
                    }
                    database.close();
                }
                QSqlDatabase::removeDatabase(name);
            }
            return allConnected?0:1;
        }
        if (checkHistory) {
            // Always use the SELECT-only driver, regardless of configured mode.
            const QString backendName="history_read_backend";
            QSqlDatabase backend=QSqlDatabase::addDatabase(new basket::NativeSqlServerDriver(
                basket::sqlServerConnectionString(basketCraneConfig(),"BasketDatabase"),
                basketCraneConfig().text("BasketDatabase/Password")),backendName);
            if (!backend.open()) { std::fprintf(stderr,"History connection failed: %s\n",qPrintable(backend.lastError().text())); return 1; }
            QSqlDatabase database=QSqlDatabase::addDatabase(new BasketReadOnlyDriver(backend),bDb);
            if (!database.open()) return 1;
            const auto history=readCraneControllerHistory(100);
            for (const auto& event : history) {
                if (event[0].toString().startsWith("Move basket"))
                    std::printf("%s\n",qPrintable(controllerHistoryText(event[0].toString(),event[3])));
            }
            database.close(); backend.close();
            return 0;
        }
        QApplication::setStyle("Fusion");
        applyModernCranePalette();
        qApp->setStyleSheet(modernCraneTheme());
        if (parser.isSet(previewOption)) return basketUiPreview(parser.value(previewOption), basketCraneConfig().environment()) ? 0 : 1;
        crane2Ip = basketCraneConfig().plcEndpoint("Plc/Crane2Ip");
        oldOvenIp = basketCraneConfig().plcEndpoint("Plc/OldOvenIp");
        newOvenIp = basketCraneConfig().plcEndpoint("Plc/NewOvenIp");
        passWeak = basketCraneConfig().text("Access/WeakPassword");
        passStrong = basketCraneConfig().text("Access/StrongPassword");

        // One instance per environment, shared across renamed executable copies.
        QSharedMemory instance("ApexBasketCrane2_" + basketCraneConfig().environment());
        if (!instance.create(1)) {
            QMessageBox::critical(0, "Already running", "Cannot acquire the instance lock for " + basketCraneConfig().environment());
            return 1;
        }
        EmailAlerts emailAlerts(basketCraneConfig().EmailSettings());
        qInstallMessageHandler(messageOutPut);
        QScopedPointer<mainWindowClass> window;
        QScopedPointer<QMainWindow> startupErrorWindow;
        try {
            StartupProgress progress;
            window.reset(new mainWindowClass);
            progress.Finish(*window);
            window->showMaximized();
        } catch (const OperatorError& error) {
            // Startup cannot continue with a partly constructed control window.
            // Keep a usable error window open instead of terminating the process.
            startupErrorWindow.reset(new QMainWindow);
            startupErrorWindow->setWindowTitle("Basket Crane 2 - Startup needs attention");
            QLabel* message=new QLabel("Basket Crane 2 could not finish starting.\n\n" +
                operatorErrorExplanation(error.details) +
                "\n\nMachine controls are unavailable. After IT resolves the problem, close this window and reopen the application.");
            message->setWordWrap(true); message->setMargin(24);
            startupErrorWindow->setCentralWidget(message);
            startupErrorWindow->resize(620,300); startupErrorWindow->show();
            showOperatorError(error,startupErrorWindow.data());
        }
        return app->exec();
    }
};
} // namespace basket
