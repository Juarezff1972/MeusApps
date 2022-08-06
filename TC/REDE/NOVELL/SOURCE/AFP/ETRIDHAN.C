/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

/*__________________ AFPGetEntryIDFromFileHandle ___________________________

   Input:    WORD connectionID;		connection indicates file server
					                     request will be made to
      	    int  fileHandle;	   	DOS file handle of open file.
	          BYTE *volumeID;  		Volume number where the directory
                      					entry is.
      	    long *AFPEntryID;		AFP base ID.
	          BYTE *forkIndicator;	0 means data, 1 means resource

 Output:     0 is successful or a NetWare error code if unsuccessful.

___________________________________________________________________________*/

/*!!!!!!!!!!!!!!!!! GetNetWareHandleFromDOS does not yet exit !!!!!!!!!!!!

int AFPGetEntryIDFromFileHandle(connectionID, fileHandle, volumeID,
	    AFPEntryID, forkIndicator)
WORD	connectionID;
int	fileHandle;
BYTE   *volumeID;
long   *AFPEntryID;
BYTE   *forkIndicator;

{
int	ccode;
BYTE	NetWareHandle[6];
WORD	oldConnectionID;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID(connectionID);
    ccode = GetNetWareHandleFromDOS(fileHandle, NetWareHandle);
    if ( ccode == 0 )
	ccode = AFPGetEntryIDFromNetWareHandle( NetWareHandle, volumeID,
		    AFPEntryID, forkIndicator );
    SetPreferredConnectionID(oldConnectionID);
    return( ccode );
}
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

int AFPGetEntryIDFromNetWareHandle(NetWareHandle, volumeID, AFPEntryID,
		forkIndicator)
BYTE   *NetWareHandle;
BYTE   *volumeID;
long   *AFPEntryID;
BYTE   *forkIndicator;

{
BYTE sendPacket[9];
BYTE receivePacket[6];
int  ccode;

    /*							 */
    /* build the packet for creating the directory entry */
    /*							 */
    *((int *)sendPacket) = IntSwap(7);
    sendPacket[2] = 6;	/* Create NetWare Directory Handle request */
    memmove( sendPacket + 3, NetWareHandle, 6 );

    /*							   */
    /* Request that the directory be created		   */
    /*							   */
    if ((ccode = _FileServiceRequest((BYTE)35, sendPacket, (WORD)9,
	   receivePacket, (WORD)6)) == 0) {
	if ( volumeID )
	    *volumeID = receivePacket[0];
	if ( AFPEntryID )
	    *AFPEntryID = *((long *)(receivePacket + 1));
	if ( forkIndicator )
	    *forkIndicator = receivePacket[ 5 ];
    }
    return( ccode );
}
