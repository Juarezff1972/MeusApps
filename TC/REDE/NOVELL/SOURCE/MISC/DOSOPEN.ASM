;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	DOSOPEN
	INCLUDE c.mac

	DSeg
	DExtern <<DOSError, byte>>
	EndDS

	DOSExterns

	PSeg

	ProcDef DOSOpen, <<fileName, dptr>, <mode, byte>>
	PushDS
	LoadPtr dx, fileName, ds
	mov	al, mode
	mov	ah, 3Dh 	;Code for a DOS Open
	EnterDOS		;Make the call
	RetVal	word, reg, ax
	jnc	Done
	IF Model AND 1
	mov	bx, SEG DOSError
	mov	es, bx
	mov	es:DOSError, al
	ELSE
	mov	DOSError, al
	ENDIF
	RetVal	word, imm, -1	;Set error code
Done:
	PopDS
	Return
	ProcEnd DOSOpen
	EndPS
	END
