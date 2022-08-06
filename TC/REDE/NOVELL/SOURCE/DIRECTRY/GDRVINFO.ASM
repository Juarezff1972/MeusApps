;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GDRVINFO
	INCLUDE c.mac

;****************************************************************************
;		   ------ GetDriveInformation ------
;
; Example:  statusFlags = GetDriveInformation(driveNumber, &connectionID,
;					      &driveHandle);
;
; Input:    BYTE driveNumber;	 The drive number for which the information
;				 should be returned (0=A, 1=B, etc.)
;	    WORD *connectionID;  Pointer to where the connection handle
;				 of the server that the drive is mapped to
;				 will be returned if the drive is mapped to
;				 a network directory.
;	    BYTE *driveHandle;	 Pointer to the byte where the drive handle
;				 associated with the specified drive letter
;				 will be returned if the drive is mapped
;				 to a network directory
;
; Output:   Receives the status of the drive.  The status indicates if there
;	    drive is a local drive at this workstation and also if the drive
;	    has been mapped to a network directory.
;
; Comments: This function returns information about a drive.
;*****************************************************************************

	DOSExterns

	PSeg

	ProcDef GetDriveInformation, <<drive,byte>,<connID,dptr>,<dirHandle,dptr>>
	PushES
	PushDS
	PushSI
	LoadPtr si, connID, ds
	ChkNull bx, si, ds
	jz	no_conn_handle
	mov	ax, 0ef02h	;Function 239 (2) Get drive server table addr
	EnterDOS
	mov	bl, drive
	xor	bh, bh
	mov	al, es:[si + bx] ;AL gets the drive server number
	LoadPtr si, connID, ds
	xor	ah,ah
	mov	[si], ax
no_conn_handle:
	mov	ax, 0e900h	;Function 233 (0) Get Base Drive Status
	mov	dh, 0
	mov	dl, drive
	EnterDOS
	LoadPtr si, dirHandle, ds
	ChkNull bx, si, ds
	jz	no_dir_handle
	mov	[si], al
;	mov	BYTE PTR [si+1],0   DONT TREAT directoryHandle AS A WORD
no_dir_handle:
	RetVal	byte, reg, ah
	PopSI
	PopDS
	PopES
	Return
	ProcEnd GetDriveInformation

	EndPS

	END


