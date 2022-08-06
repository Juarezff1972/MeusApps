;   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETEATTR
	INCLUDE c.mac

;-----------------------------------------------------------------------------
; NAME
;
;    GetExtendedFileAttributes - Get Extended File Attributes.
;
;
; SYNOPSIS
;
;    ccode = GetExtendedFileAttributes(filePath, extendedFileAttributes);
;
;    int  ccode;		    Return value - Zero if successful or an
;						   error if unsuccessful.
;    char *filePath;		    Pointer to the ASCIIZ string containing the
;				    name of the file to be changed.
;    char *extendedFileAttributes;  Pointer to the attribute mask byte.
;
;
; DESCRIPTION
;
;    Gets the extended attributes of a file.
;
;
; RETURNS
;
;   ccode:
;
;	 0  (0x00)  --	SUCCESSFUL
;	      Extended attributes of the file set successfully.
;
;	 255  (0xFF) -- FILE_NOT_FOUND
;	      The indicated file was not found.
;
;	 254  (0xFE)  --  INCORRECT_ACCESS_PRIVILEGES
;	      The caller lacks Personal and Modify access privileges.
;
; SEE ALSO
;
;    File(s)	  - Function name(s)
;    ------------   -------------------------
;    SETEATTR.ASM - SetExtendedFileAttributes.
;
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef GetExtendedFileAttributes,<<filePath,dptr>,<attributes,dptr>>
	PushDS
	PushSI
	LoadPtr dx, filePath, ds
	mov	ax, 0B600h
	EnterDOS
	jc	error_occurred
	LoadPtr si, attributes, ds
	mov	[si], cl
	RetVal	word, imm, 0
	jmp	short done
error_occurred:
	RetVal	byte, reg, al
done:
	PopSI
	PopDS
	Return
	ProcEnd GetExtendedFileAttributes

	EndPS

	END

