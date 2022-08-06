;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Change Segment To Code
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ChangeSegmentToCode
.LALL

;------------------------------------------------------------------------------
;	PROGRAM NAME: ChangeSegmentToCode
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SEGTOCOD.ASM
;	FUNCTION:     Change a VAP segment to Execute_read code
;	USAGE:	      ChangeSegmentToCode(SegmentValue);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef ChangeSegmentToCode,<<segmentvalue,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, segmentvalue
	mov	di, 02h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd ChangeSegmentToCode

	EndPS

	END
