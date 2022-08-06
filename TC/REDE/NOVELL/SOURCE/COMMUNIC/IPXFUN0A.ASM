;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  IPXFUN0A - IPX Relinquish Control (IPX Call, Int 7Ah, Function 0Ah)
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  IPXFUN0A
.LALL

;*******************************************************************************
;	PROGRAM NAME:	IPX Relinquish Control (IPX Call, Int 7Ah, Function 0Ah)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXFUN0A.ASM
;	FUNCTION:	Relinquish Control for this function.
;	USAGE:		IPXRelinquishControl();
;	INPUTS: 	None
;	PROCESSES:	Prepare for INT 7Ah (IPX Call), function 0Ah (Relinquish Control)
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	None
;*******************************************************************************
;	DATE:		12 Dec 85
;	VERSION:	01.01
;	REMARKS:	This routine relinquishes control to IPX.
;			IPX calls (INT 7Ah) guarantee to preserve only DS, SS, SP.
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef IPXRelinquishControl
	push	bp			;Save BP (IPX does not preserve)
	PushDI
	PushES
	PushSI
	mov	bx, 0Ah 		;IPX Function 0Ah
	EnterIPX				;IPX Interrupt
	PopSI
	PopES
	PopDI
	pop	bp
	Return
	ProcEnd IPXRelinquishControl

	EndPS

	END

