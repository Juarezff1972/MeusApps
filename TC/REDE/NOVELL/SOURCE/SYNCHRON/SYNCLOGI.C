/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/**************************************************************************
ClearLogicalRecord()
ClearLogicalRecordSet()
LockLogicalRecordSet()
LogLogicalRecord()
ReleaseLogicalRecord()
ReleaseLogicalRecordSet()

In all cases, the timeout is the amount of time the network should
wait in trying to lock a record or record set.  Each unit is approximately
1/18 of a second.
**************************************************************************/

#include <nwsync.h>
#include <nwmisc.h>

/*____________________________ ClearLogicalRecord ____________________________

  Input:    char *logicalRecordName;       The name of the logical record
  Output:   Returns a 0 if successful or 0xff if unsuccessful.

    This routine will unlock a logical record and remove the logical
    record from the log table.
  __________________________________________________________________________*/

int     ClearLogicalRecord(logicalRecordName)
char    *logicalRecordName;
{
char    name[100];
int     i;
        i = ASCIIZToLenStr(name, logicalRecordName);
        return(_clrRelReq(0xd4, name));
}


/*__________________________ClearLogicalRecordSet__________________________

    Input:    None.
    Output:   None.

    This routine unlocks all of the logical records in the logical record
    log table and then removes the records from the log table.
  _________________________________________________________________________*/

void ClearLogicalRecordSet()
{
        _setClearReq(0xd5);
}


/*_________________________LockLogicalRecordSet__________________________

  int LockLogicalRecordSet(lockDirective, timeout)
            log table

  Input:    BYTE lockDirective;
            WORD timeout;
  Output:   Returns a 0 if successful or
                0xff Failure
                0xfe Timeout failure

    This routine will lock all the records in the logical record log table.
    If the records can not be locked then the timeout is used to deter-
    mine the amount of time the network should wait to see it the records
    can be locked.
  __________________________________________________________________________*/

int LockLogicalRecordSet(lockDirective, timeout)
BYTE    lockDirective;
WORD    timeout;
{
        return( _lockReq(0xd1, lockDirective, timeout) );
}

/*_________________________LockLogicalRecord ____________________________

  Input:    char *logicalRecordName;       The name of the logical record
            BYTE lockDirective;            Flags indicating: bit 0 (0x01)-
                                           lock the record now, bit 1 (0x02)-
                                           lock the record as shareable
            WORD timeout;

  Output:   Returns a 0 if successful or
                0xff Failure
                0xfe Timeout failure
                0x96 No dynamic memory for file

    This routine will log a logical record into the log table so that the
    application can create a complete log of all records it needs and then
    lock them all at once, or the record may be locked as it is logged.
    If the record is locked as it is logged, it may be specified as locked
    shareable.  A record that is locked as shareable can be accessed by
    other stations, but can not be locked exclusively by another station.
    The time out limit is only used if the lock the record now flag is set.
  __________________________________________________________________________*/

int LogLogicalRecord(logicalRecordName, lockDirective, timeout)
char    *logicalRecordName;
BYTE    lockDirective;
WORD    timeout;
{
char    name[100];

        ASCIIZToLenStr(name, logicalRecordName);
        return(_logReq(0xd0, name, lockDirective, timeout));
}

/*_________________________ ReleaseLogicalRecord ________________________

  Input:    char *logicalRecordName;       The name of the logical record
  Output:   Returns a 0 if successful or 0xff if unsuccessful

    This routine will unlock a logical record but not remove the logical
    record from the log table.
  _______________________________________________________________________*/

int ReleaseLogicalRecord(logicalRecordName)
char    *logicalRecordName;
{
char    name[100];

        ASCIIZToLenStr(name, logicalRecordName);
        return(_clrRelReq(0xd2, name));
}

/*_______________________ ReleaseLogicalRecordSet ________________________

    Input:    None.
    Output:   None.

    This routine unlocks all of the logical records in the logical record
    log table, but does not remove the records from the log table.
  _______________________________________________________________________*/

void ReleaseLogicalRecordSet()
{
        _setClearReq(0xd3);
}
