;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	PRCONNRQ
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef _PrCapRequest <<function,byte>,<subfunction,byte>>
	mov	ah, function
	mov	al, subfunction
	EnterDOS
	RetVal	word, reg, ax
	Return
	ProcEnd _PrCapRequest

	EndPS

	END
