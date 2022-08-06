;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	CLRRELRQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _clrRelReq <<function,byte>,<clrName, dptr>>
	PushDS
	mov	ah, function
	LoadPtr dx, clrName, ds
	EnterDOS
	RetVal	byte, reg, al
	PopDS
	Return
	ProcEnd _clrRelReq

	EndPS

	END

