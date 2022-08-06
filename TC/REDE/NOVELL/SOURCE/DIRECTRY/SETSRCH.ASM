;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETSRCH
	INCLUDE c.mac


;**************************************************************************
; Example:   SetSearchDriveVector(vectorBuffer);
;
; Inputs:    char vectorBuffer[17];
;
; Comments:  This function sets the search vector in the shell.  Each element
;	     of the vector is a drive handle.  The vector list is terminated
;	     by a byte = 0xFF.
;**************************************************************************

	DOSExterns

	PSeg

	ProcDef SetSearchDriveVector, <<vector, dptr>>
	PushDS
	LoadPtr dx, vector, ds
	mov	ax, 0E902H
	EnterDOS
	PopDS
	Return
	ProcEnd SetSearchDriveVector

	EndPS

	END

