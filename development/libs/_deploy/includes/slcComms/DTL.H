/* Copyright 1995 Rockwell Software Inc.
 *
 *  NAME
 *	dtl.h
 *
 *  DESCRIPTION
 *	This file is included by `C' programs which call routines in the
 *	Data Table Library.  It defines the DTL error codes, manifest
 *	constants such as DTL_FOREVER, and function prototypes for all the
 *	DTL routines.
 */
#ifndef __DTL
#define __DTL

#pragma pack(push,1)

/*------------------------------ defines -------------------------------*/

#define LIBPTR
#define LIBMEM		__stdcall
#define DTL_CALLBACK	__stdcall
#define DTL_RETVAL		int

#define	DTL_VERSION_ID	-1	/* print DTL version #	*/
#define	DTL_SUCCESS	 0	/* Operation Successful	*/
#define	DTL_PENDING	 1	/* I/O operation in progress */ 
#define	DTL_E_DEFBAD1	 2	/* Invalid DEFINE string */
#define	DTL_E_DEFBAD2	 3	/* Invalid Number of Elements to DEFINE */
#define	DTL_E_DEFBAD3	 4	/* Invalid Data Type */
#define	DTL_E_DEFBAD4	 5	/* Invalid Access Rights */
#define	DTL_E_DEFBAD5	 6	/* Invalid Module, Pushwheel, or Channel */
#define	DTL_E_DEFBAD6	 7	/* Invalid Remote Station Address */
#define	DTL_E_DEFBAD7	 8	/* Invalid PLC Processor Type */
#define	DTL_E_DEFBADN	 9	/* Invalid Number of DEFINE Parameters */ 
#define	DTL_E_DEFCONF	10	/* Conflicts in DEFINE parameter number 5 */
#define	DTL_E_FULL	11	/* DEFINE Table Full */ 
#define	DTL_E_DEFID	12	/* Loading DEFINE Table ID Conflict */ 
#define	DTL_E_DEFNOF	13	/* DEFINE Input File Error */ 
#define DTL_E_INVALID_DTSA_TYPE	14	/* Invalid DTSA atype member */
#define	DTL_E_R_ONLY	15	/* Data Item is Read Only */ 
#define	DTL_E_INVTYPE	16	/* Data is Invalid Type for Operation */ 
#define	DTL_E_NO_MEM	17	/* Not Enough Memory Available */ 
#define	DTL_E_TIME	18	/* I/O operation did not complete in time */ 
#define	DTL_E_NOINIT	19	/* Define Table Not Initialized */ 
#define	DTL_E_BADID	20	/* Define ID out of range */ 
#define DTL_E_NO_BUFFER 21	/* No buffer space available for I/O */
#define	DTL_E_NOSUPPORT	22	/* PLC Processor Type Not Supported */ 
#define	DTL_E_NOS_TMR	23	/* NOS Timer Error */ 
#define	DTL_E_FAIL	24	/* I/O completed with errors */ 
#define	DTL_E_BADPARAM	25	/* Bad parameter Value */ 
#define	DTL_E_NOPARAM	26	/* Expected parameter is missing */ 
#define	DTL_E_NOATMPT	27	/* I/O Operation Not Attempted */ 
#define DTL_E_NOS_MSG	29	/* NOS Message Packet Error */
#define	DTL_E_TOOBIG	31	/* Data Item is > Max Allowed */
#define	DTL_E_NODEF	32	/* No Such Data Item Defined */
#define	DTL_E_BAD_WAITID	33	/* Wait ID out of range */ 
#define	DTL_E_TOOMANYIO	34	/* Too many pending I/O requests */
#define	DTL_E_NOS_OE_INIT	35	/* NOS Initialization error */ 
#define	DTL_E_NOS_OET_INIT	37	/* NOS Initialization error */ 
#define	DTL_E_DFBADADR	38	/* Bad DEFINE Address */ 
#define	DTL_E_NOREINIT	39	/* DTL System already initialized */ 
#define	DTL_E_INPTOOLONG	40	/* Input string too long */ 
#define	DTL_E_CNVT	41	/* Data Conversion Error */ 
#define	DTL_E_GETIME	42	/* PLC-5/250 time invalid */
#define	DTL_E_SETIME	43	/* VMS error setting time */ 
#define	DTL_E_GETSYM	44	/* Error getting symbol expansion */ 
#define	DTL_E_APPBAD	45	/* Bad application address */
#define	DTL_E_BADNIID	46	/* Invalid Network Interface identifier */
#define	DTL_E_NORECONN	47	/* Network Interface already connected */
#define	DTL_E_IPBAD	48	/* Bad IP address */
#define	DTL_E_SYMBAD	49	/* Symbol expansion invalid */ 
#define	DTL_E_INVDEF	50	/* Invalid use of definition */
#define	DTL_E_UDEFBAD2	51	/* Invalid number of elements */ 
#define	DTL_E_UDEFBAD3	52	/* Invalid host data type keyword */ 
#define	DTL_E_UDEFBAD4	53	/* Invalid PLC data type */ 
#define	DTL_E_UDEFBAD5	54	/* Invalid Network Interface identifier */ 
#define	DTL_E_DEFBAD8	56	/* Invalid Network Interface identifier */
#define	DTL_E_NOTCONNECT	57	/* No connection to Network Interface */
#define	DTL_E_RECVPEND	58	/* Receive operation already pending */
#define	DTL_E_READCNVT	59	/* Conversion error for READ data */
#define	DTL_E_WRITECNVT	60	/* Conversion error for WRITE data */
#define	DTL_E_COMPARE	61	/* Data comparison failure */
#define	DTL_E_CANCELED	63	/* Operation was canceled */
#define	DTL_E_NORECV	64	/* RECEIVE operation is not pending */
#define	DTL_SESSION_LOST	65	/* Session to Network Interface was lost */
#define	DTL_SESSION_ESTAB	66	/* Session to Network Interface is established */
#define	DTL_E_SMALLNFDS	67	/* DTL_SET_FDS nfds parameter is too small */
#define	DTL_E_NOT_SUPPORTED	68	/* Operation not supported */
#define DTL_E_BAD_ADDRESS       69	/* Bad DTSA_TYPE Station Address */
#define DTL_E_BAD_CHANNEL       70	/* Bad DTSA_TYPE Communications Channel */
#define DTL_E_BAD_MODULE     	71	/* Bad DTSA_TYPE Module-type */
#define	DTL_E_NOBOTHREJECT	73	/* Cannot specify DTL_REJECT for both handlers */
#define	DTL_E_ADRSUPPORT	74	/* Address specified is not support by DTL function */
#define DTL_E_BAD_PUSHWHEEL     75	/* Bad DTSA_TYPE Pushwheel */
#define DTL_E_DISCONNECT	76	/* Operation cancelled by DTL_DISCONNECT */
#define DTL_E_MAXCONN		77	/* N.I. cannot support more connections */
#define DTL_E_MISMATCH		78	/* N.I. software revision incompatible */
#define DTL_E_DUPADR		79	/* Duplicate application address */
#define DTL_E_NOTOWNER		80	/* Application address in use by other user */
#define DTL_E_UNDEFINED		81	/* I/O was canceled by DTL_UNDEF */
#define DTL_E_NOTAPLC2		82	/* Access mode reserved for PLC-2s */
#define DTL_E_MEMFORMAT		84	/* Archive file format error */
#define DTL_E_DISK		90	/* Error accessing disk */
#define DTL_E_PLCMISMATCH	96	/* PLC types do not match */
#define DTL_E_VRNA_INIT		97	/* Internal error initializing VRNA.386 */
#define DTL_E_NOCOMPARE		101	/* Compare failed */
#define DTL_E_INVALID_MODE	102	/* PLC has invalid mode for attempted operation */
#define DTL_E_PLCFAULTED	103	/* PLC is faulted */

