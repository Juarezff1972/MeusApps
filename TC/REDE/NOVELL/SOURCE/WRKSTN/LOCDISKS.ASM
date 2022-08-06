;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LOCDISKS
	INCLUDE c.mac

;******************************************************************************
;
;		     ---- GetNumberOfLocalDrives ----
;
; Output:   Returns the number of local disk drives
;
; Comments:
;   This routine is used by the shell upon initialization in order to map
;   the network drives in the proper order.
;******************************************************************************

	DOSExterns

	PSeg

	ProcDef GetNumberOfLocalDrives
	mov	ah, 219
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd GetNumberOfLocalDrives

	EndPS

	END

