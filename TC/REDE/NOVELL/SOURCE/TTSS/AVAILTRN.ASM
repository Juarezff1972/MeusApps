;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	AVAILTRN
	INCLUDE c.mac

;-----------------------------------------------------------------------------
;NAME
;
;   TTSIsAvailable - Checks if Transaction Tracking is
;				     available.
;
;
;SYNOPSIS
;
;   ccode = TTSIsAvailable();
;
;   int  ccode; 	Return value.
;
;
;DESCRIPTION
;
;   Determines if the preferred server supports transaction tracking.
;
;
;RETURNS
;
;   0x00 transaction tracking is NOT available.
;   0xFF transaction tracking is available.
;   0xFD transaction tracking is available but is presently disabled.
;
;
;ERRORS       (none)
;
;COMMENTS     (none)
;
;CAUTIONS     (none)
;
;SEE ALSO
;
;   File(s)	 - Function name(s)
;   ------------   -------------------------
;   EBEGTRAN.ASM - TTSBeginTransaction,
;   EENDTRAN.ASM - TTSEndTransaction,
;   EABTTRAN.ASM - TTSAbortTransaction,
;   ESTATRAN.ASM - TTSTransactionStatus.
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSIsAvailable
	mov	ax, 0C702h
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd TTSIsAvailable

	EndPS

	END

