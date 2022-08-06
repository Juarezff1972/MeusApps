;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  IPXFUN02 - IPX Get Local Target (IPX Call, Int 7Ah, Function 02h)
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  IPXFUN02
.LALL

;*******************************************************************************
;	PROGRAM NAME:	IPX Get Local Target (IPX Call, Int 7Ah, Function 02h)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXFUN02.ASM
;	FUNCTION:	Get Local Target for the ECB Immediate Address field.
;	USAGE:		ccode = IPXGetLocalTarget(networkAddress, targetNode, transportTime);
;	INPUTS: 	char networkAddress[12];  /* char    network[4];   high-low
;						     char    node[6];	   high-low
;						     char    socket[2];    high-low */
;	PROCESSES:	Prepare for INT 7Ah (IPX Call), function 02h (Get Local Target)
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	char immediateAddress[6];
;			ccode = 0x0000 (Local target located)
;				0x00FA (No local target identified)
;*******************************************************************************
;	DATE:		12 Dec 85
;	VERSION:	01.01
;	REMARKS:	This routine gets the immediate address for a node.
;			IPX calls (INT 7Ah) guarantee to preserve only DS, SS, SP.
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef IPXGetLocalTarget, <<networkAddress,dptr>,<targetNode,dptr>,<transportTime,dptr>>
	PushDI
	PushDS
	PushES
	PushSI
	sub	sp, 18	;Allocate local storage for addresses in same seg
	LoadPtr si, networkAddress, ds
	mov	ax, ss
	mov	es, ax
	mov	di, sp
	mov	cx, 12
	cld
	rep	movsb		;DS:SI to ES:DI for 12
;Set up pointer to 12 byte address in ES:SI and point ES:DI to target buffer
	mov	si, sp
	mov	di, sp
	add	di, 12
	mov	bx, 02h 	;IPX Function 02h
	push	di		;Save target node address
	push	bp		;int 7a does not preserve bp
	EnterIPX		;IPX Interrupt
	pop	bp
	pop	di
	cmp	al, 0
	RetVal	byte, reg, al
	jne	error_locating_target
;Copy the target node to the users buffer
	push	ss
	pop	ds
	mov	si, di
	LoadPtr di, targetNode, es
	push	cx		;Save the transport time
	mov	cx, 6
	rep	movsb		;DS:SI to ES:DI for 6
	LoadPtr si, transportTime, ds
	pop	cx
	mov	[si], cx
error_locating_target:
	add	sp, 18
	PopSI
	PopES
	PopDS
	PopDI
	ResetD
	Return
	ProcEnd IPXGetLocalTarget

	EndPS

	END

