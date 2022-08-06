;	(c) COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
        NAME	SERVNAME
	INCLUDE c.mac

;_____________________________________________________________________________
;
; GetFileServerName(connectionID, fileServerName);
;
; Inputs:  WORD connectionID;	   Number of the slot for which the server name
;				   should be returned.
;	   char *fileServerName;   Receives the file server name should be
;
; Outputs:  None.
;
; Comments:   This function returns the name of the file server for a given
;	      server number.  If a server number of 0 is given, then the
;	      default server is used.
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef GetFileServerName,<<connectionID,byte>,<fileServerName,dptr>>
	PushES
	PushDS
	PushSI
	PushDI
	mov	bl, connectionID
	or	bl, bl
	jnz	got_number

;---- The server number is 0, use the default server number ----

	mov	ax, 0f002h	;Function 240 (2) Get Effective Server
	EnterDOS
	mov	bl, al

;---- Get the address of the shell Server Name Table ----

got_number:
	mov	ax, 0ef04h	; Function 239 (4) Get Server Name Table Address
	EnterDOS

; ---- Move to the appropriate slot in the table (each entry is 48 bytes) ----

check_slot:
	dec	bl		; Make number 0 based / move to next entry
	or	bl, bl
	jz	found_slot
	add	si, 48
	jmp	short check_slot


;    ---- Copy the server name ----

found_slot:
	LoadPtr di, fileServerName, ds
next_character:
	mov	al, es:[si]
	mov	[di], al
	inc	si
	inc	di
	or	al, al
	jnz	next_character
	PopDI
	PopSI
	PopDS
	PopES
	Return
	ProcEnd GetFileServerName

	EndPS

	END
