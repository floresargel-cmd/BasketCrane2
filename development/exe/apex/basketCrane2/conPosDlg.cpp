#include "conPosDlg.h"
cPosDlgClass::cPosDlgClass(int posId_,int plcId_,QString posDescription,QWidget *p):QDialog(p)
{
	posId=posId_;
	plcId=plcId_;
	//
	setMinimumWidth(600);
	setWindowTitle(tr("Position %1(%2)").arg(posId).arg(posDescription));
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QGridLayout *dataLayout=new QGridLayout();mainLayout->addLayout(dataLayout);
	dataLayout->addWidget(new QLabel(tr("Basket:")),0,0);
	basketSpinBox=new QSpinBox();
	basketSpinBox->setMinimum(0);
	basketSpinBox->setMaximum(INT_MAX);
	basketSpinBox->setValue(basketNum);
	connect(basketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(basketSpinBoxChangedSlot(int)));
	dataLayout->addWidget(basketSpinBox,0,1);
	basketInfoLabel=new QLabel();
	dataLayout->addWidget(basketInfoLabel,0,2);
	basketInfoLabel->setMinimumWidth(400);
	dataLayout->addWidget(new QLabel(tr("Destination:")),1,0);
	destinationsCombo=new QComboBox();
	dataLayout->addWidget(destinationsCombo,1,1,1,2);
	for (const basket::Destination& destination : basket::DestinationCatalog::Entries())
    {
        destinationsCombo->addItem(destination.title, basket::DestinationCatalog::PlcValue(destination.id));
    }
	//
	QVBoxLayout *statusLayout=new QVBoxLayout();mainLayout->addLayout(statusLayout);
	bitVector<<new bitClass("bConveyorState",plcId,0,bitClass::TYPE_CHANGE_COLOR,tr("Auto"),tr("Auto"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("bConveyorState",plcId,1,bitClass::TYPE_CHANGE_COLOR,tr("Manual"),tr("Manual"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("bConveyorState",plcId,6,bitClass::TYPE_CHANGE_COLOR,tr("Ready"),tr("Not ready"),styleLabelG,styleLabelR,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("bConveyorState",plcId,19,bitClass::TYPE_CHANGE_COLOR,tr("Ready for job"),tr("Not ready for job"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("bConveyorState",plcId,22,bitClass::TYPE_CHANGE_COLOR,tr("With basket"),tr("No basket"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	//
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	QPushButton *cancelBtn=new QPushButton(tr("Cancel"),this);
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	btnsLayout->addWidget(cancelBtn);
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsLayout->addWidget(okBtn);
	okBtn->setDefault(true);
	okBtn->setAutoDefault(true);
	mainLayout->addStretch(1);
}
cPosDlgClass::~cPosDlgClass()
{
}
int cPosDlgClass::exec()
{
	return QDialog::exec();
}
int cPosDlgClass::getBasketNum()
{
	return basketSpinBox->value();
}
int cPosDlgClass::getBasketDestination()
{
	return destinationsCombo->currentData(Qt::UserRole).toInt();;
}
void cPosDlgClass::accept()
{
	QDialog::accept();
}
void cPosDlgClass::reject()
{
	QDialog::reject();
}
void cPosDlgClass::setData(int b,int d)
{
	basketNum=b;
	basketDestination=d;
	if (basketSpinBox->value()!=basketNum)
		basketSpinBox->setValue(basketNum);
	destinationsCombo->setCurrentIndex(destinationsCombo->findData(basketDestination));
}
void cPosDlgClass::basketSpinBoxChangedSlot(int b)
{
	QString desc=getDescriptionOfPosWithBasket(b,QVector<int>()<<posId,QVector<int>()<<1);
	if ((b!=0)&&(!desc.isEmpty()))
	{
		basketInfoLabel->setText(tr("Basket %1 is on position %2)").arg(b).arg(desc));
		basketInfoLabel->setStyleSheet("QLabel{color:red;}");
		okBtn->setEnabled(false);
	}
	else
	{
		basketInfoLabel->setText(tr(""));
		basketInfoLabel->setStyleSheet("QLabel{color:blue;}");
		okBtn->setEnabled(true);
	}
}
void cPosDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		if ((table=="bConveyorState")&&(index==plcId))
		{
			for (int i=0;i<bitVector.count();i++)
				bitVector[i]->updateValue(table,index,value.toInt());
		}
	}
}