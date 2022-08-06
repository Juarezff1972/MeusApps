/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwprint.h>
#include <nwlocal.h>

/*___________________________ SetSpoolFlags ________________________________
|                      is queued to the printer
| Input:    char printFlags;        The print flags specifying the
|                                   desired print options.
|           int tabSize;            The column width to which tabs
|                                   should be expanded if the Expand
|                                   tabs bit is set.
|           int printerNumber;      The server printer number on which
|                                   the file should be printed.
|           int numberOfCopies;     The number of copies that should be
|                                   printed.
|           int formType;           The form type that should be mounted
|                                   on the printer when the file is
|                                   printed.
|           char *bannerName;       An optional banner name that should
|                                   be used instead of the file name.
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This routine sets spool flags for the next spool file that is placed in a
|   print queue.  When a file is queued to the printer, these flags are
|   automatically reset to the default values.  The default values are print-
|   Flags = 0, tabSize = 0, printer = 0, copies = 1, formType = 0, and no
|   banner name.  The print flags are bit 5 (0x20)- Delete the file after
|   printing, bit 6 (0x40)- Expand tabs to spaces, bit 7 (0x80)- Print
|   a banner page.  The banner name is optional.  If the print a banner
|   page flag is set but no banner name is given (either by passing NIL
|   as the banner name or passing a pointer to a string whose first character
|   is a null) then the file name will be used as the default banner name.
|
| Cautions:
|   The banner name should be no longer than 15 characters.  If it is longer
|   it will be truncated.
|____________________________________________________________________________*/

int SetSpoolFlags( printFlags, tabSize, printerNumber, 
                   numberOfCopies, formType, bannerName )
char printFlags, *bannerName;
int tabSize, printerNumber, numberOfCopies, formType;
{
    char sendPacket[22], receivePacket[2];

    sendPacket[2] = 2;
    sendPacket[3] = printFlags;
    sendPacket[4] = (char)tabSize;
    sendPacket[5] = (char)printerNumber;
    sendPacket[6] = (char)numberOfCopies;
    sendPacket[7] = (char)formType;
    if (bannerName == (char *)0)
        *((int *)sendPacket) = 6;
    else
    {
        memset(sendPacket + 8, 0, 14);
        strncpy(sendPacket + 9, bannerName, 13);
        *((int *)sendPacket) = 21;
    }

    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)224, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
