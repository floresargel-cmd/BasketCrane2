#include "posDlg.h"
posDlgClass::posDlgClass(allPossClass *allPoss_,posClass *parrentPos_,QString user,QWidget *p):QDialog(p)
{
	allPoss=allPoss_;
	parrentPos=parrentPos_;
	//
	resize(900,700);
	setWindowTitle(tr("Position %1,%2").arg(parrentPos->getPosIndexDisplayString()).arg(parrentPos->getDescription()));
	QWidget *posWidget=new QWidget(this);
	//basket
	QGridLayout *dataWidgetLayout=new QGridLayout();
	dataWidgetLayout->addWidget(new QLabel(tr("basket:")),0,0);
	basketSpinBox=new QSpinBox();
	basketSpinBox->setMinimum(0);
	basketSpinBox->setMaximum(INT_MAX);
	basketSpinBox->setValue(parrentPos->getBasketNumber());
	basketSpinBox->setStyleSheet("QSpinBox{font-size:20px;border-radius:7px;border-color:#8888aa;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#555599;}");

	dataWidgetLayout->addWidget(basketSpinBox,0,1);
	connect(basketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(basketSpinBoxValueChangedSlot(int)));
	basketInfoLabel=new QLabel(tr(""));basketInfoLabel->setMinimumWidth(200);
	basketInfoLabel->setStyleSheet("QLabel{color:red;}");
	dataWidgetLayout->addWidget(basketInfoLabel,0,2);

	lockedCheckBox=new QCheckBox(tr("locked"));
	dataWidgetLayout->addWidget(lockedCheckBox,1,0);
	lockedCheckBox->setChecked(getOne(QString("select locked from positions where posNumber=%1 and posIndex=%2").arg(parrentPos->getPositionNumber()).arg(parrentPos->getPositionIndex()),bDb).toInt()==2);
	dataWidgetLayout->setColumnStretch(3,1);
	//
	QWidget *basketWidget=NULL;

	QVBoxLayout *posLogWidgetLayout=new QVBoxLayout();

	QTableWidget *posLogTable=new QTableWidget();posLogWidgetLayout->addWidget(posLogTable);
	posLogTable->setAlternatingRowColors(true);
	posLogTable->horizontalHeader()->setStretchLastSection(true);
	posLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	posLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	posLogTable->setColumnCount(3);
	posLogTable->setHorizontalHeaderLabels(QStringList()<<tr("basket")<<tr("info")<<tr("time"));
	posLogTable->setWordWrap (true);

	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select basket,info,time from c2logPos where basket=%1 order by tid desc").arg(basketSpinBox->value()),bDb);
	posLogTable->setRowCount(dataVV.count());
	for (int i=0;i<dataVV.count();i++)
	{
		posLogTable->setItem(i,0,new QTableWidgetItem(dataVV[i][0].toString()));
		posLogTable->setItem(i,1,new QTableWidgetItem(dataVV[i][1].toString().trimmed()));
		posLogTable->setItem(i,2,new QTableWidgetItem(dataVV[i][2].toString().trimmed()));
	}
	QVBoxLayout *posWidgetLayout=new QVBoxLayout();
	posWidgetLayout->addLayout(dataWidgetLayout);
	posWidgetLayout->addLayout(posLogWidgetLayout);
	posWidget->setLayout(posWidgetLayout);
	//
	basketWidget=new QWidget(this);
	QVBoxLayout *basketWidgetLayout=new QVBoxLayout();

	basketContents=new basketContentsClass(this);
	basketWidgetLayout->addWidget(basketContents);

	basketLogTable=new QTableWidget();
	basketLogTable->setAlternatingRowColors(true);
	basketLogTable->horizontalHeader()->setStretchLastSection(true);
	basketLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	basketLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	basketLogTable->setColumnCount(3);
	basketLogTable->setHorizontalHeaderLabels(QStringList()<<tr("position")<<tr("info")<<tr("time"));
	basketLogTable->setWordWrap (true);
	basketWidgetLayout->addWidget(basketLogTable);
	basketWidget->setLayout(basketWidgetLayout);
	//
	mainTabWidget=new QTabWidget(this);
	mainTabWidget->addTab(basketWidget,tr("basket %1 data").arg(parrentPos->getBasketNumber()));
	mainTabWidget->addTab(posWidget,tr("position data"));
	//
	QHBoxLayout *btnsLayout=new QHBoxLayout();
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsLayout->addWidget(okBtn);

	updateBasketData();
	QVBoxLayout *mainLayout=new QVBoxLayout();
	mainLayout->addWidget(mainTabWidget);
	mainLayout->addLayout(btnsLayout);
	setLayout(mainLayout);
    if (!basketCraneConfig().databaseWritesAllowed()) {
        basketSpinBox->setEnabled(false);
        lockedCheckBox->setEnabled(false);
        okBtn->hide();
        QPushButton *closeButton = new QPushButton("Close", this);
        btnsLayout->addWidget(closeButton);
        connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
    }
}
posDlgClass::~posDlgClass()
{
}
int posDlgClass::myExec(bool checkPassword)
{
	if ((!checkPassword)||(getPassword("Password is needed to open this window.","Please give the password",passWeak)))
		return QDialog::exec();
	return 0;
}
int posDlgClass::exec()
{
	return QDialog::exec();
}
int posDlgClass::getBasketNum()
{
	return basketSpinBox->value();
}
void posDlgClass::accept()
{
    if (!basketCraneConfig().databaseWritesAllowed()) { reject(); return; }
	basketExecQuery(QString("update positions set locked=%1 where posNumber=%2 and posIndex=%3").arg(lockedCheckBox->isChecked()?2:1).arg(parrentPos->getPositionNumber()).arg(parrentPos->getPositionIndex()),bDb);
	QDialog::accept();
}
void posDlgClass::reject()
{
	QDialog::reject();
}
void posDlgClass::basketSpinBoxValueChangedSlot(int b)
{
	QString posOfBasket=getDescriptionOfPosWithBasket(b,QVector<int>()<<parrentPos->getPositionNumber(),QVector<int>()<<parrentPos->getPositionIndex());
	if ((b!=0)&&(posOfBasket.length()!=0))
	{
		basketInfoLabel->setText(tr("basket %1 is on position %2").arg(b).arg(posOfBasket));
		okBtn->setEnabled(false);
	}
	else
	{
		basketInfoLabel->setText(tr(""));
		okBtn->setEnabled(true);
	}
	updateBasketData();
}
void posDlgClass::setBasketNum(int b)
{
	basketSpinBox->setValue(b);
	updateBasketData();
}
void posDlgClass::updateBasketData()
{
	if (basketSpinBox->value()>0)
		mainTabWidget->setTabText(0,tr("basket %1 data").arg(basketSpinBox->value()));
	else
		mainTabWidget->setTabText(0,tr("No basket"));
	//
	if (basketSpinBox->value()!=0)
	{
		basketContents->updateTable(basketSpinBox->value());
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select pos,info,time from c2logPos where basket=%1 order by tid desc").arg(basketSpinBox->value()),bDb);
		basketLogTable->setRowCount(dataVV.count());
		for (int i=0;i<dataVV.count();i++)
		{
			basketLogTable->setItem(i,0,new QTableWidgetItem(dataVV[i][0].toString()));
			basketLogTable->setItem(i,1,new QTableWidgetItem(dataVV[i][1].toString().trimmed()));
			basketLogTable->setItem(i,2,new QTableWidgetItem(dataVV[i][2].toString().trimmed()));
		}
	}
	else
	{
		basketContents->updateTable(0);
		basketLogTable->setRowCount(0);
	}
}
