;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LSTRCAT
	INCLUDE c.mac

;******************************************************************************
;
; Input;    char *destStr	    String to be concatinated to
;	    char *srcStr	    String to concatinate
;
; Output:   returns a pointer to destStr;
;
; Comments:
;   This function concatinates two ASCII strings with a byte length specified.
;   It is the programmer's responsibility to make sure destStr is large
;   enough to contain to concatinated string.
;
;******************************************************************************

	DSeg
	EndDS

	DOSExterns

	PSeg

	ProcDef LenStrCat,<<destStr, dptr>,<srcStr, dptr>>
	PushSI
	PushDI
	PushES
	PushDS
	resetd			    ;positive direction of copy
	LoadPtr di, destStr, es
	LoadPtr si, srcStr, ds
	mov	AL,ES:[DI]	    ;length of destination string
	xor	AH,AH		    ;convert to unsigned word
	mov	CL,DS:[SI]	    ;length of source string
	xor	CH,CH		    ;convert to unsigned word
	add	ES:[DI],CL	    ;length of resultant string
	add	DI,AX		    ;last byte of string
	inc	DI		    ;first byte to copy to
	inc	SI		    ;first character of source string
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
	ProcEnd LenStrCat

	EndPS

	End
