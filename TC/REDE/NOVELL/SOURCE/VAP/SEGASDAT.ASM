;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Declare Segment As Data
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  DeclareSegmentAsData
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: DeclareSegmentAsData
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SEGASDAT.ASM
;	FUNCTION:     Declare a segment as data
;	USAGE:	      segment = DeclareSegmentAsData(SegmentValue);
;	VERSION:	01.00
;_____________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef DeclareSegmentAsData,<<paragraphNumber,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, paragraphNumber
	mov	di, 03h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax	;segment descriptor
	Return

	ProcEnd DeclareSegmentAsData

	EndPS

	END
