#ifndef __PBAPI_H__ 
#define __PBAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------	*/
/* PRIMEBASE API HEADER FILE												*/
/*																			*/
/* 26 June 2000																*/
/*																			*/
/* SNAP Innovation GmbH														*/
/* Grosse Elbstrasse 39														*/
/* 22767 Hamburg															*/
/* http://www.snap.de														*/
/*																			*/
/* This header file may be freely used and changed by developers.			*/
/* It contains the prototypes and specifications for the PrimeBase API.		*/
/*																			*/
/* ------------------------------------------------------------------------	*/
/* Return values fron API function calls. */
#define PB_NODATA		2			/* Call completed successfully, no data returned.*/
#define PB_EXEC			1			/* The call is still being executed.	*/
#define PB_OK			0			/* Call completed successfully.			*/
#define PB_ERROR		-1			/* Execution ended in error.			*/
#define PB_CANCELED		-2			/* An asynchronous call was canceled.	*/

/* Data type codes: */

#define PB_BOOLEAN		1			/* 1-byte value, 0 = FALSE, 1 = TRUE.	*/
#define PB_SMINT		2			/* 2-byte signed integer value.			*/
#define PB_INTEGER		3			/* 4-byte signed integer value.			*/
#define PB_SMFLOAT		4			/* 4-byte floating point value.			*/
#define PB_FLOAT		5			/* 8-byte floating point value.			*/
#define PB_DATE			6			/* 4-byte DALDateRec value.				*/
#define PB_TIME			7			/* 8-byte DALTimeRec value.				*/
#define PB_TIMESTAMP	8
#define PB_CHAR			9
#define PB_DECIMAL		10
#define PB_MONEY		11
#define PB_VCHAR		12
#define PB_VBIN			13
#define PB_LCHAR		14
#define PB_LBIN			15

#define PB_SINT1		19			/* One byte signed integer value.		*/
#define PB_TINYINT		20			/* One byte unsigned integer value.		*/
#define PB_UINT2		21			/* An unsigned 2-byte integer.			*/
#define PB_UINT4		22			/* An unsigned 4-byte integer.			*/
#define PB_BIN			25			/* fixed length binary value.			*/
#define PB_CSTRING		26			/* Zero terminated character string.	*/

#define PB_UNICODE		27			/* Zero terminated double byte strings  */

/* Wait time values (>= 0 is hundredths of seconds): */
#define PB_FOREVER		-1
#define PB_DEFAULT		-2

/* ------------------------------------------------------------------------	*/

/* ------------------------------------------------------------------------	*/
#define PBI_USER_TYPE		65	/* The starting point for user defined types. */

#define PB_FETCH_FIRST		0
#define PB_FETCH_NEXT		-1

/* PrimeBase Cursor state flags: */
#define PB_CURSOR_FREE		0
#define PB_CURSOR_UNBIND	1
#define PB_CURSOR_REBIND	2

/* PrimeBase Server types: */
#define	PB_DATA_SERVER		1
#define PB_OPEN_SERVER		2

/* PrimeBase communication protocols: */
#define PB_TCP				1	/* Recomended! Use TCP protocol. (Good for local and remote communication.) */
#define PB_SHARED_MEM		2	/* Use Shared Memory. (Good for local communications only.)*/
#define PB_ADSP				3	/* Apple data stream protocol. Where supported.*/
#define PB_PROGRAM_LINKING	4	/* Program linking. Mac platforms only.*/
#define PB_RUNTIME			5	/* Connecting to a runtime server.*/

/* The following are used in PBIExecute()*/
#define PB_NTS				-1	/* Null terminated string flag for PBIExecute()*/
#define PB_EXECUTE_NOW		0	/* Indicates  the end of the text and execution can begin.*/
#define PB_EXECUTE_LATER	1	/* Indicates that there is more text to come.*/

/* Data length info for NULL data*/
#define PB_NULL_DATA		(0XFFFFFFFF)
			
/* ------------------------------------------------------------------------	*/
/* DATA TYPES																*/
/* ------------------------------------------------------------------------	*/
typedef struct PBDateRec {
	short				year;		/* 2-byte year value.					*/
	unsigned char		month;
	unsigned char		day;
} PBDateRec, *PBDatePtr;

typedef struct PBTimeRec {
	unsigned char		hour;
	unsigned char		min;
	unsigned char		sec;
	unsigned char		hu;
} PBTimeRec, *PBTimePtr;

/* 
NOTE for DAL API Users: The format of PBDateTimeRec
differs from DALTimeStampRec, the time and date are swapped. 
*/
typedef struct PBDateTimeRec {
	PBDateRec			date;
	PBTimeRec			time;
} PBDateTimeRec, *PBDateTimePtr;

typedef struct PBBlobRec { 	/* The content of the blob record is not directly*/
	long blob_info[16]; 	/* accessable by the user.*/
} PBBlobRec, *PBBlobRecPtr;

typedef	struct PBVarType
{
	unsigned short		length;
	char				data[1]; /* Variable length. The actual length is determined by the 'length' field.*/
} PBVarType, *PBVarPtr;


#define PBVarcharType		PBVarType
#define PBVarcharPtr		PBLVarPtr
#define PBVarbinrType		PBVarType
#define PBVarbinPtr			PBLVarPtr

/*--------------------*/

typedef struct PBConnInfo
{
	unsigned long	client_version;
	unsigned long	server_version;
	char	server_name[32];
	char	server_brand[32];
} PBConnInfo, *PBConnInfoPtr;


typedef struct PBDataFmt
{
	long	len;
	short	type;
	short	scale;
	
	short	precision;
	void	*user_info;
} PBDataFmt, *PBDataFmtPtr;

typedef struct PBColumnInfo
{
	char			name[32]; 		/* NULL terminated.						*/
	unsigned long	reserved;		/* Reserved for future use.				*/
	short			width; 			/* Display size if fetched as a string.	*/
	
	/* The following fields are for future use. 
	As of PrimeBase 4.0 they are not yet filled in.*/	
	signed char		can_contain_nulls;
	signed char		case_sensitive;
	signed char		updateable;
	char			table[32];
	char			owner[32];
	char			database[32];
} PBColumnInfo, *PBColumnInfoPtr;

/* Cursor Modes. */
#define PB_CURSOR_UNKNOWN		0
#define PB_CURSOR_SCROLLABLE	1	/* The cursor can be positioned.*/
#define PB_CURSOR_UPDATEABLE	2	/* Can be used in UPDATE CURRENT OF statement.*/

typedef struct PBCursorInfo
{
	char			name[32];
	long			rows;
	long			columns;
	signed char		mode;
} PBCursorInfo, *PBCursorInfoPtr;

