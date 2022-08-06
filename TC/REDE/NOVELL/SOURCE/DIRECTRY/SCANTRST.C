/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*________________________ScanDirectoryForTrustees_________________________
|
| Output:   0                  --  SUCCESSFUL
|           network error code --  UNSUCCESSFUL
|
| Comments:
|   This function returns a directory's trustees.  Directories may have any
|   number of bindery objects.  Directory trustees are stored and retrieved
|   in groups of 5.  Sequence number is set to 0 to get the first trustee
|   group, and then incremented to get additional groups.  A trusteeID of 0
|   indicates the end of a group.  To get all groups, searching should continue
|   until the NO_MORE_TRUSTEES error is returned.  Slots where trustees were
|   removed will contain an object ID of 0L.  Since compaction occurs in each
|   group of 5, an algorithm can stop examining the group when it finds an
|   objectID of 0L, and make another call to ScanDirectoryForTrustees for the
|   next group.
|____________________________________________________________________________*/

int ScanDirectoryForTrustees(directoryHandle, directoryPath, sequenceNumber,
                             directoryName, creationDateAndTime, ownerID,
                             trusteeIDs, trusteeRights)

BYTE directoryHandle;        /* directoryPath is relative to this directory  */
char *directoryPath;         /* Path specification of the directory whose
                                trustees are returned (max 255 characters    */
int *sequenceNumber;         /* Initially 0, then incremented on next calls  */
char *directoryName;         /* Receives the directory name for which trustees
                                are returned (max 16 characters INCLUDING null */
BYTE *creationDateAndTime;   /* Receives the date and time the directory was
                                created                                       */
long *ownerID;               /* Receives the unique bindery object IDs for the
                                object that created the directory             */
long *trusteeIDs;            /* Receives the unique bindery object IDs for 5
                                trustees (5 long integers)                    */
BYTE *trusteeRights;         /* Receives the trustee rights mask for 5
                                trustees (5 BYTES)                            */
{
    BYTE sendPacket[261], receivePacket[51];
    int ccode, index, len;

    sendPacket[2] = 12;
    sendPacket[3] = directoryHandle;
    if( *sequenceNumber == 0 )
      *sequenceNumber = 1;
    sendPacket[4] = (BYTE)*sequenceNumber;
    (*sequenceNumber)++;
    len = strlen(directoryPath);
    sendPacket[5] = (BYTE)len;
    memmove(sendPacket + 6, directoryPath, len);

    *((int *)sendPacket) = len + 4;
    *((int *)receivePacket) = 49;

    ccode = _ShellRequest((BYTE)226, sendPacket, receivePacket);
    if (ccode)
       return(ccode);

    if (directoryName != (char *)0)
        strcpy(directoryName, receivePacket + 2);
    if (creationDateAndTime != (BYTE *)0)
       memmove(creationDateAndTime, receivePacket + 18, 4);
    if (ownerID != (long *)0)
        *ownerID = LongSwap(*((long *)(receivePacket + 22)));

    for (index = 0; index < 5; index++)
    {
        trusteeIDs[index] = LongSwap(*((long *)(receivePacket + 26 +
                (index * 4))));
        trusteeRights[index] = receivePacket[46 + index];
    }
    return (ccode);
}
