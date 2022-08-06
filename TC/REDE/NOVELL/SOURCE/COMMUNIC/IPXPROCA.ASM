;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	IPXPROCA
;************************************************************************
;	PROGRAM NAME:	IPX Get Procedure Address (Offset-Segment)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXPROCA.ASM
;	FUNCTION:	Get Procedure Address for IPX/AES ECB use
;	USAGE:		IPXGetProcAddress(procedure, address);
;	INPUTS: 	int (*procedure)();  /* Pointer to a procedure */
;			unsigned address[2];  /* Pointer for address of item
;						 (Offset-Segment order) */
;	PROCESSES:	Get Address of Procedure
;			Return to C
;	OUTPUTS:	unsigned address[2];  /* Pointer to address of item
;						 (Offset-Segment order) */
;	REMARKS:	This routine gets the procedure's address for use by IPX/ECB.
;			The address is in offset-segment order!
;			The Procedure pointer is a code pointer and the address pointer
;			is a data pointer.  Be careful in different memory models.
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************
;	For large code programs LProg is defined in the makefiles.
;	For large data programs LData is defined in the makefiles.
;*******************************************************************************

	INCLUDE c.mac

	PSeg

	ProcDef IPXGetProcAddress, <<procedure, cptr>, <address, dptr>>
	; Do this section of code if the model you are using is
	; small data and small code.
	IFNDEF LData
	    IFNDEF LProg
	    mov di, [bp + CPSIZE + 2]		;Procedure pointer
	    mov si, [bp + CPSIZE + 4]		;Address pointer
	    mov [si], di			;Load address
	    mov [si+2], cs			;Code segment
	    ENDIF
	ENDIF

	; Do this section of code if the model you are using is
	; large data and either small or large code
	IFDEF	LData
	push	ds				;Save DS
	lds	di, dword ptr [bp + CPSIZE + 2] ;Procedure pointer
	les	si, dword ptr [bp + CPSIZE + 6] ;Address pointer
	mov	es:[si], di			;Load address
	mov	es:[si+2], ds			;Code segment
	pop	ds				;Restore DS
	ENDIF

	; Do this section of code if the model you are using is
	; small data and large code
	IFDEF	LProg
	    IFNDEF LData
	    mov di, [bp + CPSIZE + 2]		;Procedure pointer pointer
	    mov si, [bp + CPSIZE + 4]		;Address pointer
	    mov ax, [di]			;Load address
	    mov [si], ax
	    mov ax, [di + 2]			;Code segment
	    mov [si + 2], ax
	    ENDIF
	ENDIF

	Return
	ProcEnd IPXGetProcAddress

	EndPS

	END

