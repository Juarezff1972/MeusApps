;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Set Screen Mode
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  SetScreenMode
.LALL

;------------------------------------------------------------------------------
;	PROGRAM NAME: SetScreenMode
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SETSCRN.ASM
;	FUNCTION:     Get Control Of System Console
;	USAGE:	      SetScreenMode(VAPID);
;	OUTPUTS:      Clears the Screen
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef SetScreenMode,<<vapid,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, vapid
	mov	di, 08h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd SetScreenMode

	EndPS

	END
