;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	EOJFLAG
	INCLUDE c.mac
;******************************************************************************
;
;		       ---- SetEndOfJobStatus ----
;
; Input:    int enableEndOfJob	       Flag to indicate if End Of Job should be
;				       enabled (1) or disabled (0)
;
; Output:   Returns the old End Of Job status so that the previous status can be
;	    restored if desired.
;
; Comments:
;   This routine allows an application to enable or disable the EOJs sent
;   when command.com is executing.  This call is useful if the application
;   is invoking a secondary command processor and doesn't want to lose its
;   files due to an EOJ.  The EOJ status flag will remain set to the current
;   setting until it is explicitly reset.
;*******************************************************************************
	DOSExterns

	PSeg

	ProcDef SetEndOfJobStatus, <<mode, byte>>
	mov	ah, 0bbh			;Function number 187
	mov	al, mode
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd SetEndOfJobStatus

	EndPS

	END
