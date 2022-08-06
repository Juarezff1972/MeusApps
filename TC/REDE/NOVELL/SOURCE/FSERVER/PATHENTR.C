/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwwrkenv.h>



int GetPathFromDirectoryEntry(connectionID, volumeNumber, directoryEntry, path)

WORD	connectionID;		/* Workstation's connection ID of the server
				   to get information on		     */
BYTE	volumeNumber;		/* Number of volume			     */
WORD	directoryEntry; 	/* File server directory table entry	     */
char   *path;			/* Receives the path for the directory entry */
{
int    ccode;
WORD   oldConnectionID;
BYTE   sendPacket[6], replyPacket[258];



    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    *((WORD *)sendPacket) = 4;
    sendPacket[2] = 0x1A;
    *(sendPacket + 3) = volumeNumber;
    *((WORD *)(sendPacket + 4)) = directoryEntry;
    *((WORD *)replyPacket) = 256;
    if ( (ccode = _ShellRequest((BYTE)0xE2, sendPacket, replyPacket))==0 )
	if ( path ) {
	    memmove( path, replyPacket + 3, replyPacket[2] );
	    path[ replyPacket[2] ] = '\0';      /* null terminate */
	}
    SetPreferredConnectionID( oldConnectionID );
    return( ccode );
}
