;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	TABLEREQ
	INCLUDE c.mac

	DOSExterns

	PSeg

	ProcDef _TableReq, <<functionNumber,byte>,<subFunction,byte>,<tableAddr,dptr>>
	PushDS
	PushSI
	PushDI
	PushES
	mov	ah, functionNumber
	mov	al, subFunction
	EnterDOS			;Execute the shell function
	push	ax
	LoadPtr di, tableAddr, ds	;get the table address
	ChkNull ax, di, ds
	jz	no_adr
	mov	[di], si		;and store the table address
	mov	[di + 2], es
no_adr: pop	ax
	RetVal	byte, reg, al
	PopES
	PopDI
	PopSI
	PopDS
	Return
	ProcEnd _TableReq

	EndPS

	END

