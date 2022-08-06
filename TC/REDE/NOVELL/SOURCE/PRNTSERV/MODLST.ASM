;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	MODLST
	INCLUDE c.mac

;**** ModifyLPTDevice -- low level interface to all LPT function
;
; Input:    BYTE function;		* function to be put in DL
;	    BYTE device;		* LPT device to deal with
;
; Output:   None.
;
; Comments:
;   This routine allows interfacing into the modify LPT device function and
;   all its sub functions.  Including the new 2.1 functions.  Later it
;   could be used to provide specific function interface routines.
;
;   This routine is not in any header file.  It it has been replaced by
;   more specific routines.
;***/
	DOSExterns

	PSeg

	ProcDef ModifyLPTDevice <<function, byte>, <device, byte>>
	mov	ah, 0dfh		;function 223
	mov	dl, function
	mov	dh, device
	EnterDOS
	Return
	ProcEnd ModifyLPTDevice

	EndPS

	END


