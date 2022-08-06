;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  IPXFUN01 - IPX Close Socket (IPX Call, Int 7Ah, Function 01h)
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  IPXFUN01
.LALL

;*******************************************************************************
;	PROGRAM NAME:	IPX Close Socket (IPX Call, Int 7Ah, Function 01h)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXFUN01.ASM
;	FUNCTION:	Close Socket for this node
;	USAGE:		IPXCloseSocket(socketNumber);
;	INPUTS: 	char socketNumber[2];  /* Pointer to the socket number
;						  (socket number is high-low order)
;	PROCESSES:	Prepare for INT 7Ah (IPX Call), function 01h (Close Socket)
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	None
;*******************************************************************************
;	DATE:		12 Dec 85
;	VERSION:	01.01
;	REMARKS:	This routine closes a socket from IPX use.
;			Socket number is in high-low byte order.
;			IPX calls (INT 7Ah) guarantee to preserve only DS, SS, SP.
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef IPXCloseSocket, <<socketNumber, word>>
	PushDI
	PushES
	PushSI
	mov	dx, socketNumber
	mov	bx, 01h 			;IPX Function 01h
	EnterIPX				;IPX Interrupt
	PopSI
	PopES
	PopDI
	Return
	ProcEnd IPXCloseSocket

	EndPS

	END

