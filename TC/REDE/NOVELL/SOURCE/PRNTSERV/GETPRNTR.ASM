;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETPRNTR
;**** GetDefaultLocalPrinter - Get the default printer for capture calls
;
; Input:    None;
;
; Output:   BYTE localPrinterNumber;
;
; Comment:  This routine will obtain the number of the local printer that is
;	    being used for the LPT capture and spool flag modification calls.
;
;***/
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef GetDefaultLocalPrinter
	mov	ax, 0b804h	;function 184
	EnterDOS
	RetVal byte, reg, dh
	Return
	ProcEnd GetDefaultLocalPrinter

	EndPS

	END
