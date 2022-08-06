/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwdir.h>
#include <nwwrkenv.h>

/*_________________________________________________________________________
  |
  |  Comments:	This routine forces the drive vector to match the PATH
  |		environment variable.
  |_______________________________________________________________________*/


#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))

void AllignDriveVectorToPath(pathVariable)
char   *pathVariable;

{
BYTE	driveStatus;
BYTE	driveVector[17];
int	vectorIndex;

    GetSearchDriveVector( driveVector );
    vectorIndex = 0;
    while ( ( pathVariable != (char *)NULL ) && ( vectorIndex < 17 ) ) {
	if ( pathVariable[1] == ':' ) {
	    driveStatus = GetDriveInformation( (BYTE)(upcase(*pathVariable) - 'A'),
		    (WORD *)NULL, (BYTE *)NULL );
	    if ( driveStatus != 0x80 ) {
		driveVector[ vectorIndex++ ] = upcase(*pathVariable) - 'A';
	    }
	}
	pathVariable = strchr( pathVariable, ';' );
	if ( pathVariable != (char *)NULL )
	    pathVariable++;
    }
    for ( ; vectorIndex <= 17; vectorIndex++ )
	driveVector[ vectorIndex ] = 0xFF;
    SetSearchDriveVector( driveVector );
}
