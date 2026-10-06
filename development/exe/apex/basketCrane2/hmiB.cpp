#include "hmiB.h"
hmiDlgBasicClass::hmiDlgBasicClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Basket crane basic HMI"));
	resize(800,600);
	tuxip=tuxip_;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);
	setLayout(mainLayout);
	QScrollArea *mainScrollArea=new QScrollArea();mainScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	mainScrollArea->setWidgetResizable(true);
	QWidget *mainScrollAreaWidget=new QWidget();
	QVBoxLayout *mainScrollAreaLayout=new QVBoxLayout();mainScrollAreaLayout->setMargin(2);mainScrollAreaWidget->setLayout(mainScrollAreaLayout);
	mainScrollArea->setWidget(mainScrollAreaWidget);
	mainLayout->addWidget(mainScrollArea);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Reset Alarms"),"",tuxip,"toPcI",22,23,0,"","toPcI",24,2,"","","","","",buttonStyleActive,buttonStyleRed,tr("No Alarms"),this);mainScrollAreaLayout->addWidget(pushButtonsVector.last());
	bitWarningsVector=new bitLogVectorClass("toPcI","plcWarnings.txt",bitClass::TYPE_SHOW_HIDE,styleLabelO,styleLabelG,"","","","",false,this);mainScrollAreaLayout->addWidget(bitWarningsVector);
	bitErrorsVector=new bitLogVectorClass("toPcI","plcAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelG,"","","","",false,this);mainScrollAreaLayout->addWidget(bitErrorsVector);
}
hmiDlgBasicClass::~hmiDlgBasicClass()
{
}
int hmiDlgBasicClass::exec()
{
	return QDialog::exec();
}
void hmiDlgBasicClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (crane2Ip==ip)
	{
		if("toPcI"==table)
		{
			if ((index==21)||(index==23)||(index==24))
			{
				for (int i=0;i<pushButtonsVector.count();i++)
					pushButtonsVector[i]->updateValue(table,index,value.toInt());
			}
			bitErrorsVector->updateValue(table,index,value.toInt());
			bitWarningsVector->updateValue(table,index,value.toInt());
		}
	}
}