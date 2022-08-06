;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	EBEGTRAN
	INCLUDE c.mac
;-----------------------------------------------------------------------------
;NAME
;
;   TTSBeginTransaction - Explicit begin transaction tracking.
;
;
;SYNOPSIS
;
;   ccode = TTSBeginTransaction();
;
;   int  ccode; 	Return value.
;
;
;DESCRIPTION
;
;   Explicitly starts the tracking of a transaction.
;
;
;RETURNS
;
;   ccode:
;	 0  (0x00) = Transaction was started.
;      150  (0x96) = Out of dynamic work space.
;      255  (0xFF) = An explicit transaction is already active.
;      254  (0xFE) = An implicit transaction was active and converted to an
;		     explicit transaction.
;
;
;ERRORS
;
;   Returns an error if there is already an explicit transaction in progress.
;
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
;   EENDTRAN.ASM - TTSEndTransaction,
;   EABTTRAN.ASM - TTSAbortTransaction,
;   ESTATRAN.ASM - TTSTransactionStatus.
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSBeginTransaction
	mov	ax, 0c700h
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd TTSBeginTransaction

	EndPS

	END

