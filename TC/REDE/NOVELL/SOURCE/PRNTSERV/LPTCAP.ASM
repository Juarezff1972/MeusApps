;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LPTCAP
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _LPTCapReq <<functionNumber,byte>,<device,byte>>

	mov	ah,0DFh
	mov	dl,functionNumber
	mov	dh,device
	EnterDOS
	sub	ah,ah		;clear high byte
	RetVal	word, reg, ax
	Return

	ProcEnd _LPTCapReq

	EndPS

	END
