;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Console Message
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ConsoleMessage
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ConsoleMessage
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CONMSG.ASM
;	FUNCTION:     Display A Message On The System Console
;	USAGE:	      ConsoleMessage(StringLoc);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ConsoleMessage,<<msg,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, msg, ds
	mov	di, 03h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ConsoleMessage

	EndPS

	END
