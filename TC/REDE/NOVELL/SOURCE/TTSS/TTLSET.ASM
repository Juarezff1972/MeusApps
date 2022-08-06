;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	TTLSET
	INCLUDE c.mac

;----------------------------------------------------------------------------
; ccode = TTSSetApplicationThresholds(logicalLevel, physicalLevel);
;
; Input:   BYTE logicalLevel;
;	   BYTE physicalLevel;
;
; Output:  Returns 0 if successful.
;____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSSetApplicationThresholds,<<logicalLevel,byte>,<physicalLevel,byte>>
	mov	cl, logicalLevel
	mov	ch, physicalLevel
	mov	ax, 0c706h	;Function 199 (6) Set tasks TTS Level
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd TTSSetApplicationThresholds

	EndPS

	END

