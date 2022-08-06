/*      COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.   */
#include <fcntl.h>
#include <errno.h>
#ifndef LC60
#include <io.h>
#endif
#include <nwlocal.h>
#include <nwmisc.h>
#include <nitq.h>
#include <ndt.h>


/*_________________________ CloseFileAndStartQueueJob ______________________
|
|  Output:    0                   --  SUCCESSFUL
|             network error code  --  UNSUCCESSFUL
|
|  Comments:   This function closes a queue job and marks it ready for exec-
|              ution.  This function can be made only by the station that
|              created the queue job.
|_________________________________________________________________________*/

int CloseFileAndStartQueueJob(queueID, jobNumber, fileHandle)
long queueID;    /* Object ID of the queue where the specified job was placed */
WORD jobNumber;  /* Number of the job that needs to be closed                 */
int  fileHandle; /* Handle of the job file to close and start                 */
{
BYTE sendPacket[9], receivePacket[2];
int  ccode = 0;


   if( fileHandle != -1 )
      {
      ccode = close( fileHandle );
      if( ccode )
         return( errno );
      }

   *((WORD *)sendPacket) = 7;
   sendPacket[2] = 105;
   *((long *)(sendPacket + 3)) = LongSwap(queueID);
   *((WORD *)(sendPacket + 7)) = IntSwap(jobNumber);
   *((WORD *)receivePacket) = 0;
   ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
   return( ccode );
}
