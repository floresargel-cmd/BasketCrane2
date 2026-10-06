#ifndef _AB_H
#define _AB_H

#include "TuxDef.h"
#include "CIP_Types.h"
#include "Ethernet_IP.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "CIP_IOI.h"
#include "ErrCodes.h"
#include "CM.h"
#pragma pack (1)

typedef enum _Plc_Type{Unknow,PLC5,SLC500,LGX} Plc_Type;
typedef enum _DHP_Channel{Channel_A=0x01,Channel_B} DHP_Channel;
typedef enum _AB_Data_Type{AB_UNKNOW,AB_BIT,AB_SINT,AB_INT,AB_DINT,AB_REAL,AB_TIMER,AB_COUNTER} Data_Type;
typedef enum _PLC_Data_Type{PLC_BIT=1,PLC_BIT_STRING,PLC_BYTE_STRING,PLC_INTEGER,PLC_TIMER,PLC_COUNTER,PLC_CONTROL,PLC_FLOATING,PLC_ARRAY,PLC_ADRESS=15,PLC_BCD} PLC_Data_Type;
typedef enum _LGX_Data_Type{LGX_BOOL=0xC1,LGX_BITARRAY=0xD3,LGX_SINT=0xC2,LGX_INT=0xC3,LGX_DINT=0xC4,LGX_REAL=0xCA} LGX_Data_Type;

#define CNET_Connexion_Parameters 0x4320 //f8
#define DHP_Connexion_Parameters 0x4302

Eip_Connection *_ConnectPLCOverCNET(
		Eip_Session *session,
		Plc_Type Plc,
		BYTE Priority,
		CIP_USINT TimeOut_Ticks,//used to calculate request timeout information
		CIP_UDINT TO_ConnID, //originator's CIP consumed session ID
		CIP_UINT ConnSerialNumber,// session serial number
		CIP_UINT OriginatorVendorID,
		CIP_UDINT OriginatorSerialNumber,
		CIP_USINT TimeOutMultiplier,
		CIP_UDINT RPI,// originator to target packet rate in msec
		CIP_USINT Transport,
		BYTE *path,CIP_USINT pathsize);
#define ConnectPLCOverCNET(session,Plc,TO_ConnID,ConnSerialNumber,RPI,path,pathsize)\
		_ConnectPLCOverCNET(session,Plc,_Priority,_TimeOut_Ticks,TO_ConnID,ConnSerialNumber,\
		_OriginatorVendorID,_OriginatorSerialNumber,_TimeOutMultiplier,RPI,\
		_Transport,path,pathsize)

Eip_Connection *_ConnectPLCOverDHP(
		Eip_Session *session,
		Plc_Type Plc,
		BYTE Priority,
		CIP_USINT TimeOut_Ticks,//used to calculate request timeout information
		CIP_UDINT TO_ConnID, //originator's CIP consumed session ID
		CIP_UINT ConnSerialNumber,// session serial number
		CIP_UINT OriginatorVendorID,
		CIP_UDINT OriginatorSerialNumber,
		CIP_USINT TimeOutMultiplier,
		CIP_UDINT RPI,// originator to target packet rate in msec
		CIP_USINT Transport,
		DHP_Channel channel,
		BYTE *path,CIP_USINT pathsize);
#define ConnectPLCOverDHP(session,Plc,TO_ConnID,ConnSerialNumber,RPI,channel,path,pathsize)\
		_ConnectPLCOverDHP(session,Plc,_Priority,_TimeOut_Ticks,TO_ConnID,ConnSerialNumber,\
		_OriginatorVendorID,_OriginatorSerialNumber,_TimeOutMultiplier,RPI,\
		_Transport,channel,path,pathsize)
#pragma pack ()

#endif /* _AB_H */
