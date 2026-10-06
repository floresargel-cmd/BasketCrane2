/***************************************************************************
 *  Copyright (C) 2006                                                     *
 *  Author : Stephane JEANNE	stephane.jeanne@gmail.com                  *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   * 
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        * 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#ifndef _SENDDATA_H
#define _SENDDATA_H
#include <QtGui>
#include "TuxDef.h"
#include "Ethernet_IP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "ErrCodes.h"
#ifdef _WIN32
		#include <winsock.h>
		#define MAXFILEDESCRIPTORS FD_SETSIZE
		#include <io.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#define MAXFILEDESCRIPTORS getdtablesize()
#endif


#ifdef _Windows
	int _InitWSA(); //(WORD version);
#endif
	
class sendDataClass
{
public:
	sendDataClass();
	~sendDataClass();
	char *getSocketError(int code);
	static SOCKET cipOpenSock(char *serveur,int port);
	static int cipSendData(int sock,Encap_Header *header);
	static int cipEmptyBuffer(int sock);
	static Encap_Header *cipRecvData(int sock,int timeout);
	static Encap_Header *cipSendData_WaitReply(int sock,Encap_Header *header,int sendtimeout,int rcvtimeout);
	void cipFlushBuffer(void *buffer,int size);
};
#endif /* _SENDDATA_H */
