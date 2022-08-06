;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	IPXSCHED
;--------------------------------------------------------------------------------
;	PROGRAM NAME:	IPXrequest (IPX Call, Int 7Ah)
;	FILENAME:	IPXSCHED.ASM
;	INPUTS: 	unsigned timeUnits;  /* 1 =< timeUnits =< 65535 */
;			ECB *eventControlBlock;
;	PROCESSES:	Prepare for INT 7Ah (IPX Call),
;			Make INT 7Ah call
;			Return to C
;	OUTPUTS:	None
;--------------------------------------------------------------------------------
;	DATE:		 5 Apr 89
;	VERSION:	01.01
;--------------------------------------------------------------------------------

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef _IPXrequest,<<timeDelay,word>,<address,dptr>,<functionNo, word>>
	PushDI
	PushES
	PushSI
	mov	ax, timeDelay
	LoadPtr si, address, es
	mov	bx, functionNo			;IPX Function number
	EnterIPX				;IPX Interrupt
	PopSI
	PopES
	PopDI
	Return
	ProcEnd _IPXrequest

	EndPS

	END

