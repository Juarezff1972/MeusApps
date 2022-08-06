;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	_PRCONRQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _PrConnRequest <<functionNumber,byte>,<connID,byte>>
	mov	ah, 0f0h
	mov	al, functionNumber
	mov	dl, connID
	EnterDOS			;Execute the shell function
	RetVal	byte, reg, al
	Return
	ProcEnd _PrConnRequest

	EndPS

	END

