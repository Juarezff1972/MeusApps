;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETERROR
	INCLUDE c.mac

;******************************************************************************
;
;		       ---- SetNetWareErrorMode ----
;
; Input:    BYTE  errorMode	  The value of the error mode to be set.
;
; Output:   Previous netware error mode.
;
; Comments:
;   This routine allows the application to determine if network errors will
;   be treated as normal DOS errors or will be trapped so the application can
;   handle the error specificially.  Errors such as file locked by another user
;   or no privileges do not occur normally in DOS.  Error mode 0 is the default
;   error mode, and the error mode is automatically reset to 0 whenever an end
;   of job occurs.  In error mode 0 all errors are returned as standard DOS
;   errors.  In error mode 1 network errors will be returned to the program so
;   that it can take specific action.
;******************************************************************************

	DOSExterns

	PSeg

	ProcDef SetNetWareErrorMode, <<errorMode, byte>>
	mov	ah, 221 	      ;Function 221 (DDh) Set Error Mode
	mov	dl, errorMode
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd SetNetWareErrorMode

	EndPS

	END

