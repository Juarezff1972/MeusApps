;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETSRCH.ASM
	INCLUDE c.mac

;*****************************************************************************
;
;		 GetSearchDriveVector(vectorBuffer)
;
; Inputs:	 char vectorBuffer[17];        Receives the search vector.
;
; Outputs:   None.
;
; Comments:
;     This function returns the search vector from the shell.  Each element of
;     the vector is a drive handle.  The vector list is terminated by a
;     byte = 0xFF.
;******************************************************************************

	DOSExterns

	PSeg

	ProcDef GetSearchDriveVector, <<vector, dptr>>
	PushDS
	LoadPtr dx, vector, ds
	mov	ax, 0E901H
	EnterDOS
	mov	bx, dx
	mov	byte ptr [bx + 16], 0ffh
	PopDS
	Return
	ProcEnd GetSearchDriveVector

	EndPS

	END

