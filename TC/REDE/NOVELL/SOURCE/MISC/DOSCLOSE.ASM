;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	DOSCLOSE
	INCLUDE C.MAC

	DSeg
	DExtern <<DOSError, byte>>
	EndDS

	DOSExterns

	PSeg

	ProcDef DOSClose, <<handle, word>>
	mov	bx, handle
	mov	ah, 3Eh
	EnterDOS
	jc	SetError
	RetVal	word, imm, 0
	jmp	Done
SetError:
	IF Model AND 1
	mov	bx, SEG DOSError
	mov	es, bx
	mov	es:DOSError, al
	ELSE
	mov	DOSError, al
	ENDIF
	RetVal	word, imm, -1
Done:
	Return
	ProcEnd DOSClose

	EndPS
	END

