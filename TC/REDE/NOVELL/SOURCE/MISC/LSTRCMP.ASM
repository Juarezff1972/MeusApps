;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LSTRCMP
	INCLUDE c.mac

;******************************************************************************
;
; Input;    char *string1	    First string to compare
;	    char *string2	    Second string to compare
;
; Output:   returns value < 0 if string1 < string2, 0 if string1 = string2,
;	    value > 0 if string1 > string2;
;
; Comments:
;	This function compares two ASCII strings with a byte length specified.
;
;******************************************************************************

	DSeg
	EndDS

	DOSExterns

	PSeg

	ProcDef LenStrCmp <<string1, dptr>,<string2, dptr>>
	PushSI
	PushDI
	PushES
	PushDS
	resetd			    ;positive direction of copy
	LoadPtr si, string1, ds
	LoadPtr di, string2, es
	mov	CL,DS:[SI]	    ;length of string 1
	cmp	CL,ES:[DI]	    ;is string 2 smaller?
	jle	ASC_00		    ;no - use string 1 length
	mov	CL,ES:[DI]
ASC_00:
	xor	CH,CH		    ;convert to unsigned word
	inc	DI		    ;point to first character
	inc	SI		    ;point to first character
	repe	cmpsb		    ;compare the strings
	jcxz	COMPARELENGTH
	dec	DI		    ;point to differing characters
	dec	SI
	jmp	FINALCOMPARE
COMPARELENGTH:
	LoadPtr si, string1, ds
	LoadPtr di, string2, es
FINALCOMPARE:
	mov	AL,DS:[SI]
	sub	AL,ES:[DI]
	cbw
	RetVal	word, reg, AX
	PopDS
	PopES
	PopDI
	PopSI
	Return
	ProcEnd LenStrCmp

	EndPS

	End
