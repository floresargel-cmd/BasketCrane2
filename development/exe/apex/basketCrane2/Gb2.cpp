#include "Gb2.h"
#include "auditLogger.h"
#include "emailAlerts.h"
#include "passwordAuthorization.h"
#include <cstdio>

namespace {
void reportAuditFailure(const QString& error)
{
    EmailAlerts::Notify("Audit logging failed",error);
    // Avoid invoking Qt's message handler recursively from the logger.
    std::fprintf(stderr, "Audit logging failed: %s\n", error.toLocal8Bit().constData());
}
}

void logV(QString description, QString type)
{
    if (type.compare("error",Qt::CaseInsensitive)==0) EmailAlerts::Notify("Application error",description);
    basket::SqlCommandExecutor executor(basketCraneConfig(), bDb);
    basket::AuditLogger logger(executor, maxMessageLength, maxDescriptionLength);
    if (!logger.Verbose(description, type, timeForLog)) reportAuditFailure(executor.LastError());
}

void log(QString description, QString type)
{
    if (type.compare("error",Qt::CaseInsensitive)==0) EmailAlerts::Notify("Application error",description);
    basket::SqlCommandExecutor executor(basketCraneConfig(), bDb);
    basket::AuditLogger logger(executor, maxMessageLength, maxDescriptionLength);
    if (!logger.Operational(description, type, timeForLog)) reportAuditFailure(executor.LastError());
}

void logGui(QString message, QString description, QString sender, QString type)
{
    if (type.compare("error",Qt::CaseInsensitive)==0) EmailAlerts::Notify(message,description);
    basket::SqlCommandExecutor executor(basketCraneConfig(), bDb);
    basket::AuditLogger logger(executor, maxMessageLength, maxDescriptionLength);
    if (!logger.Gui(message, description, sender, type, timeForLog)) reportAuditFailure(executor.LastError());
}

QString	getDescriptionOfPosWithBasket(int b,QVector<int> ignorePos,QVector<int> ignoreIndex)
{
	if (b!=0)
	{
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select description,posNumber,posIndex from positions where basketTableId=%1").arg(b),bDb);
		if (dataVV.count()==1)
		{
			for (int i=0;i<ignorePos.count();i++)
			{
				if ( (ignorePos[i]==dataVV[0][1].toInt())&&(ignoreIndex[i]==dataVV[0][2].toInt()) )
					return QString();
			}
			return QString("%1 (%2.%3)").arg(dataVV[0][0].toString().trimmed()).arg(dataVV[0][1].toInt()).arg(dataVV[0][2].toInt()).trimmed();
		}
	}
	return QString();
}
///////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////////////////
QString passWeak;
QString passStrong;
bool getPassword(QString title, QString text, QString pass, QWidget* parent)
{
    bool accepted = false;
    const QString supplied = QInputDialog::getText(parent, title, text, QLineEdit::Password,
        QString(), &accepted);
    if (!accepted) return false;

    const basket::PasswordAuthorization authorization(passStrong,
        [](const QString& password) { return uCheckPassword(password); });
    if (authorization.Authorize(supplied, pass)) return true;
    if (!supplied.isEmpty())
        QMessageBox::critical(parent, "Wrong password", "This is the wrong password.");
    return false;
}
int getBasketNumberInPos(int p)
{
	return getFirst(QString("select number from baskets where pid in (select basketTableId from positions where posNumber=%1)").arg(p),bDb).toInt();
}
bool isPosActiveG(int posNumber)
{
	int tId=getFirst(QString("select tId from c2missions where fromPosNumber=%1 or toPosNumber=%1").arg(posNumber),bDb).toInt();
	return tId!=0;
}
