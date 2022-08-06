;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	WSENV2
	INCLUDE c.mac

;******************************************************************************
;
;		    ---- _GetWorkstationEnvironment ----
;
; Input:    char shellVersion[40]	Pointer to buffer where shell version
;					description will be returned.
;
; Output:   None.
;
; Comments:
;   This routine will return a 40 byte shell version description in the
;   buffer which is passed to it.  It is meant to be called by the routine
;   GetWorkstationEnvironment which will break this 40 byte description apart into
;   the operating system type and version and the hardware type.
;*******************************************************************************

	DOSExterns

	PSeg

	ProcDef _GetWorkstationEnvironment, <<versionString, dptr>>
	PushDI
	mov	ax, 0ea01h	;Function 234 (1) Return Shell Version String
	LoadPtr di, versionString, es
	EnterDOS
	PopDI
	Return
	ProcEnd _GetWorkstationEnvironment

	EndPS

	END

