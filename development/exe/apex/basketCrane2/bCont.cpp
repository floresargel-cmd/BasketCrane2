#include "bCont.h"
basketContentsClass::basketContentsClass(QWidget *w):QWidget(w)
{
	basketNum=0;
	QVBoxLayout *mainLayout=new QVBoxLayout(this);setLayout(mainLayout);
	//upLayout
	tableView=new QTableWidget();
	tableView->setAlternatingRowColors(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setColumnCount(3);
	tableView->setHorizontalHeaderLabels(QStringList()<<tr("Profile")<<tr("Length")<<tr("Quantity"));
	mainLayout->addWidget(tableView,1);
}
basketContentsClass::~basketContentsClass()
{
}
void basketContentsClass::updateTable(int basketNum_)
{
	basketNum=basketNum_;
	updateTable();
}
void basketContentsClass::updateTable()
{
	if (basketNum==0)
	{
		tableView->clearContents();
		tableView->setRowCount(0);
		return;
	}
	QVector<QVector<QVariant>> bContVV;//=execTableQuery(QString("select tId,profile,quantity,weight,length,color,sapNumber from basketsContents where basket=%1").arg(basketNum),bDb);
	tableView->clearContents();
	tableView->setRowCount(bContVV.count()+1);
	for (int i=0;i<bContVV.count();i++) 
	{
		//QString profileName=bContVV[i][1].toString().trimmed();
		//QWidget *profileWidget=new QWidget(this);
		//QVBoxLayout *profileWidgetLayout=new QVBoxLayout();
		//profileWidget->setLayout(profileWidgetLayout);
		//QToolButton *profileButton=new QToolButton(this);profileButton->setStyleSheet("QToolButton{font-size:16px;text-align:center bottom;}");

		//QString picFileName=getPicFileNameFromProfile(profileName);

		//profileButton->setIcon(QIcon(picFileName));
		//profileButton->setStatusTip(tr("Edit record"));
		//profileButton->setIconSize(QSize(95,95));
		//profileButton->setToolTip(QString("<div style='color:rgb(100,100,150);font-size:20pt'> <b><span style='color:rgb(150,100,100);font-size:20pt'>%1</span></b> <br></div><img src=\"%2\">").arg(profileName.trimmed().length()>0?QString("%1").arg(profileName):"").arg(picFileName));

		//profileWidgetLayout->addWidget(profileButton,1,Qt::AlignHCenter);
		//profileWidgetLayout->addWidget(new QLabel(profileName),1,Qt::AlignHCenter);
		//tableView->setCellWidget(i,0,profileWidget);

		//QString colorStr,qualityStr;
		//tableView->setItem(i,1,new QTableWidgetItem(tr("%1mm").arg(bContVV[i][4].toFloat())));
		//tableView->setItem(i,2,new QTableWidgetItem(bContVV[i][2].toString()));
		//tableView->setItem(i,3,new QTableWidgetItem(tr("%1Kgr").arg(bContVV[i][3].toFloat(),0,'f',2,' ')));
		//tableView->setItem(i,4,new QTableWidgetItem(bContVV[i][5].toString()));									
		//tableView->setItem(i,5,new QTableWidgetItem(bContVV[i][6].toString()));									
		//for (int k=0;k<tableView->columnCount();k++)
		//{
		//	QTableWidgetItem *a=tableView->item(i,k);
		//	if (a)
		//		a->setTextAlignment(Qt::AlignCenter);
		//}
	}
	tableView->resizeColumnsToContents();
	tableView->horizontalHeader()->setStretchLastSection(true);
}