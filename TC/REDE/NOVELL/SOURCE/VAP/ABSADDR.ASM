;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Calculate Absolute Address
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  CalculateAbsoluteAddress
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: CalculateAbsoluteAddress
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     ABSADDR.ASM
;	FUNCTION:
;	USAGE:	      laddr = CalculateAbsoluteAddress(Segment, Offset);
;
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef CalculateAbsoluteAddress,<<aseg,word>,<aoff,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	bx, aseg
	mov	ax, aoff
	mov	di, 11h
	mov	cx, seg ProcessControlServices
	mov	bp, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bp]

	PopSI
	PopES
	PopDS
	PopDI

	xor	bh, bh
	RetVal	long,reg,ax,bx		;24 bit address
	Return

	ProcEnd CalculateAbsoluteAddress

	EndPS

	END
