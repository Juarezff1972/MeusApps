;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  SPX LISTEN FOR SEQUENCED PACKET
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  SPXLSSPK
.LALL

;*******************************************************************************
;	PROGRAM NAME: SPXListenForSequencedPacket
;		      Novell 'C' Network Interface Library (NIT.LIB)
;
;	FILENAME:     SPXLSSPK.ASM
;	FUNCTION:     Listen for a sequenced packet
;	USAGE:	      SPXListenForSequencedPacket(eventcontrolblock)
;	INPUTS:       ECB *eventcontrolblock
;
;	PROCESSES:    Prepare for IPX call, SPX function 17h.
;		      Make the call.
;		      Return to C.
;
;	OUTPUTS:      NONE
;*******************************************************************************
;	DATE:		23 MAR 87
;	VERSION:	01.01
;	REMARKS:	This function delivers an ECB and the buffer space it
;			describes to SPX for the purpose of receiving a
;			sequenced packet.  Having doneso, it returns immediately
;			to the calling program.
;
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef SPXListenForSequencedPacket,<<eventcontrolblock,dptr>>
	PushDI
	PushES
	PushSI
	LoadPtr si, eventcontrolblock, es
	push	bp
	mov	bx,17h
	EnterIPX				;IPX Interrupt
	pop	bp
	PopSI
	PopES
	PopDI
	Return
	ProcEnd SPXListenForSequencedPacket

	EndPS

	END