#define DTL_E_NOCMP		104	/* Compare Utility compare failed */
#define	DTL_I_CMP		105	/* Compare Utility compare success */
#define DTL_E_FAULTS		106	/* Get Faults Utility - found faults */
#define DTL_I_NOFAULTS		107	/* Get Faults Utility - found no faults */
#define DTL_I_RUN		108	/* Sense Mode Utility - run mode */
#define DTL_I_RRUN		109	/* Sense Mode Utility - remote run mode */
#define DTL_I_TEST		110	/* Sense Mode Utility - test mode */
#define DTL_I_RTEST		111	/* Sense Mode Utility - remote test mode */
#define DTL_I_PROGRAM		112	/* Sense Mode Utility - program  mode */
#define DTL_I_RPROGRAM		113	/* Sense Mode Utility - remote program mode */
#define	DTL_E_WM_QUIT		114	/* DLL detected WM_QUIT message */
#define	DTL_E_NOCOPY		115 	/* can't copy PLC image */
#define	DTL_E_COPYWARN		116 	/* can copy image, but with warnings */
#define DTL_E_MODNTCHG		117	/* PLC mode not changed to requested mode  */
#define DTL_E_BAD_DTSA_TYPE	118	/* Illegal DTSA_TYPE Address Type */
#define DTL_E_BAD_FILENAME	119	/* DTSA structure file name is NULL or zero */
#define DTL_E_BAD_FILELENGTH	120	/* DTSA_TYPE File Length is Zero */
#define DTL_E_BAD_FILETYPE	121	/* Invalid DTSA_TYPE File Type */
#define	DTL_E_NO_SERVER			122	/* Server is not loaded */
#define	DTL_E_SERVER_NOT_RUNNING	123	/* Server is not running */
#define DTL_E_BUFFER_TOO_SMALL	124	/* Reply buffer is too small */
#define DTL_E_BAD_MASK		125	/* Bit mask contains illegal bits */
#define DTL_E_NO_HANDLER	126	/* PCCC packet handler is NULL */
#define DTL_E_BAD_OPT		127	/* Invalid option parameter  */
#define DTL_E_BAD_BACKLOG	128	/* Invalid backlog value  */
#define DTL_E_NO_PROGRAM	129	/* Can't find subprogram to execute */
#define DTL_E_BAD_OPTNAME	130	/* Invalid option name parameter  */
#define DTL_E_BAD_OPTVAL	131	/* Invalid option value parameter  */
#define DTL_E_STOPPED		132	/* U/D/C operation terminated by user */
#define DTL_E_GETALL_ACTIVE     133     /* DTL_UNSOL_GETALL already active */
#define DTL_E_UDEFCONF          134     /* DTL_UNSOL_GETALL and DTL_UNSOL_DEF used simulta
neously with same NI */
#define DTL_I_TCSN		135	/* Sense Mode Utility - test cont scan mode  */
#define DTL_I_TSSN		136	/* Sense Mode Utility - test sing scan mode  */
#define DTL_I_TSRG		137	/* Sense Mode Utility - test sing step mode  */
#define DTL_E_BAD_ASA_PATH	138	/* Uninterpretable path in DTSA_ASA */
#define DTL_E_BAD_CID		139	/* Invalid connection ID in DTSA_CONN */
#define DTL_E_BAD_SVC_CODE	140	/* Disallowed ASA service code */
#define DTL_E_BAD_IOI		141	/* Invalid ASA Internal Object Identifier */
#define DTL_E_MAX_SIZE		142	/* Data exceeds maximum size allowed */
#define DTL_E_MAX_ASA_CONN	143	/* No more ASA connections can be opened */
#define DTL_E_CONN_BUSY		144	/* Connection not ready to send */
#define DTL_E_CONN_LOST		145	/* Connection lost */
#define DTL_E_CTYPE		146	/* Invalid connection structure */
#define DTL_E_ASA_MODE		147	/* Invalid ASA mode */
#define DTL_E_ASA_TRIGGER	148	/* Invalid ASA trigger */
#define DTL_E_ASA_TRANSPORT	149	/* Invalid ASA transport */
#define DTL_E_ASA_TMO_MULT	150	/* Invalid ASA timeout multiplier */
#define DTL_E_ASA_CONN_TYPE	151	/* Invalid ASA network connection type */
#define DTL_E_ASA_CONN_PRI	152	/* Invalid ASA connection priority */
#define DTL_E_ASA_PKT_TYPE	153	/* Invalid ASA connection packet type */
#define DTL_E_ASA_PKT_SIZE	154	/* Invalid ASA connection max packet size */
#define DTL_E_DRIVER_ID_ILLEGAL	155 /* Driver ID was illegal. */
#define DTL_E_DRIVER_ID_INVALID	156 /* Driver ID was invalid. */
#define DTL_E_DRIVER_ID_INUSE	157 /* Driver ID is already in use. */
#define DTL_E_DRIVER_NAME_INVALID		158 /* Driver name is invalid. */
#define DTL_E_BROADCAST		159	/* Failed attempt to register/unregister broadcast unsolicited request. */
#define	DTL_E_PLC2MEMORY	160	/* Failed attempt to register/unregister plc2 memory unsolicited request. */
#define	DTL_E_VIRTUAL_LINK	161	/* Failed attempt to register/unregister virtual link unsolicited request. */
#define DTL_E_ADR_NOT_IN_USE	162	/* PLC-2 address not in use by this application. */
#define DTL_E_NODE_NOT_IN_USE	163	/* Virtual link node not in use by this application. */
#define	DTL_E_DEF_PW_REPEAT	164	/* DTL_C_DEFINE pushwheel parameter was specified more than once. */
#define	DTL_E_DEF_PW_RANGE	165	/* DTL_C_DEFINE pushwheel parameter was out of range. */
#define	DTL_E_DEF_MOD_REPEAT	166	/* DTL_C_DEFINE module parameter was specified more than once. */
#define	DTL_E_DEF_MOD_RANGE	167	/* DTL_C_DEFINE module parameter was out of range. */
#define	DTL_E_DEF_CH_REPEAT	168	/* DTL_C_DEFINE channel parameter was specified more than once. */
#define	DTL_E_DEF_CH_RANGE	169	/* DTL_C_DEFINE channel parameter was out of range. */
#define	DTL_E_DEF_EISTN_REPEAT	170	/* DTL_C_DEFINE EI station parameter was specified more than once. */
#define	DTL_E_DEF_EISTN_RANGE	171	/* DTL_C_DEFINE EI station parameter was out of range. */
#define	DTL_E_DEF_BRIDGE_REPEAT	172	/* DTL_C_DEFINE bridge parameter was specified more than once. */
#define	DTL_E_DEF_BRIDGE_RANGE	173	/* DTL_C_DEFINE bridge parameter was out of range. */
#define	DTL_E_DEF_LINK_REPEAT	174	/* DTL_C_DEFINE link parameter was specified more than once. */
#define	DTL_E_DEF_LINK_RANGE	175	/* DTL_C_DEFINE link parameter was out of range. */
#define	DTL_E_DEF_GW_REPEAT	176	/* DTL_C_DEFINE gateway parameter was specified more than once. */
#define	DTL_E_DEF_GW_RANGE	177	/* DTL_C_DEFINE gateway parameter was out of range. */
#define	DTL_E_DEF_KA_REPEAT	178	/* DTL_C_DEFINE ka flag parameter was specified more than once. */
#define	DTL_E_NO_RSLINX_INI			179	/* Can not find RSLinx in ini file */
#define	DTL_E_NO_WINLINX_INI		180 /* Can not find WinLinx in ini file */
#define	DTL_E_SENDING_TO_SERVER		181 /* Error sending message to server */
#define	DTL_E_NO_NAME_MAPPING		182 /* Specified hostname is not mapped to a station address. */
#define	DTL_E_CANT_CREATE_RSLINX	183	/* Unable to create RSLinx process */
#define	DTL_E_CANT_FIND_RSLINX		184	/* Unable to communicate with RSLinx process */
#define	DTL_E_MISSING_RSLINX_ACTIVATION	185	/* Unable to find activation key */
#define	DTL_E_MISSING_ACTIVATION	DTL_E_MISSING_RSLINX_ACTIVATION
#define DTL_E_NULL_POINTER			186 /* One or more pointers were NULL */
#define	DTL_E_INVALID_WHOACTIVE_TYPE	187 /* Who active struct type is invalid. */
#define	DTL_E_ILLEGAL_WHOACTIVE_TYPE	188 /* Who active struct type is illegal. */
#define DTL_E_BAD_WHOACTIVE_SIZE		189	/* Who active struct size is wrong for struct type. */
#define DTL_E_INVALID_WHOACTIVE_MFG		190	/* Who active manufacturer type is invalid. */
#define DTL_E_ILLEGAL_WHOACTIVE_MFG		191	/* Who active manufacturer type is illegal. */
#define	DTL_E_BAD_REQUESTID			192	/* Specified request id was bad. */
#define	DTL_E_CANT_CREATE_WINLINX		193	/* Unable to create WinLinx process. */
#define	DTL_E_CANT_FIND_WINLINX			194	/* Unable to communicate with WinLinx process. */
#define	DTL_E_MISSING_WINLINX_ACTIVATION	195	/* Unable to find activation key */
#define DTL_E_NO_ASA_SUPPORT	196	/* RSLinx ASA subsystem not found */
#define DTL_E_ASA_REG_TIMEOUT	197	/* No response to ASA registration */
#define DTL_E_ASA_REG_REJECT	198	/* Rejection of ASA registration */
#define DTL_E_ASA_BAD_REG_ID	199	/* Invalid registration ID */
#define DTL_E_MAX_ASA_SYMBOL	200	/* ASA Symbol exceeds max length allowed */
#define DTL_E_ASA_SYMBOL_FMT	201	/* ASA Symbol has invalid format */
#define DTL_E_ASA_BAD_TRANS_ID	202	/* Invalid transaction ID */
#define DTL_E_DDE_TOPIC_EXISTS	203	/* DDE Topic already exists. */
#define DTL_E_SOURCE			204	/* Failed attempt to register/unregister source unsolicited request. */
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_NAME			205
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_POLL_MODE		206
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_POLL_RATE		207
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_MFG_TYPE		208
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_PROC_TYPE		209
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_PROC_STA		210
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_ROUTE			211
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_DRIVER		212
#define	DTL_E_DDE_TOPIC_EXISTS_WITH_DIFFERENT_COMM_TIMEOUT	213
#define	DTL_E_WINLINX_VERSION_MISMATCH	214	/* Version does not match WinLinx version. */
#define	DTL_E_RSLINX_VERSION_MISMATCH	215	/* Version does not match RSLinx version. */
#define	DTL_E_WINLINX_CONNECTION_BROKEN	216	/* The connection to WinLinx server is broken. */
#define	DTL_E_RSLINX_CONNECTION_BROKEN	217	/* The connection to RSLinx server is broken. */
#define DTL_E_DUP_ASA_ADDRESS	218	/* ASA link address was already registered. */
#define DTL_E_DUP_ASA_SYMBOL	219	/* ASA symbol was already registered. */
#define DTL_E_ASA_LINK_ADDRESS	220	/* Invalid link address */
#define DTL_E_ASA_STATUS_CODE	221	/* Invalid return status code */
#define DTL_E_NO_IC_MAPPINGS	222	/* No INTERCHANGE compatibility mappings exist. */
#define	DTL_E_DDE_TOPIC_IS_ACTIVE	223
#define	DTL_E_DDE_TOPIC_NO_PROJECT	224
#define	DTL_E_RSHARMONY_INITIALIZE		225	/* Could not initialize RSHarmony sub-system. */
#define	DTL_E_NOT_SUPPORTED_SERVER		226	/* Operation not supported by server. */
#define	DTL_E_RSHARMONY_OPEN_PROJECT	227	/* Could not open RSHarmony project. */
#define	DTL_E_RSHARMONY_BIND_OBJECT		228	/* Could not bind to RSHarmony object. */
#define	DTL_E_RSHARMONY_INVOKE_METHOD	229	/* Could not invoke method on RSHarmony object. */
#define	DTL_E_RSHARMONY_GET_PROPERTY	230	/* Could not get property of RSHarmony object. */
#define	DTL_E_RSHARMONY_SET_PROPERTY	231	/* Could not set property of RSHarmony object. */
#define	DTL_E_IC_MAPPING_EXISTS			232 /* INTERCHANGE mapping exists for specified pushwheel. */

#define	DTL_E_INVALID_TARGET_TYPE	233 /* Target struct type is invalid. */
#define	DTL_E_ILLEGAL_TARGET_TYPE	234 /* Target struct type is illegal. */
#define DTL_E_INVALID_TARGET_SIZE	235	/* Target struct size is wrong for struct type. */

#define	DTL_E_DLL_SERVER_MISMATCH	236	/* DTL32.DLL version mismatch with Linx server. */

#define	PCCCSTSBASE	0x100	/* pccc sts base offset	*/
#define PCCCSTS01	PCCCSTSBASE+1	/* Station cannot buffer command */
#define	PCCCSTS02	PCCCSTSBASE+2	/* Cannot guarantee delivery, link layer timed out or received a NAK. */
#define	PCCCSTS03	PCCCSTSBASE+3	/* Duplicate token holder detected by link layer. */
#define	PCCCSTS04	PCCCSTSBASE+4	/* Local port is disconnected. */
#define	PCCCSTS05	PCCCSTSBASE+5	/* Application layer timed out waiting for a response. */
#define	PCCCSTS06	PCCCSTSBASE+6	/* Duplicate node detected. */
#define	PCCCSTS07	PCCCSTSBASE+7	/* Station is off-line. */
#define	PCCCSTS08	PCCCSTSBASE+8	/* Hardware fault. */
#define	PCCCSTS10	PCCCSTSBASE+0x10	/* Illegal command or format, including an odd address. */
#define	PCCCSTS20	PCCCSTSBASE+0x20	/* Host has a problem and will not communicate. */
#define	PCCCSTS30	PCCCSTSBASE+0x30	/* Remote station host is not there, disconnected, or shutdown. */
#define	PCCCSTS40	PCCCSTSBASE+0x40	/* Host could not complete function due to hardware fault. */
#define	PCCCSTS50	PCCCSTSBASE+0x50	/* Addressing problem or memory protect rungs. */
#define	PCCCSTS60	PCCCSTSBASE+0x60	/* Function disallowed due to command protection selection. */
#define	PCCCSTS70	PCCCSTSBASE+0x70	/* Processor is in program mode. */
#define	PCCCSTS80	PCCCSTSBASE+0x80	/* Compatibility mode file missing or communication zone */
#define	PCCCSTS90	PCCCSTSBASE+0x90	/* Remote station cannot buffer command. */
#define PCCCSTSA0       PCCCSTSBASE+0xA0        /* No ACK received */
#define PCCCSTSA2       PCCCSTSBASE+0xA2        /* Network is dead */
#define PCCCSTSA4       PCCCSTSBASE+0xA4        /* COM port hardware problem */
#define PCCCSTSA5       PCCCSTSBASE+0xA5        /* Packet is too large */
#define PCCCSTSA7       PCCCSTSBASE+0xA7        /* Illegal station address seen */
#define PCCCSTSA8       PCCCSTSBASE+0xA8        /* Not getting solicited */
#define PCCCSTSA9       PCCCSTSBASE+0xA9        /* Service/LSAP not supported */

#define	PCCCSTSB0	PCCCSTSBASE+0xB0	/* Remote station problem, due to download. */
#define	PCCCSTSC0	PCCCSTSBASE+0xC0	/* Cannot execute command, due to IBP's. */

#define PCCCEXTBASE	0x200	/* pccc Extended sts base  */
#define	PCCCEXT01	PCCCEXTBASE+1	/* Illegal Address Format; a field has an illegal value. */
#define	PCCCEXT02	PCCCEXTBASE+2	/* Illegal Address Format; not enough fields specified. */
#define	PCCCEXT03	PCCCEXTBASE+3	/* Illegal Address Format; too many fields specified. */
#define	PCCCEXT04	PCCCEXTBASE+4	/* Illegal Address; symbol not found. */
#define	PCCCEXT05	PCCCEXTBASE+5	/* Illegal Address Format; symbol is 0 or greater than 8 characters. */
#define	PCCCEXT06	PCCCEXTBASE+6	/* Illegal Address; address does not exist. */
#define	PCCCEXT07	PCCCEXTBASE+7	/* Illegal size. */
#define	PCCCEXT08	PCCCEXTBASE+8	/* Cannot complete request, situation changed during multipacket operation. */
#define	PCCCEXT09	PCCCEXTBASE+9	/* Data is too large. */
#define	PCCCEXT0A	PCCCEXTBASE+0x0a	/* Size too big */
#define	PCCCEXT0B	PCCCEXTBASE+0x0b	/* No access, privilege violation. */
#define	PCCCEXT0C	PCCCEXTBASE+0x0c	/* Resource is not available */
#define	PCCCEXT0D	PCCCEXTBASE+0x0d	/* Resource is already available. */
#define	PCCCEXT0E	PCCCEXTBASE+0x0e	/* Command cannot be executed. */
#define	PCCCEXT0F	PCCCEXTBASE+0x0f	/* Overflow; histogram overflow. */
#define	PCCCEXT10	PCCCEXTBASE+0x10	/* No access. */
#define	PCCCEXT11	PCCCEXTBASE+0x11	/* Incorrect Type data */
#define	PCCCEXT12	PCCCEXTBASE+0x12	/* Bad Parameter */
#define	PCCCEXT13	PCCCEXTBASE+0x13	/* Address reference exists to deleted area */
#define	PCCCEXT14	PCCCEXTBASE+0x14	/* Command Execution failure for unknown reason */
#define	PCCCEXT15	PCCCEXTBASE+0x15	/* Data conversion error */
#define	PCCCEXT16	PCCCEXTBASE+0x16	/* 1771 rack adapter not responding */
#define	PCCCEXT17	PCCCEXTBASE+0x17	/* Timed out, 1771 backplane module not responding */
#define	PCCCEXT18	PCCCEXTBASE+0x18	/* 1771 module response was not valid: size, checksum,etc */
#define	PCCCEXT19	PCCCEXTBASE+0x19	/* Duplicated label */
#define	PCCCEXT1A	PCCCEXTBASE+0x1a	/* File is open - another station owns it */
#define	PCCCEXT1B	PCCCEXTBASE+0x1b	/* Another station is the program owner */

