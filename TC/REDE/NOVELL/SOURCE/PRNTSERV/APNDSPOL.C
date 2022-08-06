/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwprint.h>
#include <nwlocal.h>


/*_____________________________AppendToCaptureFile ___________________________
|
| Output:                   0  --  SUCCESSFUL
|          Network Error Code  --  UNSUCCESSFUL
|
| Comments:
|   This routine appends the given text to the current spool file.  If no
|   spool file is currently opened for the station the network will create
|   a spool file with the name SPLpp$ss.xxx, where pp is the printer number
|   that the file should be queued to, ss is the connection number of the
|   station, and xxx is an incrementing number indicating the number of catch
|   files the network has made for this station.  A spool file can be explicit-
|   ly created by using the CreateSpoolFile call. The text may only be sent
|   in packets of from 1 to 512 characters in length.  Any text passed to
|   AppendToCaptureFile that is longer than 512 characters will be sent as
|   multiple packets.
|____________________________________________________________________________*/

int AppendToCaptureFile(length, buffer)
WORD  length;         /* The length of the buffer                            */
char *buffer;         /* The text which is to be appended to the spool file  */
{
char  sendPacket[515], receivePacket[2];
int   ccode;
WORD  startOfPacketData, sizeOfPacketData;


    sendPacket[2] = 0;      /* Print function:  Spool Data to a Capture File */
    *((int *)receivePacket) = 0;
    startOfPacketData = 0;
    do
    {
        sizeOfPacketData = 512;
        if ((sizeOfPacketData + startOfPacketData) > length)
            sizeOfPacketData = length - startOfPacketData;
        memmove(sendPacket + 3, buffer + startOfPacketData, sizeOfPacketData);

        *((WORD *)sendPacket) = 1 + sizeOfPacketData;


        if ((ccode = _ShellRequest((BYTE)224, (BYTE *)sendPacket,
                                   (BYTE *)receivePacket)) != 0)
            return (ccode);

        startOfPacketData += sizeOfPacketData;
    } while (startOfPacketData < length);

    return (0);
}
