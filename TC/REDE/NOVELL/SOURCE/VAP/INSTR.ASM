;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Input A String
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  InString
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: InString
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     INSTR.ASM
;	FUNCTION:     Input A String From The System Monitor At Specified Coordinates.
;	USAGE:	      length = InString(Row, Column, PromptLoc, BufLoc, BufLength);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef InString,<<row,byte>,<column,byte>,<promptloc,dptr>,<bufloc,dptr>,<buflen,byte>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ah, row
	mov	al, column
	LoadPtr si, promptloc, ds
	LoadPtr bx, bufloc, es
	mov	dl, buflen
	mov	di, 0Bh

	mov	cx, word ptr ConsoleControlServices+2
	push	cx
	mov	cx, word ptr ConsoleControlServices
	push	cx
	mov	bp, sp
	call	dword ptr ss:[bp]
	add	sp, 4

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax
	Return

	ProcEnd InString

	EndPS

	END
