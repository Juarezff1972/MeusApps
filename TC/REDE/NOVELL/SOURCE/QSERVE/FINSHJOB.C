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

/*______________________ FinishServicingQueueJobAndFile _______________________
|
|   Output:                   0  --  SUCCESSFUL
|            Network Error Code  --  UNSUCCESSFUL
|
|   Comments:   This function signals the queue management software that it has
|               successfully completed servicing a job.  This call can be made
|               only by a queue server that has previously accepted a job for
|               service.
|____________________________________________________________________________*/

int FinishServicingQueueJobAndFile(queueID, jobNumber, charge, fileHandle)
long queueID;
WORD jobNumber;
long charge;
int  fileHandle;
{
int     ccode;
int     rcode;
BYTE    sendPacket[13];
BYTE    receivePacket[2];

    rcode = (fileHandle == -1) ? 0 : close( fileHandle );
    if( rcode )
      return( errno );
    *((WORD *)sendPacket) = 11;
    sendPacket[2] = 114;
    *((long *)(sendPacket + 3)) = LongSwap(queueID);
    *((WORD *)(sendPacket + 7)) = IntSwap(jobNumber);
    *((long *)(sendPacket + 9)) = LongSwap(charge);
    *((WORD *)receivePacket) = 0;
    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if( ccode == 0 )
        ccode = rcode;
    return( ccode );
}
