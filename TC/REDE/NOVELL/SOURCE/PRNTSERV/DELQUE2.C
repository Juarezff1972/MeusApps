/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */

#include <string.h>
#include <nwlocal.h>
#include <nwprint.h>

/*________________________ DeletePrintQueueEntries _________________________

  Input:    int printerNumber;      The number of the printer that
                                    should have the job removed from
                                    its queue.
            int numberOfJobs;       The number of jobs to be deleted.
            BYTE *jobNumberArray;   A pointer to a character array of
                                    numberOfJobs byte entries of the
                                    job numbers to be deleted.
 
  Output:    0  -- successful 
  network error code  --  unsuccessful.
 
  Comments:
    This routine deletes multiple entries from a print queue.  The job numbers
    of the entries to be deleted should be obtained by calling
    GetPrintQueueEntry.
 _________________________________________________________________________*/

int DeletePrintQueueEntries(printerNumber, numberOfJobs, jobNumberArray)
int   printerNumber, numberOfJobs;
BYTE *jobNumberArray;
{
BYTE    sendPacket[103];
BYTE    receivePacket[2];
int     ccode;
    
    sendPacket[2] = 5;
    sendPacket[3] = (char)( (BYTE)printerNumber );
    memmove( sendPacket + 4, jobNumberArray, numberOfJobs );
    *((int *)sendPacket) = 2 + numberOfJobs;
    
    *((int *)receivePacket) = 0;
    
    ccode = _ShellRequest( (BYTE)224, sendPacket, receivePacket );
    return( ccode );
}

