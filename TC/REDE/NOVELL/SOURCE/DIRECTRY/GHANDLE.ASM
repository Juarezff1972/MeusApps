;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GHANDLE
	INCLUDE c.mac

	DOSExterns

	PSeg

	ProcDef GetDirectoryHandle, <<drive,byte>>
	PushES
	PushDS
	PushSI
	mov	ax, 0e900h	;Function 233 (0) Get Base Drive Status
	mov	dh, 0
	mov	dl, drive
	EnterDOS
	RetVal	byte, reg, al
	PopSI
	PopDS
	PopES
	Return
	ProcEnd GetDirectoryHandle

	EndPS

	END
