#ifndef SMTP_H
#define SMTP_H

#include "smtpclient.h"
#include "mimetext.h"
#include "mimeattachment.h"
class smtpClass : public QObject
{												
	Q_OBJECT								
public:										
	smtpClass(QString subject,QString emailText,QObject *o=NULL,QString user="assimakis.notify@gmail.com",QString password="sx35311!",QString fromMail="assimakis.notify@gmail.com",QString fromName="assimakis notify",QStringList toMail=QStringList()<<"assimakis.notify@gmail.com",QStringList toName=QStringList()<<"assimakis notify",QStringList attacments=QStringList());
	~smtpClass();
private:									
};											
#endif
