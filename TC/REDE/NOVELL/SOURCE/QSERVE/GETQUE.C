/*   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwprint.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*________________________ GetPrintQueueEntry _______________________________
|
| Output:		     0	--  SUCCESSFUL
|			   255	--  BAD_PRINTER_ERROR
|
| Comments:
|   This function gets information about an entry in the specified printer's
|   job queue.	This call must be made repetitivily to get all the entries in
|   the print queue.  For the first call the job number should be set to 0,
|   for next calls, the job number returned by the previous call should be
|   used.  The printFlags are: bit 5 (0x20) - Delete the file after printing,
|   bit 6 (0x40) - Expand tabs to spaces, bit 7 (0x80) - Print a banner page.
|   The spoolTime is in the following format: byte 0 - Year (80-99 = 1980-1999,
|   0-79 = 2000-2079), byte 1 - Month (1-12), byte 2 - Day (1-31), byte 3 -
|   Hour (0-23), byte 4 - Minute (0-59), byte 5 - Second (0-59).
|_____________________________________________________________________________*/

int GetPrintQueueEntry(printerNumber, jobNumber, fileName, printFlags,
		       tabSize, numberOfCopies, formType, bannerName,
		       connectionNumber, userObjectID, spoolTime)

int    printerNumber;	  /* Number of the printer that the queue entry should
			     be returned				      */
BYTE  *jobNumber;	  /* Receives the number of the job		      */
char  *fileName;	  /* Receives the name of the file in the print queue
			     (15 characters)				      */
BYTE  *printFlags;	  /* Receives the print flags			      */
int   *tabSize; 	  /* Receives the column width to which tabs will be
			     expanded					      */
int   *numberOfCopies;	  /* Receives the number of copies to be printed      */
int   *formType;	  /* Receives the form type that should be mounted
			     while the job prints			      */
char  *bannerName;	  /* Receives the banner name (15 characters)	      */
WORD  *connectionNumber;  /* Receives the connection number of the workstation
			     that placed the entry into the queue	      */
long  *userObjectID;	  /* Receives the bindery object ID of the user that
			     placed the entry in the print queue	      */
BYTE  *spoolTime;	  /* Receives the date and time that the entry was
			     placed in the print queue (6 BYTES)	      */
{
char  sendPacket[5], receivePacket[84];
int   ccode;


    sendPacket[2] = 4;
    sendPacket[3] = (char)printerNumber;
    sendPacket[4] = *jobNumber;
    *((int *)sendPacket) = 3;

    *((int *)receivePacket) = 82;

    ccode = _ShellRequest((BYTE)224, (BYTE *)sendPacket, (BYTE *)receivePacket);
    if (ccode == 0)
     {
	*jobNumber = receivePacket[2];
       if (fileName != (char *)0)
	{
	   memmove(fileName, receivePacket + 5, 14);
	    fileName[14] = (char)'\0';
	}
       if (printFlags != (BYTE *)0)
	   *printFlags = receivePacket[20];
       if (tabSize != (int *)0)
	   *tabSize = receivePacket[21];
       if (numberOfCopies != (int *)0)
	   *numberOfCopies = receivePacket[23];
       if (formType != (int *)0)
	   *formType = receivePacket[24];
       if (connectionNumber != (WORD *)0)
	   *connectionNumber = receivePacket[25];
       if (spoolTime != (BYTE *)0)
	   memmove(spoolTime, receivePacket + 26, (WORD)6);
       if (bannerName != (char *)0)
	{
	   memmove(bannerName, receivePacket + 47, (WORD)14);
	   bannerName[14] = (char)'\0';
	}
       if (userObjectID != (long *)0)
	    *userObjectID = LongSwap(*((long *)(receivePacket + 79)));
     }
    return (ccode);
}
