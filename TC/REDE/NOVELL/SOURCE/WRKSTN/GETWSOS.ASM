;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETWSOS
;*** GetWorkstationOS
; GetWorkstationOS(BYTE *workstationOS);
;
; Input:    BYTE *workstationOS;	* Pointer to a byte where shell
;					* os will be placed
;
; Output:   None.
;
; Comments:
;***/
	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef GetWorkstationOS <<osnum, dptr>>
	sub	sp, 40		; make room on stack for buffer
	PushDI
	mov	ax, 0ea01h	;Function 234 (1) Return Shell Version String
	mov	bx, 0		;assume non-supported function
	mov	di, sp
	push	ss
	pop	es
	EnterDOS		;if successful 2.1 shell BX contains version
	LoadPtr di, osnum
	mov	es:[di], ah
	add	sp, 40		; remove room on stack for buffer
	PopDI
	Return
	ProcEnd GetWorkstationOS

	EndPS

	END


