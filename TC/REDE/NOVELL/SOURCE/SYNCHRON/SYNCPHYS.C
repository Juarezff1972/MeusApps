/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/**************************************************************************
ClearPhysicalRecord()
ClearPhysicalRecordSet()
LockPhysicalRecordSet()
LogPhysicalRecord()
ReleasePhysicalRecord()
ReleasePhysicalRecordSet()

In all cases, the timeoutLimit is the amount of time the network should
wait in trying to lock a record or record set.  Each unit is approximately
1/18 of a second.
**************************************************************************/

#include <nwsync.h>


/*________________________ ClearPhysicalRecord ______________________________
 |
 | Input:        int fileHandle;         The DOS file handle of the file that
 |                                       the record is in.
 |               long recordStartOffset; The offset in the file where the
 |                                       record begins.
 |               long recordLength;      The record length in bytes.
 |
 | Output:       Returns a 0 if successful, or 0xff if the record is invalid.
 |
 | Comments:     This routine will unlock the specified record and remove it
 |               from the log table.
 |____________________________________________________________________________*/

int  ClearPhysicalRecord(fileHandle, recordStartOffset, recordLength)
int     fileHandle;
long    recordStartOffset;
long    recordLength;
{
int     ccode;
        ccode = _physRecReq(0xbe,fileHandle,recordStartOffset,recordLength,0,0);
        return(ccode);
}


/*________________________ ClearPhysicalRecordSet ___________________________
    Input:    None.
    Output:   None.

    This routine unlocks all of the records in the log table and then removes
    them from the log table.
  ___________________________________________________________________________*/

void ClearPhysicalRecordSet()
{
        _setClearReq(0xc4);
}


/*________________________ LockPhysicalRecordSet ___________________________

  Input:    int sharedLock;            Flag to indicate 1- The record is to
                                       be locked shareable (it can still be
                                       accessed by others, but can not be
                                       locked exclusively by another), or
                                       0- The record is to be locked
                                       exclusively
            int timeOutLimit;

  Output:   Returns a 0 if successful or
                    0xff Failure
                    0xfe Timeout failure

    This routine will cause all the records in the log table to be locked.
    The records may be locked for exclusive use (sharedLock = 0) or locked
    shareable (sharedLock = 1).  If they are locked shareable others may
    still access the record, but they may not lock the record for exclusive
    access themselves.
  ___________________________________________________________________________*/

int  LockPhysicalRecordSet(lockDirective,timeoutLimit)
BYTE    lockDirective;
WORD    timeoutLimit;
{
        return(_lockReq(0xc2,lockDirective,timeoutLimit));
}

/*________________________ LogPhysicalRecord ___________________________
|
| Input:        int fileHandle;            The dos file handle of the file
|                                          that the record is in
|               long recordStartOffset;    The seek offset into the file where
|                                          the record to be logged begins
|               long recordLength;         The length of the record to be
|                                          logged
|               BYTE lockDirective;        Flags indicating bit 0 (0x01)- lock
|                                          the record now, bit 1 (0x02)- lock
|                                          the record shareable
|               WORD timeoutLimit;
|
| Output:   Returns a 0 if successful or
|               0xff Failed
|               0xfe Timeout fail
|               0x96 No dynamic memory for file
|
|   This routine will log a record into the log table so that the application
|   can create a complete log of all records it needs and then lock them all
|   at once, or the record may be locked as it is logged.  If the record is
|   locked as it is logged, it may be specified as locked shareable.  A locked
|   shareable record still allows others to access the record, but does not
|   let another lock the record exclusively.
|_____________________________________________________________________________*/

int LogPhysicalRecord (fileHandle, recordStartOffset, recordLength,
                       lockDirective, timeoutLimit)
int     fileHandle;
long    recordStartOffset;
long    recordLength;
BYTE    lockDirective;
WORD    timeoutLimit;
{
int     ccode;
        ccode = _physRecReq(0xbc,fileHandle,recordStartOffset,
                            recordLength,lockDirective,timeoutLimit);
        return(ccode);
}

/*________________________ ReleasePhysicalRecord ___________________________

  Input:        int fileHandle;         The DOS file handle of the file
                                        that the record is in.
                long recordStartOffset; The offset within the file where
                                        the record begins.
                long recordLength;      The record length in bytes.

  Output:       Returns a 0 if successful or a 0xff if there record was
                invalid.

        This routine will unlock a record which the application has locked,
        but does not remove the record from the log table.
  __________________________________________________________________________*/

int ReleasePhysicalRecord(fileHandle, recordStartOffset, recordLength)

int     fileHandle;
long    recordStartOffset;
long    recordLength;
{
int     ccode;
        ccode = _physRecReq(0xbd,fileHandle,recordStartOffset,recordLength,
                            0,0);
        return(ccode);
}

/*________________________ ReleasePhysicalRecordSet ________________________

    Input:    None.
    Output:   None.

    This routine releases all of the records in the log table, but does not
    remove them from the log table.
  __________________________________________________________________________*/

void ReleasePhysicalRecordSet()
{
        _setClearReq(0xc3);
}
