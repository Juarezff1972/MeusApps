;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	AZLENSTR
	INCLUDE c.mac

;****************************************************************************
;			------ ASCIIZToLen ------
; Example:  ccode = ASCIIZToLen(destStr, srcStr);
;
; Input;    char *destStr;	Receives the  ASCIIZ string
;	    char *srcStr;	Pointer to the string to convert
;
; Output:	0	string was converted
;		>0	string is not convertable (over 255 characters)
;
; Comments:
;   This function converts an ASCIIZ string to a length specified string.
;   It is the programmer's responsibility to make sure destStr is large
;   enough to contain the converted string.
;
;***/

	DSeg
	EndDS

	DOSExterns

	PSeg

	ProcDef ASCIIZToLenStr,<<destStr, dptr>,<srcStr, dptr>>
	PushSI
	PushDI
	PushES
	PushDS
	resetd			    ;positive direction of copy, search
	LoadPtr di, srcStr, es
	xor	AL,AL		    ;search for null
	mov	CX,255		    ;CX is 255-len(srcStr)
	repnz	scasb		    ;first occurance of null
	jcxz	ERROR		    ;string is longer than 255
	neg	CX		    ;len(srcStr)-255
	add	CX,255		    ;len(srcStr)
	dec	CX
	LoadPtr si, srcStr, ds
	LoadPtr di, destStr, es
	mov	ES:[DI],CL	    ;length of string
	inc	DI
	rep	movsb		    ;copy the string
	RetVal	word,imm,0	    ;return no error
	jmp	DONE
ERROR:	RetVal	word,imm,1
DONE:
	PopDS
	PopES
	PopDI
	PopSI
	Return
	ProcEnd ASCIIZToLenStr

	EndPS

	End
