;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LSTRCPY
	INCLUDE c.mac

;******************************************************************************
;
; Input;    char *destStr	    String to be copied to
;	    char *srcStr	    String to copied
;
; Output:   Returns a pointer to destStr;
;
; Comments:
;   This function copies an ASCII string with a byte length specified.
;   It is the programmer's responsibility to make sure destStr is large
;   enough to contain the string.
;
;******************************************************************************

	DSeg
	EndDS

	DOSExterns

	PSeg

	ProcDef LenStrCpy,<<destStr, dptr>,<srcStr, dptr>>
	PushSI
	PushDI
	PushES
	PushDS
	resetd			    ;positive direction of copy
	LoadPtr di, destStr, es
	LoadPtr si, srcStr, ds
	mov	CL,DS:[SI]	    ;length of source string
	xor	CH,CH		    ;convert to unsigned word
	inc	CX		    ;copy string plus length byte
	rep	movsb		    ;copy the string
;
;RetVal wont work with a DPTR argument passed on the stack
;   the following code is to overcome this limitation
;
IF	Model and 1		    ;large data model
	lds	AX,destStr
	mov	DX,DS
ELSE				    ;small data model
	mov	AX,destStr
ENDIF
	PopDS
	PopES
	PopDI
	PopSI
	Return
	ProcEnd LenStrCpy

	EndPS

	End
