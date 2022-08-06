;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	IPXINIT
;*******************************************************************************
;	PROGRAM NAME:	IPX Initialization
;	FILENAME:	IPXINIT.ASM
;	FUNCTION:	Get Entry Address for IPX Interface
;	USAGE:		ccode = IPXInitialize();
;	INPUTS: 	NONE
;						 (Offset-Segment order) */
;	PROCESSES:	Get Address of IPX Interface Item
;			Return to C
;	OUTPUTS:	unsigned address[2];  /* Pointer to address of IPX
;						 Interface (returned in
;						 IPXLocation variable) */
;*******************************************************************************
;	DATE:		19 Aug 1987
;	VERSION:	01.01
;	REMARKS:	This routine gets the IPX interface address for use by
;			IPX/ECB.
;			The address is in offset-segment order!
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac

	PSeg

	public	IPXLocation
IPXLocation	dd	0		;Address of IPX/SPX interface

	ProcDef IPXInitialize
IFDEF VAP
	RetVal	byte,imm,0
ELSE
	PushDI
	PushES
	PushSI
	mov	ax, 7a00h
	int	2fh
	cmp	al, 0ffh
	jne	NoIPX
	mov	word ptr IPXLocation, di
	mov	word ptr IPXLocation + 2, es
	RetVal	byte,imm,0
	jmp	short Done
NoIPX:
	RetVal	byte,imm,0f0h
Done:
	PopSI
	PopES
	PopDI
ENDIF
	Return
	ProcEnd IPXInitialize

	EndPS

	END


