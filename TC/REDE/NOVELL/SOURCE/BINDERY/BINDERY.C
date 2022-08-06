/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwbindry.h>
#include <nwlocal.h>

     /*___________________________________
		|    Name:	 BINDERY.C	           |
		|				                       |
		|    Functions:  CloseBindery()    |
		|		           OpenBindery()     |
		|__________________________________*/



/*_______________________ CloseBindery _____________________________________
|
| Output:   --	0                  if SUCCESSFUL
|        		bindery error code if UNSUCCESSFUL
|
| Comments:
|   This function is used before archiving or restoring the bindery.
|   Normally, the bindery files are kept open and locked.  Therefore,
|   the bindery must be closed before the bindery files can be archived
|   or restored.  Only the SUPERVISOR or a bindery object that is security
|   equivalent to the SUPERVISOR can close the bindery.
|   WARNING:  If this function is used, the OpenBindery() call should be made
|	      before an EOJ has occurred since many of the network functions
|	      are disabled when the bindery is closed.
|___________________________________________________________________________*/

int CloseBindery()
{
    BYTE sendPacket[3], receivePacket[2];

    sendPacket[2] = 68;
    *((int *)sendPacket) = 1;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}




/*___________________________ OpenBindery ____________________________________
| Input:    None.
|
| Output:   0			--  SUCCESSFUL
|	    bindery error code	--  UNSUCCESSFUL
|
| Comments:
|   This routine reopens the bindery files, which are normally kept opened and
|   locked while the network is running, after the bindery was closed by a
|   previous call to CloseBindery.  While the bindery is closed all other bindery
|   calls can not be serviced.	This call is for the use of the archive program
|   which must close the bindery in order to access the bindery files during a
|   system backup.  This call will only be honored if the calling station is
|   logged in as supervisor.
|_____________________________________________________________________________*/

int OpenBindery()
{
    BYTE sendPacket[3], receivePacket[2];

    sendPacket[2] = 69;
    *((int *)sendPacket) = 1;
    *((int *)receivePacket) = 0;

    return (_ShellRequest((BYTE)227, sendPacket, receivePacket));
}
