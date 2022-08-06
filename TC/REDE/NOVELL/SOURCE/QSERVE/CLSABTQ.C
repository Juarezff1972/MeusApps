/*      COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.   */
#include <fcntl.h>
#include <errno.h>
#ifndef LC60
#include <io.h>
#endif
#include <nitq.h>
#include <nwmisc.h>
#include <ndt.h>

/*_____________________  CloseFileAndAbortQueueJob _________________________
|
|  Output:    0                   --  SUCCESSFUL
|             network error code  --  UNSUCCESSFUL
|
|   Comments:   This function closes a queue job and marks it ready for
|               execution.  This call may be made only by the station that
|               created the queue job.
|____________________________________________________________________________*/

int CloseFileAndAbortQueueJob(queueID, jobNumber, fileHandle)
long queueID;     /* Object ID of the queue                                  */
WORD jobNumber;   /* Number of the job to be closed                          */
int fileHandle;   /* Handle of the file to be closed                         */
{
int  ccode = 0;

   if ( fileHandle != -1 )
      {
      ccode = close( fileHandle );
      if( ccode )
         {
         return( errno );
         }
      }

   if( !ccode )
      ccode = RemoveJobFromQueue( queueID, jobNumber );
   
   return( ccode );
}
