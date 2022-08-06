;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Console Display
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ConsoleDisplay
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ConsoleDisplay
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CONDSPLY.ASM
;	FUNCTION:     Queue A Message To The System Console
;	USAGE:	      ConsoleDisplay(StringLoc);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ConsoleDisplay,<<msg,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, msg, ds
	mov	di, 05h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ConsoleDisplay

	EndPS

	END
