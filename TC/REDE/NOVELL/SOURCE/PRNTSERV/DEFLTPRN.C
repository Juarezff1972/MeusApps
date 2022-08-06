/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>
/*____________________________________________________________________________
|
|  Name:       DEFLTPRN.C
|
|  Functions:
|	       SetDefaultLocalPrinter -  Sets default LPT device for doing
|					 default LPC capture functions.
|____________________________________________________________________________*/

int SetDefaultLocalPrinter(LPTDevice)
BYTE	LPTDevice;
{
int	ccode;
	return(ccode = _PrnSrvRequest(5,0,0,LPTDevice));
}
