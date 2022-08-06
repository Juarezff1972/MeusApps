;   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETEATTR
	INCLUDE c.mac

;----------------------------------------------------------------------------
; NAME
;
;    SetExtendedFileAttributes - Set Extended File Attributes.
;
; SYNOPSIS
;
;    ccode = SetExtendedFileAttributes(filename, extendedAttributes);
;
;    int  ccode;		   Return value - Zero if successful or an
;						  error if unsuccessful.
;    char *filename;		   Pointer to the ASCIIZ string containing the
;				   name of the file to be changed.
;    char extendedAttributes;	   The extended attribute byte that is to be
;				   used to set the files extended attribute.
;
;				   Valid Mask Values:
;
;					0x10 = file should be marked as
;					       transactional.
;					0xEF = file should be marked as
;					       nontransactional.
;
; DESCRIPTION
;
;    Sets the extended attributes of a file.
;
;
; RETURNS
;
;   ccode:
;
;	 0  (0x00)   --   SUCCESSFUL
;	      Extended attributes of the file set successfully.
;
;	 255  (0xFF)  --  FILE_NOT_FOUND
;	      The indicated file was not found.
;
;	 254  (0xFE)  --  INCORRECT_ACCESS_PRIVILEGES
;	      The caller lacks Personal and Modify access privileges.
;
; SEE ALSO
;
;    File(s)	  - Function name(s)
;    ------------   -------------------------
;    AVAILTRN.ASM - TTSIsAvailable,
;    EBEGTRAN.ASM - TTSBeginTransaction,
;    EENDTRAN.ASM - TTSEndTransaction,
;    EABTTRAN.ASM - TTSAbortTransaction,
;    ESTATRAN.ASM - TTSTransactionStatus.
;
;______________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef SetExtendedFileAttributes, <<fileName,dptr>,<attributes,byte>>
	PushDS
	LoadPtr dx, fileName, ds
	mov	cl, attributes
	mov	ax, 0B601h
	EnterDOS
	jc	error_occurred
	RetVal	word, imm, 0
	jmp	short done
error_occurred:
	RetVal	byte, reg, al
done:
	PopDS
	Return
	ProcEnd SetExtendedFileAttributes

	EndPS

	END

