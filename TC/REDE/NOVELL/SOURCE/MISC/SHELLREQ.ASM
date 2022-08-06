;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
	INCLUDE c.mac

	NAME	SHELLREQ

;*****************************************************************************
;
;		      ----  _ShellRequest ----
;
; Input:  BYTE shellFunctionNumber  -  The shell function number to be executed.
;	  BYTE *sendPacket	    -  The address of the IPX packet to be sent
;				       to a server
;	  BYTE *receivePacket	    -  The address where the returned IPX packet
;				       should be stored
;
; Output:   Returns a 0 in ax if successful or a network or bindry error
;	    code if unsuccessful.
;
; Comments:
;   This routine is the interface between the Novell 'C' Library routines
;   that need to execute shell functions to send and receive IPX packets
;   and the shell.  The 'C' routines set up the send packet and the receive
;   packet length before calling _ShellRequest.
;*******************************************************************************

	DOSExterns

	PSeg

	ProcDef _ShellRequest, <<functionNumber,byte>,<sendPacket,dptr>,<replyPacket,dptr>>
	PushDS
	PushSI
	PushDI
	mov	ah, functionNumber
	LoadPtr si, sendPacket, ds
	LoadPtr di, replyPacket, es
	EnterDOS			;Execute the shell function
	RetVal	byte, reg, al
	PopDI
	PopSI
	PopDS
	Return
	ProcEnd _ShellRequest

	EndPS

	END

