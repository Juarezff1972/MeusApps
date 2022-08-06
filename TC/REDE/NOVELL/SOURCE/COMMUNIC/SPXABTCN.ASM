;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  SPX ABORT CONNECTION
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  SPXABTCN
.LALL

;*******************************************************************************
;	PROGRAM NAME: SPXAbortConnection
;		      Novell 'C' Network Interface Library (NIT.LIB)
;
;	FILENAME:     SPXABTCN.ASM
;	FUNCTION:     Abort SPX connection.
;	USAGE:	      SPXAbortConnection(connectionidnumber)
;	INPUTS:       WORD connectionidnumber ; id for connection number.
;
;	PROCESSES:    Prepare for IPX call, SPX function 14h.
;		      Make the call.
;		      Return to C.
;
;	OUTPUTS:      NONE
;*******************************************************************************
;	DATE:		23 MAR 87
;	VERSION:	01.01
;	REMARKS:	This causes SPX to unilaterally abort a connection.  No
;			attempt is made to inform the connection partner of the
;			decision  to abort the connection; the partner will
;			discover that the connection is no longer valid when
;			it attempts to send a packet on the connection or when its
;			its watchdog checks the connection after the inactivity
;			timer expires.
;
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef SPXAbortConnection,<<connectionidnumber,word>>
	PushDI
	PushES
	PushSI
	mov	dx,connectionidnumber
	push	bp
	mov	bx,14h
	EnterIPX				;IPX Interrupt
	pop	bp
	PopSI
	PopES
	PopDI
	Return
	ProcEnd SPXAbortConnection
	EndPS

	END


