;   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	FSREQ
	INCLUDE c.mac

;----------------------------------------------------------------------------
; _FileServiceRequest			Send a request to the file server
;
; SYNOPSIS
;	ccode = _FileServiceRequest(requestCode, requestBuffer, requestLength,
;			replyBuffer, replyLength);
;
;	int ccode;			Zero if okay, non-zero for error.
;	char requestCode;		The service request code.
;	char *requestBuffer;		Buffer containing request.
;	int requestLength;		Length of request buffer.
;	char *replyBuffer;		Buffer for reply to request.
;	int replyLength;		Length of reply buffer.
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef _FileServiceRequest,<<function,byte>,<sendPacket,dptr>,<sendLength,word>,<replyPacket,dptr>,<replyLength,word>>
	PushDS
	PushSI
	PushDI
	mov	al, function
	LoadPtr si, sendPacket, ds
	mov	cx, sendLength
	LoadPtr di, replyPacket, es
	mov	dx, replyLength
	mov	ah, 0F2h		;Send/Receive Packet Request
	EnterDOS
	RetVal	byte, reg, al
	PopDI
	PopSI
	PopDS
	Return
	ProcEnd _FileServiceRequest

	EndPS

	END

