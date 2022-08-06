;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Change Segment To Data
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ChangeSegmentToData
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ChangeSegmentToData
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SEGTODAT.ASM
;	FUNCTION:     Change a VAP Execute-Read code segment to Read-Write data
;	USAGE:	      ChangeSegmentToData(SegmentValue);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef ChangeSegmentToData,<<segmentvalue,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, segmentvalue
	mov	di, 01h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ChangeSegmentToData

	EndPS

	END
