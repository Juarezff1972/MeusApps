;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LENAZSTR
	INCLUDE c.mac

;***************************************************************************
;		       ----- LenToASCIIZStr -----
;
; Example:  ccode = LenToASCIIZStr(destStr, srcStr);
;
; Input:    char *destStr	 Receives the ASCIIZ string
;	    char *srcStr	 Pointer to string to convert
;
; Output:	0	string was converted
;		>0	string is not convertable (contains nulls)
;			(destination string will contain characters up to
;			the first null).  The number returned is the number
;			of characters not copied to the destination string.
;
; Comments:
;   This function converts a length specified string to an ASCIIZ string.
;   It is the programmer's responsibility to make sure destStr is large
;   enough to contain the converted string
;
;***************************************************************************

	DSeg
	EndDS

	DOSExterns

	PSeg

	ProcDef LenToASCIIZStr,<<destStr, dptr>,<srcStr, dptr>>
	PushSI
	PushDI
	PushES
	PushDS
	resetd			    ;positive direction of copy
	LoadPtr di, srcStr, es	    ;check for null characters
	mov	CL,ES:[DI]	    ;length of source string
	xor	CH,CH		    ;convert to unsigned word
	xor	AL,AL		    ;search for NULL
	inc	DI
	repnz	scasb
	mov	DX,CX		    ;number of characters not copied
	LoadPtr si, srcStr, ds
	jcxz	NONULL
	neg	CL
	add	CL,DS:[SI]
	jmp	CONTINUE
NONULL:
	LoadPtr si, srcStr, ds
	mov	CL,DS:[SI]
CONTINUE:
	xor	CH,CH		    ;convert string length to WORD
	LoadPtr di, destStr, es
	inc	SI		    ;first character of source string
	rep	movsb		    ;copy the string - stop if NULL found
;	 inc	DI
	mov	BYTE PTR ES:[DI],0  ;null terminated
	RetVal	word,reg,dx	    ;return number of chars not copied
	PopDS
	PopES
	PopDI
	PopSI
	Return
	ProcEnd LenToASCIIZStr

	EndPS

	End
