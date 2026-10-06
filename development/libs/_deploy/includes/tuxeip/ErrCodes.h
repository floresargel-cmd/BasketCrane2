#ifndef _ERRCODES_H
#define _ERRCODES_H

#include "tuxG.h"
#include "TuxDef.h"
#define MAX_ERR_MSG_LEN 255
#define MAX_CDE_LEN 20

/********** Internal codes ******************/
#define tuxSuccess 0
#define tuxError -1
#define tuxE_SeeErrorno -2
#define tuxE_PendingBufferFull -3
#define tuxE_NoMoreData -4
#define tuxE_TimeOut -5
#define tuxE_ConnectionFailed -6
#define tuxE_UnsolicitedMsg -7
#define tuxE_NothingToSend -8
#define tuxE_NoReply -9
#define tuxE_ItemUnknow -10

#define tuxE_InvalidReply -20
#define tuxE_OutOfRange -21
#define tuxE_UnsupportedService -22
//#define E_RW	-23
#define tuxE_UnsupportedDataType -24

#define tuxE_InternalMemory -30
#define tuxE_BufferTooSmall -31

#define tuxE_SendTimeOut -40
#define tuxE_RcvTimeOut -41
#define tuxE_SendError -42
#define tuxE_RcvError -43

#define tuxE_MR	-100
#define tuxE_PLC -101
#define tuxE_LGX -102


/******************** AB error codes *********************/

typedef enum _Error_type
{
	Internal_Error,
	Sys_Error,
	EIP_Error,
	MR_Error,
	CM_Error,
	AB_Error,
	PCCC_Error
} Error_type;
typedef enum _LogLevel
{
	LogNone,
	LogError,
	LogTrace,
	LogDebug
} LogLevel;

extern THREAD_VAR int _cip_debuglevel;
extern THREAD_VAR unsigned int _cip_errno;
extern THREAD_VAR unsigned int _cip_ext_errno;
extern THREAD_VAR Error_type _cip_err_type;
extern THREAD_VAR char _cip_err_msg[MAX_ERR_MSG_LEN+1];

void FlushCipBuffer(int level,void *buffer,int size);
void LogCip(int level,char *format,...);

char *CIPGetInternalErrMsg(unsigned int ErrorCode);
char *CIPGetEipErrMsg(unsigned int ErrorCode);
char *CIPGetMRErrMsg(unsigned int ErrorCode,unsigned int Ext_ErrorCode);
char *CIPGetABErrMsg(unsigned int ErrorCode,unsigned int Ext_ErrorCode);
char *CIPGetPCCCErrMsg(unsigned int ErrorCode,unsigned int Ext_ErrorCode);

char *CIPGetErrMsg(int s_err_type,unsigned int s_errno,unsigned int Ext_ErrorCode);
#define CIPERROR(type,no,ext_no) {if ((type!=0)||(no!=0)||(ext_no!=0))tuxLog(QString("error"),QString("type:%1,number:%2,external error number:%3,message:%4").arg(type).arg(no).arg(ext_no).arg(CIPGetErrMsg(type,no,ext_no)));}

#define cip_debuglevel _cip_debuglevel

//#ifndef _WINDLL
//	#define cip_errno _cip_errno
//	#define cip_ext_errno _cip_ext_errno
//	#define cip_err_type _cip_err_type
//	#define cip_err_msg _cip_err_msg
//#else
//	EXPORT unsigned int cip_errno();
//	EXPORT unsigned int cip_ext_errno();
//	EXPORT Error_type cip_err_type();
//	EXPORT char *cip_err_msg();
//#endif 
void tuxipLog(QString type,QString msg);

#endif /* _ERRCODES_H */
