/*      COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.   */
#include <fcntl.h>
#include <errno.h>
#ifndef LC60
#include <io.h>
#endif
#include <nitq.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <ndt.h>


/*__________________  AbortServicingQueueJobAndFile __________________________
|
|  Output:    0                   --  SUCCESSFUL
|             network error code  --  UNSUCCESSFUL
|
|
|  Comments:  This routine signals the queue management software that a job
|             cannot be completed successfully.
|_____________________________________________________________________________*/

int AbortServicingQueueJobAndFile(queueID, jobNumber, fileHandle)

long queueID;      /* Object ID of the queue where the aborted job is placed  */
WORD jobNumber;    /* Number of the aborted job                               */
int  fileHandle;   /*                                                         */
{
int     ccode, rcode;
BYTE    sendPacket[9], receivePacket[2];

    rcode = (fileHandle == -1) ? 0 : close( fileHandle );
    if( rcode )
      return( errno );
    *((WORD *)sendPacket) = 7;
    sendPacket[2] = 115;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)(sendPacket + 7)) = IntSwap(jobNumber);
    *((WORD *)receivePacket) = 0;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (!ccode)
        ccode = rcode;
    return( ccode );
}
