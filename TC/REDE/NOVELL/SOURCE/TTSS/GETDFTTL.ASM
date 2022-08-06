;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
NAME	GETDFTTL
	INCLUDE c.mac

;----------------------------------------------------------------------------
; ccode = TTSGetWorkstationThresholds(logicalLevel, physicalLevel);
;
; Input:   BYTE *logicalLevel;
;	   BYTE *physicalLevel;
;
; Output:  Returns 0 if successful.
;____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSGetWorkstationThresholds,<<logicalLevel,dptr>,<physicalLevel,dptr>>
	PushDS
	PushSI
	mov	ax, 0c707h	;Function 199 (7) Get default TTS Level
	EnterDOS
	cmp	al, 0
	jne	error_occurred
	LoadPtr si, logicalLevel, ds
	ChkNull ax, si, ds
	jz	null_logical
	mov	[si], cl
null_logical:
	LoadPtr si, physicalLevel, ds
	ChkNull ax, si, ds
	jz	null_physical
	mov	[si], ch
null_physical:
	mov	al, 0
error_occurred:
	RetVal	byte, reg, al
	PopSI
	PopDS
	Return
	ProcEnd TTSGetWorkstationThresholds

	EndPS

	END


