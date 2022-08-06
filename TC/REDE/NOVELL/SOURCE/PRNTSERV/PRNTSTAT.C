/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>
#include <nwlocal.h>
#include <nwmisc.h>

/*___________________________GetPrinterStatus _______________________________
|
| Output:                   0  --  SUCCESSFUL
|          Network Error Code  --  UNSUCCESSFUL
|
| Comments:
|   This routine will return the current status of the specified printer.  If
|   the printer is halted or offline, the corresponding flag will be set to
|   0xff, otherwise it will be set to 0.  The target printer will normally
|   be the same number as the printer number that the status was requested
|   for, however if jobs going to that printer have been rerouted to another
|   printer then that printer's number will be returned as the targetPrinter.
|
___________________________GetPrinterQueue ___________________________________
|
| Output:                   0  --  SUCCESSFUL
|          Network Error Code  --  UNSUCCESSFUL
|
| Comments:
|  This routine returns the queue number of the printer specified.
|_____________________________________________________________________________*/


int GetPrinterStatus(printerNumber, printerHalted, printerOffLine, formType,
                     targetPrinter)
int   printerNumber;   /* Number of the printer to return information on      */
int  *formType;        /* Receives the form type currently mounted on printer */
int  *targetPrinter;   /* Receives the printer number of the printer that
                          jobs are to be printed on                           */
BYTE *printerHalted;   /* Receives a flag indicating whether specified printer
                          is active (0) or stopped (255)                      */
BYTE *printerOffLine;  /* Receives a flag indicating whether the specified
                          printer is off-line or on-line ( 1 = off-line )     */
{
char    sendPacket[4];
char    receivePacket[6];
int     ccode;

    sendPacket[2] = 6;
    sendPacket[3] = (char)((BYTE)printerNumber);
    *((int *)sendPacket) = 2;

    *((int *)receivePacket) = 4;

    if ((ccode = _ShellRequest((BYTE)0xE0, (BYTE *)sendPacket,
                               (BYTE *)receivePacket)) == 0) {
        if (printerHalted)
            *printerHalted = receivePacket[2];
        if (printerOffLine)
            *printerOffLine = receivePacket[3];
        if (formType)
            *formType = receivePacket[4];
        if (targetPrinter)
            *targetPrinter = receivePacket[5];
    }
    return (ccode);
}


int GetPrinterQueue (LPTDevice, printQueueID)
BYTE  LPTDevice;
long *printQueueID;
{
char    sendPacket[4];
char    receivePacket[6];
int     ccode;

    sendPacket[2] = 0x0A;
    sendPacket[3] = (char)LPTDevice;
    *((int *)sendPacket) = 2;

    *((int *)receivePacket) = 4;

    if ((ccode = _ShellRequest((BYTE)0xE0, (BYTE *)sendPacket,
                               (BYTE *)receivePacket)) == 0)
      {
       *printQueueID = LongSwap(*((long *)(receivePacket + 2)));
      }
    return (ccode);
}