#define ASAERRBASE	0x400	/* ASA error base offset with which ASA General Status is OR'ed */
#define ASAEXTTRUNC	0x100	/* Flag OR'ed with ASAERRBASE to indicate truncated extended status */
#define ASADATTRUNC	0x200	/* Flag OR'ed with ASAERRBASE to indicate truncated data */

#define	DTL_NOS_ERROR_BASE	0x10000	/* Allen-Bradley NOS error base.*/
#define	DTL_UNIX_ERROR_BASE	0x20000	/* UNIX(TM) error base.		*/
#define	DTL_VMS_ERROR_BASE	0x30000	/* VMS(R) error base.		*/
#define DTL_WIN16_ERROR_BASE	0x40000	/* Win16 error base.		*/
#define DTL_WIN32_ERROR_BASE	0x50000	/* Win32 error base.		*/


/* Wait timeout defs	*/
#define 	DTL_FOREVER    0x7fffffff	/* Don't timeout	*/


/* Maximum wait identifier number  */
#define		DTL_MAX_WAIT_ID		40


/*---- solicited and unsolicited host data type codes ----*/
#define 	DTL_TYP_WORD		0	/* signed word 		*/
#define 	DTL_TYP_UWORD		1	/* unsigned word	*/
#define 	DTL_TYP_LONG		2	/* signed long 		*/
#define 	DTL_TYP_FLOAT		3	/* single precision float */
#define 	DTL_TYP_RAW		4	/* no conversion	*/
#define 	DTL_TYP_BCD		5	/* BCD 			*/
#define 	DTL_TYP_BIT		6	/* bit field operation	*/
#define 	DTL_TYP_STRUCT		7	/* structure		*/
#define 	DTL_TYP_FLT_IEEE	8	/* IEEE float		*/
#define 	DTL_TYP_FLT_DEC		9	/* DEC float		*/
#define 	DTL_TYP_IO		10	/* block I/O type	*/
#define 	DTL_TYP_PLC3_LONG	11	/* PLC-3 long		*/
#define 	DTL_TYP_SYMBOL		12	/* symbol translation	*/
#define		DTL_TYP_BYT		13	/* plc-5/250 byte	*/
#define		DTL_TYP_FLT_SLC500	14	/* slc500 IEEE float    */
#define		DTL_TYP_RESERVED1	14	/* reserved for copro.	*/
#define		DTL_TYP_RESERVED2	15	/* reserved for copro.	*/
#define		DTL_TYP_RESERVED3	16	/* reserved for copro.	*/
#define		DTL_TYP_DIAG_STATUS	100	/* diag host status	*/
#define		DTL_TYP_DIAG_COUNTERS	101	/* diag counters read	*/
#define		DTL_TYP_DIAG_RESET	102	/* diag counters reset	*/
#define		DTL_TYP_DIAG_ECHO	103	/* diag echo		*/
#define		DTL_TYP_BYTE		200	/* byte			*/
#define 	DTL_TYP_TIMER   	201	/* timer		*/
#define 	DTL_TYP_COUNTER 	202	/* counter		*/
#define 	DTL_TYP_CONTROL     	203	/* control		*/
#define 	DTL_TYP_PID     	204	/* pid			*/
#define 	DTL_TYP_MSG     	205	/* message		*/
#define		DTL_TYP_STRING		206	/* string		*/
#define		DTL_TYP_PLC3_IO		207	/* plc-3 i/o section	*/
#define		DTL_TYP_PLC3_POINTER	208	/* plc-3 ptr section	*/
#define		DTL_TYP_PLC3_STATUS	209	/* plc-3 status section	*/
#define		DTL_TYP_STATUS		210	/* generic status section*/
#define		DTL_TYP_TOKENDATA	211	/* global status section*/
#define		DTL_TYP_SFCSTATUS	212	/* sfc status data section*/
#define		DTL_TYP_BLOCKTRANS	213	/* block transfer data section*/
#define		DTL_TYP_CN			214 /* control net 'ct'. */
#define		DTL_TYP_DEVCONF		254	/* internal		*/

/*---- data type sizes ----*/
#define 	DTL_TYP_WORD_SIZ	2	/* signed word 		*/
#define 	DTL_TYP_UWORD_SIZ	2	/* unsigned word	*/
#define 	DTL_TYP_LONG_SIZ	4	/* signed long 		*/
#define 	DTL_TYP_FLOAT_SIZ	4	/* single precision float */
#define 	DTL_TYP_RAW_SIZ		2	/* raw size, to start	*/
#define		DTL_TYP_BYT_SIZ		1	/* signed char		*/

/*---- unsolicited plc data type codes ----*/
#define 	DTL_PLC_WORD		DTL_TYP_WORD
#define 	DTL_PLC_UWORD		DTL_TYP_UWORD
#define 	DTL_PLC_LONG		DTL_TYP_LONG
#define 	DTL_PLC_IEEE_FLOAT	DTL_TYP_FLT_IEEE
#define 	DTL_PLC_PLC3_LONG	DTL_TYP_PLC3_LONG
#define 	DTL_PLC_PLC3_FLOAT	DTL_TYP_FLT_DEC
#define 	DTL_PLC_DECIMAL		DTL_TYP_BCD
#define 	DTL_PLC_RAW		DTL_TYP_RAW
#define 	DTL_PLC_SLC500_FLOAT	DTL_TYP_FLT_SLC500

/*---- plc data type sizes ----*/
#define 	DTL_PLC_WORD_SIZ	DTL_TYP_WORD_SIZ
#define 	DTL_PLC_UWORD_SIZ	DTL_TYP_UWORD_SIZ
#define 	DTL_PLC_LONG_SIZ	DTL_TYP_LONG_SIZ
#define 	DTL_PLC_IEEE_FLOAT_SIZ	4
#define 	DTL_PLC_PLC3_LONG_SIZ	4
#define 	DTL_PLC_PLC3_FLOAT_SIZ	4
#define 	DTL_PLC_DECIMAL_SIZ	2
#define 	DTL_PLC_RAW_SIZ		2
#define		DTL_PLC_SLC500_FLOAT_SIZ 4

/*---- plc command protocols ----*/
#define		DTL_PROTO_NONE		0
#define		DTL_PROTO_PCCC		1
#define		DTL_PROTO_ASA		2

/*---- module id bit masks ----*/
#define 	MODID_MOD_TYPE	   0x000000FF	/* module type		*/
#define 	MODID_MEM_SIZE	   0x0000FF00	/* module memory size	*/
#define 	MODID_FLAGS	   0x00FF0000	/* module flags		*/
#define 	MODID_STATUS	   0xFF000000	/* module status	*/
#define 	MODID_MEM_SIZE_OS  8	        /* size bit offset	*/
#define 	MODID_ID_LONG	   0x00800000   /* ID Prom arrangement  */
#define 	MODID_POWERUP	   0x00200000	/* PWR UP Participation */
#define 	MODID_SMART	   0x00100000	/* Intelligent module	*/
#define 	MODID_32BACK	   0x00080000	/* 32bit backplane	*/
#define 	MODID_SYSCLASS	   0x00040000	/* system class		*/
#define 	MODID_COMCLASS	   0x00020000	/* communication class	*/
#define 	MODID_PROCLASS	   0x00010000	/* Processor class	*/

#define 	MODID_EXCLUDED	   0x01000000	/* excluded @power-up	*/
#define 	MODID_PWRUPERR	   0x02000000	/* power-up problem	*/
#define 	MODID_MEMBAD	   0x04000000	/* module memory invalid*/
#define 	MODID_REQUIRED	   0x08000000	/* Critical module	*/

/* PLC image types */
#define	DTL_ACH_X5	1	/* X5 archive file */
#define	DTL_ACH_AF5	2	/* AF5 archive file */
#define	DTL_ACH_PLC	3	/* PLC */
#define	DTL_ACH_APS	4	/* APS archive file */

/* PLC image access modes */
#define	DTL_PI_READ	1	/* read only */
#define	DTL_PI_WRITE	2	/* write only */

/*---- address types (suitable for DTSA_TYPE) -------------*/
#define	DTSA_TYP_DH	2	/* data highway / data highway plus */
#define	DTSA_TYP_BKPLN	3	/* local backplane access */
#define	DTSA_TYP_FILE	4	/* archive file (default type) access */
#define	DTSA_TYP_DH_R	5	/* remote DH/DH+ */
#define	DTSA_TYP_ARCH	6	/* archive file (specific type) access */
#define	DTSA_TYP_ASA	7	/* ASA unconnected access */
#define	DTSA_TYP_CONN	8	/* general connected access */
#define	DTSA_TYP_AB_DH_LOCAL		100	/* AB DH local			*/
#define	DTSA_TYP_AB_DH_OFFLINK_OBSOLETE	101	/* AB DH offlink Obsolete	*/
#define	DTSA_TYP_AB_PIGATEWAY_OBSOLETE	102	/* AB PI Gateway Obsolete	*/
#define	DTSA_TYP_AB_DF1MASTER		103	/* AB DF1 Master		*/
#define	DTSA_TYP_AB_DH_OFFLINK		104	/* AB DH offlink		*/
#define	DTSA_TYP_AB_PIGATEWAY		105	/* AB PI Gateway		*/
#define	DTSA_TYP_AB_DH_LONG_LOCAL	106	/* AB DH local 4-byte addr	*/
#define	DTSA_TYP_AB_NAME			107	/* AB ASCII			*/
#define	DTSA_TYP_AB_PIGATEWAY_IP	108	/* AB PI Gateway IP addr	*/
#define	DTSA_TYP_AB_PIGATEWAY_NAME	109	/* AB PI Gateway hostname	*/
#define	DTSA_TYP_AB_ASA_PATH		110	/* AB Path			*/
#define	DTSA_TYP_AB_ASA_CONN		111	/* AB Connection		*/
#define	DTSA_TYP_AB_DH_ASA_PATH		112	/* AB Path to DH+ Interface	*/
#define	DTSA_TYP_AB_DH_ASA_CONN		113	/* AB Connection to DH+ Interface */

/*---- communications channels (suitable for DTSA_TYPE) ----*/
#define	DTL_CHANNEL_MASK	0x0f

#define DTL_CHAN_2_PRI  0x02
#define DTL_CHAN_2_ALT  0x12
#define DTL_CHAN_2      DTL_CHAN_2_PRI

#define DTL_CHAN_3_PRI  0x03
#define DTL_CHAN_3_ALT  0x13
#define DTL_CHAN_3      DTL_CHAN_3_PRI

/*---- Supported communications modules (suitable for DTSA_TYPE) ----*/
#define	DTL_MODULE_RM		0x09
#define	DTL_MODULE_KA		0x40
#define	DTL_MODULE_UAMS		0x60
#define	DTL_MODULE_UAME		0x61
#define	DTL_MODULE_KT		0xF8

