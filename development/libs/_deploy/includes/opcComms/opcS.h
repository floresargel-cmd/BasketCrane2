#ifndef OPCS_H
#define OPCS_H

#include "utilities.h"
#include <QtGui>
#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include "qaxtypes.h"

#include <stdio.h>
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#include "opcda.h"
#include "OPCClient.h"
#include "OPCHost.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "OPCItem.h"
#include <sys\timeb.h>
#include "ping.h"
class opcVarClass				
{
	QTreeWidgetItem			*treeWidgetItem;
public:
	QString							fullPath;
	COPCItem						*opcItem;
enum
{
	userRoleNone=Qt::UserRole,
	userRoleName=userRoleNone+1,
	userRoleFullPath=userRoleName+1,
	userRoleValue=userRoleFullPath+1,
};
public:
	opcVarClass(QString fullPath_,QTreeWidgetItem *itm);
	void dataFromOpcServ(QString varName,QVariant value,QString timeStab);
	void setOpcItem(COPCItem* item);
};
////////////////////////////////////////////////////////////////////
class CMyCallback:public IAsynchDataCallback
{
	void OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes);
};
////////////////////////////////////////////////////////////////////
class opcServerDlgClass : public QDialog				
{
	QTreeWidget													*treeWidget;
	COPCHost														*host;
	QVector<COPCServer*>								opcServers;
	QVector<opcVarClass*>								opcVars;
	CMyCallback													myCallBack;
	COPCGroup														*group;
	bool																isServerOnline;
	QTreeWidgetItem											*serverItem;
	QListWidget													*logListW;
	Q_OBJECT								
public:										
	opcServerDlgClass(QString hostName,QWidget *p=NULL);							
	~opcServerDlgClass();							
	void connectToServer(QString serverName);
	opcVarClass* addItemToItem(QTreeWidgetItem *topItem,QString fullName);
	void watchVariable(QString name);
	void dataFromOpcServ(QString varName,QVariant value,QString timeStab);
	void watchAllVariables();
	void writeVal(QString varName,QVariant val);
	void logMessage(QString mess);
private:
signals:
	void dataInPlcChangedSignal(QString ip,QString table,int index,QVariant value);
public slots:
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void treeWidgetItemDoubleClickedSlot(QTreeWidgetItem *item,int column);
};											
#endif
