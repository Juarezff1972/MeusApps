;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Set Hardware Interrupt Vector
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  SetHardwareInterruptVector
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: SetHardwareInterruptVector
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SETHWINT.ASM
;	FUNCTION:
;	USAGE:	      EOIflag = SetHardwareInterruptVector(intNumber, ISRSegment, ISROffset);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef SetHardwareInterruptVector,<<intNumber,byte>,<isrsegment,word>,<isroffset,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	al, intNumber
	mov	si, isrsegment
	mov	bx, isroffset
	mov	di, 0Eh
	mov	cx, seg ProcessControlServices
	mov	bp, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bp]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	byte,reg,al	;EOI flag
	Return

	ProcEnd SetHardwareInterruptVector

	EndPS

	END