#define	DTL_ROUTE_AB_NAME			DTSA_TYP_AB_NAME
#define	DTL_ROUTE_AB_DH_LOCAL		DTSA_TYP_AB_DH_LOCAL
#define	DTL_ROUTE_AB_DH_LONG_LOCAL	DTSA_TYP_AB_DH_LONG_LOCAL
#define	DTL_ROUTE_AB_DH_OFFLINK		DTSA_TYP_AB_DH_OFFLINK
#define	DTL_ROUTE_AB_PIGATEWAY		DTSA_TYP_AB_PIGATEWAY
#define	DTL_ROUTE_AB_PIGATEWAY_IP	DTSA_TYP_AB_PIGATEWAY_IP
#define	DTL_ROUTE_AB_PIGATEWAY_NAME	DTSA_TYP_AB_PIGATEWAY_NAME
#define	DTL_ROUTE_AB_DF1MASTER		DTSA_TYP_AB_DF1MASTER
#define	DTL_ROUTE_AB_ASA_PATH		DTSA_TYP_AB_ASA_PATH
#define	DTL_ROUTE_AB_ASA_CONN		DTSA_TYP_AB_ASA_CONN
#define	DTL_ROUTE_AB_DH_ASA_PATH	DTSA_TYP_AB_DH_ASA_PATH
#define	DTL_ROUTE_AB_DH_ASA_CONN	DTSA_TYP_AB_DH_ASA_CONN

/*---- Supported manufacturer types (suitable for DTSA_TYPE) ----*/
#define DTL_MANUFACTURER_AB	0xAB

/*---- Defined connection status events ----*/
#define	DTL_CONN_ESTABLISHED	1	/* connection successfully opened */
#define	DTL_CONN_ERROR		2	/* error establishing connection */
#define	DTL_CONN_FAILED		3	/* connection failed */
#define	DTL_CONN_TIMEOUT	4	/* connection timed out */
#define	DTL_CONN_CLOSED		5	/* connection successfully closed */
#define	DTL_CONN_PKT_DUP	6	/* duplicate packet received */
#define	DTL_CONN_PKT_LOST	7	/* packet lost */
#define	DTL_CONN_ACK		8	/* ACK received */
#define	DTL_CONN_NAK_GENERAL	9	/* NAK - unspecified type received */
#define	DTL_CONN_NAK_BAD_CMD	10	/* NAK - "Bad Command" received */
#define	DTL_CONN_NAK_SEQ_ERR	11	/* NAK - "Sequence Error" received */
#define	DTL_CONN_NAK_NO_MEM	12	/* NAK - "Not Enough Memory" received */

/*---- Supported connection types ----*/
#define	DTL_CONN_ASA		0x100	/* basic ASA connection */

/*---- ASA connection parameters ----*/
#define	DTL_ASA_CLIENT		0	/* connection target is client */
#define	DTL_ASA_SERVER		1	/* connection target is server */

#define	DTL_ASA_MODE_IS_SERVER	  0x00	/* RSLinx node is connection server */
#define	DTL_ASA_MODE_IS_CLIENT	  0x01	/* RSLinx node is connection client */

#define	DTL_ASA_MODE_FILTER_DUPS  0x00	/* Filter out duplicate packets */
#define	DTL_ASA_MODE_FILTER_NONE  0x02	/* Don't filter out duplicate packets */
#define	DTL_ASA_MODE_FILTER_SAME  0x04	/* Filter out unchanged packets */

#define	DTL_ASA_MODE_NOTIFY_NAKS  0x08	/* notify app about NAKs received */
#define	DTL_ASA_MODE_NOTIFY_ACKS  0x10	/* notify app about ACKs received */
#define	DTL_ASA_MODE_NOTIFY_DUPS  0x20  /* notify app about duplicates received */
#define	DTL_ASA_MODE_NOTIFY_LOST  0x40  /* notify app about lost packets */

#define	DTL_ASA_TRIG_CYC	0	/* cyclic trigger */
#define	DTL_ASA_TRIG_COS	1	/* change-of-state trigger */
#define	DTL_ASA_TRIG_APP	2	/* application trigger */

#define	DTL_ASA_MULTI		1	/* multicast connection */
#define	DTL_ASA_PT_PT		2	/* point-to-point connection */

#define	DTL_ASA_PRI_LOW		0	/* low-priority connection */
#define	DTL_ASA_PRI_HIGH	1	/* high-priority connection */
#define	DTL_ASA_PRI_SCHED	2	/* scheduled-priority connection */

#define	DTL_ASA_PKT_FIXED	0	/* fixed-size packets */
#define	DTL_ASA_PKT_VARIABLE	1	/* variable-size packets */

#define DTL_ASA_MAX_PKT_SIZE	502     /* maximum ASA message size */
#define	DTL_ASA_MAX_UNCONN_SIZE	502	/* maximum ASA unconnected packet size */
#define	DTL_ASA_MAX_CONN_SIZE	504	/* maximum ASA connected packet size */

/*---- ASA unconnected parameters ----*/
#define	DTL_ASA_PRIORITY	1	/* high-priority message */

/*---- ASA symbol segment formats ----*/
#define DTL_ASA_SYMBOL_ASCII	0x00	/* ASCII symbol */
#define DTL_ASA_SYMBOL_2BYTE	0x20	/* Double-byte symbol */
#define DTL_ASA_SYMBOL_3BYTE	0x40	/* Triple-byte symbol */
#define DTL_ASA_SYMBOL_UNICODE	0x80	/* Unicode symbol */

/*---- ASA RSI-specific class codes ----*/
#define DTL_ASA_CLASS_RSI_SW_REG 0x77	/* RSI Software Registration Object */

//---- Valid options for DTL_SETOPT
//	DTL_OPT_BACKLOG		Change the number of unsolicited messages that the driver will hold on behalf of the application
//				before the driver will begin to reject further packets. INTERCHANGE only.
//	DTL_OPT_PEEK_MESSAGE	Determine whether the DLL should call PeekMessage() while blocked. INTERCHANGE only.
//	DTL_OPT_ENET_UNSOL_DTSA	Select the DTSA type the the DLL should return to the application for unsolicited requests coming
//				from Ethernet targets and Ethernet gateways. RSLinx only.
//	DTL_OPT_MULTI_SYNC_IO	Determine whether the DLL should permit simultaneous synchronous operations. RSLinx only.
//	DTL_OPT_RETRY_NAK_RD	Determine whether the DLL should retry NAKed DTL_READ packets.
//	DTL_OPT_RETRY_NAK_WR	Determine whether the DLL should retry NAKed DTL_WRITE packets.
//	DTL_OPT_RETRY_NAK_RMW	Determine whether the DLL should retry NAKed DTL_RMW packets.
#define	DTL_OPT_BACKLOG		(1)
#define	DTL_OPT_PEEK_MESSAGE	(2)
#define	DTL_OPT_ENET_UNSOL_DTSA	(3)
//	These are the valid optname values for the DTL_OPT_ENET_UNSOL_DTSA.
//	See the help macros, DTL_SET_ENET_xxxx_UNSOL_DTSA_TYPE() later in this header for more information.
#	define	DTL_OPTNAME_ENET_UNSOL_DTSA_TARGET	(1)
#	define	DTL_OPTNAME_ENET_UNSOL_DTSA_GATEWAY	(2)
#define	DTL_OPT_MULTI_SYNC_IO	(4)
#define	DTL_OPT_RETRY_NAK_RD	(5)
#define	DTL_OPT_RETRY_NAK_WR	(6)
#define	DTL_OPT_RETRY_NAK_RMW	(7)

/*---- Access Machine states ----*/
#define DTL_NOCONNECT           0

/*---- DTL_UNSOL_DEF constants for DTL_REJECT and DTL_ACCEPT ----*/
#define	DTL_ACCEPT	((unsigned long (DTL_CALLBACK *)())1)
#define	DTL_REJECT	((unsigned long (DTL_CALLBACK *)())2)

/*---- structures ----*/
#pragma pack(push,1)

#define 	MODID_TYPE	struct modid_type

    MODID_TYPE 	{
		long		status;
		long		slot;
		long		pushwheel;
		char		idstr[ 20 ];
    		};

typedef struct {
	unsigned char	conn_type;	/* network connection type	*/
	unsigned char	priority;	/* network connection priority	*/
	unsigned char	pkt_type;	/* fixed/variable packets	*/
	unsigned short	pkt_size;	/* fixed/maximum packet size	*/
	unsigned long	rpi;		/* requested packet interval	*/
	unsigned long	api;		/* actual packet interval	*/
} ASA_NET_CONNECTION;

typedef struct {
	unsigned long	ctype;		/* connection structure type	*/
	unsigned char	mode;		/* client/server mode		*/
	unsigned char	trigger;	/* trigger mode			*/
	unsigned char	transport;	/* transport type		*/
	unsigned char	tmo_mult;	/* timeout multiplier		*/
	ASA_NET_CONNECTION OT;		/* originator-to-target net conn*/
	ASA_NET_CONNECTION TO;		/* target-to-originator net conn*/
} ASA_CONNECTION;

typedef struct {
	unsigned short	vendor;		/* ASA vendor ID		*/
	unsigned short	prod_type;	/* ASA product type		*/
	unsigned short	prod_code;	/* ASA product code		*/
	unsigned char	major_rev;	/* major revision		*/
	unsigned char	minor_rev;	/* minor revision		*/
	unsigned short	status;		/* should be zero		*/
	unsigned long	serial_num;	/* ASA serial number		*/
	unsigned char	name_len;	/* # of bytes in product name	*/
	char		name[32];	/* product name/description	*/
} ASA_IDENTITY;

/* In the INTERCHANGE API the maximum size of a DTSA structure was 24 bytes and remained constant.
 * In the RSLinx API it is currently 64 bytes and will grow as required.
 * There is a potential mismatch between the largest DTSA known to your application
 * when you built and the largest DTSA supported by the running RSLinx API.
 * To help manage this potential your application can make use of the following two aids:
 * 1. Your application can use the DTL_DTSA_SIZE_MAX define below
 *    to determine the compile-time size of the largest DTSA structure.
 * 2. Your application can use the DTL_GET_DTSA_SIZE_MAX() function
 *    to determine the run-time size of the largest DTSA structure.
 */
#define DTL_DTSA_SIZE_MAX	(64)

/* This is the largest name a DTSA can have.
 * Such DTSAs include:
 *	DTSA_AB_NAME
 *	DTSA_AB_PIGATEWAY_NAME
 */
#define	DTL_DTSA_NAME_MAX	(32)

/* Applications that use DTL_C_CONNECT/DTL_DISCONNECT instead of using
 * DTL_DRIVER_OPEN/DTL_DRIVER_CLOSE should use the following DTSA types:
 *	DTSA_ARCH
 *	DTSA_BKPLN
 *	DTSA_DH
 *	DTSA_DH_R
 *	DTSA_FILE
 * NOTE: The DTSA types DTSA_ARCH and DTSA_FILE are only used by the
 * upload, download, and compare functions which are not supported by RSLinx.
 *
 * Applications that use DTL_DRIVER_OPEN/DTL_DRIVER_CLOSE instead of
 * using DTL_C_CONNECT/DTL_DISCONNECT should use the following DTSA types:
 *	DTSA_AB_DH_LOCAL
 *	DTSA_AB_DH_LONG_LOCAL
 *	DTSA_AB_DH_OFFLINK
 *	DTSA_AB_NAME
 *	DTSA_AB_PIGATEWAY
 *	DTSA_AB_PIGATEWAY_IP
 *	DTSA_AB_PIGATEWAY_NAME
 *	DTSA_AB_DF1MASTER
 */
#define DTSA_TYPE  struct dtsa_type
DTSA_TYPE {
	unsigned long	atype;		/* Address type			*/
	char		pad[DTL_DTSA_SIZE_MAX - 4];
};

#define DTSA_DH  struct dtsa_dh
DTSA_DH {
	unsigned long	atype;		/* Address type			*/
	unsigned long	ni_id;		/* Network Interface id		*/
	unsigned long	module;		/* DTL_MODULE_XX		*/
	unsigned long	pushwheel;	/* pushwheel			*/
	unsigned long	channel;	/* DTL_CHAN_xx			*/
	unsigned long	stn_addr;	/* Data Highway [Plus] sta addr	*/
	char		pad[DTL_DTSA_SIZE_MAX - 24];
};

#define DTSA_BKPLN  struct dtsa_bkpln
DTSA_BKPLN {
	unsigned long	atype;		/* Address type			*/
	unsigned long	ni_id;		/* Network Interface id		*/
	unsigned long	module;		/* DTL_MODULE_XX		*/
	unsigned long	pushwheel;	/* pushwheel			*/
	unsigned long	channel;	/* DTL_CHAN_xx			*/
	char		pad[DTL_DTSA_SIZE_MAX - 20];
};

