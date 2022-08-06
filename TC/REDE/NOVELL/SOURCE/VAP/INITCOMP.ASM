;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Complete Installation
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  InitializationComplete
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: InitializationComplete
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     INITCOMP.ASM
;	FUNCTION:     Complete Initialization To Allow NetWare To Resume
;	USAGE:	      InitializationComplete();
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef InitializationComplete,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 07h
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

	ProcEnd InitializationComplete

	EndPS

	END
