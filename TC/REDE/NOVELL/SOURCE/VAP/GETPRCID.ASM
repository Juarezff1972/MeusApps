;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Get ProcessID
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  GetProcessID
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: GetProcessID
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     GETPRCID.ASM
;	FUNCTION:     Places THe Specified ProcessID Back On The Run Queue
;	USAGE:	      id = GetProcessID();
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef GetProcessID,<>
	push	bp
	PushDI
	PushDS
	PushES
	PushSI

	mov	di, 0Bh
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI
	pop	bp

	RetVal	word,reg,ax
	Return

	ProcEnd GetProcessID

	EndPS

	END
