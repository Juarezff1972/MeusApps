;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Gets (Reads) A Command String From The Console
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ReadKeyboard
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ReadKeyboard
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     READKBRD.ASM
;	FUNCTION:     Gets (Reads) A Command String From The Console
;	USAGE:	      ReadKeyboard(StringLoc, StringLength);
;	OUTPUTS:      none
;	VERSION:	01.00
;_____________________________________________________________________________


	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ReadKeyboard,<<bufloc,dptr>,<buflength,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, buflength
	push	ax
	LoadPtr di, bufloc, ds
	push	ds
	push	di
	mov	di, 02h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]
	add	sp, 6
	RetVal	word, reg, ax

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ReadKeyboard

	EndPS

	END
