/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwlocal.h>



/*______________________ RestoreDirectoryHandle _______________________________
|
| Output:		     0 --  SUCCESSFUL
|	    network error code --  UNSUCCESSFUL
|
| Comments:
|   This routine restores the path environment information which had been saved
|   with the SaveDirectoryHandle call.	This call is included so a station's
|   executing environment can be reproduced at some remote job execution site
|   (e.g. print spooling, batch job execution, etc.).
|_____________________________________________________________________________*/

int RestoreDirectoryHandle(saveBuffer, newDirectoryHandle, effectiveRightsMask)
char *saveBuffer;            /* Points to the directory handle information
                                that is to be restored                      */
BYTE *newDirectoryHandle;    /* Receives the new directory handle           */
BYTE *effectiveRightsMask;   /* Receives the effective rights mask          */
{
BYTE  sendPacket[19], receivePacket[4];
int   ccode;

    sendPacket[2] = 24;
    memmove( sendPacket + 3, saveBuffer, 14 );
    *((int *)sendPacket) = 17;

    *((int *)receivePacket) = 2;

    ccode = _ShellRequest( (BYTE)226, sendPacket, receivePacket );
    if( ccode )
	   return( ccode );

    *newDirectoryHandle = receivePacket[2];
    if (effectiveRightsMask != (BYTE *)0)
	   *effectiveRightsMask = receivePacket[3];

    return( 0 );
}
