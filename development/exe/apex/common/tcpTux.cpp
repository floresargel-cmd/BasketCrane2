#include "tcpTux.h"
tcpServerClass::tcpServerClass(tuxipClass *tuxip_,QWidget *parent):QWidget(parent)
{
	QWidget::hide();
	tuxip=tuxip_;
}
void tcpServerClass::sendString(QString outgoingString)
{
	tuxip->sendString(outgoingString);
}
tuxipClass *tcpServerClass::getTuxipClass()
{
	return tuxip;
}
