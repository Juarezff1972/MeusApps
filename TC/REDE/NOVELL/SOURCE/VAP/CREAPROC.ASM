;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Create Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  CreateProcess
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: CreateProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CREAPROC.ASM
;	FUNCTION:     Creates a new process that runs in the NetWare internal
;			multi-tasking environment
;	USAGE:	      id = CreateProcess(CFlag, Process, DataSeg, VAPHeaderSeg);
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef CreateProcess,<<connection,word>,<procedure,cptr>,<dataseg,word>,<header,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, connection
ifdef LProg
	les	cx, procedure
	mov	bx, es
else
	push	cs
	pop	bx
	mov	cx, procedure
endif
	mov	dx, dataseg
	mov	bp, header
	mov	di, 018h
	mov	si, seg ProcessControlServices
	mov	es, si
	mov	si, offset ProcessControlServices
	call	dword ptr es:[si]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax
	Return

	ProcEnd CreateProcess

	EndPS

	END
