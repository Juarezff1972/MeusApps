/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwlocal.h>
#include <nwconn.h>

/*___________________________ EnterLoginArea _________________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This routine will direct the network to look in a specific subdirectory
|   under sys:login to find the proper versions of login and the operating
|   system (for auto-boot roms) for the calling station.  If a nil pointer
|   is passed for the loginSubdirectoryName, the directory sys:login will
|   be assumed.
|____________________________________________________________________________*/

int EnterLoginArea(loginSubdirectoryName, numberOfLocalDrives)
char *loginSubdirectoryName;	/*  Subdirectory name within sys:login
				    where LOGIN.EXE can be found.      */
int numberOfLocalDrives;	/*  Number of local drives in the calling
				    workstation.		       */
{
char	sendPacket[260];
char	receivePacket[2];

    sendPacket[2] = 10;
    sendPacket[3] = (char)numberOfLocalDrives;

    if (loginSubdirectoryName != (char *)0)
    {
	sendPacket[4] = (char)((BYTE)strlen(loginSubdirectoryName));
	strcpy(sendPacket + 5, loginSubdirectoryName);
    }
    else
	sendPacket[4] = 0;

    *((int *)sendPacket) = 3 + *(BYTE *)(sendPacket + 4);
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
