/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdlib.h>
#include <nwdir.h>
#include <nwwrkenv.h>

int IsSearchDrive(driveLetter)

char	driveLetter;

{
char   *pathVariable;
int	searchOrder;
int	ccode;


    ccode = GetPathEnvironmentVariable(&pathVariable);
    if ( ccode )
       return( FALSE);

    searchOrder = GetSearchOrderFromDriveLetter( pathVariable, driveLetter );
    free( pathVariable );
    return( searchOrder != 0 ? TRUE : FALSE );

}
