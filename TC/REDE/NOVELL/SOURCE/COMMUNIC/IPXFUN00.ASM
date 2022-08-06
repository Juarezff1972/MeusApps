;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  IPXFUN00 - IPX Open Socket (IPX Call, Int 7Ah, Function 00h)
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  IPXFUN00
.LALL

;*******************************************************************************
;	PROGRAM NAME:	IPX Open Socket (IPX Call, Int 7Ah, Function 00h)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXFUN00.ASM
;	FUNCTION:	Open Socket for this node
;	USAGE:		ccode = IPXOpenSocket(socketNumber, socketType);
;	INPUTS: 	int ccode;		 /* Condition code to be returned. */
;			char socketNumber[2];  /* Pointer to the socket number
;						  (socket number is high-low order)
;						  0x0000   = IPX will dynamically
;							     assign a socket number
;							     and return it here.
;						  Non-zero = IPX will use this
;							     socket number. */
;			char socketType;  /* Flag indicating temporary or permenant;
;					     0x00 = Stay open until program closes it
;						    or program terminates.
;					     0xFF = Stay open until program closes it. */
;	PROCESSES:	Prepare for INT 7Ah (IPX Call), function 00h (Open Socket)
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	ccode = 0x0000 (Socket opened successfully)
;				0x00FE (Socket table full, this node already using 50 sockets)
;				0x00FF (Socket already open)
;*******************************************************************************
;	DATE:		12 Dec 85
;	VERSION:	01.01
;	REMARKS:	This routine opens a socket for IPX to use.
;			SocketNumber is in high-low byte order!
;			IPX calls (INT 7Ah) guarantee to preserve only DS, SS, SP.
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef IPXOpenSocket, <<socketNumber,dptr>,<socketType,byte>>
	PushDI
	PushDS
	PushES
	PushSI
	LoadPtr di, socketNumber, ds
	mov	dx, [di]
	push	di				;Save socketNumber pointer
	mov	al, socketType
	mov	bx, 00h 			;IPX Function 00h
	EnterIPX				;IPX Interrupt
	pop	di				;Restore socketNumber pointer
	mov	[di], dx
	PopSI
	PopES
	PopDS
	PopDI
	RetVal	byte, reg, al
	Return
	ProcEnd IPXOpenSocket

	EndPS

	END

