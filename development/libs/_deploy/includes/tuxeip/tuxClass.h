#ifndef TUXCLASS_H
#define TUXCLASS_H

#include <QtGui>
#include <QScrollArea>
//#include <time.h> 
#include "tuxG.h"
#include "myTuxEip.h"
class plcTableClass;
#include "plcTable.h"
////////////////////////////////reconectDialog////////////////////////////////////
class reconectDialogClass : public QDialog
{
	QTimer										mainTimer;
	tuxipClass								*tuxip;
	QLabel										*errorLabel;
	QLabel										*timeLabel;
	QPushButton								*reconnectBtn;
	QPushButton								*quitBtn;
	int												timeRemainingForTheNextAttempt;
	bool											isTryingToReconnect;
	Q_OBJECT								
public:										
	reconectDialogClass(tuxipClass *t);							
	~reconectDialogClass();
	void startReconnectAttempts(QString errorMessage);
public slots:
	void onMainTimer();
	void tryToReconnectSlot();
	void quitSlot();
};
////////////////////////////////tuxipClass////////////////////////////////////
class tuxipClass : public QWidget
{												
	QTimer										mainTimer;
	int												connectionId;
	QString										comment;
	tuxipConnectionClass			*tuxipConnection;
	QString										ip;//ip of the logix cpu or the 1756-DHRIO card
	
	QStringList								path;//for lgx: 1 for backplane
																					//# for the slot that the cpu is on the rack
																 //for lgx it usually is 1,0 (1 for backplane and 0 for cpu slot (cpu is usually in 0 slot of the rack))
																 //for slc: 1 for backplane
																					//# for the slot of the Interface Module
																					//# for interface module exit:
																							//Interface Module					Port 1					Port 2									Port 3
																							//1756- ENET/ENBT						Backplane				Ethernet network				N/A
																							//1756-DHRIO								Backplane				DH+ Network on Ch. A		DH+ Network on Ch. B
																							//1756-CNET									Backplane				ControlNet network			N/A
																	//connect through 1756-DHRIO,Ch. A:1 for backplane,2 for the slot of 1756-DHRIO, 2 for Port 2 of 1756-DHRIO (DH+ Network on Ch. A)->1,2,2
																  //to continue to an other Interface Module add the node is from rslinx and then backplane 
	DHP_Header								dhp;
	DHP_Channel								dhpChannel;
	
	QVector<plcTableClass*>		tablesVector;
	QHBoxLayout								*mainLayout;
	bool											emulateMode;
	bool											hasError;
	reconectDialogClass				*reconectDialog;
	int												plcType;
	enum{//plcType
		typeNone=0,
		typeLGX=1,
		typeSLC=2,
	};
	int												maxVarsToRead;
	//
	Q_OBJECT								
public:										
	tuxipClass(QString ip_,QString comment_,bool emulateMode_,QWidget *p);							
	~tuxipClass();
	void addTable(QString tableName,int type,int minIndex,int maxIndex,int updateInterval);
	void setUpdatePlcReadIntervalForAllTables(int ms);
	plcTableClass* getTableWithName(QString name);
	QString getIp();
	QString getComment();
	bool getIsOnEmulationMode();
	int getConnectionId();
	int getNumberOfTables();
	int getTypeOfTable(QString address);
	void setHasError(bool e,QString errorMessage);
	bool getHasError();
	void connectToPlc();
	void setIsAutoUpdatingFromPlc(bool u);
	void setVarComment(QString table,int index,QString comment);
	void setBitComment(QString table,int index,int bit,QString comment);
	void dataWritenToPlc(QString table,int index);

	void writeBit(QString table,int tableIndex,int bitIndex,bool value);
	void toggleBit(QString table,int tableIndex,int bitIndex);
	void writeInteger(QString table,int index,int value);
	void writeIntegers(QString table,int index,QVector<int> values);
	int readIntegerValue(QString table,int index);
	int readIntegerValue(QString table);
	QVector<int> readIntegerValues(QString table,int index,int numberOfValues);

	void writeFloat(QString table,int index,float value);
	void writeFloats(QString table,int index,QVector<float> values);
	float readFloatValue(QString table,int index);
	QVector<float> readFloatValues(QString table,int index,int numberOfValues);
	QString createAdress(QString table,int index);

	//for backwards compatibility
	void sendString(QString outgoingString);
signals:
	void dataInPlcChangedSignal(QString ip,QString table,int index,QVariant value);
public slots:
	void dataInPlcChangedSlot(QString,int,QVariant);
public:
	int getOldIntegerValue(QString table,int index);
	int getOldIntegerValue(QString table);
	QVector<int> getOldIntegerValues(QString table,int index,int numberOfValues);
	float getOldFloatValue(QString table,int index);
	QVector<float> getOldFloatValues(QString table,int index,int numberOfValues);
};											
#endif