/* Data conversion function pointer. */
typedef short (*PBConvFuncType)(long session_id, void *src, PBDataFmtPtr sfmt, void *dst, PBDataFmtPtr dfmt, unsigned long *size);


/* Callback function type used for asynchonous completion notification. */
typedef void (*PBCallBackFuncType)(void *user_data, short result, long primary_err, long secondary_err, char *msg);

/* The default name of the API tracing log. */
#define DFLT_GLOBAL_TRACE_LOG	"PBITracing.log"


#ifndef DLLENTRY
#define DLLENTRY
#endif

extern DLLENTRY short PBIInit(int global_api_tracing);

extern DLLENTRY void PBIDeinit(void);

extern DLLENTRY void PBIGetError(long sessid, long *perr, long *serr, char *msg, long size);

extern DLLENTRY short PBIConnect(long 	*sessid,
						char 			*pbserver,
						short 			server_type,
						short 			protocol,
						char 			*connection_options,
						char 			*user,
						char 			*passwd,
						char 			*database);

extern DLLENTRY void PBIDisconnect(long sessid);

extern DLLENTRY short PBIConnectionInfo(long session_id, PBConnInfoPtr info);

extern DLLENTRY short PBIExecute(
						long			session_id, 
						char			*text,
						long			len,
						short			execute,
						long			*rows_effected,
						PBCallBackFuncType	callback,
						void			*user_data);

extern DLLENTRY short  PBIPoll(
						long			session_id, 
						long			timeout);
	
extern DLLENTRY short PBIGetCursorID(long session_id, 
					char 			*cursor_name, 
					unsigned long 	*cursor_id);

extern DLLENTRY void  PBICancel(long session_id);

extern DLLENTRY short PBISetCursorState(long session_id, 
					unsigned long 	cursor_id, 
					long 			flag);

extern DLLENTRY short PBIColumnInfo(long session_id, 
					unsigned long 	cursor_id, 
					long 			col,
					PBDataFmtPtr 	type, 
					PBColumnInfoPtr 	info);
					 
extern DLLENTRY short PBIBindColumn(long session_id, 
					unsigned long 	cursor_id, 
					long 			col, 
					PBDataFmtPtr 	type, 
					void 			*storage, 
					long 			s_inc, 
					void 			*info,
					long 			i_size, 
					long 			i_inc);

extern DLLENTRY short PBIFetchRow(long session_id, 
					unsigned long 	cursor_id, 
					long 			*rows, 
					long 			position, 
					short 			*data_truncated,
					PBCallBackFuncType	callback,
					void			*user_data);

extern DLLENTRY short PBINewBlob(long session_id, PBBlobRecPtr	vblob);

extern DLLENTRY void PBIFreeBlob(long session_id, PBBlobRecPtr	vblob);

extern DLLENTRY short PBIGetBlobData(
					long			session_id, 
					PBBlobRecPtr	blobid, 
					unsigned long	offset,		
					char			*buffer, 	
					unsigned long	*size,
					PBCallBackFuncType	callback,
					void			*user_data); 
					
extern DLLENTRY short PBIPutBlobData(
					long			session_id, 
					PBBlobRecPtr	blobid, 
					unsigned long	offset,		
					char			*buffer, 	
					unsigned long	length);

extern DLLENTRY short PBIGetColumnData(
					long			session_id, 
					unsigned long	cursor_id, 
					long			col, 
					PBDataFmtPtr 	type, 		
					void			*storage, 	
					long			*info,		
					short			*data_truncated);
					
extern DLLENTRY short PBIGetValue(
					long			session_id, 
					char			*identifier, 
					PBDataFmtPtr 	type, 		
					void			*storage, 	
					unsigned long	*info,		
					short			*data_truncated);

extern DLLENTRY short PBIPutValue(
					long			session_id, 
					char			*identifier, 
					PBDataFmtPtr 	type, 		
					void			*storage);

extern DLLENTRY short PBICursorInfo(long session_id, 
					unsigned long cursor_id, 
					PBCursorInfoPtr info);


extern DLLENTRY short PBIGetCursorPosition(long session_id, 
					unsigned long cursor_id, 
					long *pos);

extern DLLENTRY short PBIConvert(long session_id,
					void 			*src,
					PBDataFmtPtr 	src_fmt,
					void 			*dst,
					PBDataFmtPtr 	dst_fmt,
					unsigned long 	*size);
					
extern DLLENTRY short PBISetConvert(long session_id,
					short src_type,
					short dst_type,
					PBConvFuncType func);

extern DLLENTRY short PBIGetConvert(long session_id,
					short src_type,
					short dst_type,
					PBConvFuncType *func);

extern DLLENTRY void PBISetLogName(char *name);

extern DLLENTRY void PBILogStr(char *str);

extern DLLENTRY void PBITrace(long session_id, int tracing);

#ifdef __cplusplus
}
#endif

#ifdef PRIMEBASE_API_SPECS 
===============================================================================================================
NOTES:
===============================================================================================================
Asynchronous Execution Mode:

	When an asynchronous call is made any further asynchronous calls on the same connection will be queued.
	If the application makes a non asynchronous call while an asynchronous call is in progress, the non
	asynchronous may block and wait for the completion of any outstanding asynchronous calls on the same
	connection. Any time the application makes an API call the application must be prepared for the event that
	an asynchronous calls callback may be executed during the call.
	
Examples: 

Included in the PrimeBase SDK is a file called "pbshell.c" which contains examples of the API calls
	and how to use them. Anyone planning on writing an application using the PrimeBase API should have
	a look at this file. pbshell.c has been designed so that it can be easily extended by application
	builders to include their own tests.
	
Reporting problems:

	If a bug is suspected in the PrimeBase Virtual Machine or the PrimeBase API you can help speed up 
	the problem resolution process by activating the PrimeBase API logging and sending a log showing
	the calls to the API during which the problem occurs. Ideally if you could create a test which could
	be added to the pbshell.c file to reproduce the problem it would be greatly appreciated and you can be
	guaranteed a prompt response.

===============================================================================================================
CALLBACK FUNCTIONS
===============================================================================================================
void User_CallBackFunc(void *user_data, short result, long primary_err, long secondary_err, char *msg)

PARAMETERS:

user_data:		A reference to the user data passed into the PrimeBase API function that was called
				asynchronously.
								
result: 		The completion result from the PrimeBase API call:

					PB_NODATA:	Call completed successfully, no data returned.
					PB_OK:		Call completed successfully.
					PB_ERROR:	Execution ended in error.
					PB_CANCELED: The call was canceled.

primary_err:	If 'result' is PB_ERROR 'primary_err' will contain the primary error number.

