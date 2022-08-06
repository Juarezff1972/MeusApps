;	(c) COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
	NAME	GETDATE
	INCLUDE c.mac


;______________________ GetFileServerDateAndTime ____________________________
;
; Input:    char dateAndTime[7]        Buffer where the date and time will be
;				       returned
; Output:   None.
;
; Comments:
;   This function returns the network date and time.  The returned value is
;   in the following format: byte 0- Year (80-99 = 1980-1999, 0-79 = 2000-
;   2079), byte 1- Month (1-12), byte 2- Day (1-31), byte 3- Hour (0-23),
;   byte 4- Minute (0-59), byte 5- Second (0-59), byte 6- Day of the week
;   (0-6 where 0 = Sunday).
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef GetFileServerDateAndTime, <<dateString, dptr>>
	PushDS
	mov	ah, 231
	LoadPtr dx, dateString, ds
	EnterDOS
	PopDS
	Return
	ProcEnd GetFileServerDateAndTime

	EndPS

	END

