/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*___________________________ AFPCreateFile ________________________________
   Description:  Creates a file using an AFP name.
   Input:        int connectionID;          Number of the server where the
                                            transaction is to be performed.
                 BYTE volumeNum;            Volume number where the directory
                                            entry is.
                 long AFPEntryID;           AFP base ID.
                 BYTE deleteExistingFile    If a file of the same name exist,
                                            deleted it? (0=no, 1=yes)
                 BYTE *finderInfo;          Finder information for the file.
                 char *AFPPathString;       AFP style directory path, relative to
                                            AFPbaseID.
                 long *newAFPEntryID;       ID of the newly created directory.

   Output:       0 is successful or a bindery error code if unsuccessful.

   Comments:     The resulting file is NOT opened. The file is created as a 
                 normal read-write file with the system and hidden bits 
                 cleared.
 __________________________________________________________________________*/

int AFPCreateFile( connectionID, volumeNum, AFPEntryID, deleteExistingFile,
                   finderInfo, AFPPathString, newAFPEntryID )
WORD connectionID;
BYTE volumeNum;
long AFPEntryID;
BYTE deleteExistingFile;
BYTE *finderInfo;
char *AFPPathString;
long *newAFPEntryID;
{
BYTE sendPacket[298];
BYTE receivePacket[4];
int  ccode;
int  packetLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    /*                                                   */
    /* build the packet for creating the file            */
    /*                                                   */
    packetLength = 3 + (39 + (int)AFPPathString[0]); /* header+(parameters) */
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 2; /* create file request number */
    sendPacket[3] = (BYTE)volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    sendPacket[8] = deleteExistingFile;
    memmove(sendPacket + 9, (char *)finderInfo, (WORD)32);
    memmove(sendPacket + 41, AFPPathString, (WORD)(AFPPathString[0] + 1));


    /*                                                     */
    /* Request that the file be created                    */
    /*                                                     */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
                receivePacket, 4)) == 0)
        *newAFPEntryID = *((long *)receivePacket);

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
