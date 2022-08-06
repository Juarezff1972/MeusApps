;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  SPX INITIALIZE
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  SPXINIT
.LALL

;*******************************************************************************
;	PROGRAM NAME: SPX initialize
;		      Novell 'C' Network Interface Library (NIT.LIB)
;
;	FILENAME:     SPXINIT.ASM
;	FUNCTION:     CHECK AVAILABILITY OF SPX.
;	USAGE:	      SPXInitialize().
;	INPUTS:       None
;
;	PROCESSES:    Prepare for IPX call, SPX function 10h
;
;
;	OUTPUTS:      If available major and minor versions of SPX in ah and al
;		      respectfully.  Pointer to address of IPX Interface
;		      (returned in IPXLocation variable)
;*******************************************************************************
;	DATE:		23 MAR 87
;	VERSION:	01.01
;	REMARKS:	This one is pretty basic.
;
;*******************************************************************************

	INCLUDE c.mac
	IPXExterns

	PSeg

	ProcDef SPXInitialize	<<majorrevnumber,dptr>,<minorrevnumber,dptr>,<spxmaxconnections,dptr>,<spxavailconnections,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

IFNDEF VAP
	push	ds
	mov	ax, seg IPXLocation
	mov	ds, ax
	mov	ax, 7a00h
	int	2fh
	cmp	al, 0ffh
	je	IPXInstalled
	pop	ds
	RetVal	byte,imm,0f0h
	jmp	done1
IPXInstalled:
	mov	word ptr ds:IPXLocation, di
	mov	word ptr ds:IPXLocation + 2, es
	pop	ds
ENDIF
	mov	al,00h
	push	bp
	mov	bx,10h
	EnterIPX				;IPX Interrupt
	pop	bp
	push	ax
	LoadPtr si, majorrevnumber, ds
	ChkNull ax, si, ds
	jz	minorrev
	mov	[si], bh
minorrev:
	LoadPtr si, minorrevnumber, ds
	ChkNull ax, si, ds
	jz	maxconnections
	mov	[si], bl
maxconnections:
	LoadPtr si, spxmaxconnections, ds
	ChkNull ax, si, ds
	jz	availconnections
	mov	[si], cx
availconnections:
	LoadPtr si, spxavailconnections, ds
	ChkNull ax, si, ds
	jz	done
	mov	[si], dx
done:
	pop ax
	RetVal	byte,reg,al
done1:
	PopSI
	PopES
	PopDS
	PopDI
Return
	ProcEnd SPXInitialize

	EndPS

	END

