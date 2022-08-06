;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LOCKREQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _lockReq,<<function,byte>,<mode,byte>,<timeOut,word>>
	mov	ah, function
	mov	al, mode
	mov	bp, timeOut
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd _lockReq

	EndPS

	END

