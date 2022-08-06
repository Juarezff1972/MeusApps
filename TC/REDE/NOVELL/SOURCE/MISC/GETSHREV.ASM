;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETSHREV
	INCLUDE c.mac

;******************************************************************************
;
;  void  GetShellRevision(shellMajorRevision, shellMinorRevision)
;
; Input:   BYTE *shellMajorRevision	 Receives the shell's major revision #
;	   BYTE *shellMinorRevision	 Receives the shell's minor revision #
;
; Output:  None.
;
; Comments:
;   This routine returns the shell's major and minor revision numbers.
;   Since this function was not available in shells previous to 2.1 Beta 4
;   it will return a zero for the major & minor version #'s.
;******************************************************************************

	DOSExterns

	PSeg

	ProcDef GetShellRevision, <<major, dptr>,<minor, dptr>>
	sub	sp, 40		; make room on stack for buffer
	PushDI
	mov	ax, 0ea01h	;Function 234 (1) Return Shell Version String
	mov	bx, 0		;assume non-supported function
	mov	di, sp
	push	ss
	pop	es
	EnterDOS		;if successful 2.1 shell BX contains version
	PopDI
	LoadPtr di, major
	mov	es:[di], bh
	LoadPtr di, minor
	mov	es:[di], bl
	add	sp, 40		; remove room on stack for buffer
	Return
	ProcEnd GetShellRevision

	EndPS

	END


