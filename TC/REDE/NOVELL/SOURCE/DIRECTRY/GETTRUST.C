/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>



/*________________________ ScanBinderyObjectTrusteePaths ____________________
|
| Output:   0		       --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine returns the directory paths that an object has trustee rights
|   to.  The volume number should be obtained by a call to MapVolumeToNumber
|   prior to making the call to ScanBinderyObjectTrusteePaths.	For the first
|   call the sequence number should be set to 0.  Upon return, the sequence
|   number is set to the value needed for the next call.  The user may only
|   obtain information about the trustee rights of himself or someone with whom
|   he is SECURITY_EQUALS, unless the user is a SUPERVISOR.
|___________________________________________________________________________*/

int ScanBinderyObjectTrusteePaths( objectID, volumeNumber, sequenceNumber,
                                   trusteeAccessMask, trusteePathName )

long  objectID;          /* Unique bindery object ID for which trustee
                            information should be found                      */
BYTE  volumeNumber;      /* Volume number of the volume to be searched(0..31)*/
int  *sequenceNumber;    /* Contains the sequenceNumber from the previous
                            search (initial search requires a 0)             */
char *trusteeAccessMask; /* Receives the object's access mask to the specified
                            directory path                                   */
char *trusteePathName;   /* Receives a string containing a path for which the
                            the object is listed as a directory trustee in the
                            form VOLUMENAME:DIRECTORY/.../DIRECTORY (maximum
                            255 characters INCLUDING the null terminator     */
{
BYTE	sendPacket[10], receivePacket[265];
int	ccode, len;

    sendPacket[2] = 71;
    sendPacket[3] = volumeNumber;
    *((int *)(sendPacket + 4)) = IntSwap(*sequenceNumber);
    *((long *)(sendPacket + 6)) = LongSwap(objectID);

    *((int *)sendPacket) = 8;
    *((int *)receivePacket) = 263;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
	return (ccode);

    *sequenceNumber = IntSwap(*((int *)(receivePacket + 2)));
    *trusteeAccessMask = receivePacket[8];
    len = receivePacket[9];
    if (len != 0)
	memmove(trusteePathName, (receivePacket + 10), len);
    trusteePathName[len] = '\0';

    return (0);
}
