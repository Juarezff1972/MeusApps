;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  Console Query
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  ConsoleQuery
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: ConsoleQuery
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     CONQUERY.ASM
;	FUNCTION:     Display A Prompt On System Console And Get Y/N Response
;	USAGE:	      response = ConsoleQuery(StringLoc);
;	OUTPUTS:      Outputs The Prompt
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ConsoleControlServices,dword

	PSeg

	ProcDef ConsoleQuery,<<msg,dptr>>
	PushDI
	PushDS
	PushES
	PushSI

	LoadPtr si, msg, ds
	mov	di, 06h
	mov	cx, seg ConsoleControlServices
	mov	bx, offset ConsoleControlServices
	mov	es, cx
	call	dword ptr es:[bx]

	PopSI
	PopES
	PopDS
	PopDI

	RetVal	word,reg,ax
	Return

	ProcEnd ConsoleQuery

	EndPS

	END
