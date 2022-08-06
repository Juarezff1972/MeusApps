;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Change Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ChangeProcess
.LALL

;------------------------------------------------------------------------------
;	PROGRAM NAME: ChangeProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CHNGPROC.ASM
;	FUNCTION:     Puts Current Process At End Of Its Priority Level In The Internal RunQueue
;			And Reschedules The Highest Priority Task For Execution
;	USAGE:	      ChangeProcess();
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef ChangeProcess,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 09h
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

	ProcEnd ChangeProcess

	EndPS

	END
