;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Kill Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  KillProcess
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: KillProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     KILLPROC.ASM
;	FUNCTION:     Terminates Calling Process
;	USAGE:	      KillProcess();
;	VERSION:	01.00
;	REMARKS:	InitializationComplete() MUST have been called previously
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef KillProcess,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 08h
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

	ProcEnd KillProcess

	EndPS

	END