secondary_err:	If 'result' is PB_ERROR 'secondary_err' will contain the secondary error number.

msg:			If 'result' is PB_ERROR 'msg' will be a zero terminated error message. 

DESCRIPTION:
 
This is the user supplied callback function used for asynchronous completion notification. The following 
API functions take an PBCallBackFunc pointer as  a parameter:
	PBIExecute()
	PBIFetchRow()
	PBIGetBlobData()

These are all functions that may have to send a request to the server and then wait for a reply. When
executed in asynchronous mode rather then waiting on the server they will complete with a return code
of PB_EXEC and the application must call PBIPoll() to check for completion. 

Execution of the callback may occur from within the API call if the function could be completed with out
having to wait on the server. In this case the function will still return PB_EXEC even though the callback
has already been called.

===============================================================================================================
short User_ConvFunc(long session_id, void *src, PBDataFmtPtr sfmt, void *dst, PBDataFmtPtr dfmt, unsigned long *size)

DESCRIPTION:
 
This is the user supplied conversion function that the application can register via the PBISetConvert() API
call to be called when a specific data type conversion takes place.

See the description of PBIConvert() for details.

===============================================================================================================
PRIMEBASE API FUNCTIONS
===============================================================================================================
short PBIInit(int global_api_tracing)

PARAMETERS:

global_api_tracing:		A boolean flag to activate API tracing on all connections.
						By activating tracing in PBInit() you will also be able
						to trace the calls to PBIConnect(). Unless PBISetLogName()
is called to set the log file to be used, all logging will
						be written to a file called "pbvm.log" (PrimeBase Virtual
						Machine Log) in the applications current working directory.
								
RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

This function initializes the PrimeBase API. It should be called once before any other
PrimeBase API call is made.


===============================================================================================================
void PBIDeinit(void)

DESCRIPTION:

This function deinitializes the PrimeBase API. It should be called once after all other
PrimeBase API call have been made. Once called the user should call PBIInit() before making
any further calls to the PrimeBase API.


===============================================================================================================
void PBIGetError(long session_id, long *perr, long *serr, char *msg, long size)

PARAMETERS:

session_id:		The session id returned by PBIConnect() or zero if the error occurred in a
				PrimeBase API call that was not associated with a session such as PBIInit().
				
perr:			A pointer to a long value in which will contain the primary error is returned.
				The parameter may be NULL if the caller is not interested in the primary error number.

serr:			A pointer to a long value in which will contain the secondary error number will be
				returned. The parameter may be NULL if the caller is not interested in the secondary 
				error number.

msg:			A reference to a buffer into which the zero terminated error message is written. 
				If the error mesage with zero terminator is greater then the buffer size then
				the message is truncated and zero terminated.
				This parameter may be NULL if the caller is not interested in the error message. 

size:			The size in bytes of the buffer referenced by 'msg'.

RETURN VALUES:

PB_OK:			Error information has been returned.

PB_NODATA:		There was no error reported on 'session_id'.

				
DESCRIPTION:

This function returns error information from the last PrimeBase API call for 'session_id'. Once
called the error message is cleared. Error messages are not stacked, only the error, if any, from
the last call using 'session_id' is available. As a result, you should always call PBIGetError()
right after any PrimeBase API call that returns the value PB_ERROR.


===============================================================================================================
short PBIConnect(long 	*session_id,
				char 	*pbserver,
				short 	server_type,
				short 	protocol,
				char 	*connection_options,
				char 	*user,
				char 	*passwd,
				char 	*database)


PARAMETERS:

session_id:		A pointer to a long in which the session id is be returned. The session id is used 
				in all other PrimeBase API calls requiring a session id to refer to this session.
				
pbserver:		The name of the server to connect to. For example "PrimeServer". If 'pbserver'
				is NULL then a session will be opened but it will not be connected to a
				server.

server_type:	The type of server to connect to. There are currently two types of PrimeBase
				servers:
				
					PB_DATA_SERVER: This is the PrimeBase data server which is a relational
						database engine.  

					PB_OPEN_SERVER: This is the PrimeBase open server which is a server that
						can be used to connect to a database engine or be programmed to
perform specific tasks such as acting as a file server etc.. Exactly
						what the Open server does and what subset of PrimeBase Talk commands
						and SQL it implements is dependent on the open server plugin that it
						has loaded. Please see the documentation for the PrimeBase Open Server 
						for more details.

protocol:		The communication protocol to use to connect to the server.
					
					PB_TCP: Uses the TCP protocol. This is the recommended protocol. It can be used
						to communicate with both remote and local servers and is available on all
						platforms.
						
					PB_SHARED_MEM: Use shared memory communications. This can be used for local
						communications only. On some platforms shared memory communications can be
						slightly faster than TCP. The user can test the communications speed by using
						the different communications protocols and executing the following test.
						
							int t, cnt;
							cnt = 0;
							t = $now() + 10;
							while ( t  > $now()) {
								cnt++;
								describe databases;
							}
							print cnt, " Describe databases done in 10 seconds.";
							
					PB_ADSP: Use the Apple data stream protocol (ADSP). This is only supported on
						Apple Macintosh machines. 
						
					PB_PROGRAM_LINKING: Use Apple Program Linking. This is only supported on
						Apple Macintosh machines. 
						
					PB_RUNTIME: The application has been linked to a PrimeBase runtime library
						and the values of 'pbserver' and 'server_type' can be ignored.

connection_options:	Any protocol specific options required to connect to the PrimeBase server.

					PB_TCP: The IP address of the host machine on which the PrimeBase server
						is running. If 'connection_options' is blank or NULL then the PrimeBase 
						server is assumed to be running on the same host as the client application.
						
					PB_SHARED_MEM: Requires no connection options. 	'connection_options' is ignored.
					
					PB_ADSP: The apple zone in which the PrimeBase server is running. 
if 'connection_options' is blank or NULL then the PrimeBase server is assumed 
						to be running in the same zone as the client application.
						
					PB_PROGRAM_LINKING: The apple zone in which the PrimeBase server is running. 
						if 'connection_options' is blank or NULL then the PrimeBase server is assumed 
						to be running in the same zone as the client application.
						
					PB_RUNTIME: The location of the database root for the databases to be used.
						You can think of the database root as the directory of folder in which the
						PrimeBase server would be running if you where to access the database via
						a normal server. For example, on unix,  if you had installed a PrimeBase 
						server in '/home/PrimeBase/server1' then  this would be your  database root 
						and you would expect to find the directories 'db' and 'setup' in this location.
						
						If the application is running in the database root, i.e. the 'db' and 'setup'
						directories are in its current working directory, then a NULL pointer can be
						passed in as connection_options.
						
						
