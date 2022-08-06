;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GTCAPINF
	INCLUDE c.mac
; GetCaptureInfo
;
; SYNOPSIS
;
;	ccode = GetCaptureInfo(LPTCatchStatus, fileCatchFlag,
;			headerAddressLow, headerAddressHigh, tailAddressLow,
;			tailAddressHigh, queueServerNumber,
;			spoolingToQueueFlag, queueID);
;
;	int ccode;		Non-zero if wrong shell version.
;	BYTE *LPTCatchStatus;	0 if enabled, non-zero if disabled. (opt)
;	BYTE *fileCatchFlag;	non-zero = catch to file active
;	WORD *headerAddressOffset;  low word of header sequence buffer (opt)
;	WORD *headerAddressSegment; high word of header sequence buffer (opt)
;	WORD *tailAddressOffset;    low word of tail sequence buffer (opt)
;	WORD *tailAddressSegment;   high word of tail sequence buffer (opt)
;	WORD *queueServerNumber;    server number where capture is going (opt)
;	BYTE *spoolingToQueueFlag;  0 if not spooling to a queue
;	long *queueID;
;
; DESCRIPTION
;
;	This function gets the specified parameters' current values.
;	A pointer may be NULL if no value is required.
;

IF	MODEL AND 1
	DPSIZE EQU 4
ELSE
	DPSIZE EQU 2
ENDIF
IF	MODEL AND 2
	CPSIZE EQU 4
ELSE
	CPSIZE EQU 2
ENDIF
	DOSExterns

	PSeg

	ProcDef GetCaptureInfo,<<stat,dptr>,<cflg,dptr>,<hl,dptr>,<hh,dptr>,<tl,dptr>,<th,dptr>,<srv,dptr>,<toq,dptr>,<qid,dptr>>
	sub	sp, 63
	PushSI
	PushDI

	;
	; Read in the table
	;
	lea	bx, [bp - 63]
	IF	MODEL AND 1
	push	ss
	pop	es
	ENDIF
	mov	ax, 0B800h		;Specify Get Printer Defaults
	mov	cx, 63			;# of bytes to get
	EnterDOS
	or	ax, ax
	jz	GCIOK
	jmp	GCIError

GCIOK:
	mov	si, 42

	;
	; Get the value of the LPT catch set flag
	;
	LoadPtr bx, stat, es
	ChkNull dx, bx, es
	jz	GCI0
	mov	al, [bp + si - 63]
	mov	es:[bx], al
	;
	; Get the value of the file catch flag
	;
GCI0:
	inc	si
	LoadPtr bx, cflg, es
	ChkNull dx, bx, es
	jz	GCI1
	mov	ax, [bp + si - 63]
	mov	es:[bx], al
	;
	; Get the offset of the header pointer
	;
GCI1:
	mov	si, 45
	LoadPtr bx, hl, es
	ChkNull dx, bx, es
	jz	GCI2
	mov	ax, [bp + si - 63]
	mov	es:[bx], ax
	;
	; Get the segment of the header pointer
	;
GCI2:
	inc	si
	inc	si
	LoadPtr bx, hh, es
	ChkNull dx, bx, es
	jz	GCI3
	mov	ax, [bp + si - 63]
	mov	es:[bx], ax
	;
	; Get the offset of the tail pointer
	;
GCI3:
	inc	si
	inc	si
	LoadPtr bx, tl, es
	ChkNull dx, bx, es
	jz	GCI4
	mov	ax, [bp + si - 63]
	mov	es:[bx], ax
	;
	; Get the segment of the tail pointer
	;
GCI4:
	inc	si
	inc	si
	LoadPtr bx, th, es
	ChkNull dx, bx, es
	jz	GCI5
	mov	ax, [bp + si - 63]
	mov	es:[bx], ax
	;
	; Get the server number pointer
	;
GCI5:
	mov	si,53
	LoadPtr bx, srv, es
	ChkNull dx, bx, es
	jz	GCI6
	mov	al, [bp + si - 63]
	mov	es:[bx], al
	;
	; Get the spoolingToQueue flag pointer
	;
GCI6:
	inc	si
	inc	si
	LoadPtr bx, toq, es
	ChkNull dx, bx, es
	jz	GCI7
	mov	al, [bp + si - 63]
	mov	es:[bx], al
	;
	; Get the queue ID pointer
	;
GCI7:
	inc	si
	inc	si
	LoadPtr bx, qid, es
	ChkNull dx, bx, es
	jz	GCI8
	mov	ax, [bp + si - 61]	;get the low word first
	mov	es:[bx], ah		; swap it when putting it down
	inc	bx
	mov	es:[bx], al
	inc	bx
	mov	ax, [bp + si - 63]	;now get the high word
	mov	es:[bx], ah		; and swap it when putting it down
	inc	bx
	mov	es:[bx], al

GCI8:
	xor	ax,ax

GCIError:
	PopDI
	PopSI
	add	sp, 63
	RetVal	word, reg, ax
	Return
	ProcEnd GetCaptureInfo

	EndPS

	END

