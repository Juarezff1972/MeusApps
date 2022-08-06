/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/**************************************************************************
GetLockMode()
SetLockMode()
***************************************************************************/
#include <nwsync.h>

/*_______________________________ GetLockMode ________________________________
|
| Input:    None.
| Output:   Returns the current lock mode.
|               0 Old 'compatibility mode'
|               1 New extended locks mode
|
|   This routine will return the current lock mode.  The lock mode determines
|   how the network handles locking requests.  If the lock mode is 0 then
|   the network will handle locking requests exactly the same as it did in
|   versions prior to Netware 2.0.  If the lock mode is 1 then the network
|   will do the extended locks that were implemented in versions 2.0 and above.
|_____________________________________________________________________________*/

int GetLockMode()
{
int     mode;
        mode = _lockReq(0xc6,2,0);
        return(mode);
}

/*_______________________________ SetLockMode ________________________________

  Input:    int lockMode;              The value that the lock mode should
                                       be set to (0=old 'compatibility mode',
                                       1=new extended locks mode)
  Output:   None.

    This routine will set the lock mode.  If the lock mode is set to 0 then
    the network will respond to locking calls as it did in versions prior to
    Netware 2.0.  If the lock mode is set to 1 then the network will respond
    to locking calls with the new extended lock features.  The locking routines
    in this library assume that SetLockMode has been called with a value of 1.
    The default is set lock mode to 0 so that older applications will run
    as if they were still on an older version of netware.
  ____________________________________________________________________________*/

int SetLockMode(lockMode)
BYTE    lockMode;
{
int     ccode;
        ccode = _lockReq(0xc6,lockMode,0);
        return(ccode);
}
