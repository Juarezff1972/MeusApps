/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*_____________________________________________________________________
  |                                                                   |
  |  Output:  0  --  AFP NOT Supported                                |
  |           1  --  AFP IS Supported                                 |
  |  Comments:  This function determines if both the effective        |
  |             server and the shell support AFP system calls.        |
  |___________________________________________________________________|*/
#include <string.h>
#include <nwwrkenv.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nat.h>

int AFPSupported(connectionID)
WORD connectionID;
{
int            ccode;
BYTE           major;
BYTE           minor;
BYTE           revision;
long           AFPEntryID;

    GetNetWareShellVersion(&major, &minor, &revision );
    ccode = AFPGetEntryIDFromPathName( connectionID, (BYTE)0,
                                       "SYS:", &AFPEntryID );
    return(ccode == 0  && ( major>2 || ((major == 2) && (minor >= 15 ))));
}
