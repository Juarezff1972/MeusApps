;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Sleep Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  SleepProcess
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: SleepProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SLEEPROC.ASM
;	FUNCTION:     Remove Current Process From Active Run Queue
;	USAGE:	      SleepProcess();
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef SleepProcess,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 0Ch
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI
	pop	bp

	Return

	ProcEnd SleepProcess

	EndPS

	END
