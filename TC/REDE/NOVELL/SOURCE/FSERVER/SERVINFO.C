/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconsol.h>
#include <nwlocal.h>
#include <nwmisc.h>


#define MIN(a,b)	 ((a) < (b) ? (a) : (b))

int GetServerInformation(structSize, serverInfo)
int  structSize;		/* Size in bytes to return in serverInfo   */
FILE_SERV_INFO	*serverInfo;	/* Receives information on the Server	   */
{
int  ccode, bufferLength;
BYTE sendPacket[3], receivePacket[130];


     bufferLength = MIN(structSize, 128);
     *((int *)(sendPacket)) = 1;
     sendPacket[2] = 0x11;
     *((int *)(receivePacket)) = 128;
     memset(&receivePacket[2], 0, 128);

     ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
     if (!ccode)
      {
	 *((int *)(receivePacket+52)) = IntSwap(*((int *)(receivePacket+52)));
	 *((int *)(receivePacket+54)) = IntSwap(*((int *)(receivePacket+54)));
	 *((int *)(receivePacket+56)) = IntSwap(*((int *)(receivePacket+56)));
	 *((int *)(receivePacket+61)) = IntSwap(*((int *)(receivePacket+61)));
	 memmove(serverInfo, &receivePacket[2], bufferLength);
      }
     return(ccode);
}