user:				The user to log into the server as.

passwd:				The users password. If the user has no password then pass in a NULL pointer
					for 'passwd'.	
					
database:			The name of the database to open. If a NULL pointer is passed in as 'database'
					then the connection will be made without opening a database. In this case the
					user name and password given will not be verified until the application tries
					to open a database. 		
						
RETURN VALUES:

PB_OK:			Every thing is ok, the connection has been established.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

				
DESCRIPTION:

This function establishes a connection to a PrimeBase server and returns a unique
session id by which to refer to it. If a database is specified it will be opened
for the caller. 


===============================================================================================================
void PBIDisconnect(long session_id)

PARAMETERS:

session_id:		The session id returned by PBIConnect() .

DESCRIPTION:

This function disconnects a session opened by PBIConnect() and frees any resources associated with
it.


===============================================================================================================
extern DLLENTRY short PBIConnectionInfo(long session_id, PBConnInfoPtr info);

PARAMETERS:

session_id:		The session id returned by PBIConnect().

info:			A pointer to a connection info structure in which the connection information is
				returned.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

This function returns the following information in the structure pointed to by 'info'.

	client_version: A 4 byte value representing the version of the PrimeBase virtual machine.
	
	server_version: A 4 byte value representing the version of the PrimeBase server that the connection
					is connected to.

	server_name: 	The name of the server that the connection is connected to.

	server_brand: 	The brand name of the server that the connection is connected to. If the connection is to
					a PrimeBase Data Server then the brand name will always be "PrimeBase". If the connection
					is to a PrimeBase Open Server then the brand name will be the brand name supplied by the
					PrimeBase Open Server plugin that the Open Server has loaded.
					

===============================================================================================================
short PBIExecute(
	long			session_id, 
	char			*text,
	long			len,
	short			execute,
	long			*rows_effected,
	PBCallBackFuncType	callback,
	void			*user_data)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

text:			A program segment.

len:			The length of text. If 'len' = PB_NTS then text is assumed to be a null terminated
				string.
				
execute:		A flag which  can contain one of the following values:
					PB_EXECUTE_NOW: The program segment passed in is concatenated to any previous 
						unexecuted segments and the resulting program is executed.

					PB_EXECUTE_LATER: The program segment passed in is concatenated to any previous 
						unexecuted segments and the function is to return with out executing the result.
						The values of the parameters 'rows_effected', 'callback', and 'user_data' is
						ignored in this case.
						
rows_effected	A reference to a long value in which the rows effected by the program just executed
				will be returned if applicable. This parameter can be set to NULL if the user is not
				interested in this information. 
				
				If 'rows_effected' is not null and the caller has supplied a callback routine then the 
				location referenced by 'rows_effected' will be set when the callback is called and so 
				it is up to the caller to make sure that 'rows_effected' remains valid until after the 
				callback is called.

callback:		For synchronous execution the 'callback' parameter should be NULL. If not NULL then execution
				will be done asynchronous and 'callback' is a pointer to a callback function which will be
				called on completion. Please refer to the descriptions of Asynchronous execution for more details.
				
				
user_data:		A pointer to user data to be passed into the callback routine apon completion. If no 
				callback routine is supplied then this parameter is ignored.
								
RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_EXEC:		Execution has started and the caller will be notified of completion via the callback 
				routine.

PB_ERROR:		An error occurred. Call PBIGetError() for details.


DESCRIPTION:

This function builds a PrimeBase Talk program consisting of one or more statements and then executes it if the
'execute' parameter is set to PB_EXECUTE_NOW. Depending on if a callback is supplied, this will either be done
synchronously or asynchronously. When done asynchronously the caller must call PBIPoll() until the callback
is called to indicate completion.

===============================================================================================================
short  PBIPoll(long	session_id,  long timeout)
	
PARAMETERS:

session_id:		The session id returned by PBIConnect().

timeout:		This is the maximum time to wait for asynchronous tasks to complete for the session.
				The time given is in hundredths of seconds. If 'timeout' is zero then asynchronous tasks
				are checked once and the function returns immediately. If 'timeout' is PB_FOREVER then				
				the function will not return until all asynchronous tasks have completed. 
				

RETURN VALUES:

	This function returns the number of asynchronous tasks completed.


DESCRIPTION:

This function is used to poll for asynchronous task completion. Callback routines will only be called from
within this function. 

===============================================================================================================
void  PBICancel(long session_id)
	
PARAMETERS:

session_id:		The session id returned by PBIConnect().

DESCRIPTION:

This function cancels all  asynchronous tasks for the session.

===============================================================================================================
short PBIGetCursorID(long session_id,char *cursor_name, unsigned long *cursor_id)
	
PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_name:	The name of the cursor for which the ID is to be returned. If cursor_name is NULL then
				the cursor id of the default cursor is returned. 
				
cursor_id:		A pointer to an unsigned long into which the cursor id is returned.
				

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.


DESCRIPTION:

This function returns the cursor id for a cursor. The cursor id can then be used in other PrimeBase
API calls that require a cursor id. After the application is finished with the cursor it is important to call  
PBISetCursorState() for the cursor id with the flag PB_CURSOR_FREE to release all resources associated with the
cursor id.

Example:

PBIExecute(session_id,"select * from mytab into my_cursor;", PB_NTS, PB_EXECUTE_NOW, NULL, NULL, NULL);
PBIGetCursorID(session_id, "my_cursor", &cursor_id);

PBIFetchRow(session_id, cursor_id,&rows, PB_FETCH_FIRST, NULL, NULL, NULL);
PBIGetColumnData(session_id, cursor_id,1, NULL, buffer, NULL, NULL);

PBISetCursorState(session_id, cursor_id, PB_CURSOR_FREE);

===============================================================================================================
short PBICursorInfo(long session_id, unsigned long cursor_id, PBCursorInfoPtr info)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().

info:			A pointer to a CursorInfo structure to be filled out.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.


DESCRIPTION:

The application can use this function to get cursor information structure which has the following fields:
	 name: 		The name of the cursor. (In case you forgot it?)
	 rows: 		The number of rows in the cursor. This will only be valid if the select was
	 			done for extract.
	 columns: 	The number of columns in the cursor.
	 mode: 		The mode of the cursor which will be one of the following:
					PB_CURSOR_UNKNOWN: 		A nonscrolling nonupdateable cursor.
					PB_CURSOR_SCROLLABLE: 	The application can position the cursor using PBIGetCursorPosition() but
											cannot update the cursor via an "UPDATE CURRENT OF" statement. 
					PB_CURSOR_UPDATEABLE:	The application can not position the cursor but the cursor name can
											be used in an "UPDATE CURRENT OF" statement.

