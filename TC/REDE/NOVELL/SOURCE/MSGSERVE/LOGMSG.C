/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwmsg.h>
#include <nwlocal.h>


/*_____________________ LogNetworkMessage ____________________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This function appends a message to file NET$LOG.MSG in the SYS:SYSTEM
|   directory of the default file server.  The message must contain ascii
|   characters between the values of 7Eh and 20h.  The message is automatically
|   prefaced with the current date, time, and logical connection number before
|   writing it to the NET$LOG.MSG file.
|___________________________________________________________________________*/

int LogNetworkMessage(message)
char *message;	     /*  Null terminated string containing the message
			 to be logged (maximum of 80 characters).      */
{
char	sendPacket[84], receivePacket[2];

   sendPacket[2] = 13;
   sendPacket[3] = (char)((BYTE)strlen(message));
   memmove((sendPacket + 4), message, (int)sendPacket[3]);

   *((int *)sendPacket) = 2 + sendPacket[3];
   *((int *)receivePacket) = 0;

   return (_ShellRequest((BYTE)227, (BYTE *)sendPacket, (BYTE *)receivePacket));
}
