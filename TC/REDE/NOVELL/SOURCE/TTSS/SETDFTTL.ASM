;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETDFTTL
	INCLUDE c.mac

;------------------------------------------------------------------------------
; ccode = TTSSetWorkstationThresholds(logicalLevel, physicalLevel);
;
; Input:   BYTE logicalLevel;
;	   BYTE physicalLevel;
;
; Output:  Returns 0 if successful.
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef TTSSetWorkstationThresholds,<<logicalLevel,byte>,<physicalLevel,byte>>
	mov	cl, logicalLevel
	mov	ch, physicalLevel
	mov	ax, 0c708h	;Function 199 (8) Set default TTS Level
	EnterDOS
	RetVal	byte, reg, al
	Return
	ProcEnd TTSSetWorkstationThresholds

	EndPS

	END


