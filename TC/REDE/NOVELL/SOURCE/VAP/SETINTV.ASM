;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Set Interrupt Vector
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  SetInterruptVector
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: SetInterruptVector
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SETINTV.ASM
;	FUNCTION:
;	USAGE:	      SetInterruptVector(intNumber, ISRSegment, ISROffset);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef SetInterruptVector,<<intNumber,byte>,<isrsegment,word>,<isroffset,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	al, intNumber
	mov	cx, isrsegment
	mov	dx, isroffset
	mov	di, 10h
	mov	bx, seg ProcessControlServices
	mov	es, bx
	mov	bx, offset ProcessControlServices
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd SetInterruptVector

	EndPS

	END
