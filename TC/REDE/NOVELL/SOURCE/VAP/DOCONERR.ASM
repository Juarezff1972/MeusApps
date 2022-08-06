;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Do Console Error
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  DoConsoleError
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: DoConsoleError
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     DOCONERR.ASM
;	FUNCTION:     Display An Error Message On The System Console
;	USAGE:	      DoConsoleError(StringLoc);
;	OUTPUTS:      Outputs The String
;	VERSION:	01.00
;_____________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef DoConsoleError,<<errormsg,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, errormsg, ds
	mov	di, 13h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd DoConsoleError

	EndPS

	END
