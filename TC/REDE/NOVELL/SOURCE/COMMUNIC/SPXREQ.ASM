;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.

PAGE	  54,132
TITLE	  _SPXrequest
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  SPXREQ
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: _SPXRequest
;		      Novell 'C' Network Interface Library (NIT.LIB)
;
;	FILENAME:     SPXREQ.ASM
;	FUNCTION:
;	USAGE:	      ccode = _SPXRequest(connectionIDnumber,buffer,functionNo)
;	INPUTS:       WORD connectionIDnumber -  id for connection number.
;		      buffer		      -  pointer to 44-byte buffer
;		      BYTE functionNo	      -  the function number to send IPX
;						 interrupt.
;
;	PROCESSES:    Prepare for IPX call.
;		      Make the call.
;		      Return to C.
;
;	OUTPUTS:      al  -  return code
;-------------------------------------------------------------------------------
;	DATE:		 5 APR 89
;	VERSION:	 1.01
;
;-------------------------------------------------------------------------------

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef _SPXrequest,<<connectionidnumber,word>,<buffer,dptr>, <functionNo, word>>
	PushDI
	PushES
	PushSI
	mov	dx,connectionidnumber
	LoadPtr si, buffer, es
	push	bp
	mov	bx, functionNo
	EnterIPX				;IPX Interrupt
	pop	bp
	PopSI
	PopES
	PopDI
	RetVal	byte,reg,al
	Return
	ProcEnd _SPXrequest

	EndPS

	END
