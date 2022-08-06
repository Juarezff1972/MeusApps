/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwacct.h>
#include <nwbindry.h>
#include <nwconsol.h>
#include <nwwrkenv.h>

/*_____________________________AccountingInstalled ____________________________
|
|  Output:  1  --  Accounting IS Installed
|
|  Comments:  This function returns a 1 if Accounting is installed on the
|	      file server.
|_____________________________________________________________________________*/

int AccountingInstalled(connectionID)

WORD	connectionID;

{
int	accountingIsInstalled;		/* return value */
BYTE	accountingVersion;		/* 1 means accounting installed */
int	ccode;				/* completion code */
int	ccode2; 			/* completion code */
char	fileServerName[48];		/* name of server being accessed */
WORD	oldConnectionID;		/* ID to restore after execution */
long	sequence = -1;			/* get first in sequence */

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );
    GetFileServerName( connectionID, fileServerName );
    ccode = GetFileServerExtendedInfo(&accountingVersion, NULL, NULL, NULL,
		NULL, NULL, NULL );
    ccode2 = ScanProperty( fileServerName, OT_FILE_SERVER, "ACCOUNT_SERVERS",
		&sequence, NULL, NULL, NULL, NULL, NULL );
    accountingIsInstalled = ( ( ccode == 0 ) && ( ccode2 == 0 ) &&
			      ( accountingVersion == 1 ) );
    SetPreferredConnectionID( oldConnectionID );
    return( accountingIsInstalled );
}