===============================================================================================================
short PBIColumnInfo(long session_id, 
					unsigned long 	cursor_id, 
					long 			col,
					PBDataFmtPtr 	type, 
					PBColumnInfoPtr 	info)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().

col:			The column number for which the information is requested. Column numbers start at
				1 (i.e. 0 (zero) is not a valid column number).

type:			A pointer to a data format structure to be filled in. If NULL no
				data type information is returned.

info:			A pointer to a column information structure to be filled in. If NULL no
				column information is returned.


RETURN VALUES:

A_OK:			Every thing is ok.

A_ERROR:		An error occured. Call CLGetError() for details.

DESCRIPTION:

After executing a "select" statement and then getting the cursor id via the PBIGetCursorID()
function. The application can use the PBIColumnInfo() function to get information about the
individual columns in the cursor. The  data format structure pointed to by 'type' contains the
following fields:
	type:		The data type of the column, PB_CHAR for example.
	len:		The length of the data. A varchar(11) column will have a len of 11. 
	scale:		The scale of the data type if it is PB_DECIMAL or PB_MONEY.
	
	precision:	The size of the buffer required to ensure that the data is not truncated when fetched without
				conversion.
				
	*user_info:	This field is not effected.
	
The column information structure pointed to by 'info' contains the following fields:
	name: 		The columns name.
	width:		The maximum size of the data if fetched as a C string.
	
	All other fields of this structure are left untouched.
	
===============================================================================================================
short PBISetCursorState(long session_id, unsigned long cursor_id, long flag)
	
PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().
				
flag:			One of the following values:

					PB_CURSOR_FREE: Free the cursor and all resources associated with it. After this any
						further use of 'cursor_id' will result in an error.
						
					PB_CURSOR_UNBIND: Unbind all bound columns for the cursor. See PBIBindColumn().
					
					PB_CURSOR_REBIND: Reset the 'storage' and 'info' bind locations to the start position.
						When the 'i_inc' or 's_inc' parameters of PBIBindColumn() are non zero then
						the location into which the data and/or info is written is incremented after
						each call to  PBIFetchRow(). Calling PBISetCursorState() with flag PB_CURSOR_REBIND
						will reset the these locations to their original settings.
						
						This is equivalent to recalling PBIBindColumn() for each bound column with the
						same parameters again.
						same parameters again.
RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.


DESCRIPTION:

This function sets the state of the cursor associated with the cursor id.
===============================================================================================================					 
short PBIBindColumn(long session_id, 
					unsigned long 	cursor_id, 
					long 			col, 
					PBDataFmtPtr 	type, 
					void 			*storage, 
					long 			s_inc, 
					void 			*info,
					long 			i_size, 
					long 			i_inc)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().
				

col:			The column number for which the information is requested. Column numbers start at
				1 (i.e. 0 (zero) is not a valid column number).

type:			A pointer to a structure containing the information about the format that the 
				data is to be returned as. The 'len' field in the DataFmt structure is the 
				length of the storage supplied for each value. If the data type of the column 
is not convertible to the data type supplied  PBIBindColumn will fail. If type is
				NULL then the data will be returned unconverted and the caller must ensure that 
the storage space supplied for each data item is at least as big as the maximum 
				data length for the column. 
				
				Note that the bind buffer for a data type may require a buffer larger than the 'len' 
				field in the DataFmt structure as returned by PBIColumnInfo(). For example PB_VCHAR and
				PB_VBIN require a buffer 'len' + 2 to ensure no data truncation.
				
				
storage:		If the value of 's_inc' is -1 then 'storage' is a pointer to an array 
				of pointers that point to the location in which that the data is to be stored 
				when it is fetched. 
				
				If the value of 's_inc' is not -1 then 'storage' is a pointer to
				a location in memory large enough to hold one or more items.
				
				If 'storage' is NULL then the column will be unbound and the values of 
				'type', 'size' and 'info' will be ignored.
				
s_inc			This parameter tells the PrimeBase API how to increment the 'storage'  
				pointer after fetching a row. If 's_inc' is -1 then the pointer
				is assumed to be a pointer to arrays of pointers to storage
				locations. If 's_inc' is greater than or equal to zero then the pointer
				'storage' is incremented 's_inc' bytes after each fetch.

info:			If the value of 'i_inc' is -1 then 'info' is a pointer to an array 
				of pointers that point to the location in which the info data is 
				to be stored when after a fetch. 
				
				If the value of 'i_inc' is not -1 then 'info' is a pointer to
				a location in memory large enough to hold one or more items of
				size 'i_size' bytes long.
				
				If the type being returned is not of fixed size then the actual
				length of the data item to be fetched is returned in this location.
				If the actual length of the data was greater than 'type->len' then
				the data will be truncated and only 'type->len' bytes will be returned.
				The user can compare the length in 'info' with 'type->len' to determine
				if data was truncated.
				
				If A null item is returned then storage location will be left
				untouched and info will be set to PB_NULL_DATA.
								
				If 'info' is NULL then the user is not notified of null data items
				or the length of variable length data items.
				
i_size:			The size of the storage space available for column item info. Valid
				sizes are 0, 1, 2, and 4 bytes.
				
i_inc			This parameter tells the  PrimeBase API how to increment the 'info'  
				pointer after fetching a row. If 'i_inc' is -1 then the pointer
				is assumed to be a pointer to arrays of pointers to storage
				locations. If 'i_inc' is greater than or equal to zero then the pointer
				'info' is incremented 'i_inc' bytes after each fetch.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.


DESCRIPTION:

This function binds storage location for a specified cursor into which data will be 
placed when PBIFetchRow() is called. Space can be bound for one or more rows with one call. The 
bindings remain in effect until PBIBindColumn() is called again with the same session 
id, cursor id, and column number. A column can be unbound by calling PBIBindColumn() 
with storage set to NULL. It is the responsibility of the user to ensure that when 
PBIFetchRow() is called that the bound columns contain storage space for all rows 
fetched. The arrays pointed to by 'storage' and 'info' must remain valid until the 
column is unbound or the user no longer wishes to fetch any more data. The user can 
change the contents of the array at any time with out having to call PBIBindColumn() 
again.

example:
/*
 * This example binds space for column 3. Up to 7 rows can be fetched.
 * 
 * By setting the 'type' parameter to NULL we are telling the driver
 * that we what the data to be returned unconverted and that we
 * have supplied storage for the maximum possible value size returned.
 * In our example we are assuming that no value fetched is larger than 32
 * bytes. If we are wrong our program will crash while being demonstrated
 * to our boss and we will be fired.
 */

char space[7][32], *storage[7];
Int4 flags[7], i, rows ;

