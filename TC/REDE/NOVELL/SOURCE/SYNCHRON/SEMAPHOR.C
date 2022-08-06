/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/**************************************************************************
CloseSemaphore
SignalSempahore
WaitOnSemaphore
**************************************************************************/

#include <nwsync.h>

/*________________________CloseSemaphore ___________________________________

    This function will decrement the open count of the number of stations using
    the semaphore.  If this was the last station (so that the open count goes
    to 0) then the semaphore will be deleted.
  __________________________________________________________________________*/

int    CloseSemaphore(semaphoreHandle)
long    semaphoreHandle;
{
       return (_semaphorReq(4,semaphoreHandle,0));
}


/*________________________SignalSemaphore ___________________________________

    This function signals a semaphore that the application is done using it.
    This signal causes the semaphore value to be incremented by one.  If
    the semaphore value was negative then the first station in the semaphore
    queue will be released.
  _________________________________________________________________________*/
int    SignalSemaphore(semaphoreHandle)
long    semaphoreHandle;
{
        return (_semaphorReq(3,semaphoreHandle,0));
}


/*________________________WaitOnSemaphore ___________________________________

    This function will decrement the semaphore value, then check the new
    value.  If the value is 0 or greater then the routine will return
    with a success completion code (0).  If the semaphore value is negative
    then it will wait for other stations using the semaphore to signal.
    If the timeout is reached then a 0xfe completion code is returned.
  _________________________________________________________________________*/

int    WaitOnSemaphore(semaphoreHandle, timeout)
long    semaphoreHandle;
WORD    timeout;
{
        return(_semaphorReq(2,semaphoreHandle,timeout));
}
