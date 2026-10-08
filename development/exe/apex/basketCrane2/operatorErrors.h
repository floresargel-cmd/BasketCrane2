#pragma once
#include <QtWidgets>
#include <exception>

namespace basket {
class OperatorError : public std::exception {
public:
    QString title, details;
    OperatorError(const QString& title, const QString& details)
        : title(title), details(details), bytes(details.toUtf8()) {}
    const char* what() const noexcept override { return bytes.constData(); }
private:
    QByteArray bytes;
};

inline QString operatorErrorExplanation(const QString& details) {
    if (details.contains("Cannot open",Qt::CaseInsensitive) && details.contains("database",Qt::CaseInsensitive))
        return "The application could not connect to the plant database.\n\n"
            "Check the computer's network connection and contact IT if the connection cannot be restored.";
    if (details.contains("query",Qt::CaseInsensitive) || details.contains("SQL",Qt::CaseInsensitive))
        return "The application could not read or save plant information.\n\n"
            "Check the network connection. Before repeating a move, confirm the actual basket positions. "
            "Contact IT if the message returns.";
    if (details.contains("moveBasket",Qt::CaseInsensitive) || details.contains("declare basket",Qt::CaseInsensitive)) {
        const QRegularExpressionMatch move=QRegularExpression("from==([^ ,]+) to==([^ ,]+)(?:, fromBasket:(\\d+) toBasket:(\\d+))?").match(details);
        QString context;
        if (move.hasMatch()) {
            context=QString("\n\nSource: %1\nDestination: %2").arg(move.captured(1)).arg(move.captured(2));
            if (!move.captured(3).isEmpty()) context+=QString("\nBasket at source: %1\nBasket at destination: %2 (0 means empty)")
                .arg(move.captured(3)).arg(move.captured(4));
        }
        return QString("The basket information does not match the requested move. The source may be empty, "
            "the destination may already contain a basket, or a position may be missing.") + context + "\n\n"
            "Check the source and destination on the plant floor and compare them with the screen. "
            "Ask a supervisor to correct the basket information before trying again.";
    }
    if (details.contains("file",Qt::CaseInsensitive))
        return "A file needed by the application could not be loaded.\n\n"
            "Contact IT to check the installation and restore the missing or damaged file.";
    return "The application could not complete the requested action.\n\n"
        "Check the current machine and basket status before trying again. "
        "If the message returns, contact IT and provide the error details.";
}

inline void showOperatorError(const OperatorError& error, QWidget* parent=nullptr) {
    // File logging is best-effort and never launches an editor or terminates.
    QDir directory(QCoreApplication::applicationDirPath());
    directory.mkpath("errorLogs");
    QFile file(directory.filePath("errorLogs/operator-errors.txt"));
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString(Qt::ISODate) << " | " << error.title << "\n" << error.details << "\n\n";
    }
    static bool displaying=false;
    static QString lastMessage;
    static QElapsedTimer lastShown;
    if (displaying || (lastMessage==error.details && lastShown.isValid() && lastShown.elapsed()<30000)) return;
    displaying=true;
    lastMessage=error.details; lastShown.start();
    QMessageBox dialog(QMessageBox::Warning,"Basket Crane 2 - Action could not be completed",
        operatorErrorExplanation(error.details),QMessageBox::Ok,parent);
    dialog.setObjectName("operatorErrorDialog");
    dialog.setTextFormat(Qt::PlainText);
    dialog.setInformativeText("The application remains open. This action was stopped. "
        "Dismiss this message after checking the information above.");
    dialog.setDetailedText(error.title + "\n\n" + error.details);
    dialog.exec();
    displaying=false;
}

class OperatorApplication : public QApplication {
public:
    OperatorApplication(int& argc,char** argv) : QApplication(argc,argv) {}
    bool notify(QObject* receiver,QEvent* event) override {
        try { return QApplication::notify(receiver,event); }
        catch (const OperatorError& error) {
            showOperatorError(error,activeWindow());
            return false;
        }
    }
};
} // namespace basket
