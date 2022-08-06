;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Get Interrupt Vector
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  GetInterruptVector
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: GetInterruptVector
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     GETINTV.ASM
;	FUNCTION:
;	USAGE:	      GetInterruptVector(intNumber, &ISRSegment, &ISROffset);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef GetInterruptVector,<<intNumber,byte>,<isrsegment,dptr>,<isroffset,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	al, intNumber
	mov	di, 0Fh
	mov	cx, seg ProcessControlServices
	mov	es, cx
	mov	bx, offset ProcessControlServices
	push	bp
	call	dword ptr es:[bx]
	pop	bp

	LoadPtr si, isrsegment, ds
	mov	ds:[si], cx
	LoadPtr si, isroffset, ds
	mov	ds:[si], dx

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd GetInterruptVector

	EndPS

	END
