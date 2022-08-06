;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	PHYSRREQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _physRecReq,<<function,byte>,<handle,word>,<off_set,long>,<length,long>,<directive,byte>,<timeOut,word>>
	PushSI
	PushDI
	mov	ah, function
	mov	al, directive
	mov	bx, handle
	mov	dx, word ptr (off_set)
	mov	cx, word ptr (off_set + 2)
	mov	di, word ptr (length)
	mov	si, word ptr (length + 2)
	mov	bp, timeOut
	EnterDOS
	RetVal	byte, reg, al
	PopDI
	PopSI
	Return
	ProcEnd _physRecReq

	EndPS

	END

