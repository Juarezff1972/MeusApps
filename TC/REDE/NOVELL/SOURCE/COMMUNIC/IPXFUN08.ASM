;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  IPXFUN08 - IPX Get Interval Marker (IPX Call, Int 7Ah, Function 08h)
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  IPXFUN08
.LALL

;*******************************************************************************
;	PROGRAM NAME:	IPX Get Interval Marker (IPX Call, Int 7Ah, Function 08h)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXFUN08.ASM
;	FUNCTION:	Get Interval Marker for this function.
;	USAGE:		timeUnits = IPXGetIntervalMarker();
;	INPUTS: 	None
;	PROCESSES:	Prepare for INT 7Ah (IPX Call), function 08h (Get Interval Marker)
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	unsigned timeUnits;
;*******************************************************************************
;	DATE:		12 Dec 85
;	VERSION:	01.01
;	REMARKS:	This routine gets a time marker from IPX.
;			IPX calls (INT 7Ah) guarantee to preserve only DS, SS, SP.
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef IPXGetIntervalMarker
	push	bp			;Save BP since IPX does not preserve
	PushDI
	PushES
	PushSI
	mov	bx, 08h 		;IPX Function 08h
	EnterIPX			;IPX Interrupt
	RetVal	word, reg, ax
	PopSI
	PopES
	PopDI
	pop	bp
	Return
	ProcEnd IPXGetIntervalMarker

	EndPS

	END

