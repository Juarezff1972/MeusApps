;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	ESTATRAN
	INCLUDE c.mac

;-----------------------------------------------------------------------------
;NAME
;
;   TTSTransactionStatus - Explicit Transaction Status.
;
;
;SYNOPSIS
;
;   ccode = TTSTransactionStatus(transactionNumber);
;
;   int  ccode; 		  Return value - Zero if successful or an
;						 error if unsuccessful.
;   long transactionNumber;	Transaction Number
;
;
;DESCRIPTION
;
;   Determines the status of an explicit transaction.  The status can be one
;   of two things.  Either the transaction has been entirely written to the
;   disk or it has not been entirely written to the disk.
;
;
;RETURNS
;
;   ccode:
;	   0  (0x00) = Transaction ended successfully.
;	 255  (0xFF) = Transaction has not been written to the disk yet.
;
;
;ERRORS       (none)
;
;COMMENTS
;
;   Any application that uses the transaction number returned by the
;   TTSEndTransaction function, must keep track of and verify the correctness
;   of it's own transaction numbers.
;
;
;CAUTIONS
;
;   If an invalid transaction number is passed a zero (0) is returned,
;   indicating that the bogus transaction ended successfully.  Any application
;   that uses the Transaction Tracking System must keep track of and verify
;   the correctness of the transaction numbers it uses.
;
;
;SEE ALSO
;
;   File(s)	 - Function name(s)
;   --------------------------------------
;   AVAILTRN.ASM - TTSIsAvailable,
;   EBEGTRAN.ASM - TTSBeginTransaction,
;   EABTTRAN.ASM - TTSAbortTransaction,
;   EENDTRAN.ASM - TTSEndTransaction.
;
;
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSTransactionStatus,<<transactionNumber, long>>
	mov	dx, word ptr (transactionNumber)
	mov	cx, word ptr (transactionNumber + 2)
	mov	ax, 0C704h
	EnterDOS		     ;Explicit Transaction Status function
	RetVal	byte, reg, al
	Return
	ProcEnd TTSTransactionStatus

	EndPS

	END

