;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETBROD
	INCLUDE c.mac

 ;___________________________________________________________________________
 ;
 ; Output:    Returns the current broadcast mode
 ;
 ; Example:   BYTE  mode;
 ;
 ;	      mode = GetBroadcastMode();
 ;
 ;
 ; Comments:  This function returns the broadcast mode of the requesting
 ;	      workstation.  The valid modes are: 0, 1, 2, and 3.
 ;___________________________________________________________________________*/

	DOSExterns

	PSeg

	ProcDef GetBroadcastMode
	mov	ah, 0DEh
	mov	dl, 04h
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd GetBroadcastMode

	EndPS

	END

