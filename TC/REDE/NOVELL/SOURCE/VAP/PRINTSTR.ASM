;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Print A String
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  PrintString
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: PrintString
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     PRINTSTR.ASM
;	FUNCTION:     Print A String To The System Monitor At Specified Coordinates.
;	USAGE:	      PrintString(Row, Column, StringLoc, StringLength);
;	OUTPUTS:      Outputs The String
;	VERSION:	01.00
;_______________________________________________________________________________


	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef PrintString,<<row,word>,<column,word>,<stringloc,dptr>,<stringlength,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, stringlength
	push	ax
	LoadPtr di, stringloc, ds
	push	ds
	push	di
	mov	ax, column
	push	ax
	mov	ax, row
	push	ax
	mov	di, 01h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]
	add	sp, 10

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd PrintString

	EndPS

	END
