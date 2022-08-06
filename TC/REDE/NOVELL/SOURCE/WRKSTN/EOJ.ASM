;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	EOJ
	INCLUDE c.mac

;******************************************************************************
;
;			 ---- EndOfJob ----
;
; Output:   None.
;
; Comments:
;   This routine causes and end of job to be issued by the PC-Shell.  An
;   end of job is automatically issued by the PC_Shell whenever a program
;   exits unless EndOfJob has been disabled by a call to SetEndOfJobStatus.
;   When an end of job occurs all locked files and records are cleared and any
;   open files are closed.  This call can be used anytime that the program
;   needs the network enviornment to return to a beginning-of-program state
;   with no files or records logged or locked.
;******************************************************************************

	DOSExterns

	PSeg

	ProcDef EndOfJob
	mov	ah, 214 		;Function 214 (D6h) End Of Job
	EnterDOS
	Return
	ProcEnd EndOfJob

	EndPS

	END
