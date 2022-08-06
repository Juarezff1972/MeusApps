;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	DISABLPD
	INCLUDE c.mac
;****************************************************************************
;
; DisablePrinterDefaults
;
; SYNOPSIS
;
;   ccode = DisablePrinterDefaults();
;
;   int ccode;  	If wrong shell version, nonzero return.
;
; DESCRIPTION
;
; This function disables the shell's printer default table.  When this
; capability is disabled, the default printer parameters are not sent
; to the File server when a ModifyLPTDevice call is made.
;***************************************************************************

	DOSExterns

	PSeg

	ProcDef DisablePrinterDefaults
	sub	sp, 1			;Allocate 1 byte for default info.
	mov	bx, sp
	IF	Model AND 1
	mov	ax, ss
	mov	es, ax
	ENDIF
	mov	ax, 0B800h		;Specify Get Printer Defaults
	mov	cx, 1			;# of bytes to get
	EnterDOS
	or	ax, ax
	jnz	DPDError
	mov	byte ptr [bx], 1	;Disable printer defaults
	;
	; Send the altered table back to the shell
	;
	mov	cx, 1
	mov	ax, 0B801h
	EnterDOS

DPDError:

	;
	; All Done!!
	;
	RetVal	word, reg, ax
	add	sp, 1
	Return
	ProcEnd DisablePrinterDefaults

	EndPS

	END

