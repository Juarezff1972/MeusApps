;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  SPX LISTEN FOR CONNECTION
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  SPXLSNCN
.LALL

;*******************************************************************************
;	PROGRAM NAME: SPX Listen For Connection
;		      Novell 'C' Network Interface Library (NIT.LIB)
;
;	FILENAME:     SPXLSNCN.ASM
;	FUNCTION:     Listen for an SPX connection
;	USAGE:	      SPXListenForConnection(RetryCount,WatchDogFlag,EventControlBlock)
;	INPUTS:       RetryCount = int 0-255.
;		      WatchDogFlag = int zero enables, non-zero disables.
;		      ECB = *EventControlBlock.
;
;	PROCESSES:    Prepare for IPX call, SPX function 12h.
;		      Make the call.
;		      Return to C.
;
;
;	OUTPUTS:      None
;*******************************************************************************
;	DATE:		23 MAR 87
;	VERSION:	01.01
;	REMARKS:	This function indicates a client's willingness to start and
;			SPX connection with a remote partner.  In order for a connection
;			one side must listen using this function, and the other
;			side must call using the ESTABLISH CONNECTION function.
;
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef SPXListenForConnection,<<retrycount,byte>,<watchdog,byte>,<eventcontrolblock,dptr>>
	PushDI
	PushES
	PushSI
	mov	al,retrycount
	mov	ah,watchdog
	LoadPtr si, eventcontrolblock, es
	push	bp
	mov	bx,12h
	EnterIPX				;IPX Interrupt
	pop	bp
	PopSI
	PopES
	PopDI
	Return
	ProcEnd SPXListenForConnection

	EndPS

	END
