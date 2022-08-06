;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Declare Extended Segment
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  DeclareExtendedSegment
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: DeclareExtendedSegment
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     EXTNDSEG.ASM
;	FUNCTION:
;	USAGE:	      segment = DeclareExtendedSegment(low16, high8);
;	VERSION:	01.00
;_____________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef DeclareExtendedSegment,<<low16,word>,<high8,byte>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, low16
	mov	bl, high8
	mov	di, 04h
	mov	cx, seg ProcessControlServices
	mov	bp, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bp]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax	;segment descriptor
	Return

	ProcEnd DeclareExtendedSegment

	EndPS

	END
