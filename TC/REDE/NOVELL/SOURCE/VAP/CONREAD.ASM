;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Console Read
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ConsoleRead
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ConsoleRead
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CONREAD.ASM
;	FUNCTION:     Display A Prompt On System Console And Read Input
;	USAGE:	      length = ConsoleRead(PromptLoc, BufLoc, BufLength);
;	OUTPUTS:      Outputs The Prompt
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ConsoleRead,<<msg,dptr>,<buf,dptr>,<buflen,byte>>
	PushDI
	PushDS
	push	es
	PushSI

	LoadPtr si, msg, ds
	LoadPtr bx, buf, es
	mov	dl, buflen
	mov	di, 07h

	mov	cx, word ptr ConsoleControlServices+2
	push	cx
	mov	cx, word ptr ConsoleControlServices
	push	cx
	mov	bp, sp
	call	dword ptr ss:[bp]
	add	sp, 4

	PopSI
	pop	es
	PopDS
	PopDI

	RetVal	word,reg,ax
	Return

	ProcEnd ConsoleRead

	EndPS

	END
