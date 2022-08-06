;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SERVUSE
	INCLUDE c.mac

;******************************************************************************
;		------- IsConnectionIDInUse -------
;
;  Example:  WORD  connectionID;       Number of the server to be checked
;
;	     ccode = IsConnectionIDInUse(connectionID);
;
;  Input:   connectionID
;
;  Output:  Returns TRUE if in use or FALSE if no server is attached.
;
;*****************************************************************************
	DOSExterns

	PSeg

	ProcDef IsConnectionIDInUse, <<connectionID, word>>
	PushES
	PushSI
	;Get the shell table address in es:si
	mov	ax, 0ef03h	;Function 239 (3) Get Shell Table Address
	EnterDOS
	;Get the server number to check is ax and make it 0 based
	mov	bx, connectionID
	dec	bx
	;Multiply by 32, which is the size of each server entry
	;Note that bh is cleared out by the shift left
	mov	cl, 5
	shl	bx, cl
	;Check the in use flag, which is the first byte of the entry
	cmp	byte ptr es:[si + bx], 0
	jne	SlotIsInUse
	;Return FALSE, the slot is not in use
	RetVal	word, imm, 0
	jmp	short Done
SlotIsInUse:
	;Return TRUE, the slot is being used
	RetVal	word, imm, 1
Done:
	PopSI
	PopES
	Return
	ProcEnd IsConnectionIDInUse

	EndPS

	END

