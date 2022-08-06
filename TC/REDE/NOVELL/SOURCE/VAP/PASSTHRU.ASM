;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Shell Pass Thru Enable
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ShellPassThroughEnable
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ShellPassThroughEnable
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     PASSTHRU.ASM
;	FUNCTION:     Enables/Disables VAP's ability to access DOS
;	USAGE:	      ShellPassThroughEnable(flag);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef ShellPassThroughEnable,<<flag,byte>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	al, flag
	mov	di, 06h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ShellPassThroughEnable

	EndPS

	END
