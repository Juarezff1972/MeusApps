/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>


/*___________________________ SaveDirectoryHandle ____________________________
|
| Output:		     0	--  SUCCESSFUL
|	    Network Error Code	--  UNSUCCESSFUL
|
| Comments:
|   This function is included to facilitate storing and restoring a station's
|   path environment for job queuing by remote queue managers.	It will
|   store all the information necessary to restore a directory handle in the
|   save buffer.  This information can then be restored with a call to
|   RestoreDirectoryHandle.
|___________________________________________________________________________*/

int SaveDirectoryHandle(directoryHandle, saveBuffer)
BYTE   directoryHandle;   /* Information is saved on this directory handle  */
char  *saveBuffer;        /* Receives the directory handle information      */
{
BYTE   sendPacket[4], receivePacket[16];
int    ccode;


    sendPacket[2] = 23;
    sendPacket[3] = directoryHandle;
    *((int *)sendPacket) = 2;

    *((int *)receivePacket) = 14;

    ccode = _ShellRequest( (BYTE)226, sendPacket, receivePacket );
    if( ccode == 0 )
	   memmove( saveBuffer, receivePacket + 2, (WORD)14 );

    return( ccode );
}
