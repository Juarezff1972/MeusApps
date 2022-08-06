;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	EXAMSMPH
;__________________________________________________________________________
; ExamineSemaphore(semaphoreHandle, semaphoreValue, openCount)
;
; Input:    long semaphoreHandle;	/* The semaphore handle returned
;					   by the openSemaphore call */
;	    int *semaphoreValue;	/* Pointer where the semaphore
;					   value will be returned */
;	    WORD *openCount;		/* Pointer where the number of
;					   stations that have the semaphore
;					   open will be returned */
;
; Output:   Returns a 0 if successful or a 0xff if the semaphore handle
;	    is invalid.
;
; Comments:
;   This routine returns the current value of the semaphore.  The current
;   value will be the value the semaphore was initialized to when it was
;   created less the number of stations that have done WaitSemaphore calls
;   but no matching SignalSemaphore call.  If the semaphore value is negative
;   it is the number of stations currently waiting on the semaphore.  The
;   open count is the number of stations that have the semaphore open.
;___________________________________________________________________________

	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef ExamineSemaphore,<<handle,long>,<value,dptr>,<count,dptr>>
	mov	ax, 0c501h		;Function 197 (1)
	mov	dx, word ptr (handle)
	mov	cx, word ptr (handle + 2)
	EnterDOS
	PushDS
	PushSI
	LoadPtr si, value, ds
	ChkNull bx, si, ds
	jz	no_value
	mov	[si], cx
no_value:
	LoadPtr si, count, ds
	ChkNull bx, si, ds
	jz	no_count
	mov	[si], dl		;Store open count in openCount parameter
	mov	byte ptr [si + 1], 0
no_count:
	PopSI
	PopDS
	RetVal	byte, reg, al
	Return
	ProcEnd ExamineSemaphore

	EndPS

	END

