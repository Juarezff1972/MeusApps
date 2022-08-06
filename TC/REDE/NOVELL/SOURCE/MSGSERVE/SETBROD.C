/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdio.h>
#include <nwconsol.h>
#include <nwmsg.h>


/*___________________________________________________________________________

   Output:    Current Broadcast Mode  --  SUCCESSFUL
				  7D  --  INVALID_MODE

   Comments:  This function sets the broadcast mode of the requesting
	      workstation.  The valid modes are: 0, 1, 2, and 3.
  ___________________________________________________________________________*/


int SetBroadcastMode(mode)
BYTE  mode;
{
int   ccode;


      if (mode == 0 || mode == 1 || mode == 2 || mode == 3)
       {
	  _ServerRequest( mode, (BYTE)0xDE, (BYTE)0 );
	  ccode = 0;
       }
      else
	  ccode = 0x7D;

      return(ccode);
}