#define DTSA_FILE  struct dtsa_file
DTSA_FILE {
	unsigned long	atype;		/* Address type			*/
	char LIBPTR *fname;		/* File Name 			*/
	unsigned long	length;		/* Length of file name		*/
	char		pad[DTL_DTSA_SIZE_MAX - 12];
};

#define DTSA_ARCH  struct dtsa_arch
DTSA_ARCH {
	unsigned long	atype;		/* Address type			*/
	char LIBPTR	*fname;		/* File Name 			*/
	unsigned long	length;		/* Length of file name		*/
	unsigned long 	file_type; 	/* File type */
	char		pad[DTL_DTSA_SIZE_MAX - 16];
};

#define DTSA_DH_R struct dtsa_dh_r
DTSA_DH_R {
	unsigned long	atype;		/* Address type		*/
	unsigned long	ni_id;		/* Network Interface id	*/
	unsigned short	module;		/* DTL_MODULE_XX	*/
	unsigned short	pushwheel;
	unsigned short	channel;	/* DTL_CHAN_XX		*/
	unsigned char	dst;
	unsigned char	flags;
	unsigned short	rsrc_link;
	unsigned short	rsrc_stn;
	unsigned short	rdst_link;
	unsigned short	rdst_stn;
	char		pad[DTL_DTSA_SIZE_MAX - 36];
};

#define DTSA_ASA struct dtsa_asa
DTSA_ASA {
	unsigned long		atype;	/* Address type		*/
	unsigned char LIBPTR	*path;
	unsigned long		length;	/* Bytes in path	*/
	unsigned long		flags;
	char		pad[DTL_DTSA_SIZE_MAX - 24];
};

#define DTSA_CONN struct dtsa_conn
DTSA_CONN {
	unsigned long	atype;		/* Address type		*/
	unsigned long	conn_id;	/* Connection ID	*/
	char		pad[DTL_DTSA_SIZE_MAX - 8];
};

#define	DTSA_AB_DH_LOCAL struct dtsa_ab_dh_local
DTSA_AB_DH_LOCAL
{
	unsigned long	atype;
	long		driver_id;
	unsigned char	bStation;
};

#define	DTSA_AB_DH_LONG_LOCAL	struct dtsa_ab_dh_long_local
DTSA_AB_DH_LONG_LOCAL
{
	unsigned long	atype;
	long		driver_id;
	unsigned long	dwStation;
};

#define	DTSA_AB_NAME	struct dtsa_ab_name
DTSA_AB_NAME
{
	unsigned long	atype;
	long		driver_id;
	unsigned long	dwLength;
	char		szName[DTL_DTSA_NAME_MAX];
};

#define DTSA_AB_DH_OFFLINK struct dtsa_ab_dh_offlink
DTSA_AB_DH_OFFLINK
{
	unsigned long	atype;
	long		driver_id;
	unsigned short  wControl;
	unsigned char   bLocalDST;
	unsigned char   bDLSAP;
	unsigned short	wDLink;
	unsigned short	wDStn;
	unsigned char	bSLSAP;
	unsigned short	wSLink;
	unsigned short	wSStn;
	unsigned char	bLftm;
};

#define DTSA_AB_PIGATEWAY struct dtsa_ab_pigateway
DTSA_AB_PIGATEWAY
{
	unsigned long	atype;		// NOTE: This begins a DTSA_AB_DH_OFFLINK.
	long		driver_id;
	unsigned short  wControl;
	unsigned char   bLocalDST;
	unsigned char   bDLSAP;
	unsigned short	wDLink;
	unsigned short	wDStn;
	unsigned char	bSLSAP;
	unsigned short	wSLink;
	unsigned short	wSStn;
	unsigned char	bLftm;		// NOTE: This ends a DTSA_AB_DH_OFFLINK.
	unsigned char	bEIStn;		// PI Gateway mapped station number.
	unsigned char	bModuleType;	// PI Gateway module (KA or RM).
	unsigned char	bPushwheel;	// PI Gateway pushwheel (0-4).
	unsigned char	bChannel;	// PI Gateway channel (2-3).
};

#define	DTSA_AB_PIGATEWAY_IP	struct dtsa_ab_pigateway_ip
DTSA_AB_PIGATEWAY_IP
{
	unsigned long	atype;		// NOTE: This begins a DTSA_AB_DH_OFFLINK.
	long		driver_id;
	unsigned short  wControl;
	unsigned char   bLocalDST;
	unsigned char   bDLSAP;
	unsigned short	wDLink;
	unsigned short	wDStn;
	unsigned char	bSLSAP;
	unsigned short	wSLink;
	unsigned short	wSStn;
	unsigned char	bLftm;		// NOTE: This ends a DTSA_AB_DH_OFFLINK.
	unsigned char	bModuleType;	// PI Gateway module (KA or RM).
	unsigned char	bPushwheel;	// PI Gateway pushwheel (0-4).
	unsigned char	bChannel;	// PI Gateway channel (2-3).
	unsigned long	dwIPAddress;	// PI Gateway IP address.
};

#define	DTSA_AB_PIGATEWAY_NAME	struct dtsa_ab_pigateway_name
DTSA_AB_PIGATEWAY_NAME
{
	unsigned long	atype;		// NOTE: This begins a DTSA_AB_DH_OFFLINK.
	long		driver_id;
	unsigned short  wControl;
	unsigned char   bLocalDST;
	unsigned char   bDLSAP;
	unsigned short	wDLink;
	unsigned short	wDStn;
	unsigned char	bSLSAP;
	unsigned short	wSLink;
	unsigned short	wSStn;
	unsigned char	bLftm;		// NOTE: This ends a DTSA_AB_DH_OFFLINK.
	unsigned char	bModuleType;	// PI Gateway module (KA or RM).
	unsigned char	bPushwheel;	// PI Gateway pushwheel (0-4).
	unsigned char	bChannel;	// PI Gateway channel (2-3).
	unsigned long	dwLength;
	char			szName[DTL_DTSA_NAME_MAX];	// PI hostname.
};

#define DTSA_AB_DF1MASTER struct dtsa_ab_df1master
DTSA_AB_DF1MASTER
{
	unsigned long	atype;
	long		driver_id;
	unsigned char	bDstStation;	// Final destination station
	unsigned char	bPollStation;	// Polling station address on DF1
};

#define DTSA_AB_ASA_PATH struct dtsa_ab_asa_path
DTSA_AB_ASA_PATH
{
	unsigned long	atype;
	long		driver_id;
	unsigned short	wControl;
	unsigned long	dwLength;
	unsigned char	baPath[1];
};

#define DTSA_AB_ASA_CONN struct dtsa_ab_asa_conn
DTSA_AB_ASA_CONN
{
	unsigned long	atype;
	long		driver_id;
	unsigned long	dwConnection;
};

#define DTSA_AB_DH_ASA_PATH struct dtsa_dh_ab_asa_path
DTSA_AB_DH_ASA_PATH
{
	unsigned long	atype;		// NOTE: This begins a DTSA_AB_DH_OFFLINK.
	long		driver_id;
	unsigned short  wControl;
	unsigned char   bLocalDST;
	unsigned char   bDLSAP;
	unsigned short	wDLink;
	unsigned short	wDStn;
	unsigned char	bSLSAP;
	unsigned short	wSLink;
	unsigned short	wSStn;
	unsigned char	bLftm;		// NOTE: This ends a DTSA_AB_DH_OFFLINK.
	unsigned long	dwLength;
	unsigned char	baPath[1];
};

#define DTSA_AB_DH_ASA_CONN struct dtsa_ab_dh_asa_conn
DTSA_AB_DH_ASA_CONN
{
	unsigned long	atype;		// NOTE: This begins a DTSA_AB_DH_OFFLINK.
	long		driver_id;
	unsigned short  wControl;
	unsigned char   bLocalDST;
	unsigned char   bDLSAP;
	unsigned short	wDLink;
	unsigned short	wDStn;
	unsigned char	bSLSAP;
	unsigned short	wSLink;
	unsigned short	wSStn;
	unsigned char	bLftm;		// NOTE: This ends a DTSA_AB_DH_OFFLINK.
	unsigned long	dwConnection;
};

/* The following defines are valid flag bits for the wControl member of these DTSA structures:
 *	DTSA_AB_DH_OFFLINK
 *	DTSA_AB_PIGATEWAY
 *	DTSA_AB_PIGATEWAY_IP
 *	DTSA_AB_PIGATEWAY_NAME
 */
#define	DTL_ROUTEFLAG_BRIDGETYPE_MASK		(0x000f)
#define		DTL_ROUTEFLAG_BRIDGETYPE_NONE	(0x0000)
#define		DTL_ROUTEFLAG_BRIDGETYPE_KA		(0x0001)
#define		DTL_ROUTEFLAG_BRIDGETYPE_KA5GW	(0x0002)
#define		DTL_ROUTEFLAG_BRIDGETYPE_IP		(0x0003)
#define		DTL_ROUTEFLAG_BRIDGETYPE_RAW	(0x0004)
#define	DTL_ROUTEFLAG_SOURCEUSE_MASK		(0x0010)
#define		DTL_ROUTEFLAG_SOURCEUSE_YES		(0x0010)
#define		DTL_ROUTEFLAG_SOURCEUSE_NO		(0x0000)
#define	DTL_ROUTEFLAG_BRIDGESTN_MASK		(0x0020)
#define		DTL_ROUTEFLAG_BRIDGESTN_YES		(0x0020)
#define		DTL_ROUTEFLAG_BRIDGESTN_NO		(0x0000)

/* The following defines are valid flag bits for the flags member of the DTSA structures. */
#define	DTL_VIA_1785_KA	1		/* 1785-KA bit for flags field */

#define DTUD_TYPE  struct dtud_type

DTUD_TYPE {
	unsigned short	app_addr;
	short		elements;
	short		app_data_type;
	short		plc_data_type;
};


/*----  report types  ----*/
#define REPORT_TYP_UDC		0x0001

/*----  U/D/C progress report structure  ----*/
typedef struct {
	unsigned long	rtype;		/* type and version of report struct */
	unsigned long	phase;		/* phase of U/D/C operation          */
	unsigned long	section;	/* section type of current file      */
	unsigned long	file;		/* # of current file in its section  */
	unsigned long	file_bytes;	/* # of bytes in current file        */
	unsigned long	bytes_done;	/* # of bytes processed in file      */
	unsigned long	total_bytes;	/* # of bytes for entire operation   */
	unsigned long	total_done;	/* # of bytes processed in operation */
	unsigned long	total_files;	/* # of files for entire operation   */
	unsigned long	files_done;	/* # of files processed in operation */
} REPORT_UDC;

/*----  U/D/C phases  ----*/
#define DTL_UDC_CHK_MATCH	1	/* check if source/dest types match  */
#define DTL_UDC_CHK_COMPAT	2	/* check image compatibility         */
#define DTL_UDC_GET_FILES	3	/* transfer/compare files            */

/*----  U/D/C section types  ----*/
#define DTL_UDC_FILE_DIR	1	/* directory file                    */
#define DTL_UDC_FILE_DATA	2	/* data tables                       */
#define DTL_UDC_FILE_PROGRAM	3	/* program files (ladder/SFC)        */
#define DTL_UDC_FILE_FORCE	4	/* force tables                      */
#define DTL_UDC_FILE_PLC2	5       /* PLC-2 compatibility files         */
#define DTL_UDC_FILE_CONFIG	6	/* unspecified configuration files   */
#define DTL_UDC_FILE_IO_CFG	7	/* I/O configuration files           */
#define DTL_UDC_FILE_COMM_CFG	8	/* communication configuration files */
#define DTL_UDC_FILE_INTERNAL   9	/* internal files                    */
#define DTL_UDC_FILE_COPROC	10	/* coprocessor section               */

/*
 * Control flag bits for DTL_COMPARE...
 */
#define DTL_CMP_PROCTYPE	0x001	/* Memory images must be of the same */
					/* processor type (eg.PLC-5/15)      */

#define DTL_CMP_PROCSER_BIT	0x002
#define DTL_CMP_PROCSER		0x003	/* Memory images must be of the same */
					/*  series. This mask includes       */
					/* DTL_CMP_PROCTYPE.                 */

#define DTL_CMP_PROCREV_BIT	0x004
#define DTL_CMP_PROCREV		0x007	/* Memory images must be of the same */
					/* revision. This mask includes      */
					/* DTL_CMP_PROCTYPE, DTL_CMP_PROCSER */

