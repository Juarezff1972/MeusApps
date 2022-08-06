;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Clear System Monitor
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ClearScreen
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ClearScreen
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CLRSCRN.ASM
;	FUNCTION:     Clear The System Monitor.
;	USAGE:	      ClearScreen();
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ClearScreen,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 00h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI
	pop	bp

	Return

	ProcEnd ClearScreen

	EndPS

	END

