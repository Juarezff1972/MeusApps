/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/**************************************************************************
ClearFile()
ClearFileSet()
LockFileSet()
LogFile()
ReleaseFile()
ReleaseFileSet()

In all cases, the timeoutLimit is the amount of time the network should
wait in trying to lock a file or file set.  Each unit is approximately
1/18 of a second.
**************************************************************************/

#include <nwsync.h>

/*_____________________________ ClearFile ________________________________

  Input:    char *fileName;	   The name of the file to be unlocked and
				   removed from the file log table

  Output:   Returns a 0 if successful of a 0xff if the file name is not
	    in the log table.

    This routine will unlock a file that is in the file log table, and then
    remove the file from the file log table.
  ________________________________________________________________________*/

int ClearFile(fileName)
char	*fileName;
{
	return(_clrRelReq(0xed,fileName));
}

/*__________________________ ClearFileSet __________________________________

    Input:    None.
    Output:   None.

    This routine will unlock all the files that are logged in the file log
    table, and then it will remove them from the file log table.
 ___________________________________________________________________________*/

void ClearFileSet()
{
	_setClearReq(0xcf);
}

/*__________________________ LockFileSet __________________________________

  Input:    WORD timeoutLimit;
  Output:   Returns a 0 if successful or
		0xff Failure
		0xfe Timeout failure

    This routine will lock all the files logged in to the file log table
    for the exclusive use of the station.  If all the files can not be
    locked, the network will wait up to the timeOutLimit specified to see
    if the files become free and can be locked.
 _________________________________________________________________________*/

int LockFileSet(timeoutLimit)
WORD	timeoutLimit;
{
	return(_lockReq(0xcb,0,timeoutLimit));
}


/*__________________________ LogFile __________________________________

  Input:    char *fileName;	   The name of the file to be logged
	    char logFlags;	   Flags specifying: bit 0 (0x01)- lock
				   the file as it is logged, and if bit
				   0 is set then bit 1 (0x02)- lock the
				   file as shareable
	    int timeOutLimit;

  Output:   Returns a 0 if successful or
		0xff Failure
		0xfe Timeout failure
		0x96 No dynamic memory for file

    This routine will log a file into the file log table so that the
    application can create a complete log of all the files that it will
    need and then cause them all to be locked at once; OR if the lock
    flag is set, the file is locked as it is logged.  If the lock flag
    is set then the lock shareable flag may also be set to indicate that
    the file should be locked as shareable.  A file locked as shareable
    can still be accessed by others, but it can not be locked by another
    for their exclusive use.  If the lock flag is set then a time out limit
    should be given to specify how long the network should continue trying
    to lock the files before giving up and returning an error if they could
    not all be locked during the allotted time.
 _________________________________________________________________________*/

int LogFile(fileName, lockDirective, timeoutLimit)
char	*fileName;
BYTE	lockDirective;
WORD	timeoutLimit;
{
	return(_logReq(0xeb,fileName,lockDirective,timeoutLimit));
}


/*__________________________ ReleaseFile __________________________________

  Input:    char *fileName;	  Name of the file to be unlocked
  Output:   Returns 0 if successful or 0xff if the file name is invalid.

    This routine will unlock the given file in the file log table, but does
    not remove the file from the log table.  It allows the application to
    release files for the use of other stations without disrupting the
    integrity of the personal file table.
 ____________________________________________________________________________*/

int ReleaseFile(fileName)
char	*fileName;
{
	return(_clrRelReq(0xec,fileName));
}

/*__________________________ ReleaseFileSet __________________________________

    Input:    None.
    Output:   None.

    This routine unlocks all the files in the file log table, but does not
    remove the files from the log table.
 ___________________________________________________________________________*/

void ReleaseFileSet()
{
	_setClearReq(0xcd);
}
