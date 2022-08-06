/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>
#include <nat.h>

/*___________________________ AFPRename _________________________________

   Description:  Renames a macintosh file.
   Input:        WORD connectionID;      Number of the server where the
                                         transaction is to be performed.
                 BYTE volumeNum;         Volume number where the directory
                                         entry is.
                 long AFPSourceEntryID;  AFP base ID.
                 long AFPDestEntryID;    AFP base ID.
                 char *AFPSourcePath;    AFP style directory path, relative to
                                         AFPbaseID.
                 char *AFPDestPath;      AFP style directory path, relative to
                                         AFPbaseID.

   Output:   Returns a 0 is successful or a bindery error code if unsuccessful.
 __________________________________________________________________________*/

int AFPRename(connectionID, volumeNum, AFPSourceEntryID, AFPDestEntryID,
                  AFPSourcePath, AFPDestPath)
WORD connectionID;
BYTE volumeNum;
long AFPSourceEntryID;
long AFPDestEntryID;
char *AFPSourcePath;
char *AFPDestPath;
{
BYTE sendPacket[526],
     receivePacket[2];
int  ccode,
     packetLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID((WORD)connectionID);

    packetLength = 3 + (11 + (int)AFPSourcePath[0] + (int)AFPDestPath[0]);
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 7;                      /* request number        */
    sendPacket[3] = volumeNum;
    *((long *)(sendPacket + 4)) = AFPSourceEntryID;
    *((long *)(sendPacket + 8)) = AFPDestEntryID;
    memmove(sendPacket + 12, AFPSourcePath, (WORD)(AFPSourcePath[0] + 1));
    memmove(sendPacket+12+(int)AFPSourcePath[0]+1, AFPDestPath,
                (WORD)(AFPDestPath[0] + 1));

    ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
                receivePacket, 2);
    SetPreferredConnectionID( oldConnectionID );
    return (ccode);

}
