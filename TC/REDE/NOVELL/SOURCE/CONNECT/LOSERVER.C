/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwconsol.h>

/*________________________ LogoutFromFileServer _______________________________
|
| Output:   None.
|
| Comments:
|   This routine logs an object out, but does not detach it from the file
|   server.  All drives mapped to the specified file server are removed, and
|   then one drive is mapped to the SYS:LOGIN directory on that file server.
|____________________________________________________________________________*/

void LogoutFromFileServer(connectionID)
WORD  connectionID;
{
BYTE  functionNumber, subFunctionNumber;

     functionNumber = 0xF1;
     subFunctionNumber = 0x02;

     _ServerRequest((BYTE)connectionID, functionNumber, subFunctionNumber);

}
