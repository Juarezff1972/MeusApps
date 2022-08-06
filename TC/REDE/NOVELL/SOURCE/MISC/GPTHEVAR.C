/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*___________________________________________________________________________
  |  Comments:	This function allocates space for and returns the path
  |		environtment variable.	The calling routine must use free()
  |		to deallocate the space.  If an error occurs, though, no
  |		memory will be allocated.
  |_________________________________________________________________________*/

#include <stdlib.h>
#include <nwlocal.h>
#include <nwwrkenv.h>

char  pathVariableName[] = "PATH";

int GetPathEnvironmentVariable( pathVariable )
char   **pathVariable;
{
   int	pathLength;
   int		rcode = 0;

   pathLength = GetEnvironmentVariableLength( pathVariableName );
	if ( pathLength == -1 )
	   rcode = 0xff;	      /*  255d, PATH_NOT_LOCATABLE */

   *pathVariable = (char *) calloc(1, pathLength + 256 );
	if( *pathVariable != NULL )
	   rcode = GetEnvironmentVariable( pathVariableName, *pathVariable );
	else
	   rcode = 0xb6;			    /* 182d, WORSTATION_OUT_OF_MEMORY */
   return( rcode );
}
