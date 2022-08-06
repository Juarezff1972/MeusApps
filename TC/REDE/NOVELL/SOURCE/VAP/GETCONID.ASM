;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  VAP Get Connection ID - (formerly Map File Server Name To Number)
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  VAPGetConnectionID
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: VAPGetConnectionID
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;		      formerly GetConnectionID
;	FILENAME:     GETCONID.ASM
;	FUNCTION:     Map a file server name to a number
;	USAGE:	      ccode = VAPGetConnectionID(bufr,connectionID);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef VAPGetConnectionID,<<bufr,dptr>,<connectionID,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, bufr, ds
	mov	di, 16h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	push	bp
	call	dword ptr es:[bx]
	pop	bp

	LoadPtr si, connectionID, ds
	sub	dh, dh
	mov	[si], dx

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	byte,reg,al	;completion code
	Return

	ProcEnd VAPGetConnectionID

	EndPS

	END
