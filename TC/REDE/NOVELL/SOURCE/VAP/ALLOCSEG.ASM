;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Allocate Segment
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  AllocateSegment
.LALL

;------------------------------------------------------------------------------
;	PROGRAM NAME: AllocateSegment
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     ALLOCSEG.ASM
;	FUNCTION:     Allocate System Memory
;	USAGE:	      segment = AllocateSegment(NoOfParagraphs);
;	INPUTS:
;
;	PROCESSES:
;
;
;	OUTPUTS:
;-------------------------------------------------------------------------------
;	DATE:		27 AUG 87
;	VERSION:	01.00
;	AUTHOR: 	nt
;	REMARKS:	Call before InitializationComplete()
;
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef AllocateSegment,<<segmentsize,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, segmentsize
	mov	di, 00h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax
	Return

	ProcEnd AllocateSegment

	EndPS

	END