PBIBindColumn(session_id, my_cursor_id, 3, NULL, storage, -1, flags, 4, 4);
/*
The above is equivalent to the following:
DataFmt fmt;

PBIColumnInfo(long session_id, my_cursor_id, 3, &fmt, NULL);
PBIBindColumn(session_id, my_cursor_id, 3, &fmt, storage, -1, flags, 4, 4);
*/

for (i=0; i<3; i++)
{
	storage[i] = space[i];
}

rows = 3;
PBIFetchRow(session_id, cursor_id,  &rows, FETCH_NEXT, NULL, NULL, NULL); 

/*
 * If we were to call 
 * PBISetCursorState(session_id, cursor_id, PB_CURSOR_REBIND);
 * here, the  next fetch would overwrite the data fetched by the previous fetch.
 */
  
/* Fetch the next 3 rows into the next storage locations. */
rows = 3;
PBIFetchRow(session_id, cursor_id,  &rows, FETCH_NEXT, NULL, NULL, NULL); 


===============================================================================================================					 
short PBIFetchRow(long session_id, 
					unsigned long 	cursor_id, 
					long 			*rows, 
					long 			position, 
					short 			*data_truncated,
					PBCallBackFuncType	callback,
					void			*user_data);
PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().

rows:			The address of a variable containing the number of rows to be fetched.
				Apon successful completion this variable will be set to the number
				of rows actually fetched.
				
position:		The position of the cursor in the result set from which the fetch is 
				to be done. 'position' can be any value greater than zero or one of 
				the following flags:
				
					PB_FETCH_FIRST: Positions the cursor at the first row in the result
						set before performing the fetch. This is the same as setting
						position to one.
						
					PB_FETCH_NEXT: Fetches the next row set starting at the current
						cursor position. If no previous fetch has been done this will 
						fetch the first rows in the result set. If the cursor is
						at the end of the result set PBIFetchRow() return successfully
						and set the variable pointed to by 'rows' to zero.
						
				If 'position' is greater than the number of rows in the
				row set PBIFetchRow() return A_OK and set the variable pointed 
				to by 'rows' to zero.
				
				For an updateable cursor, any value of 'position' other than  
				PB_FETCH_NEXT will result in an error.

data_truncated: The address of a variable which is set to 1 if fetched data has been
				truncated otherwise it is set to 0. If NULL is passed in for this
				parameter then the caller will not be notified when data is truncated.


callback:		For synchronous execution the 'callback' parameter should be NULL. If not NULL then execution
				will be done asynchronous and 'callback' is a pointer to a callback function which will be
				called on completion. Please refer to the descriptions of Asynchronous execution for more details.
				
				
user_data:		A pointer to user data to be passed into the callback routine apon completion. If no 
				callback routine is supplied then this parameter is ignored.
								
RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_EXEC:		Execution has started and the caller will be notified of completion via the callback 
				routine.

PB_ERROR:		An error occurred. Call PBIGetError() for details.



DESCRIPTION:

This function fetches zero or more rows of data into locations specified by the 
PBIBindColumn() function. Only columns that have been bound will be fetched. It is the
responsibility of the user to ensure that for any bound column there are sufficient 
storage locations supplied for the number of rows fetched.

If PBIFetchRow() is executed asynchronously then the user must ensure that the 'rows' pointer and the 'data_truncated'
pointer, if not NULL, remain valid until the callback has been called.


example:
/*
 * This example fetches up to 7 rows of data starting with the third row of data in 
 * the result set. 
*/
Int4 rows;

rows = 7;
	
PBIFetchRow(session_id, cursor_id,  &rows, 2, NULL, NULL, NULL); 

===============================================================================================================
short PBIGetColumnData(long			session_id, 
					unsigned long	cursor_id, 
					long			col, 
					PBDataFmtPtr 	type, 		
					void			*storage, 	
					long			*info,		
					short			*data_truncated)
					
					
PARAMETERS:

session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().


col:			The column number for which the information is requested. Column numbers start at
				1 (i.e. 0 (zero) is not a valid column number).

type:			A pointer to a structure containing the information about the format that the 
				data is to be returned as. The 'len' field in the DataFmt structure is the 
				length of the storage supplied for each value. If the data type of the column 
				is not convertible to the data type supplied  PBIGetColumnData will fail. If type is
				NULL then the data will be returned unconverted and the caller must ensure that 
				the storage space supplied for each data item is at least as big as the maximum 
				data length for the column. 
				
				Note that the bind buffer for a data type may require a buffer larger than the 'len' 
				field in the DataFmt structure as returned by PBIColumnInfo(). For example PB_VCHAR and
				PB_VBIN require a buffer 'len' + 2 to ensure no data truncation.
				
storage:		A pointer to a location in memory large enough to hold the item.
				
info:			If the type being returned is not of fixed size then the actual
				length of the data item to be fetched is returned in this location.
				If the actual length of the data was greater than 'type->len' then
				the data will be truncated and only 'type->len' bytes will be returned.
				The user can compare the length in 'info' with 'type->len' to determine
				if data was truncated.
				
				If A null item is returned then storage location will be left
				untouched and info will be set to PB_NULL_DATA.
								
				If 'info' is NULL then the user is not notified of null data items
				or the length of variable length data items.
				

data_truncated: The address of a variable which is set to 1 if fetched data has been
				truncated otherwise it is set to 0. If NULL is passed in for this
				parameter then the caller will not be notified when data is truncated.


RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

	This function is used to get data from the current row in the cursor indicated by the cursor_id
	parameter. This function has no effect on any binds that have been made to the columns in
	the cursor via calls to PBIBindColumn(). It is possible to bind a column with PBIBindColumn()
	and then call PBIGetColumnData() on the column to get the data into a different buffer than
	what was specified in PBIBindColumn(), possibly doing a type conversion on it at the same
	time.
	

===============================================================================================================
short PBIGetCursorPosition(long session_id, 
						unsigned long cursor_id, 
						long *pos)

PARAMETERS:
session_id:		The session id returned by PBIConnect().

cursor_id:		A cursor id returned by PBIGetCursorID().

pos:			A pointer to a variable into which the current cursor position is 
				placed.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

This function gets the current position of the cursor. The first row is at position 1.

===============================================================================================================
short PBIGetValue(long			session_id, 
				char			*identifier, 
				PBDataFmtPtr 	type, 		
				void			*storage, 	
				unsigned long	*info,		
				short			*data_truncated)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

identifier:		A string representing a global variable name that has previously been declared
				and initialized. Only basic types are supported, Getting the value of arrays, cursors,
				and PrimeBase Talk objects is not supported.

