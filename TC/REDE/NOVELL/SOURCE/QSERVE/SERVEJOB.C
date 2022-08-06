/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef LC60
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <ndt.h>

extern int NWopen( const char *name, int mode, ... );

/*_____________________ServiceQueueJobAndOpenFile____________________________
|
|  Output:    0                   --  SUCCESSFUL
|             network error code  --  UNSUCCESSFUL
|
|  Comments:    This function allows a queue server to select a new job for
|               servicing.  It may be made only by a station that has
|               previously attached itself to the specified queue as a server.
|____________________________________________________________________________*/


int ServiceQueueJobAndOpenFile( queueID, targetJobType, job, fileHandle )

long queueID;            /* Object ID of the queue                          */
WORD targetJobType;      /* Job type that has to be matched                 */
JobStruct *job;          /* Receives the job record of the next available
                            job returned by the queue management software   */
int *fileHandle;         /* The file handle of the job file to be opened    */
{
    BYTE sendPacket[9], receivePacket[56];
    int ccode;

    *((WORD *)sendPacket) = 7;
    sendPacket[2] = 113;
    *((WORD *)receivePacket) = 54;
    *((long *)(sendPacket + 3)) = LongSwap( queueID );
    *((WORD *)(sendPacket + 7)) = IntSwap( targetJobType );
    ccode = _ShellRequest( (BYTE)227, sendPacket, receivePacket );
    if (ccode == 0)
     {
        ccode = ReadQueueJobEntry( queueID,
                                   IntSwap(*((WORD *)(receivePacket + 24)) ),
                                   job );
        if ( !ccode )
         {
            #ifdef __WATCOMC__
            *fileHandle = NWopen( "NETQ", O_RDONLY );
            #else
            *fileHandle = open( "NETQ", O_RDONLY );
            #endif
            if ( *fileHandle == -1 )
                ccode = errno;
         }
     }
    return( ccode );
}
