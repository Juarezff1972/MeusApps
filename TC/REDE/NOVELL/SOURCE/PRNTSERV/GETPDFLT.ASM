;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETPDFLT
	INCLUDE c.mac
	.lall
;****************************************************************************
;
; GetPrinterDefaults
;
; SYNOPSIS
;
;  ccode = GetPrinterDefaults(status, printFlags, tabSize,
;  		networkPrinterNumber, numberOfCopies, formType,
;  		bannerName, localPrinterNumber, printerTimeOutCount,
;  		autoEndSpoolFlag);
;
;  int ccode;		        Non-zero if wrong shell version.
;  int *status;		        0 if enabled, non-zero if disabled. (opt)
;  int *printFlags;	        Current print flag settings. (opt)
;  int *tabSize;    	        # of spaces per tab (opt)
;  int *networkPrinterNumber;   Current printer being spooled to. (opt)
;  int *numberOfCopies;	        # of copies per print job (opt)
;  int *formType;    	        The type of form on the printer. (opt)
;  char *bannerName;	        The banner name in use (opt)
;  int *localPrinterNumber;     The local printer number being captured. (opt)
;  int *printerTimeOutCount;    The number of seconds of inactivity allowed
;  			        before doing an automatic endspool. (opt)
;  int *autoEndSpoolFlag;	The automatic Endspool/Spool function of
;  			        DOS 3.x when the printer device is closed.
;
; DESCRIPTION
;
;  This function gets the specified parameters' current values.
;  A pointer may be NULL if no value is required.
;****************************************************************************

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

	ProcDef GetPrinterDefaults,<<stat,dptr>,<flag,dptr>,<tab,dptr>,<pn,dptr>,<cop,dptr>,<form,dptr>,<ban,dptr>,<lpn,dptr>,<time,dptr>,<auto,dptr>>
	sub	sp, 25
	PushSI
	PushDI

	;
	; Read in the table
	;
	lea	bx, [bp - 25]
	IF	MODEL AND 1
	push	ss
	pop	es
	ENDIF
	mov	ax, 0B800h		;Specify Get Printer Defaults
	mov	cx, 25			;# of bytes to get
	EnterDOS
	or	ax, ax
	jnz	GPDError

	;
	; Copy the first six parameters
	;
	mov	cx, 6
	xor	si, si
	xor	di, di
	xor	ah, ah
GPD0:
;Iteratively store each parameter
	LoadPtr bx, <[bp + di + CPSIZE + 2]>, es
	ChkNull dx, bx, es
	jz	GPD1
	mov	al, [bp + si - 25]
	mov	es:[bx], ax
GPD1:	add	di, DPSIZE
	inc	si
	loop	GPD0

	;
	; Skip the reserved byte
	;
	inc	si

	;
	; Copy the banner name, if needed
	;
	mov	cx, 14
	LoadPtr bx, ban, es
	ChkNull dx, bx, es
	jz	GPD3
GPD2:	mov	al, [bp + si - 25]
	mov	es:[bx], al
	or	al, al
	jz	GPD3
	inc	bx
	inc	si
	loop	GPD2
GPD3:	add	si, cx
	;
	; Get the value of the printer number
	;
GPD4:
	LoadPtr bx, lpn, es
	ChkNull dx, bx, es
	jz	GPD5
	mov	al, [bp + si - 25]
	mov	es:[bx], ax
	;
	; Get the value of the timeout count
	;
GPD5:
	inc	si
	LoadPtr bx, time, es
	ChkNull dx, bx, es
	jz	GPD6
	mov	ax, [bp + si - 25]
	xor	dx, dx
	mov	cx, 18
	div	cx
	mov	es:[bx], ax
	;
	; Get the value of the endspool flag
	;
GPD6:
	inc	si
	inc	si
	LoadPtr bx, auto, es
	ChkNull dx, bx, es
	jz	GPD7
	mov	al, [bp + si - 25]
	xor	ah,ah
	mov	es:[bx], ax
GPD7:
	xor	ax, ax
GPDError:
	PopDI
	PopSI
	add	sp, 25
	RetVal	word, reg, ax
	Return
	ProcEnd GetPrinterDefaults

	EndPS

	END

