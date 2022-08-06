/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*_____________________  AFPAllocTemporaryDirHandle ________________________

   Description:  Allocate a directory handle from AFP information
   Output:       0 is successful or a NetWare error code if unsuccessful.
  _________________________________________________________________________*/

int AFPAllocTemporaryDirHandle( connectionID, volumeNum, AFPEntryID,
	                             AFPPathString, NetWareDirectoryHandle, 
                                NetWareAccessRights )
WORD connectionID;	          /*  connection indicates file server	       */
BYTE volumeNum; 	             /*  Volume number of the directory entry     */
long AFPEntryID;	             /*  AFP base ID. 			                   */
char *AFPPathString;	          /*  AFPdirectory path, relative to AFPbaseID */
BYTE *NetWareDirectoryHandle;  /*  returns a NetWare directory handle.	    */
BYTE *NetWareAccessRights;     /*  return the effective rights the	       */
			                      /*  requesting user has on the directory.    */

{
BYTE sendPacket[320];
BYTE receivePacket[4];
int  ccode;
int  packetLength;
WORD oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);
	   /* build the packet for creating the directory entry */
	   /*  packet length should be 8 which is packet info + length of */
	   /*  AFPPathString + 1 to include the AFPPathString length byte. */
    packetLength = 8 + AFPPathString[0] + 1;
    *((int *)sendPacket) = IntSwap(packetLength - 2);
    sendPacket[2] = 11;  /* Create NetWare Directory Handle request */
    sendPacket[3] = volumeNum;
    *((long *)(sendPacket + 4)) = AFPEntryID;
    memmove(sendPacket + 8, AFPPathString, (WORD)(AFPPathString[0] + 1));
		/* Request that the directory be created	       */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, packetLength,
	   receivePacket, (WORD)4)) == 0) {
	if ( NetWareDirectoryHandle )
	    *NetWareDirectoryHandle = receivePacket[0];
	if ( NetWareAccessRights )
	    *NetWareAccessRights = receivePacket[1];
    }

    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
