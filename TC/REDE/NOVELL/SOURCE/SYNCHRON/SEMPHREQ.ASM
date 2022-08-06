;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SEMPHREQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _semaphorReq,<<function, byte>,<handle, long>,<timeout, word>>
	mov	ah, 0c5h
	mov	al, function
	mov	dx, word ptr (handle)
	mov	cx, word ptr (handle + 2)
	mov	bp, timeout
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd _semaphorReq

	EndPS

	END

