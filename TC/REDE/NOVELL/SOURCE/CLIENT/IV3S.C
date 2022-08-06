/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <nit.h>
#include <ntt.h>


WORD IsV3Supported
(
WORD connectionID
)
{
    WORD     oldConnectionID;
    WORD     ccode;

      oldConnectionID  = GetPreferredConnectionID();
      SetPreferredConnectionID(connectionID);
      ccode = (!CheckNetWareVersion(3,0,0,0,0));
      SetPreferredConnectionID(oldConnectionID);
      return(ccode);
}
