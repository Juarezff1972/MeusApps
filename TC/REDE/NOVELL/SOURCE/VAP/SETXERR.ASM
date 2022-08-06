;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Set External Process Error
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  SetExternalProcessError
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: SetExternalProcessError
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     SETXERR.ASM
;	FUNCTION:
;	USAGE:	      SetExternalProcessError(Procedure, DataSeg);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef SetExternalProcessError,<<procedure,cptr>, <dataseg,word>>
	PushDI
	PushDS
	PushES
	PushSI

ifdef LProg
	les	dx, procedure
	mov	cx, es
else
	push	cs
	pop	cx
	mov	dx, procedure
endif
	mov	bx, dataseg
	mov	di, 12h
	mov	ax, seg ProcessControlServices
	mov	bp, offset ProcessControlServices
	mov	es, ax
	call	dword ptr es:[bp]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd SetExternalProcessError

	EndPS

	END