type:			A pointer to a structure containing the information about the format that the 
				data is to be returned as. The 'len' field in the DataFmt structure is the 
				length of the storage supplied for each value. If the data type of the variable 
is not convertible to the data type supplied  PBIGetValue will fail. If 'type' is
				NULL then the data will be returned unconverted and the caller must ensure that 
the storage space supplied for the data item is at least as big as the maximum 
				data length for the variable. 
								
storage:		A pointer to a location in memory large enough to hold the item.
				
info:			If the type being returned is not of fixed size then the actual
				length of the data item to be fetched is returned in this location.
				If the actual length of the data was greater than 'type->len' then
				the data will be truncated and only 'type->len' bytes will be returned.
				The user can compare the length in 'info' with 'type->len' to determine
				if data was truncated.
				
				If A null item is returned then storage location will be left
				untouched and info will be set to PB_NULL_DATA.
								
				If 'info' is NULL then the user is not notified of null data items
				or the length of variable length data items.
				

data_truncated: The address of a variable which is set to 1 if fetched data has been
				truncated otherwise it is set to 0. If NULL is passed in for this
				parameter then the caller will not be notified when data is truncated.



RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

An application can use this function to get the value of a variables from a previously executed PrimeBase
program.

example:

/*
 * This program tests the client/server communications speed by seeing how many times it can execute the
 * PrimeBase command "describe databases" in 10 seconds. ("describe databases" returns a cursor from
 * the server with a list of available databases.)
 */
 
 char buf[30];
 int cnt;
 PBDataFmt pb_type = {0};
 char *the_PrimeBase_program = "\
 int t, cnt;\
 timestamp the_date;\
 \
 the_date = $now();\
 cnt = 0;\
 t = $now() + 10;\
 while (t >  $now()) {\
 	describe databases;\
 	cnt++;\
 }\
 ";
 
 
PBIExecute(session_id,the_PrimeBase_program, PB_NTS, PB_EXECUTE_NOW, NULL, NULL, NULL);
PBIGetValue(session_id,"cnt", NULL, &cnt, NULL, NULL);

/* Get the timestamp as a C string. */
pb_type.len = 32;
pb_type.type = PB_CSTRING;
PBIGetValue(session_id,"the_date", &pb_type, buf, NULL, NULL);

printf("On: %s PrimeBase executed %d describe databases in 10 seconds.\n", buf, cnt);


===============================================================================================================
short PBIPutValue(long			session_id, 
				char			*identifier, 
				PBDataFmtPtr 	type, 		
				void			*data)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

identifier:		A string representing a global variable name that has previously been declared.
Only basic types are supported, Setting the value of arrays, cursors, and PrimeBase 
				Talk objects is not supported.

type:			A pointer to a structure containing the information about the format of the 
				data being assigned. If the data type of the data is not convertible to the data type 
				of the variable PBIPutValue will fail. If 'type' is NULL then the data is assumed to
				be in the same format as the data type of the variable. For variable length data
				types you should always supply a non NULL 'type' parameter.
								
data:			A pointer to a buffer containing the data.
				

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

An application can use this function to set the value of a variables from a previously executed PrimeBase
program.

example:

/*
 * This program tests the client/server communications speed by seeing how many times it can execute the
 * PrimeBase command "describe databases" in the number of seconds supplied by the application. 
 * ("describe databases" returns a cursor from the server with a list of available databases.)
 */
 
 char buf[30];
 int cnt, execution_time;
 PBDataFmt pb_type = {0};
 char *the_PrimeBase_program = "\
 int t, cnt;\
 timestamp the_date;\
 \
 the_date = $now();\
 cnt = 0;\
 t = $now() + num_secs;\
 while (t >  $now()) {\
 	describe databases;\
 	cnt++;\
 }\
 ";
 
PBIExecute(session_id,"int num_secs;", PB_NTS, PB_EXECUTE_NOW, NULL, NULL, NULL);
execution_time = 20;
PBISetValue(session_id,"num_secs", NULL, &execution_time);
 
PBIExecute(session_id,the_PrimeBase_program, PB_NTS, PB_EXECUTE_NOW, NULL, NULL, NULL);
PBIGetValue(session_id,"cnt", NULL, &cnt, NULL, NULL);

/* Get the timestamp as a C string. */
pb_type.len = 32;
pb_type.type = PB_CSTRING;
PBIGetValue(session_id,"the_date", &pb_type, buf, NULL, NULL);

printf("On: %s PrimeBase executed %d describe databases in %d seconds.\n", buf, cnt, execution_time);


===============================================================================================================
extern DLLENTRY short PBIGetBlobData(
					long			session_id, 
					PBBlobRecPtr	blobid, 
					unsigned long	offset,		
					char			*buffer, 	
					unsigned long	*size,
					PBCallBackFuncType	callback,
					void			*user_data); 

PARAMETERS:

session_id:		The session id returned by PBIConnect().

blobid:			A pointer to A blob ID data buffer as returned from PBIFetchRow() or PBIGetValue().

offset:			The offset in to the blob from which to read the data. Offsets
				are zero bases.
				
buffer:			A pointer to a buffer into which the data is written. 

size:			On input '*size' contains the size in bytes of the buffer pointed
				to by 'buffer'. On output '*size' contains the size of the blob
				minus the value of 'offset'.

callback:		For synchronous execution the 'callback' parameter should be NULL. If not NULL then execution
				will be done asynchronous and 'callback' is a pointer to a callback function which will be
				called on completion. Please refer to the descriptions of Asynchronous execution for more details.
				
				
user_data:		A pointer to user data to be passed into the callback routine apon completion. If no 
				callback routine is supplied then this parameter is ignored.
								
RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_EXEC:		Execution has started and the caller will be notified of completion via the callback 
				routine.

PB_ERROR:		An error occurred. Call PBIGetError() for details.



DESCRIPTION:

	Blob data is not returned directly in cursors. Instead the cursor contains a blob id. 
	This function can be used to get the actual blob data. Since, in general, there is no way
	that an application can bind a buffer for blob data and be sure that data will not be truncated, 
	the application could do the following:
	
EXAMPLE:
	/* 
	 * This example will assume 1 column has been selected and it is of type PB_LCHAR.
	 */
	 