#define DTL_CMP_PROCMEM		0x008	/* Memory images must have the same */
					/* memory size. */

#define DTL_CMP_NUMDT		0x010	/* Memory images must have the same */
					/* number of data table files.      */

#define DTL_CMP_NUMPROG		0x020	/* Memory images must have the same */
					/* number of program files.         */

#define DTL_CMP_DATATABLES	0x040	/* Compare the data tables. */
#define DTL_CMP_FORCES		0x080	/* Compare the forces. */
#define DTL_CMP_PROGDIR		0x100	/* Compare program directories. */
#define DTL_CMP_LADDER		0x200	/* Compare the ladder logic. */
#define	DTL_CMP_NAME		0x400	/* Compare the program name  */
#define	DTL_CMP_SFC		0x800	/* Compare the SFCs          */


/*
 * Control flag bits for DTL_DOWNLOAD...
 */
#define DTL_DWN_MATCH		0x001	/* Download only if PLC Type, */
					/* Revision and Series match exactly */

#define DTL_DWN_IGNCFGWARN	0x002	/* Ignore warnings from physical */
					/* configuration of processor */

#define DTL_DWN_REMOTE		0x004	/* Enable download in remote program */
					/* mode */

#define DTL_DWN_BEGIN_BIT	0x008	/* Single bit definition for */
					/* internal use only */

#define DTL_DWN_BEGIN		DTL_DWN_REMOTE | DTL_DWN_BEGIN_BIT
					/* Enable mode change from remote run */
					/* or remote test to remote program */
					/* before download if DTL_DWN_REMOTE */
					/* is also set */

#define DTL_DWN_END_BIT		0x010	/* Single bit definition for */
					/* internal use only */

#define DTL_DWN_END		DTL_DWN_BEGIN | DTL_DWN_END_BIT	
					/* Restore the original mode after */
					/* download if it was changed before */

#define DTL_DWN_CLR_FLT		0x020	/* Clear major faults before download */
					/*  if PLC is faulted. */

#define DTL_DWN_FORCE_SFC	0x040	/* Enable SFC forces in PLC */
					/* if they are in archive file. */

#define DTL_DWN_FORCE_OUT	0x080	/* Enable output forces in PLC */
					/* if they are in archive file. */

#define DTL_DWN_FORCE_EDITS	0x100	/* Enable test edits in PLC */
					/* if they are in archive file. */

/*
 * Control flag bits for DTL_UPLOAD...
 */
#define DTL_UP_RUN	0x01	/* Enables upload in modes where */
				/* data table values may be changing */

#define DTL_UP_FAULT	0x02	/* Enables upload when PLC is faulted */

/*
 * Master Control flag bits for DTL_UPLOAD, DTL_DOWNLOAD, DTL_COMPARE
 */
#define DTL_UP_MASTER_FLAG	( DTL_UP_RUN | DTL_UP_FAULT )
#define DTL_DWN_MASTER_FLAG	( DTL_DWN_MATCH | DTL_DWN_IGNCFGWARN | \
				  DTL_DWN_REMOTE | DTL_DWN_BEGIN | \
				  DTL_DWN_END | DTL_DWN_CLR_FLT | \
				  DTL_DWN_FORCE_SFC | DTL_DWN_FORCE_OUT | \
				  DTL_DWN_FORCE_EDITS )
#define DTL_CMP_MASTER_FLAG	( DTL_CMP_PROCTYPE | DTL_CMP_PROCSER | \
				  DTL_CMP_PROCREV | DTL_CMP_PROCMEM | \
				  DTL_CMP_NUMDT | DTL_CMP_NUMPROG | \
				  DTL_CMP_DATATABLES | DTL_CMP_FORCES | \
				  DTL_CMP_PROGDIR | DTL_CMP_LADDER | \
				  DTL_CMP_NAME | DTL_CMP_SFC )

/*
 * Control flag bits for DTL_GET_FAULTS/DTL_CLEAR_FAULT...
 */
#define DTL_MINOR_FAULTS	0x0001 /* get or clear minor faults */
#define DTL_MAJOR_FAULTS    	0x0002 /* get or clear major faults */
#define DTL_OLD_MAJOR_FAULTS    0x0004 /* get or clear old major faults */

/* PLC modes (moved here from dtl_diag.h) */
/* PLC , PLC-2, PLC-3, PLC-5, PLC-5/250, SLC-500 */
#define  DTL_MODE_PRG    0	/* Program load */
#define  DTL_MODE_TST    1	/* Test */
#define  DTL_MODE_RUN    2	/* Run */
#define  DTL_MODE_RSVD1  3	/* Reserved for future use */
#define  DTL_MODE_RPRG   4	/* Remote program load */
#define  DTL_MODE_RTST   5	/* Remote test */
#define  DTL_MODE_RRUN   6	/* Remote run */
#define  DTL_MODE_RSVD2  7	/* Reserved for future use */
#define  DTL_MODE_TCSN   7	/* SLC500 Test Continuous Scan Mode */
#define  DTL_MODE_TSSN   8	/* SLC500 Test Single Scan Mode */
#define  DTL_MODE_TSRG   9	/* SLC5/03 & above Test Single Step Mode */
#define  DTL_MODE_SLCPRG 17	/* SLC5/03 Remote Program */
#define  DTL_MODE_SLCRUN 30	/* SLC5/03 Remote Run */

/* A character buffer of this many elements is guaranteed to be
 * able to hold the string returned by the DTL_VERSION function.
 */
#define	DTL_VERSION_SIZE	(128)

/*---- Structure filled in by DTL_VERSION_EX() function ----*/
typedef struct version
{
	DWORD	dwType;		// The type of this structure. You must pass in the DTL_VERSION_TYPEx constant below.
#		define DTL_VERSION_TYPE1	(1)
	DWORD	dwSize;		// The size of the rest of this structure. You must pass in the DTL_VERSION_TYPE1 constant.
	DWORD	dwVersion[4];			// Version, stored such that 1.60.17.0 would appear in +0...+3, respectively.
	char	szName[DTL_VERSION_SIZE];	// DLL name (i.e. DTL32.DLL).
	char	szBuilt[DTL_VERSION_SIZE];	// Build date string.
	char	szVersion[DTL_VERSION_SIZE];	// Version string.
	char	szDescription[DTL_VERSION_SIZE];// DLL Description string.
	char	szCopyright[DTL_VERSION_SIZE];	// DLL Copyright string.
	char	szOldVersion[DTL_VERSION_SIZE];	// Same string returned from DTL_VERSION().
} sVERSION;
#define DTL_VERSION_SIZE1	(sizeof(sVERSION) - (size_t)&(((sVERSION*)0)->dwVersion))

/* The driver identifiers used to calls to DTL_DRIVER_OPEN and DTL_DRIVER_CLOSE must be within this range... */
#define	DTL_DRIVER_ID_MIN	(0)
#define	DTL_DRIVER_ID_MAX	(15)
#define DTL_RSLINX_DRIVER_ID	(DTL_GetRSLinxDriverID())	/* Used in DTSA_AB_ASA_PATH to access internal RSLinx ASA objects */

/* The definitions and structure used by DTL_DRIVER_LIST() to return information about drivers. */
#define DTL_DRIVER_NAME_MAX	(16)
#define DTL_DRIVER_ALIAS_MAX	(16)
#define	DTL_MAX_DRIVERS		(16)

/* If you call DTL_DRIVER_LIST() you may want to consider switching to DTL_DRIVER_LIST_EX().
 * The differences are:
 *	1. You can tell DTL_DRIVER_LIST_EX() how many DTLDRIVER structures your block of memory can hold.
 *	   To do this you set the dwNumDrivers parameters prior to making the call.
 *	2. DTL_DRIVER_LIST_EX() fetches a new list of drivers from the Linx server whereas DTL_DRIVER_LIST() reads a cached list.
 *
 * If you do change to DTL_DRIVER_LIST_EX() then you may also want to consider using a run-time check for the largest number
 * of drivers available instead of the constant DTL_MAX_DRIVERS.
 * The run-time check is made by calling DTL_MaxDrivers(), e.g.
 *	DTLDRIVER *pDtlDriver = (DTLDRIVER*)GlobalAlloc(GMEM_MOVABLE,DTL_MaxDrivers());
 */

/* Structure filled in by the DTL_DRIVER_LIST call. */
typedef struct dtldriver
{
	WORD	wType;		// The type of this structure.
#		define	DTL_DVRLIST_TYPE2	(2)
	WORD	wLength;	// The size of the rest of this structure.
				// See below for constant DTL_DVRLIST_SIZE2.
	WORD	wMfgMask;       // Bitmap indicating supported manufacturer(s)
#		define	DTL_MFG_AB		(1)
	WORD	wNetworkType;   // Network type code (DHTYPE_DHP, DHTYPE_INET, etc.)
#		define	DTL_NETTYPE_DH		(1)
#		define	DTL_NETTYPE_DHP		(2)
#		define	DTL_NETTYPE_485		(4)
#		define	DTL_NETTYPE_DHPKA5	(8)
#		define	DTL_NETTYPE_ENET	(16)
#		define	DTL_NETTYPE_CNET	(64)
	WORD	wDriverID;      // Type code for the driver instance itself
	WORD	wDstDriverID;   // Type code for the final driver to which the driver instance
                                //   is connected; normally the same as wDriverID, but may be
                                //   different if this driver is a client of a server's driver
#		define	DTL_DVRTYPE_PLC_OR_KF2	(0xA3)	// Direct connection to PLC or connection to KF2.
#		define	DTL_DVRTYPE_SLC_OR_KF3	(0xCC)	// Direct connection to SLC or connection to KF3.
#		define	DTL_DVRTYPE_KFC		(0xDB)	// Connection to KFC.
#		define	DTL_DVRTYPE_LINX_CLIENT	(0x16)	// WinLinx or RSLinx client driver connected to WinLinx or RSLinx server.
#		define	DTL_DVRTYPE_ETHERNET	(0x1C)	// Ethernet.
#		define	DTL_DVRTYPE_KT_DHP	(0xBB)	// 1784-KT on DH+.
#		define	DTL_DVRTYPE_PCMK_DHP	(0x9D)	// 1784-PCMK on DH+.
#		define	DTL_DVRTYPE_PCMK_DH485	(0xEE)	// 1784-PCMK on DH485.
#		define	DTL_DVRTYPE_KTX_DHP	(0xFD)	// 1784-KTX on DH+.
#		define	DTL_DVRTYPE_KTX_DH485	(0xDA)	// 1784-KTX on DH485.
#		define	DTL_DVRTYPE_KTC_CNET	(0xE1)	// 1784-KTC.
#		define	DTL_DVRTYPE_VLINK	(0x5E)	// RSLinx Virtual Link driver.
#		define	DTL_DVRTYPE_APP_INTFC	(0x3C)	// WinLinx AutoRouter/App Interface driver.
#		define	DTL_DVRTYPE_PIC		(0xCE)	// 1747-PIC.
#		define	DTL_DVRTYPE_SOFT5	(0x14)	// Soft 5.
#		define	DTL_DVRTYPE_EMU5	(0x0F)	// PLC-5 Emulator.
#		define	DTL_DVRTYPE_EMU500	(0x13)	// SLC-500 Emulator.
	DWORD	dwHandle;       // Handle to be used in LINX_SEND_PACKET commands
	DWORD	dwStation;      // Driver's own station address on its network
	DWORD	dwMaxStation;   // Maximum station address on driver's network
	WORD	wCapabilities;  // Bitmap of device capabilities:
#       define  DTL_DRIVER_M_REMOTE      (0x0001)  // Supports offlink routes
#       define  DTL_DRIVER_M_NODETABLE   (0x0002)  // Has a node table
#       define  DTL_DRIVER_M_UNSOLICITED (0x0004)  // Can generate unsolicited messages
	WORD	wMTU;           // Maximum Transmission Unit on the driver's network
	char	szDriverName[DTL_DRIVER_NAME_MAX];
	char	szDriverAlias[DTL_DRIVER_ALIAS_MAX];
	BYTE	bAddrRadix;     // Station address radix (8, 10, etc.)
	BYTE	bReserved[3];   // Padding to 64 bytes
} DTLDRIVER, *PDTLDRIVER;
#define DTL_DVRLIST_SIZE2	(sizeof(DTLDRIVER) - (size_t)&(((DTLDRIVER*)0)->wMfgMask))
#pragma pack(pop)

