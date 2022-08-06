;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Spawn Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  SpawnProcess
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: SpawnProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SPWNPROC.ASM
;	FUNCTION:     Creates a new process that runs in the NetWare internal
;			multi-tasking environment
;	USAGE:	      id = SpawnProcess(Process, DataSeg, VAPHeaderSeg);
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef SpawnProcess,<<procedure,cptr>,<dataseg,word>,<header,word>>
	PushDI
	PushDS
	PushES
	PushSI

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
	mov	di, 05h
	mov	ax, seg ProcessControlServices
	mov	es, ax
	mov	si, offset ProcessControlServices
	call	dword ptr es:[si]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax
	Return

	ProcEnd SpawnProcess

	EndPS

	END
