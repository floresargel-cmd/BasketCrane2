#include "destW.h"
destackerWidgetClass::destackerWidgetClass(tuxipClass *tuxip,QWidget *p):QWidget(p)
{
	setWindowTitle(tr("Destacker HMI"));
	//resize(1800,400);
	QVBoxLayout *mainL=new QVBoxLayout();setLayout(mainL);mainL->setMargin(2);
	QGroupBox *pincersG=new QGroupBox(tr("Pincers"));mainL->addWidget(pincersG);
	QGridLayout *pincersGL=new QGridLayout();pincersGL->setMargin(2);pincersG->setLayout(pincersGL);
	for (int i=0;i<8;i++)
	{
		bitVector<<new bitClass("toPcI",51,i,bitClass::TYPE_CHANGE_COLOR,tr("Ok"),tr("Not ok"),styleLabelG,styleLabelGr,"","","","",this);
		pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Disable"),"",tuxip,"toPcI",50,i+1,0,"","toPcI",51,8+i,"","","","","",buttonStyleActive,buttonStyle,tr("Enable"),this);
		pincersGL->addWidget(new QLabel(tr("Pincer %1").arg(i+1)),0,7-i);
		pincersGL->addWidget(bitVector.last(),1,7-i);
		pincersGL->addWidget(pushButtonsVector.last(),2,7-i);
	}
	QGroupBox *modeG=new QGroupBox(tr("Mode"));mainL->addWidget(modeG);
	QHBoxLayout *modeL=new QHBoxLayout();modeL->setMargin(2);modeG->setLayout(modeL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Spacers on belts"),"",tuxip,"toPcI",50,9,0,"","toPcI",51,16,"","","","","",buttonStyle,buttonStyleActive,tr("Spacers on belts"),this);modeL->addWidget(pushButtonsVector.last());
//	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"PcCom",9,2,0,"","bConveyorState",1,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeL->addWidget(pushButtonsVector.last());
	modeL->addStretch(1);
	bitLogVectors<<new bitLogVectorClass("toPcI",QString("destackerAlarms.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,this);mainL->addWidget(bitLogVectors.last(),1);
}
destackerWidgetClass::~destackerWidgetClass()
{
}
void destackerWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		if (index==52)
			double a=0.;
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<bitLogVectors.count();i++)
			bitLogVectors[i]->updateValue(table,index,value.toInt());
	}
}