/* These are the valid server type constants.
 * You will get one of the the DTL_SERVER_TYPE_xxx back from the DTL_GET_SERVER_TYPE_BY_DRIVER_NAME() call.
 */
#define	DTL_SERVER_TYPE_UNKNOWN	(0)
#define	DTL_SERVER_TYPE_WINLINX	(16)
#define	DTL_SERVER_TYPE_RSLINX	(32)
#define	DTL_SERVER_TYPE_GWLINX	(0x0100 + DTL_SERVER_TYPE_RSLINX)

/* Internal definitions. */
#define DTL_SERVER_NAME_MAX	(64)


/*-------------------------- function definitions  ---------------------*/

#ifdef __cplusplus
extern "C" {
#endif

typedef int  (DTL_CALLBACK *DTL_REPORT_PROC)(unsigned long,REPORT_UDC LIBPTR*);
typedef void (DTL_CALLBACK *DTL_IO_CALLBACK_PROC)(unsigned long,unsigned long);
typedef int  (DTL_CALLBACK *DTL_ASA_PACKET_PROC)(unsigned long,unsigned long,
	unsigned char LIBPTR*,unsigned long);
typedef int  (DTL_CALLBACK *DTL_ASA_NOTIFY_PROC)(unsigned long,unsigned long,
	unsigned long,unsigned char LIBPTR*,unsigned long);
typedef int  (DTL_CALLBACK *DTL_ASA_ACCEPT_PROC)(unsigned long,unsigned long,
	unsigned long,unsigned char LIBPTR*,ASA_CONNECTION LIBPTR*);
typedef int  (DTL_CALLBACK *DTL_ASA_SERVICE_PROC)(unsigned long,
	unsigned long,unsigned long,unsigned char,unsigned char LIBPTR*,
	unsigned char LIBPTR*,unsigned long);

DWORD LIBMEM DTL_MaxDrivers(void);
long LIBMEM DTL_GetRSLinxDriverID(void);
DTL_RETVAL LIBMEM DTL_ASA_ACCEPT ( unsigned long, unsigned long,
				   ASA_CONNECTION LIBPTR *,
				   unsigned char LIBPTR *, unsigned long,
				   DTL_ASA_PACKET_PROC, DTL_ASA_NOTIFY_PROC,
				   unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_CLOSE ( unsigned long, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_DIRECT ( DTSA_TYPE LIBPTR *,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long LIBPTR *, unsigned long,
				unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_DIRECT_CB ( DTSA_TYPE LIBPTR *,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long, DTL_IO_CALLBACK_PROC,
				unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_DIRECT_W ( DTSA_TYPE LIBPTR *,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_MSG ( DTSA_TYPE LIBPTR *, int,
				unsigned char LIBPTR *,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long LIBPTR *, unsigned long,
				unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_MSG_CB ( DTSA_TYPE LIBPTR *, int,
				unsigned char LIBPTR *,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long, DTL_IO_CALLBACK_PROC,
				unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_MSG_W ( DTSA_TYPE LIBPTR *, int,
				unsigned char LIBPTR *,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_OPEN ( DTSA_TYPE LIBPTR *, unsigned char LIBPTR *,
				 unsigned long LIBPTR *, unsigned long,
				 ASA_CONNECTION LIBPTR *, DTL_ASA_PACKET_PROC,
				 DTL_ASA_NOTIFY_PROC, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_REGISTER ( unsigned long LIBPTR *, unsigned long,
				ASA_IDENTITY LIBPTR *, unsigned long,
				void LIBPTR *, unsigned int, unsigned int,
				DTL_ASA_SERVICE_PROC, DTL_ASA_ACCEPT_PROC,
				unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_REJECT ( unsigned long, int,
				   unsigned char LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_REPLY ( unsigned long, int,
				unsigned char LIBPTR *, unsigned long,
				unsigned char LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_SEND_CONN ( unsigned long, unsigned long,
				unsigned char LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_ASA_UNREGISTER ( unsigned long, unsigned long );
DTL_RETVAL LIBMEM DTL_C_CONNECT(unsigned long,const char LIBPTR*,void (DTL_CALLBACK *)(unsigned long LIBPTR*,unsigned long LIBPTR*,unsigned long LIBPTR*));
DTL_RETVAL LIBMEM DTL_C_DEFINE ( unsigned long LIBPTR *, const char LIBPTR * );
DTL_RETVAL LIBMEM DTL_CANCEL_RECV ( void );
DTL_RETVAL LIBMEM DTL_CHANGE_MODE (DTSA_TYPE LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_CLEAR_FAULTS (DTSA_TYPE LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_CLEAR_MEMORY (DTSA_TYPE LIBPTR * );
DTL_RETVAL LIBMEM DTL_CLOCK ( void );
DTL_RETVAL LIBMEM DTL_CLR_MASK ( unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_CLR_WID ( unsigned long );
DTL_RETVAL LIBMEM DTL_COMPARE ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				unsigned long, unsigned long LIBPTR *,
				unsigned long, unsigned long );
DTL_RETVAL LIBMEM DTL_COMPARE_CB ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				   unsigned long, unsigned long,
				   DTL_IO_CALLBACK_PROC, unsigned long );
DTL_RETVAL LIBMEM DTL_COMPARE_RCB ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				    unsigned long, unsigned long,
				    DTL_REPORT_PROC, DTL_IO_CALLBACK_PROC,
				    unsigned long );
DTL_RETVAL LIBMEM DTL_COMPARE_W ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				  unsigned long, unsigned long LIBPTR *,
				  unsigned long );
DTL_RETVAL LIBMEM DTL_DOWNLOAD ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				 unsigned long, unsigned long LIBPTR *,
				 unsigned long, unsigned long );
DTL_RETVAL LIBMEM DTL_DOWNLOAD_CB ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				    unsigned long, unsigned long,
				    DTL_IO_CALLBACK_PROC, unsigned long );
DTL_RETVAL LIBMEM DTL_DOWNLOAD_RCB ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				     unsigned long, unsigned long,
				     DTL_REPORT_PROC, DTL_IO_CALLBACK_PROC,
				     unsigned long );
DTL_RETVAL LIBMEM DTL_DOWNLOAD_W ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				   unsigned long, unsigned long LIBPTR *,
				   unsigned long );
DTL_RETVAL LIBMEM DTL_DEF_AVAIL ( unsigned long LIBPTR *);
DTL_RETVAL LIBMEM DTL_DISCONNECT ( unsigned long );
void LIBMEM DTL_ERROR ( unsigned long );
void LIBMEM DTL_ERROR_S ( unsigned long, char LIBPTR *, int );
void LIBMEM DTL_ERROR_P( unsigned long, unsigned long );
#if defined(_INC_STDIO)
void LIBMEM DTL_ERROR_F(FILE LIBPTR*,unsigned long);
void LIBMEM DTL_ERROR_FP(FILE LIBPTR*,unsigned long,unsigned long);
#else
void LIBMEM DTL_ERROR_F();
void LIBMEM DTL_ERROR_FP();
#endif
DTL_RETVAL LIBMEM DTL_FCREATE ( unsigned long );
DTL_RETVAL LIBMEM DTL_FDELETE ( unsigned long );
DTL_RETVAL LIBMEM DTL_FSIZE ( unsigned long, unsigned long LIBPTR * );
DTL_RETVAL LIBMEM DTL_GET_3BCD ( unsigned char LIBPTR *,
				 unsigned long LIBPTR * );
DTL_RETVAL LIBMEM DTL_GET_4BCD ( unsigned char LIBPTR *,
				 unsigned long LIBPTR * );
DTL_RETVAL LIBMEM DTL_GET_FLT ( unsigned char LIBPTR *, float LIBPTR * );
DTL_RETVAL LIBMEM DTL_GET_SLC500_FLT ( unsigned char LIBPTR *, float LIBPTR * );
long LIBMEM DTL_GET_LONG ( unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_GET_MODIDS ( DTSA_TYPE LIBPTR *, MODID_TYPE LIBPTR * );
long LIBMEM DTL_GET_PLC3_LONG ( unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_GET_PLC3FLT ( unsigned char LIBPTR *, float LIBPTR * );
short LIBMEM DTL_GET_WORD ( unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_INIT ( unsigned long );
DTL_RETVAL LIBMEM DTL_MAKE_REPLY( unsigned char LIBPTR *, unsigned short );
DTL_RETVAL LIBMEM DTL_PCCC_DIRECT ( DTSA_TYPE LIBPTR *, unsigned char LIBPTR *,
				    unsigned long, unsigned char LIBPTR *,
				    unsigned long LIBPTR *, int, int, 
				    unsigned long LIBPTR *, unsigned long,
				    unsigned long );
DTL_RETVAL LIBMEM DTL_PCCC_DIRECT_CB ( DTSA_TYPE LIBPTR *, 
				       unsigned char LIBPTR *, unsigned long, 
				       unsigned char LIBPTR *, 
				       unsigned long LIBPTR *, int, int,
				       unsigned long, DTL_IO_CALLBACK_PROC, 
				       unsigned long );
DTL_RETVAL LIBMEM DTL_PCCC_DIRECT_W ( DTSA_TYPE LIBPTR *,
				      unsigned char LIBPTR *, unsigned long, 
				      unsigned char LIBPTR *,
				      unsigned long LIBPTR *, int, int, 
				      unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_PCCC_MSG ( DTSA_TYPE LIBPTR *, unsigned char,
				unsigned char LIBPTR *, unsigned long, 
				unsigned char LIBPTR *, unsigned long LIBPTR *,
				unsigned long LIBPTR *, unsigned long,
				unsigned long );
DTL_RETVAL LIBMEM DTL_PCCC_MSG_CB ( DTSA_TYPE LIBPTR *, unsigned char,
				   unsigned char LIBPTR *, unsigned long, 
				   unsigned char LIBPTR *, 
				   unsigned long LIBPTR *,
				   unsigned long, DTL_IO_CALLBACK_PROC, 
				   unsigned long );
DTL_RETVAL LIBMEM DTL_PCCC_MSG_W ( DTSA_TYPE LIBPTR *,unsigned char,
				  unsigned char LIBPTR *, unsigned long, 
				  unsigned char LIBPTR *,
				  unsigned long LIBPTR *,
				  unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_PING ( void );
DTL_RETVAL LIBMEM DTL_PUT_3BCD ( unsigned long, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_4BCD ( unsigned long, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_SLC500_FLT ( float, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_FLT ( float, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_LONG ( unsigned long, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_PLC3_LONG ( unsigned long, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_PLC3FLT ( float, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_PUT_WORD ( unsigned long, unsigned char LIBPTR * );
DTL_RETVAL LIBMEM DTL_READ ( unsigned long, unsigned char LIBPTR *, 
			     unsigned long LIBPTR *, unsigned long,
			     unsigned long );
DTL_RETVAL LIBMEM DTL_READ_CB ( unsigned long, unsigned char LIBPTR *, 
			       unsigned long, DTL_IO_CALLBACK_PROC,
			       unsigned long );
DTL_RETVAL LIBMEM DTL_READ_W ( unsigned long, unsigned char LIBPTR *, 
			       unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_RECEIVE ( unsigned long LIBPTR *, unsigned long,
				unsigned long );
DTL_RETVAL LIBMEM DTL_RECEIVE_ENABLE ( void );
DTL_RETVAL LIBMEM DTL_RECEIVE_W ( unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_RMW ( unsigned long, unsigned char LIBPTR *, 
			    unsigned long, unsigned long,
			    unsigned long LIBPTR *, unsigned long,
			    unsigned long );
DTL_RETVAL LIBMEM DTL_RMW_CB ( unsigned long, unsigned char LIBPTR *, 
			       unsigned long, unsigned long,
			       unsigned long, DTL_IO_CALLBACK_PROC,
			       unsigned long );
DTL_RETVAL LIBMEM DTL_RMW_W ( unsigned long, unsigned char LIBPTR *, 
			      unsigned long, unsigned long,
			      unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_SET_MASK ( unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_SET_WID ( unsigned long );
DTL_RETVAL LIBMEM DTL_SETOPT ( unsigned long, unsigned long LIBPTR *, unsigned long LIBPTR *);
DTL_RETVAL LIBMEM DTL_SIZE ( unsigned long, unsigned long LIBPTR * );
DTL_RETVAL LIBMEM DTL_SYNC ( DTSA_TYPE LIBPTR * );
DTL_RETVAL LIBMEM DTL_TODTSA ( unsigned long, DTSA_TYPE LIBPTR *);
int LIBMEM DTL_TOVMS ( unsigned long );
int LIBMEM DTL_TST_MASK ( unsigned long LIBPTR *, unsigned long );
int LIBMEM DTL_TST_WID ( unsigned long );
DTL_RETVAL LIBMEM DTL_TYPE ( unsigned long, unsigned long LIBPTR * );
DTL_RETVAL LIBMEM DTL_UNDEF ( unsigned long );
void LIBMEM DTL_UNINIT ( unsigned long );
DTL_RETVAL LIBMEM DTL_UNSOL_DEF ( unsigned long LIBPTR *, DTUD_TYPE LIBPTR *,
				  unsigned long,
				  unsigned long (DTL_CALLBACK *)(),
				  unsigned long (DTL_CALLBACK *)(),
				  void (DTL_CALLBACK *)(),
				  unsigned char LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_UNSOL_GETALL( unsigned long, void (DTL_CALLBACK *)(),
				    unsigned long LIBPTR *, unsigned long,
				    unsigned long );
DTL_RETVAL LIBMEM DTL_UNSOL_GETALL_CB( unsigned long, void (DTL_CALLBACK *)(),
				       unsigned long, 
				       DTL_IO_CALLBACK_PROC, unsigned long );
DTL_RETVAL LIBMEM DTL_UNSOL_GETALL_W( unsigned long, void (DTL_CALLBACK *)(),
				      unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_UPLOAD ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
			       unsigned long, unsigned long LIBPTR *,
			       unsigned long, unsigned long );
DTL_RETVAL LIBMEM DTL_UPLOAD_CB ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				  unsigned long, unsigned long,
				  DTL_IO_CALLBACK_PROC, unsigned long );
DTL_RETVAL LIBMEM DTL_UPLOAD_RCB ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				   unsigned long, unsigned long,
				   DTL_REPORT_PROC, DTL_IO_CALLBACK_PROC,
				   unsigned long );
DTL_RETVAL LIBMEM DTL_UPLOAD_W ( DTSA_TYPE LIBPTR *, DTSA_TYPE LIBPTR *,
				 unsigned long, unsigned long LIBPTR *,
				 unsigned long );
DTL_RETVAL LIBMEM DTL_VERSION ( char LIBPTR *, int );
DTL_RETVAL LIBMEM DTL_VERSION_EX ( sVERSION LIBPTR * pVersion );
DTL_RETVAL LIBMEM DTL_WAIT ( unsigned long LIBPTR *, unsigned long LIBPTR *,
			     unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_WRITE ( unsigned long, unsigned char LIBPTR *,
			      unsigned long LIBPTR *, unsigned long,
			      unsigned long );
DTL_RETVAL LIBMEM DTL_WRITE_CB ( unsigned long, unsigned char LIBPTR *, 
			       unsigned long, DTL_IO_CALLBACK_PROC,
			       unsigned long );
DTL_RETVAL LIBMEM DTL_WRITE_W ( unsigned long, unsigned char LIBPTR *,
				unsigned long LIBPTR *, unsigned long );
DTL_RETVAL LIBMEM DTL_ZERO_MASK ( unsigned long LIBPTR *);

DTL_RETVAL LIBMEM DTL_DRIVER_LIST(PDTLDRIVER pDtlDriver,unsigned long LIBPTR *drivers,unsigned long timeout);
DTL_RETVAL LIBMEM DTL_DRIVER_LIST_EX(PDTLDRIVER pDtlDriver,unsigned long LIBPTR *drivers,unsigned long timeout);
DTL_RETVAL LIBMEM DTL_DRIVER_OPEN(long driver_id,const char LIBPTR * szDriverName,unsigned long timeout);
DTL_RETVAL LIBMEM DTL_DRIVER_CLOSE(long driver_id,unsigned long timeout);
int        LIBMEM DTL_GET_DRIVER_RADIX_FROM_DRIVERLIST_ENTRY(PDTLDRIVER pDriver,DWORD LIBPTR *dwError);

int LIBMEM DTL_GET_SERVER_TYPE_BY_DRIVER_NAME(char *szDrivername);

DTL_RETVAL LIBMEM DTL_UNSOL_BROADCAST_REGISTER(
	long		driverid,
	void		(DTL_CALLBACK *pfCallback)(DTSA_TYPE *dtsa,BYTE *baBuffer,DWORD dwLength,DWORD dwCallback),
	unsigned long	ulCallback,
	unsigned long	timeout);
DTL_RETVAL LIBMEM DTL_UNSOL_BROADCAST_UNREGISTER(long driverid,unsigned long timeout);

DTL_RETVAL LIBMEM DTL_UNSOL_PLC2MEMORY_REGISTER(
	void		(DTL_CALLBACK *pfCallback)(DTSA_TYPE *dtsa,BYTE *baBuffer,DWORD dwLength,DWORD dwCallback),
	unsigned long	ulCallback,
	unsigned short	address,
	unsigned long	timeout);
DTL_RETVAL LIBMEM DTL_UNSOL_PLC2MEMORY_UNREGISTER(unsigned short address,unsigned long timeout);

DTL_RETVAL LIBMEM DTL_UNSOL_VIRTUAL_LINK_REGISTER(
	void		(DTL_CALLBACK *pfCallback)(DTSA_TYPE *dtsa,BYTE *baBuffer,DWORD dwLength,DWORD dwCallback),
	unsigned long	ulCallback,
	long		station,
	char		szNodeName[16],
	unsigned long	timeout);
DTL_RETVAL LIBMEM DTL_UNSOL_VIRTUAL_LINK_UNREGISTER(long station,unsigned long timeout);

void LIBMEM DTL_SEND_REPLY(DTSA_TYPE *dtsa,BYTE *packet,DWORD dwLength);

int LIBMEM DTL_GET_DTSA_SIZE_MAX(void);
DTL_RETVAL LIBMEM DTL_CANCEL(DWORD dwRequestID);
DTL_RETVAL LIBMEM DTL_CANCEL_ALL(void);

/* The following functions access members of the DTLDRIVER structure by using the driver name as the key. */
WORD  LIBMEM DTL_GetTypeByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetLengthByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetMfgMaskByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetNetworkTypeByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetDriverIDByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetDstDriverIDByDriverName ( char LIBPTR * szDriverName );
DWORD LIBMEM DTL_GetHandleByDriverName ( char LIBPTR * szDriverName );
DWORD LIBMEM DTL_GetStationByDriverName ( char LIBPTR * szDriverName );
DWORD LIBMEM DTL_GetMaxStationByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetCapabilitiesByDriverName ( char LIBPTR * szDriverName );
WORD  LIBMEM DTL_GetMTUByDriverName ( char LIBPTR * szDriverName );
BYTE  LIBMEM DTL_GetAddrRadixByDriverName ( char LIBPTR * szDriverName );

#ifdef __cplusplus
}
#endif

//
// For unsolicited requests coming directly from processors on Ethernet
// you can select the type of DTSA passed to your unsolicited request callback.
// The preferred method for selecting the DTSA type is to call the function:
DTL_RETVAL LIBMEM DTL_SET_ENET_DIRECT_UNSOL_DTSA_TYPE ( unsigned long atype );
// For backwards compatibility the DTL_SETOPT function can still be used, using:
//	opt			== DTL_OPT_ENET_UNSOL_DTSA
//	opt_name	== DTL_OPTNAME_ENET_UNSOL_DTSA_TARGET
//	opt_val		== pointer to unsigned long that contains your desired atype
// The following macro makes the DTL_SETOPT slightly more convenient:
#define	DTL_SET_ENET_TARGET_UNSOL_DTSA_TYPE(T)  DTL_SETOPT(DTL_OPT_ENET_UNSOL_DTSA,(unsigned long*)DTL_OPTNAME_ENET_UNSOL_DTSA_TARGET,(T))

//
// For unsolicited requests coming thru a Pyramid Integrator gateway
// you can select the type of DTSA passed to your unsolicited request callback.
// The preferred method for selecting the DTSA type is to call the function:
DTL_RETVAL LIBMEM DTL_SET_ENET_PIGW_UNSOL_DTSA_TYPE ( unsigned long atype );
// For backwards compatibility the DTL_SETOPT function can still be used, using:
//	opt			== DTL_OPT_ENET_UNSOL_DTSA
//	opt_name	== DTL_OPTNAME_ENET_UNSOL_DTSA_GATEWAY
//	opt_val		== pointer to unsigned long that contains your desired atype
// The following macro makes the DTL_SETOPT slightly more convenient:
#define	DTL_SET_ENET_GATEWAY_UNSOL_DTSA_TYPE(T) DTL_SETOPT(DTL_OPT_ENET_UNSOL_DTSA,(unsigned long*)DTL_OPTNAME_ENET_UNSOL_DTSA_GATEWAY,(T))


//
// For DTSAs built from the result of a who you can select
// the type of DTSA built using the following functions.
// For who selections that are direct Ethernet processors:
DTL_RETVAL LIBMEM DTL_SET_ENET_DIRECT_WHO_DTSA_TYPE ( unsigned long atype );
// For who selections that are gatewayed via a Pyramid Integrator:
DTL_RETVAL LIBMEM DTL_SET_ENET_PIGW_WHO_DTSA_TYPE ( unsigned long );

//
// The following definitions are for internal use only.
// They have no meaning and/or use to any DTL function.
//
#define	DTL_TRACE_INIT		(0x00000008)	// "Initialization"	DLL initialization and shutdown.
#define	DTL_TRACE_SESSION	(0x00000010)	// "Session"		Application session initialization and shutdown.
#define DTL_TRACE_IOBUFFERS	(0x00000004)	// "Buffers"		Data buffer allocations.
#define DTL_TRACE_IOCBS		(0x00040000)	// "Control Blocks"	I/O message control block management.
#define	DTL_TRACE_BUFFERQUEUES	(0x00080000)	// "Buffer Queues"	I/O buffer queue management.
#define DTL_TRACE_PACKETS	(0x00000002)	// "Packets"		PCCC data packet handling.
#define	DTL_TRACE_PACKETS2	(0x00000200)	// "Packets Single Line"	PCCC data packets in table form.
#define	DTL_TRACE_SUBMIT	(0x00000020)	// "Submit"		Solicited message packet submit.
#define	DTL_TRACE_COMPLETE	(0x00000040)	// "Complete"		Solicited message packet completion.
#define	DTL_TRACE_PROTOCOL 	(0x00020000)	// "Protocol"		High level data packet handling.
#define	DTL_TRACE_NAKS		(0x01000000)	// "NAKs"		Retry packet due to NAK from target communications device.
#define DTL_TRACE_ERRORS	(0x00000001)	// "Errors"		Any error encountered.
#define	DTL_TRACE_PARSERS	(0x00000100)	// "Parsers"		PLC parser operation.
#define	DTL_TRACE_WINMSGS	(0x00002000)	// "Messages"		Windows message handling.
#define	DTL_TRACE_TIMERS	(0x00004000)	// "Timers"		Timer operations.
#define	DTL_TRACE_ASYNC		(0x00008000)	// "Asynchronous"	Asynchronous operations.
#define	DTL_TRACE_UNSOL		(0x00010000)	// "Unsolicited"	Unsolicited message packets.
#define	DTL_TRACE_WINLINX1	(0x00200000)	// "WinLinx1"		Infrequently occurring WinLinx server events.
#define DTL_TRACE_WINLINX2	(0x00800000)	// "WinLinx2"		Frequently occurring WinLinx server events.
#define DTL_TRACE_RSLINX1	(0x00100000)	// "RSLinx1"		Infrequently occurring RSLinx server events.
#define DTL_TRACE_RSLINX2	(0x00400000)	// "RSLinx2"		Frequently occurring RSLinx server events.
#define	DTL_TRACE_WHOACTIVE	(0x00000080)	// "WhoActive"		WhoActive events.
#define	DTL_TRACE_00000400	(0x00000400)
#define	DTL_TRACE_00000800	(0x00000800)
#define	DTL_TRACE_00001000	(0x00001000)
#define	DTL_TRACE_02000000	(0x02000000)
#define	DTL_TRACE_04000000	(0x04000000)
#define	DTL_TRACE_08000000	(0x08000000)
#define	DTL_TRACE_10000000	(0x10000000)
#define	DTL_TRACE_20000000	(0x20000000)
#define	DTL_TRACE_40000000	(0x40000000)
#define	DTL_TRACE_80000000	(0x80000000)

#pragma pack(pop)

#endif  /* __DTL */

