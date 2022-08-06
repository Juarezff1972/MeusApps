/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>
#include <nwlocal.h>


/*__________________________________________________________________________
 |
 |  Output:                     0  --  SUCCESSFUL
 |             Network Error Code  --  UNSUCCESSFUL
 |
 |  Comments:  This function shuts down the file server.  The force flag
 |             indicates whether the file server should be forced down even
 |             if there are files open.  A forceFlag of 0 means the file
 |             the file server will not go down if there are open files.
 |__________________________________________________________________________*/


int DownFileServer(forceFlag)
int forceFlag;
{
BYTE    requestPacket[4], replyPacket[2];

    *((int *)requestPacket) = 2;
    requestPacket[2] = (BYTE)211;
    requestPacket[3] = forceFlag ? (BYTE)0xff : (BYTE)0;
    *((int *)replyPacket) = 0;

    return (_ShellRequest((BYTE)227, requestPacket, replyPacket));
}
