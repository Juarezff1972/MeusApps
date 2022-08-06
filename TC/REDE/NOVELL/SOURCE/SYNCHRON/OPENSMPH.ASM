;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	OPENSMPH
;___________________________________________________________________________
; OpenSemaphore(semaphoreName, initialValue, semaphoreHandle, openCount)
;
; Input:    char *semaphoreName;	/* Pointer to semaphoreName */
;	    int initialValue;		/* Value to initialize semaphore
;					   if it did not yet exist */
;	    long *semaphoreHandle;	/* Pointer where the semaphore
;					   handle will be returned */
;	    WORD *openCount;		 /* Pointer where number of stations
;					   that have this semaphore open
;					   will be returned */
;
; Output:   Returns a 0 if successful or
;		0xff Invalid initial code (must be in range 0-127)
;		0xfe Invalid semaphoreName length (must be 1-127 characters)
;		0x96 Out of work memory in file server
;
; Comments:
;   This routine will open a semaphore for use by the application.  If the
;   semaphore did not exist, it will be created and initialized with the
;   specified initialValue.  The semaphoreHandle returned should be used
;   in other calls to signal, wait on, examine, and close the semaphore.
;____________________________________________________________________________

	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef OpenSemaphore,<<semaphoreName,dptr>,<value,word>,<handle,dptr>,<count,dptr>>
	PushDS
	PushSI
	PushDI
	sub	sp, 128 		;Create data area to copy the
					;  name string to
	xor	si, si			;Count name length in si
	LoadPtr di, semaphoreName, ds
	mov	bx, sp
next_character:
	mov	al, [di]		;Get the next character of the
					;  source string
	or	al, al			;Check for null terminator
	jz	stop_count
	mov	ss:[bx + si + 1], al	;Copy character to destination
	inc	di
	inc	si
	jmp	next_character
stop_count:
	mov	dx, si		;Store string length as first byte
	mov	ss:[bx], dl	;  of destination string
	mov	cl, byte ptr (value)
;Point ds:dx at new copy of the
;  semaphore name
	mov	ax, ss
	mov	ds, ax
	mov	ax, 0c500h	;Function 197 (0)
	mov	dx, bx
	EnterDOS
	cmp	al, 0
	jne	error_occurred
	LoadPtr si, handle, ds
	ChkNull ax, si, ds
	jz	no_handle
	mov	[si], dx		;Store the semaphore handle in the
	mov	[si + 2], cx		;  long whose pointer was passed in the
					;  parameter list (semaphoreHandle)
no_handle:
	LoadPtr si, count, ds
	ChkNull ax, si, ds
	jz	no_count
	mov	[si], bl		;Store the number of stations that
	mov	byte ptr [si + 1], 0	;  have the semaphore open in openCount
no_count:
	RetVal	word, imm, 0
	jmp	short done
error_occurred:
	RetVal	byte, reg, al
done:
	add	sp, 128
	PopDI
	PopSI
	PopDS
	Return
	ProcEnd OpenSemaphore

	EndPS

	END

