;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.

PAGE	  54,132
TITLE	  IPXPACK - _IPXPacket (IPX Call, Int 7Ah)
NAME	  _IPXPACK
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME:	_IPXPacket (IPX Call, Int 7Ah, Function 04h)
;	FILENAME:	IPXPACK.ASM
;	FUNCTION:
;	INPUTS: 	ECB *eventControlBlock;
;	PROCESSES:	Prepare for INT 7Ah (IPX Call)
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	al  -- return value
;-------------------------------------------------------------------------------
;	DATE:		12 Dec 85
;	VERSION:	01.01
;-------------------------------------------------------------------------------

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef _IPXPacket, <<eventControlBlock, dptr>, <functionNo, word>>
	PushDI
	PushES
	PushSI
	mov	al, 0
	LoadPtr si, eventControlBlock, es
	mov	bx, functionNo		      ;IPX Function number
	EnterIPX			      ;IPX Interrupt
	RetVal byte, reg, al
	PopSI
	PopES
	PopDI
	Return
	ProcEnd _IPXPacket

	EndPS

	END

