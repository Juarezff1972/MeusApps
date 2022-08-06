;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETCLRRQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _setClearReq,<<function,byte>>
	mov	ah, function
	EnterDOS
	Return
	ProcEnd _setClearReq

	EndPS

	END

