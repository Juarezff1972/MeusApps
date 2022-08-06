/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>

#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))

char GetDriveLetterFromSearchOrder( pathVariable, searchOrder )

char   *pathVariable;
int     searchOrder;

{
char    driveLetter;

    pathVariable = _FindPathVariableElement( pathVariable, searchOrder );
    driveLetter = '\0';

    if (pathVariable != NULL)
       if ( pathVariable[1] == ':' )
           driveLetter = upcase( pathVariable[0] );

    return( driveLetter );
}
