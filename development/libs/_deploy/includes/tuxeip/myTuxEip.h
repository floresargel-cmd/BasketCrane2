#ifndef MYTUXEIP_H
#define MYTUXEIP_H
#include "TuxEip.h"
class tuxipConnectionClass
{
protected:
	Eip_Session					*session;
	Eip_Connection			*connection;
	DHP_Header					dhp;
	bool								emulateMode;
public:
	tuxipConnectionClass();
	~tuxipConnectionClass();
	bool getIsConnected();
	virtual void tuxipWriteInt(char *variable,int *dataPointer,int noOfElements);
	virtual void tuxipReadInt(char *variable,int *dataPointer,int noOfElements);
	virtual void tuxipWriteFloat(char *variable,float *dataPointer,int noOfElements);
	virtual void tuxipReadFloat(char *variable,float *dataPointer,int noOfElements);
	void tuxipCloseCommunications();
};
//////////////////////////////////////////////////////////////
class tuxipLgxConnectionClass:public tuxipConnectionClass
{
public:
	tuxipLgxConnectionClass(int connectionId,char *ip,bool emulateMode=false);
	~tuxipLgxConnectionClass();
	void tuxipWriteInt(char *variable,int *dataPointer,int noOfElements);
	void tuxipReadInt(char *variable,int *dataPointer,int noOfElements);
	void tuxipWriteFloat(char *variable,float *dataPointer,int noOfElements);
	void tuxipReadFloat(char *variable,float *dataPointer,int noOfElements);
};
class tuxipSlcConnectionClass:public tuxipConnectionClass
{
	int					requestUniqCounter;
public:
	tuxipSlcConnectionClass(int connectionId,char *ip,unsigned char *path,int pathLength,DHP_Header dhp,DHP_Channel dhpChanel,bool emulateMode_=false);
	~tuxipSlcConnectionClass();
	void tuxipWriteInt(char *variable,int *dataPointer,int noOfElements);
	void tuxipReadInt(char *variable,int *dataPointer,int noOfElements);
	void tuxipWriteFloat(char *variable,float *dataPointer,int noOfElements);
	void tuxipReadFloat(char *variable,float *dataPointer,int noOfElements);
};
#endif