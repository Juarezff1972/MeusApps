;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	PRNSRVRQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _PrnSrvRequest <<functionNum,byte>,<bufSize,word>,<replyBuf,dptr>,<prnDevice,byte>>

	PushES
	mov	ah, 0b8h
	mov	al, functionNum
	mov	cx, bufSize
	mov	dh, prnDevice
	LoadPtr bx, replyBuf, es
	EnterDOS
	sub	ah,ah
	RetVal	word, reg, ax
	PopEs

	Return

	ProcEnd _PrnSrvRequest

	EndPS

	END

