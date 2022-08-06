/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwprint.h>

/**** DeletePrintQueueEntry -- removes a job from a print queue
* Input:    int printerNumber;  The number of the printer that
*                               should have the job removed from
*                               its queue.
*           BYTE jobNumber;     The job that should be deleted
*                               from the print queue.
*
* Output:   Returns a 0 if successful or a network error code if unsuccessful.
*
* Comments:
*   This routine deletes an entry from a print queue.  The job number of the
*   entry to be deleted should be obtained by calling GetPrintQueueEntry.
****/

int DeletePrintQueueEntry(printerNumber, jobNumber)
int printerNumber;
BYTE jobNumber;
{
char    sendPacket[5];
char    receivePacket[2];

    sendPacket[2] = 5;
    sendPacket[3] = (char)((BYTE)printerNumber);
    sendPacket[4] = jobNumber;
    *((int *)sendPacket) = 3;

    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)224, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
