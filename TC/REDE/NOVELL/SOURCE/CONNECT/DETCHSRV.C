/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>

/*______________________ DetachFromFileServer _____________________________
|
| Output:   None.
|
| Comments:
|   This function logs an object out and detaches the workstation from a
|   file server.  All drives mapped to the detached file server are removed.
|____________________________________________________________________________*/


void DetachFromFileServer(connectionID)
WORD  connectionID;
{
BYTE  functionNumber, subFunctionNumber;

     functionNumber = 0xF1;
     subFunctionNumber = 0x01;

     _ServerRequest((BYTE)connectionID, functionNumber, subFunctionNumber);

}
