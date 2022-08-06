;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	ENABLEPD
	INCLUDE c.mac
;***************************************************************************
;
; EnablePrinterDefaults
;
; SYNOPSIS
;
;  ccode = EnablePrinterDefaults();
;
;  int ccode;			If wrong shell version, nonzero return.
;
; DESCRIPTION
;
;  This function enables the shell's printer default table.  When this
;  capability is enabled, the default printer parameters are sent
;  to the File server any time a ModifyLPTDevice call is made.
;***************************************************************************

	DOSExterns

	PSeg

	ProcDef EnablePrinterDefaults
	sub	sp, 1			;Allocate 1 byte
	mov	bx, sp
	IF	Model AND 1
	mov	ax, ss
	mov	es, ax
	ENDIF
	mov	ax, 0B800h		;Specify Get Printer Defaults
	mov	cx, 1			;# of bytes to get
	EnterDOS
	or	ax, ax
	jnz	EPDError
	mov	byte ptr [bx], 0	;Enable printer defaults
	;
	; Send the altered table back to the shell
	;
	mov	ax, 0B801h
	mov	cx, 1
	EnterDOS

EPDError:

	;
	; All Done!!
	;
	RetVal	word, reg, ax
	add	sp, 1
	Return
	ProcEnd EnablePrinterDefaults

	EndPS

	END

