;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETFLVER
	INCLUDE c.mac

;******************************************************************************
;
; GetNetWareShellVersion(shellMajorVersion, shellMinorVersion, shellRevision)
;
; Input:    BYTE *shellMajorVersion	 Receives the shell's major version #
;	    BYTE *shellMinorVersion	 Receives the shell's minor version #
;	    BYTE *shellRevision 	 Receives the shell's revision number
;
; Output:    0 on error, FF (255) on success
;	     255  --  ??
;
; Comments:
;   This routine will return the shell's major and minor version numbers and
;   the revision number.  Since this function was not available in the shell's
;   previous to 2.1 Beta 4, it will return a zero for the major and minor
;   version #'s.
;******************************************************************************

	DOSExterns

	PSeg

	ProcDef GetNetWareShellVersion, <<major, dptr>,<minor, dptr>,<revision, dptr>>
	sub	sp, 40		; make room on stack for buffer
	PushES
	PushDI
	PushDS
	mov	ax, 0ea01h	;Function 234 (1) Return Shell Version String
	mov	bx, 0		;assume non-supported function
	lea	di, [bp-40]
	push	SS
	pop	ES
	mov	BYTE PTR ES:[DI],0
	EnterDOS		;if successful 2.1 shell BX contains version
	mov	AL,[BP-40]
	or	AL,AL
	jz	error
	LoadPtr di, major, ds
	ChkNull ax, di, ds
	jz	loadminor
	mov	[di], bh
loadminor:
	LoadPtr di, minor, ds
	ChkNull ax, di, ds
	jz	loadrevision
	mov	[di], bl
loadrevision:
	LoadPtr di, revision, ds
	ChkNull ax, di, ds
	jz	isok
	mov	[di], cl
isok:
	RetVal	word, imm, 0FFh
	jmp	done
error:
	RetVal	word, imm, 0
done:
	PopDS
	PopDI
	PopES
	mov	SP,BP
	Return
	ProcEnd GetNetWareShellVersion

	EndPS

	END


