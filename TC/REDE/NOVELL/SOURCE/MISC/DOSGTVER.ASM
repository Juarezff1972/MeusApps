;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	DOSGTVER
	INCLUDE C.MAC

;******************************************************************************
;
; Input int version	The major version number is in the high byte and the
;			minor version is in the low byte.  DOS 3.1 would return
;			0x0301.
;*******************************************************************************


	DOSExterns

	PSeg

	ProcDef GetDosVersion
	mov	ah, 30h
	EnterDOS
	xchg	ah, al
	RetVal	word, reg, ax
	Return
	ProcEnd GetDosVersion

	EndPS
	END

