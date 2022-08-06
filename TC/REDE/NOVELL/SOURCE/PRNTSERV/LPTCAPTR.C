/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>



/*__________________________________________________________________________

	    Name:	LPTCAPTR.C

	    Functions:	int StartLPTCapture
			int EndLPTCapture
			int CancelLPTCapture
			int FlushLPTCapture
			int StartSpecificLPTCapture(LPTDevice)
			int EndSpecificLPTCapture(LPTDevice)
			int CancelSpecificLPTCapture(LPTDevice)
			int FlushSpecificLPTCapture(LPTDevice)
  ________________________________________________________________________*/


int StartLPTCapture()
{
int	ccode;
	return(ccode = _LPTCapReq(0,0));
}




int EndLPTCapture()
{
int	ccode;
	return(ccode = _LPTCapReq(1,0));
}




int CancelLPTCapture()
{
int	ccode;
	return(ccode = _LPTCapReq(2,0));
}




int FlushLPTCapture()
{
int	ccode;
	return(ccode = _LPTCapReq(3,0));
}




int StartSpecificLPTCapture(LPTDevice)
BYTE	LPTDevice;
{
int	ccode;
	return(ccode = _LPTCapReq(4,LPTDevice));
}




int EndSpecificLPTCapture(LPTDevice)
BYTE	LPTDevice;
{
int	ccode;
	return(ccode = _LPTCapReq(5,LPTDevice));
}




int CancelSpecificLPTCapture(LPTDevice)
BYTE	LPTDevice;
{
int	ccode;
	return(ccode = _LPTCapReq(6,LPTDevice));
}




int FlushSpecificLPTCapture(LPTDevice)
BYTE	LPTDevice;
{
int	ccode;
	return(ccode = _LPTCapReq(7,LPTDevice));
}

