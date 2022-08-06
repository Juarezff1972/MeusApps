/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwwrkenv.h>
#include <nwmisc.h>
#include <nwlocal.h>
#include <nwdir.h>

#define MIN(a,b) ((a)<(b)?(a):(b))



int GetVolumeInformation(connectionID, volumeNumber, structSize,
                         volumeStatistics)

WORD    connectionID;               /* Workstation's connection ID of the
                                       server for which to get information    */
BYTE    volumeNumber;               /* Volume number to return information on */
int     structSize;                 /* Size in bytes to return in
                                       volumeStatistics                       */
VOLUME_STATS   *volumeStatistics;   /* Receives information on the volume     */

{
int    ccode;
WORD   oldConnectionID;
BYTE   sendPacket[4], receivePacket[44];


    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 2;
    sendPacket[2] = 0xE9;
    sendPacket[3] = volumeNumber;
    *((WORD *)receivePacket) = 42;
    ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket);
    if (ccode == 0)
    {
     *((long *)(receivePacket + 2)) =  LongSwap(*((long *)(receivePacket + 2)));
     *((WORD *)(receivePacket + 8)) =  IntSwap(*((WORD *)(receivePacket + 8)));
     *((WORD *)(receivePacket + 10)) = IntSwap(*((WORD *)(receivePacket +10)));
     *((WORD *)(receivePacket + 12)) = IntSwap(*((WORD *)(receivePacket +12)));
     *((WORD *)(receivePacket + 14)) = IntSwap(*((WORD *)(receivePacket + 14)));
     *((WORD *)(receivePacket + 16)) = IntSwap(*((WORD *)(receivePacket + 16)));
     *((WORD *)(receivePacket + 18)) = IntSwap(*((WORD *)(receivePacket + 18)));
     *((WORD *)(receivePacket + 20)) = IntSwap(*((WORD *)(receivePacket + 20)));
     *(receivePacket+43) = '\0';     /* force terminator */

     memmove( (BYTE *)volumeStatistics, (receivePacket + 2),
              MIN(structSize,sizeof(VOLUME_STATS)) );
    }

    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
