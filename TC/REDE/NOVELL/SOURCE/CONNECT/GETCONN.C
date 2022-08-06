/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>
#include <nwconn.h>

/*_________________________ GetConnectionNumber _____________________________
|
| Output:   Returns the connection number.
|____________________________________________________________________________*/


WORD GetConnectionNumber()
{
WORD   number;


      number = _ShellRequest( (BYTE)220, 0, 0);
      return(number);
}
