/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwmisc.h>

/*___________________________________________________________________________
  |
  |  Comments:	This function returns a pointer to the Nth element in the PATH
  |		environment variable where searchOrder is N.  If N is negative,
  |		a pointer to the first element will be returned. If N is greater
  |		than the number of elements, a null pointer will be returned.
  |_________________________________________________________________________*/


char *_FindPathVariableElement(pathVariable, searchOrder)
char   *pathVariable;
int	searchOrder;
{
int	loopCount;
char   *returnPtr;



    loopCount = strlen(pathVariable);
    if (!loopCount)
	return(NULL);
    returnPtr = pathVariable;
    for ( loopCount = searchOrder - 1;
	  ((loopCount > 0) && (returnPtr != (char *)NULL)); loopCount-- )
     {
	returnPtr = strchr( returnPtr, ';' );
	if ( returnPtr != (char *)NULL )
	     returnPtr++;
     }
    if ( returnPtr )
	while ( IsWhiteSpace( *returnPtr ) )
	    returnPtr++;

    return( returnPtr );
}
