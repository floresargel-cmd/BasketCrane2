#include "autoWdg.h"
#include "automaticMissions.h"
autoWidgetClass::autoWidgetClass(craneClass *crane_,QWidget *p):QWidget(p)
{
	crane=crane_;
    const QList<QCheckBox*> controls = basket::CreateAutomaticMissionControls(this);
    exportsToDestackerCheckBtn = controls[0];
    exportsToPackingCheckBtn = controls[3];
    importsFromNorthCheckBtn = controls[4];
    importsFromSouthCheckBtn = controls[5];
    autoExportsCheckBtn = controls[6];
    calculateExportGroupsBtn = nullptr;
    for (QCheckBox* control : controls) {
        const int flag = control->property("automaticMissionFlag").toInt();
        connect(control, &QCheckBox::toggled, this, [this, flag](bool checked) {
            if (checked) crane->addToAutoMode(flag);
            else crane->removeFromAutoMode(flag);
        });
    }
}
autoWidgetClass::~autoWidgetClass()
{
}
void autoWidgetClass::exportsToDestackerCheckBtnStateChangedSlot(int state)
{
	if (exportsToDestackerCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsToDestaker);
	else
		crane->removeFromAutoMode(craneClass::autoExportsToDestaker);
}
void autoWidgetClass::exportsToPackingCheckBtnStateChangedSlot(int state)
{
	if (exportsToPackingCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoExportsToPacking);
	else
		crane->removeFromAutoMode(craneClass::autoExportsToPacking);
}
void autoWidgetClass::importsFromNorthCheckBtnStateChangedSlot(int state)
{
	if (importsFromNorthCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoImportsFromNorth);
	else
		crane->removeFromAutoMode(craneClass::autoImportsFromNorth);
}
void autoWidgetClass::importsFromSouthCheckBtnStateChangedSlot(int state)
{
	if (importsFromSouthCheckBtn->isChecked())
		crane->addToAutoMode(craneClass::autoImportsFromSouth);
	else
		crane->removeFromAutoMode(craneClass::autoImportsFromSouth);
}
void autoWidgetClass::autoExportsCheckBtnStateChangedSlot(int state)
{
	if (autoExportsCheckBtn->isChecked())
	{
		//exportsToDestackerCheckBtn->setChecked(true);exportsToDestackerCheckBtn->setEnabled(false);
		//exportsToPackingCheckBtn->setChecked(true);exportsToPackingCheckBtn->setEnabled(false);
		crane->addToAutoMode(craneClass::autoExports);
	}
	else
	{
		//exportsToDestackerCheckBtn->setEnabled(true);
		//exportsToPackingCheckBtn->setEnabled(true);
		crane->removeFromAutoMode(craneClass::autoExports);
	}
}
void autoWidgetClass::sendBasketsToCrane1Slot()
{
	//importsFromNorthCheckBtn->setChecked(false);
	//importsFromSouthCheckBtn->setChecked(false);
}
void autoWidgetClass::calculateExportGroupsBtnClickedSlot()
{
	emit calculateExportGroupsSignal();
}