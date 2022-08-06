;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETPDFLT
	INCLUDE c.mac

;******************************************************************************/
;
; SetPrinterDefaults		Get the default parameters used for spooling
;
; SYNOPSIS
;
; ccode = SetPrinterDefaults(printFlags, tabSize,
; 		networkPrinterNumber, numberOfCopies, formType,
; 		*bannerName, localPrinterNumber, printerTimeOutCount
; 		autoEndSpoolFlag);
;
;
; int ccode;		0 means okay return, non-zero means wrong shell
; 			version.
; int printFlags; 	The flags for printing the file:
; 			0x20 = delete the file after printing.
; 			0x40 = expand tabs to spaces.
; 			0x80 = print a banner page.
; 			(opt: -1)
; int tabSize;		The # of characters in one tab stop.
; 			(opt: -1)
; int networkPrinterNumber;
; 			The printer on the network to use.
; 			(opt: -1)
; int numberOfCopies;	(opt: -1)
; int formType;		(opt: -1)
; char *bannerName;	(opt)
; int localPrinterNumber; The local printer (0-2) being spooled.
; 			(opt: -1)
; int printerTimeOutCount;
; 			After this many seconds of inactivity, an
; 			automatic endspool will be performed.  A zero
; 			value means don't time out.
; 			(opt: -1)
; int autoEndSpoolFlag;	This flag controls the automatic
; 			Endspool/Spool under DOS 3.x.
; 			0 = enable endspool/spool when device closed
; 			1 = disable auto endspool
; 			(opt: -1)
; DESCRIPTION
;
; This routine sets the default printer values.  These values take effect
; any time a modify-list-devide call is made.  If an optional parameter
; is not used, then that printer parameter will not be modified.
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

	ProcDef SetPrinterDefaults,<<flag,word>,<tab,word>,<pn,word>,<cop,word>,<form,word>,<ban,dptr>,<lpn,word>,<time,word>,<auto,word>>
	sub	sp, 25			;Allocate 25 bytes for default info.
	PushSI
	PushDI
	lea	bx, [bp - 25]
	IF	MODEL AND 1
	push	ss
	pop	es
	ENDIF
	mov	ax, 0B800h		;Specify Get Printer Defaults
	mov	cx, 25			;# of bytes to get
	EnterDOS
	or	ax, ax
	jz	DontJump
	jmp	SPDError
	;
	; Copy first five values into the table (printflags - form type)
	;
DontJump:
	xor	si, si			;Zero out indices
	xor	di, di
	mov	cx, 5			;copy the 1st 5 parameters
SPD0:	mov	ax, [bp + si + CPSIZE + 2]
	cmp	ax, -1			;Skip copy if parm is -1
	jz	SPD1
	mov	[bp + di - 24], al	;Copy byte into structure
SPD1:	inc	si			;Move si to next input parameter
	inc	si
	inc	di
	loop	SPD0
	;
	; Skip past reserved byte
	;
	inc	di
	;
	; Get the banner name
	;
	PushDS
	LoadPtr bx, ban, ds
	ChkNull ax, bx, ds
	mov	cx, 13			;Copy at most 13 characters
	jz	SPD3
SPD2:	mov	al, [bx]
	mov	[bp + di - 24], al
	or	al, al
	jz	SPD3
	inc	bx
	inc	di
	loop	SPD2
SPD3:
	PopDS
	inc	cx		;Skip past the rest of the banner name
	add	di, cx
	;
	; Get the local printer number
	;
	mov	ax, lpn
	cmp	ax, -1
	jz	SPD4
	mov	[bp + di - 24], al
SPD4:
	inc	di
	;
	; Get the printer timeout value
	;
	mov	ax, time
	cmp	ax, -1
	jz	SPD5
	mov	bx, 18				;Convert seconds to ticks
	mul	bx
	mov	[bp + di - 24], ax
SPD5:
	inc	di
	inc	di
	;
	; Get the auto Endspool flag
	;
	mov	ax, auto
	cmp	ax, -1
	jz	SPD6
	mov	[bp + di - 24], al
SPD6:
	;
	; Send the altered table back to the shell
	;
	lea	bx, [bp - 25]
	mov	cx, 25
	mov	ax, 0B801h
	EnterDOS

SPDError:

	;
	; All Done!!
	;
	PopDI
	PopSI
	add	sp, 25
	RetVal	word, reg, ax
	Return
	ProcEnd SetPrinterDefaults

	EndPS

	END

