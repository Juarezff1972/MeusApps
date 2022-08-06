;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LOGREQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _logReq, <<function,byte>,<logName,dptr>,<directive,byte>,<timeOut,word>>
	PushDS
	mov	ah, function
	LoadPtr dx, logName, ds
	mov	al, directive
	mov	bp, timeOut
	EnterDOS
	RetVal	byte, reg, al
	PopDS
	Return
	ProcEnd _logReq

	EndPS

	END

