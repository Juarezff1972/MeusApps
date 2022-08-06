/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>

/*_________________________________________________________________________
  |
  |  Comments:	This function returns the order a drive letter appears in
  |		the environment variable PATH.	If the drive letter does not
  |		appear in the path, zero will be returned.  If the drive
  |		letter appears more than once in the path, the order of the
  |		first appearance will be returned.  The order of the first
  |		drive in the path is 1, the second is 2, etc.
  |_______________________________________________________________________*/

#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))

int GetSearchOrderFromDriveLetter( pathVariable, driveLetter )

char   *pathVariable;
char	driveLetter;

{
int	count;
char	*temp;

	if (pathVariable == (char *)NULL)
		goto ERROR;
    count = 0;
	temp = pathVariable;
    while ( ( pathVariable != (char *)NULL ) &&
	    ( ( *pathVariable != upcase( driveLetter ) ) ||
	      ( pathVariable[1] != ':' ) ) ) {
	pathVariable = strchr( pathVariable, ';' );
	if ( pathVariable != (char *)NULL ) {
	    pathVariable++;
	    count++;
	}
    }
	if (count > 0)
		count++;
	if ((pathVariable == temp) &&
		(count == 0))
		count++;
ERROR:
    return( (pathVariable == (char *)NULL) ? 0 : count );
}
