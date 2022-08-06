;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Get Screen Mode
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  GetScreenMode
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: GetScreenMode
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     GETSCRN.ASM
;	FUNCTION:     Gets VAPID Of VAP With Control The System Console
;	USAGE:	      VAPID = GetScreenMode();
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef GetScreenMode,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 09h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI
	pop	bp

	RetVal	word,reg,ax
	Return

	ProcEnd GetScreenMode

	EndPS

	END
