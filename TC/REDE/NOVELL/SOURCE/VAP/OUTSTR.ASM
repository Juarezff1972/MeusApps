;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Output A String
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  OutString
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: OutString
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     OUTSTR.ASM
;	FUNCTION:     Output A String To The System Monitor At Specified Coordinates.
;	USAGE:	      OutString(Row, Column, StringLoc, StringLength);
;	OUTPUTS:      Outputs The String
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef OutString,<<row,byte>,<column,byte>,<stringloc,dptr>,<stringlength,byte>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ah, row
	mov	al, column
	LoadPtr si, stringloc, ds
	mov	dl, stringlength
	mov	di, 0Ah
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd OutString

	EndPS

	END
