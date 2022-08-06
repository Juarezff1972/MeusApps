;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	CHDIR
	INCLUDE c.mac

;**** ChangeDirectory - Changes the current directory
; ChangeDirectory(newDirectory);
;
; Input:  char *newDirectory;	  The path the directory should be set to.
;
; Output: Returns a 0 if successful, or a 3 if unsuccessful.
;
; Comments:
;   If the new directory path includes a drive number, then that drive will
;   be set to the path, otherwise, the current drive will be set to the given
;   path.
;****/
	DOSExterns

	PSeg

	ProcDef ChangeDirectory, <<path, dptr>>
	PushDS
	LoadPtr dx, path, ds
	mov	ah, 3bh
	EnterDOS
	jc	error
	RetVal	byte, imm, 0
	jmp	short noerror
error:
	RetVal	byte, reg, al
noerror:
	PopDS
	Return
	ProcEnd ChangeDirectory

	EndPS
	END
