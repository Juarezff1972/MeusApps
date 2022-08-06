;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	EENDTRAN
	INCLUDE c.mac
;-----------------------------------------------------------------------------
;NAME
;
;   TTSEndTransaction - Explicit End Transaction Tracking.
;
;
;SYNOPSIS
;
;   ccode = TTSEndTransaction(transactionNumber);
;
;   int  ccode; 		  Return value - Zero if successful or an
;						 error if unsuccessful.
;   long *transactionNumber;	  Pointer to a long, were the transaction
;				  number will be returned.  (optional)
;
;DESCRIPTION
;
;   Explicitly ends the tracking of a transaction.  If a NULL pointer is
;   used as the parameter, the optional transaction number is not returned.
;   In that case a (long *)NULL should be passed.
;
;
;RETURNS
;
;   ccode:
;	 0  (0x00) = Transaction ended successfully.
;
;      EXPLICIT_TRANSACTION_NOT_ACTIVE -
;	    255  (0xFF) = No explicit transaction was active.
;
;      TRANSACTION_ENDED_RECORDS_LOCKED -
;	    254  (0xFE) = An explicit transaction ended; however, records
;			  were left locked.
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
;   AVAILTRN.ASM - TTSIsAvailable,
;   EBEGTRAN.ASM - TTSBeginTransaction,
;   EABTTRAN.ASM - TTSAbortTransaction,
;   ESTATRAN.ASM - TTSTransactionStatus.
;
;
;_____________________________________________________________________________

	DSeg
return_flag   db     0
	EndDS

	DOSExterns

	PSeg

	ProcDef TTSEndTransaction, <<transNumber, dptr>>
	PushDS
	PushSI
	mov	ax, 0C701h
	EnterDOS		     ;Explicit End Transaction function
	RetVal	byte, reg, al
	LoadPtr bx, transNumber, ds
	ChkNull si, bx, ds
	jz	null_parameter
	mov	[bx], dx		;Return Transaction number.
	mov	[bx + 2], cx
null_parameter:
	PopSI
	PopDS
	Return
	ProcEnd TTSEndTransaction

	EndPS

	END

