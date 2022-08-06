;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Get VAP Header
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  GetVAPHeader
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: GetVAPHeader
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     GETVAPH.ASM
;	FUNCTION:     Display A Message On The System Console
;	USAGE:	      header = GetVAPHeader(VAPNum);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef GetVAPHeader,<<vapnum,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	ax, vapnum
	mov	di, 17h
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

	ProcEnd GetVAPHeader

	EndPS

	END
