;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETDIR
	INCLUDE c.mac

;******************************************************************************
;
; Inputs:  BYTE driveNumber	      The drive number;
;	   char *directoryPath	      Buffer for directory spec.
;
; Outputs: int ccode		      0 if okay, otherwise error number.
;
; Comments:
;	This function gets the current directory of a dos drive.
;
;*****************************************************************************

	DOSExterns

	PSeg

	ProcDef GetCurrentDirectory, <<drive, byte>, <path, dptr>>
	PushDS
	PushSI
	inc	drive
	mov	dl, drive
	LoadPtr si, path, ds
	mov	ah, 47h
	EnterDOS
	RetVal	byte, reg, al
	jc	done
	RetVal	word, imm, 0
done:
	PopSI
	PopDS
	Return
	ProcEnd GetCurrentDirectory

	EndPS
	END
