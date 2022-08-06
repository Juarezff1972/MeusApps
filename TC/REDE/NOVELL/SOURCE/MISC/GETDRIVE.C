/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwlocal.h>


/*________________________________________________________________________

  Output:    Returns the default drive number.

  Comments:  This function returns the default drives number. (i.e. A=0,
	     B=1, C=2).
  ________________________________________________________________________*/


int  GetDefaultDrive()
{
int  driveNumber;


     driveNumber = _ShellRequest((BYTE)0x19, 0, 0);
     return(driveNumber);

}
