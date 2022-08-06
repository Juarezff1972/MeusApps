/*    (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef LC60
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <ndt.h>

extern int NWopen( const char *name, int mode, ... );

/*_________________________ CreateQueueJobAndFile ____________________________
|
|   Output:                      0  --  SUCCESSFUL
|               Netware Error Code  --  UNSUCCESSFUL
|
|   Comments:   This function enters a new job into a queue.  It may be made by
|               any station that is security equivalent to one of the objects
|               listed in the target queue's Q_USER property.
|____________________________________________________________________________*/


int CreateQueueJobAndFile(queueID, job, fileHandle)
long        queueID;     /* Object ID of the queue                          */
JobStruct  *job;         /* Points to the job record.  The user must fill in
                            targetServerIDNumber, targetExecutionTime, jobType
                            jobControlFlags, textJobDescription, and client-
                            RecordArea                                      */
int        *fileHandle;  /* File handle for the NETQ file created           */
{
BYTE    sendPacket[263], receivePacket[56];
static char fileName[5] = "NETQ";
int     ccode;



    *((int *)sendPacket) = 261;
    sendPacket[2] = 104;
    *((int *)receivePacket) = 54;
    *((long *)(sendPacket + 3)) = LongSwap( queueID );
    job->targetServerIDNumber = LongSwap( job->targetServerIDNumber );
    job->jobType = IntSwap( job->jobType );
    memmove( sendPacket + 7, (char *)job, (WORD)256 );

    ccode = _ShellRequest( (BYTE)227, sendPacket, receivePacket );
    if( ccode )
        return( ccode );

    memmove( (char *)job, receivePacket + 2, (WORD)54 );
    job->clientIDNumber = LongSwap( job->clientIDNumber );
    job->targetServerIDNumber = LongSwap( job->targetServerIDNumber );
    job->jobNumber = IntSwap( job->jobNumber );
    job->jobType = IntSwap( job->jobType );
    job->serverIDNumber = LongSwap( job->serverIDNumber );
    #ifdef __WATCOMC__
    *fileHandle = NWopen( fileName, O_RDWR ) ;
    #else
    *fileHandle = open( fileName, O_RDWR ) ;
    #endif
    if( *fileHandle == -1 )
      ccode = errno;

    return( ccode );
}
