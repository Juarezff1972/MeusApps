;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	EABRTRAN
	INCLUDE c.mac

;-----------------------------------------------------------------------------
;NAME
;
;   TTSAbortTransaction - Explicit abort transaction.
;
;
;SYNOPSIS
;
;   ccode = TTSAbortTransaction();
;
;   int  ccode; 	Return value.
;
;
;DESCRIPTION
;
;   Explicitly aborts a transaction.
;
;
;RETURNS
;
;   ccode:
;	 0  (0x00) = Transaction was aborted.
;      255  (0xFF) = No explicit transaction was active.
;      254  (0xFE) = An implicit transaction was aborted; however, records
;		     were left locked.
;
;
;ERRORS
;
;   Returns an error if no transaction is active, or if records were left
;   locked.
;
;COMMENTS     (none)
;
;CAUTIONS     (none)
;
;SEE ALSO
;
;   File(s)	 - Function name(s)
;   ------------   -------------------------
;   AVAILTRN.ASM - TTSIsAvailable,
;   EBEGTRAN.ASM - TTSBeginTransaction,
;   EENDTRAN.ASM - TTSEndTransaction,
;   ESTATRAN.ASM - TTSTransactionStatus.
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSAbortTransaction
	mov	ax, 0c703h	;Function 199 (3)
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd TTSAbortTransaction

	EndPS

	END

