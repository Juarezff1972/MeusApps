;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Console Error
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ConsoleError
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ConsoleError
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CONERR.ASM
;	FUNCTION:     Display An Error Message On The System Console
;	USAGE:	      ConsoleError(StringLoc);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ConsoleError,<<errormsg,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, errormsg, ds
	mov	di, 04h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ConsoleError

	EndPS

	END
