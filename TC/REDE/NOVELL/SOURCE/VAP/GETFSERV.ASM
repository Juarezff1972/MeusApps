;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  VAP Get File Server Name
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  VAPGetFileServerName
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: VAPGetFileServerName
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     GETFSERV.ASM
;	FUNCTION:     Get The Name Of The File Server
;	USAGE:	      ccode = VAPGetFileServerName(serverName);
;	VERSION:	01.00
;______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef VAPGetFileServerName,<<serverName,dptr>>
	PushDI
	push	ds
	PushES
	PushSI

	LoadPtr si, serverName, es
	mov	di, 14h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	ds, cx
	call	dword ptr ds:[bx]

	PopSI
	PopES
	pop	ds
	PopDI

	RetVal	byte,reg,al	;completion code
	Return

	ProcEnd VAPGetFileServerName

	EndPS

	END
