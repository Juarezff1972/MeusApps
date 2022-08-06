;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  VAP Attach To File Server
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  VAPAttachToFileServer
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: AttachToFileServer
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     ATTACHFS.ASM
;	FUNCTION:     Attach to a file server
;	USAGE:	      ccode = VAPAttachToFileServer(serverName,connectionID);
;	VERSION:	01.00
;_____________________________________________________________________________


	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef VAPAttachToFileServer,<<serverName,dptr>,<connectionID,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, serverName, ds
	mov	di, 15h
	mov	cx, seg ProcessControlServices
	mov	bx, offset ProcessControlServices
	mov	es, cx
	push	bp
	call	dword ptr es:[bx]
	pop	bp

	LoadPtr si, connectionID, ds
	sub	dh, dh
	mov	word ptr [si], dx

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	byte,reg,al	;completion code
	Return

	ProcEnd VAPAttachToFileServer

	EndPS

	END
