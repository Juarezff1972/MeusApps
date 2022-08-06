/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>

/*_________________________________________________________________________
|
|   Output:                   0  --  SUCCESSFUL
|            Network Error Code  --  UNSUCCESSFUL
|
|   Comments:  This function returns the user name that will be printed
|              on a banner page.
|
|__________________________________________________________________________*/


int GetBannerUserName(userName)
char    *userName;
{
int     ccode;

        ccode = _PrnSrvRequest(8, 12, userName, 0);
        return( ccode );
}

int SetBannerUserName(userName)
char    *userName;
{
int     ccode;

        ccode = _PrnSrvRequest(9, 12, userName, 0);
        return( ccode );
}
