/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nat.h>

/*___________________________ AFPOpenFileFork ______________________________

   Description:  Opens a AFP file fork from a DOS environment.
   Input:        int connectionID;       Number of the server where the
                                         transaction is to be performed.
                 BYTE volumeNum;         Volume number where the directory
                                         entry is.
                 long AFPEntryID;        AFP base ID.
                 BYTE forkIndicator;     Data or Resource fork indicator.
                 BYTE accessMode;        File access mode indicator.
                 char *AFPPathString;    AFP style directory path, relative to
                                         AFPbaseID.
                 long *fileID;           File entry ID.
                 long *forkLength;       Length of the opened fork.
                 int  *DOSFileHandle;    Access handle for NetWare.

   Output:       0 is successful or a bindery error code if unsuccessful.
 ___________________________________________________________________________*/

int AFPOpenFileFork(connectionID, volumeNum, AFPEntryID, forkIndicator, accessMode,
                  AFPPathString, fileID, forkLength, NetWareHandle, DOSFileHandle)
WORD connectionID;
BYTE volumeNum;
long AFPEntryID;
BYTE forkIndicator;
BYTE accessMode;
char *AFPPathString;
long *fileID;
long *forkLength;
BYTE *NetWareHandle;
int *DOSFileHandle;
{
BYTE sendPacket[267];
BYTE receivePacket[14];
int  ccode;
WORD currentServerNum;
int  packetLength;

    currentServerNum = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    packetLength = 3 + (8 + (int)AFPPathString[0]);   /* header + (parameters) */
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 8;                                /* request number        */
    sendPacket[3] = volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    sendPacket[8] = forkIndicator;
    sendPacket[9] = accessMode;
    memmove(sendPacket + 10, AFPPathString, (WORD)(AFPPathString[0] + 1));

    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
       receivePacket, 14)) == 0) {

        if ( fileID )
            *fileID = *((long *)receivePacket);
        if ( forkLength )
            *forkLength = LongSwap(*((long *)(receivePacket + 4)));
        if ( NetWareHandle )
            memmove( NetWareHandle, (receivePacket + 8), 6 );

        /*                                               */
        /* Translate the NetWare handle to a DOS handle. */
        /*                                               */
        if ( DOSFileHandle )
            ccode = _ConvertHandle(connectionID, accessMode,
                    (BYTE *)(receivePacket + 8), *((long *)(receivePacket + 4)),
                    DOSFileHandle);
    }
    SetPreferredConnectionID(currentServerNum);
    return( ccode );
}

