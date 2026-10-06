#include "conPos.h"
convPositionClass::convPositionClass(tuxipClass *tuxip_,QGraphicsScene *myScene_,
	int globalId_,QString description_,QString plcIp_,QString plcBasketNumberTable_,QString plcStateTable_,QString plcDataTable_,int plcIndex_,
		gItemClass *posTopMouseOver_,gItemClass *conveyorItem_,gItemClass *basketTop_,gItemClass *profileTop_,
		float dx,float dy,QWidget *p):QWidget(p)
{
	setVisible(false);
	tuxip=tuxip_;
	myScene=myScene_;
	setVisible(false);
	globalId=globalId_;
	description=description_;
	gXPos=dx;
	gYPos=dy;
	angle=0.f;
	rotateXoffset=0.f;
	rotateYoffset=0.f;
	inMaintenanceMode=false;
	plcIp=plcIp_;
	plcBasketNumberTable=plcBasketNumberTable_;
	plcStateTable=plcStateTable_;
	plcDataTable=plcDataTable_;
	plcIndex=plcIndex_;

	mouseOver=new gItemClass(posTopMouseOver_,gXPos,gYPos,this);
	conveyor=new gItemClass(conveyorItem_,gXPos,gYPos,this);
	basket=new gItemClass(basketTop_,gXPos,gYPos,this);
	profiles=new gItemClass(profileTop_,gXPos,gYPos,this);
	myScene->addItem(conveyor);
	myScene->addItem(basket);
	myScene->addItem(profiles);
	myScene->addItem(mouseOver);
	connect(mouseOver,SIGNAL(hoverEnterSignal()),this,SLOT(hoverEnterSlot()));
	connect(mouseOver,SIGNAL(hoverLeaveSignal()),this,SLOT(hoverLeaveSlot()));
	connect(mouseOver,SIGNAL(mouseReleasedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mouseReleasedSlot(QGraphicsSceneMouseEvent*)));
	QTransform  txtTransformation;
	txtTransformation.scale(1,-1);
//	txtTransformation.translate(gXPos,-gYPos);
	graphicsText=new myGraphicsTextItem(QString("%1").arg(globalId),mouseOver->boundingRect(),QFont("Arial",500),QColor(50,50,150),mouseOver,myScene);
	graphicsText->setTransform(txtTransformation);
	basketNumber=basketDestination=0;
	updateGui();
	posDlg=new cPosDlgClass(globalId,plcIndex,description,this);
}
convPositionClass::~convPositionClass()
{
}
void convPositionClass::addMoveClass(moveClass* m)
{
	movesVector<<m;
	m->setMaintenanceMode(inMaintenanceMode);
}
int convPositionClass::getGlobalId()
{
	return globalId;
}
float convPositionClass::getGX()
{
	return gXPos;
}
float convPositionClass::getGY()
{
	return gYPos;
}
void convPositionClass::addProximitySwitch(int bit,float offset,QString txt)
{
	addProximitySwitch(plcIp,plcStateTable,plcIndex,bit,txt,offset);
}
void convPositionClass::addProximitySwitch(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset)
{
	gProximitySwitchClass *ps=new gProximitySwitchClass(plcIpAdress_,plcTable_,plcIndex_,plcBit_,txt,offset,myScene);
	QTransform  transformation;
	transformation.translate(gXPos,gYPos);
	ps->setTransform(transformation);
	proximitySwitchVector<<ps;
}
void convPositionClass::addButton(QString plcIpAdress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,float offset,QString label,int type)
{
	buttonClass *btn=new buttonClass(tuxip,plcIpAdress_,plcCommandTable_,plcCommandIndex_,plcCommandNumber_,plcActiveTable_,plcActiveIndex_,plcActiveBit_,offset,myScene,label,type);
	QTransform  transformation;
	transformation.translate(gXPos,gYPos);
	btn->setTransform(transformation);
	buttonsVector<<btn;
}
void convPositionClass::setMaintenanceMode(bool m)
{
	inMaintenanceMode=m;
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->setMaintenanceMode(inMaintenanceMode);
	for (int i=0;i<movesVector.count();i++)
		movesVector[i]->setMaintenanceMode(inMaintenanceMode);
	updateGui();
}
void convPositionClass::myRotate(float rotateXoffset_,float rotateYoffset_,float angle_)
{
	rotateXoffset=rotateXoffset_;
	rotateYoffset=rotateYoffset_;
	angle=angle_;
	QTransform  transformation;
	transformation.rotate(angle);
	transformation.translate(rotateXoffset+gXPos*qCos(-uPi*angle/180)-gYPos*qSin(-uPi*angle/180),rotateYoffset+gXPos*qSin(-uPi*angle/180)+gYPos*qCos(-uPi*angle/180));
	for (int i=0;i<proximitySwitchVector.count();i++)
		proximitySwitchVector[i]->setTransform(transformation);
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->setTransform(transformation);
	mouseOver->setTransform(transformation);
	conveyor->setTransform(transformation);
	basket->setTransform(transformation);
	profiles->setTransform(transformation);
}
void convPositionClass::setPos(float x,float y,bool transformText)
{
	gXPos=x;
	gYPos=y;
	if (transformText)
	{
		QTransform  txtTransformation;
		txtTransformation.scale(1,-1);
		txtTransformation.translate(gXPos,-gYPos);
		graphicsText->setTransform(txtTransformation);
	}
	myRotate(rotateXoffset,rotateYoffset,angle);
}
void convPositionClass::hoverEnterSlot()
{
}
void convPositionClass::hoverLeaveSlot()
{
}
void convPositionClass::setBasketId(int b)
{
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcBasketNumberTable,plcIndex,b); }
}
void convPositionClass::setBasketDestination(int d)
{
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcDataTable,plcIndex,d); }
}
void convPositionClass::mouseReleasedSlot(QGraphicsSceneMouseEvent *ev)
{
	posDlg->setData(basketNumber,basketDestination);
	if (posDlg->exec())
	{
		setBasketId(posDlg->getBasketNum());
		setBasketDestination(posDlg->getBasketDestination());
	}
}
bool convPositionClass::getHasBasket()
{
	return basketNumber>0;
}
int convPositionClass::getBasket()
{
	return basketNumber;
}
QString convPositionClass::getDescription()
{
	return description;
}
void convPositionClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIp==ip)&&(plcBasketNumberTable==table)&&(plcIndex==index))
	{
		basketNumber=value.toInt();
		updateGui();
	}
	if ((plcIp==ip)&&(plcDataTable==table)&&(plcIndex==index))
	{
		basketDestination=value.toInt();
	}
	for (int i=0;i<proximitySwitchVector.count();i++)
		proximitySwitchVector[i]->dataInPlcChanged(ip,table,index,value);
	for (int i=0;i<buttonsVector.count();i++)
		buttonsVector[i]->dataInPlcChanged(ip,table,index,value);
	posDlg->dataInPlcChanged(ip,table,index,value);
}

void convPositionClass::updateGui()
{
	QString toolTipTxt;
	if (basketNumber>0)
	{
		graphicsText->setText(QString("%1").arg(plcIndex));
		toolTipTxt+="<font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basketNumber)+"</font><br>";
		basket->setVisible(true);
		//bool hasContents=getFirst(QString("select count(tId) from basketContents where basket=%1").arg(basketNumber),mDb).toInt()>0;
		//profiles->setVisible(hasContents);
	}
	else
	{
		basket->setVisible(false);
		profiles->setVisible(false);
		graphicsText->setText(QString("%2").arg(plcIndex));
		toolTipTxt+=tr("Empty conveyor");
	}
	toolTipTxt+="<br>"+tr("Position:%1(%2)").arg(globalId).arg(description);
	toolTipTxt+="<br>"+tr("Plc basket:%1[%2]").arg(plcBasketNumberTable).arg(plcIndex);
	toolTipTxt+="<br>"+tr("Plc state:%1[%2]").arg(plcStateTable).arg(plcIndex);
	toolTipTxt+="<br>"+tr("Plc data:%1[%2]").arg(plcDataTable).arg(plcIndex);
	mouseOver->setToolTip(toolTipTxt);
}
