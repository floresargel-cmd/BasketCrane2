#include "btns.h"
void myQPushButton::mousePressEvent(QMouseEvent *ev)
{
	QPushButton::mousePressEvent(ev);
	isPressed=true;
	tcpServer->sendString(sendCommandToServerString);
}
void myQPushButton::mouseReleaseEvent(QMouseEvent *ev)
{
	QPushButton::mouseReleaseEvent(ev);
	isPressed=false;
	tcpServer->sendString(sendZeroToServerString);
}
void myQPushButton::leaveEvent(QEvent *ev)
{
	if (isPressed)
	{
		isPressed=false;
		//tcpServer->sendString(sendZeroToServerString);
	}
	QPushButton::leaveEvent(ev);
}
void myQPushButton::setEnabled(bool e,bool sendZeroToPlc)
{
	QPushButton::setEnabled(e);
	if ((!e)&&(sendZeroToPlc))
		tcpServer->sendString(sendZeroToServerString);
}
void myQPushButton::setDisabled(bool e,bool sendZeroToPlc)
{
	QPushButton::setDisabled(e);
	if ((e)&&(sendZeroToPlc))
		tcpServer->sendString(sendZeroToServerString);
}
void myQPushButton::setSendCommandToServerString(QString cmd)
{
	sendCommandToServerString=cmd;
	setToolTip(tr("This button sends<span style='color:red'><br>%1<br><span style='color:black'>to PLC.").arg(sendCommandToServerString));
}
/////////////////////////////
void myQRadioButton::mousePressEvent(QMouseEvent *ev)
{
	QRadioButton::mousePressEvent(ev);
	isPressed=true;
	tcpServer->sendString(sendCommandToServerString);
}
void myQRadioButton::mouseReleaseEvent(QMouseEvent *ev)
{
	QRadioButton::mouseReleaseEvent(ev);
	isPressed=false;
	tcpServer->sendString(sendZeroToServerString);
}
void myQRadioButton::leaveEvent(QEvent *ev)
{
	if (isPressed)
	{
		isPressed=false;
		//tcpServer->sendString(sendZeroToServerString);
	}
	QRadioButton::leaveEvent(ev);
}
void myQRadioButton::setEnabled(bool e,bool sendZeroToPlc)
{
	QRadioButton::setEnabled(e);
	if ((!e)&&(sendZeroToPlc))
		tcpServer->sendString(sendZeroToServerString);
}
void myQRadioButton::setDisabled(bool e,bool sendZeroToPlc)
{
	QRadioButton::setDisabled(e);
	if ((e)&&(sendZeroToPlc))
		tcpServer->sendString(sendZeroToServerString);
}