PBDataFmt pb_type = {0};
char *buf;
short 	was_truncated = 0;
unsigned long blob_size;

	pb_type.type = PB_CHAR;
	pb_type.len = 1024;
	buf = malloc(pb_type.len);
	
	/*Bind the column to a fixed length buffer as CHAR. */
	PBIBindColumn(session_id, my_cursor_id, 1, &pb_type, buf, 0, &blob_size, 4, 0);
	
	/* Do the fetch. */
	PBIFetchRow(session_id, cursor_id, &rows, PB_FETCH_FIRST, &was_truncated, NULL, NULL);

	if (was_truncated) { /* If the data was truncated. */
		PBBlobRec blob_id;
		unsigned long size;
		
		/* Grow the buffer. */
		buf = realloc(blob_size);
		
		/* Get the blob id. */
		PBIGetColumnData(session_id, cursor_id,1, NULL, &blob_id, NULL, NULL);
		
		/* Get the remaining data */
		size = blob_size - pb_type.len;
		PBIGetBlobData(session_id, &blob_id, pb_type.len, buf, &size, NULL, NULL);
		
	}	 
		
					
===============================================================================================================
extern DLLENTRY short PBINewBlob(
					long			session_id, 
					PBBlobRecPtr	blobid)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

blobid:			A pointer to a blob ID data buffer which will contain the id of the new blob.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

	An application can use this function to create a blob id and then write the data to it
	by calling PBIPutBlobData(). 
	
					
===============================================================================================================
extern DLLENTRY void PBIFreeBlob(
					long			session_id, 
					PBBlobRecPtr	blobid)
					
PARAMETERS:

session_id:		The session id returned by PBIConnect().

blobid:			A pointer to a blob ID data buffer which contain the id of a blob allocated by
				PBINewBlob() to be freed. 


DESCRIPTION:

	This function frees a blob id. It should only be used on blob ids allocated by PBINewBlob().	
					
===============================================================================================================
extern DLLENTRY short PBIPutBlobData(
					long			session_id, 
					PBBlobRecPtr	blobid, 
					unsigned long	offset,		
					char			*buffer, 	
					unsigned long	length)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

blobid:			A pointer to a blob ID data buffer as returned from PBIGetValue() or PBINewBlob(). 

offset:			The offset in to the blob to which the data is to be written. If the offset is
				0XFFFFFFFF then the data will be written to the end of the blob. If the offset
				if not 0XFFFFFFFF but beyond the end of the blob an error will be reported. 
				
buffer:			A pointer to a buffer containing the data to be written. 

length:			The length of the data to be written.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

	An application can use this function to  write the data to a blob before inserting it into a table
	or assigning it to a variable.


===============================================================================================================
short PBIConvert(long session_id,
					void 			*src,
					PBDataFmtPtr 	src_fmt,
					void 			*dst,
					PBDataFmtPtr 	dst_fmt,
					unsigned long 	*size)
					
PARAMETERS:

session_id:		The session id returned by PBIConnect().

src:			A pointer to the data to be converted. If 'src' is NULL then the conversion is done
				as if the data to be converted was NULL.

src_fmt:		A pointer to a structure containing the type information of the source data.

dst:			A pointer to the storage location into which the converted data is to be placed.

dst_fmt:		A pointer to a structure containing the type information of the destination data.

size:			A pointer to a 4 byte integer. If 'dst_fmt' points to a variable length data type then 
				'size' will contain the length of the data after conversion. If the converted data
				could not be placed in the 'dst' buffer with out truncation then the data will be 
				truncated and 'size' will contain the untruncated length. The caller can test for 
				truncation by comparing 'dst_fmt->len' and 'size'. 

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

	If the conversion is not supported then a PB_ERROR is returned. If 'src' and 'dst' point to the
	same or overlapping memory locations then the result of the conversion is undefined. The built in
	conversion functions do not convert NULL data. If 'src' is NULL and the call to PBIConvert() would
	have invoked a built in conversion function then PBIConvert() will return PB_OK and the destination
	buffer will remain untouched.


===============================================================================================================
short PBISetConvert(long session_id,
					short src_type,
					short dst_type,
					PBConvFuncType func)

					
PARAMETERS:

src_type		The source type of the conversion routine.

dst_type		The destination type of the conversion routine.

func			A pointer to a conversion routine.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

	This function can be used to set or clear a user conversion function. If 'func' is NULL and if a user 
	defined conversion routine exists it is removed. After this call if there is a built in conversion 
	function for the data types then it will be used. If there is no built in function then the conversion
	from 'src_type' to 'dst_type' will not be allowed. If 'func' is not NULL then after the successful 
	completion of this call all conversions from 'src_type' to 'dst_type' will be done using the conversion 
	function 'func'. This includes conversions done explicitly via a call to 'PBIConvert' and conversions done
	implicitly as a result of binding a column of type 'src_type' to a destination type of type 'dst_type'. 
	The user can replace built in conversion routines with this call or supply conversion routines for user 
	defined data types. The values for the defined data types identifiers must start at PBI_USER_TYPE. 
	NOTE: User installed conversion functions must be prepared to be called with a NULL 'src' buffer when 
	fetch loads a NULL data item into a bound buffer location. 
				
===============================================================================================================
short PBIGetConvert(long session_id,
					short src_type,
					short dst_type,
					PBConvFuncType *func)

PARAMETERS:

src_type		The source type of the conversion routine.

dst_type		The destination type of the conversion routine.

func			A pointer to a conversion routine.

RETURN VALUES:

PB_OK:			Execution completed successfully.

PB_ERROR:		An error occurred. Call PBIGetError() for details.

DESCRIPTION:

	This function can be used to get the user defined conversion function set by a previous call to 
	PBISetConvert(). If there is no user defined conversion function then 'func' will be set to NULL. 
					
===============================================================================================================
void PBISetLogName(char *name)

PARAMETERS:

name:		The name of the log file to use for all future logging. By default the log file is called
			'pbvm.log' (PrimeBase Virtual Machine Log) and it is created in the applications current
			working directory. 

DESCRIPTION:

	This function can be used to set the applications log file name and location. If the log file name is given as
	NULL then no log file will be created. Depending on the values of the PrimeBase Virtual Machine environment 
	variables the PrimeBase Virtual Machine may write messages to the log file. By setting the log file name to
	NULL the application override the environment variables settings and prevent anything from being logged.
					
===============================================================================================================
void PBILogStr(char *str)

PARAMETERS:

str:		A string to be sent to the log file. 

DESCRIPTION:

	This function can be used to add application specific messages to the log file. The application is responsible
	for formatting the string and adding any CR and/or LF characters to the end of the string.
					
===============================================================================================================
extern DLLENTRY void PBITrace(long session_id, int api_tracing)

PARAMETERS:

session_id:		The session id returned by PBIConnect().

api_tracing:	A boolean flag to activate API tracing on the connection. 

DESCRIPTION:

	This function can be used to activate API tracing for a specific connection. When API tracing is activated
	all calls made to the PrimeBase API will bee logged along with their parameters and return values. This is
	a very useful feature which can be used for debugging and reporting PrimeBase API problems to SNAP.
					
===============================================================================================================
#endif 


#endif 
