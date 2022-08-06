;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Delay Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  DelayProcess
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: DelayProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     DLAYPROC.ASM
;	FUNCTION:
;	USAGE:	      DelayProcess(ticks);
;	VERSION:	01.00
;_____________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef DelayProcess,<<ticks,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	dx, ticks
	mov	di, 0Ah
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd DelayProcess

	EndPS

	END

