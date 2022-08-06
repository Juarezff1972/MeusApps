/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*--------------------------------------------------------------------------

   Example:   ccode = GetLPTCaptureStatus(serverNumber);
  __________________________________________________________________________*/


#include <nwprint.h>
#include <nwmisc.h>

int GetLPTCaptureStatus(serverNumber)
WORD	*serverNumber;	  /* Points to where the current LPT server number
			     will be returned (optional NULL)		    */
{
int	ccode;		  /* 0xFF if LPT capture is active or 0 if not	    */
int	active;


	ccode = _PrCapRequest((BYTE)0xF0, (BYTE)0x3);
	if (serverNumber)
	  *serverNumber = (WORD)(ccode & 0x00FF); /* low byte returned */
	active = (0x00FF & (IntSwap(ccode)));  /* high byte returned */
	return(active);
}
