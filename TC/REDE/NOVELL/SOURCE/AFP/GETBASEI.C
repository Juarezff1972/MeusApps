/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*_______________________ AFPGetEntryIDFromPathName ________________________

    Description:  Returns a unique AFP baseID from a NetWare-style path 
                  specification
    Input:        int connectionID;       Number of the server where the
                                          transaction is to be performed.
                  BYTE directoryHandle;   Directory handle for the pathName.
                  char *pathName;         Path given relative to the directoryHandle.
                  long *AFPEntryID;       Unique AFP baseID.

    Output:       0 is successful or a bindery error code if unsuccessful.

 Comments:
   Converts a length specified string containing a NetWare-style path
   specification into a unique 32-bit AFP File or Directory ID. The
   Directory Base and Path specifications are given in Netware "short name"
   format.  The AFP Base ID is the unique 32-bit identifier of the file or
   directory.
 ___________________________________________________________________________*/

int AFPGetEntryIDFromPathName(connectionID, directoryHandle, pathName, AFPEntryID)
WORD connectionID;
BYTE directoryHandle;
char *pathName;
long *AFPEntryID;
{
BYTE sendPacket[261];
BYTE receivePacket[4];
char junk[48];
char *ptr;
int  ccode;
int  packetLength;
int  pathLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);

    /*                                                   */
    /* build the packet for getting the base ID          */
    /*                                                   */
    ptr = StripFileServerFromPath(pathName, junk);
    pathLength = strlen(ptr);
    packetLength = 3 + (2 + pathLength);      /* header + (parameters) */
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = (BYTE)12;                 /* request number        */
    sendPacket[3] = directoryHandle;
    sendPacket[4] = (BYTE)pathLength;
    strcpy((char *)(&sendPacket[5]), ptr);

    /*                                                   */
    /* Send the request for the base ID                  */
    /*                                                   */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
       receivePacket, 4)) == 0)
        *AFPEntryID = *((long *)receivePacket);

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
