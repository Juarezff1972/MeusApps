;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  IPXDATAA - IPX Get Data Address (Offset-Segment)
SUBTTL	  Novell 'C' Network Interface Library (NIT.LIB)
NAME	  IPXDATAA
.LALL

;*******************************************************************************
;	PROGRAM NAME:	IPX Get Data Address (Offset-Segment)
;			Novell 'C' Network Interface Library (NIT.LIB)
;	FILENAME:	IPXDATAA.ASM
;	FUNCTION:	Get Data Address for IPX/AES ECB use
;	USAGE:		IPXGetDataAddress(data, address);
;	INPUTS: 	char *data;  /* Pointer to a data item */
;			unsigned address[2];  /* Pointer for address of item
;						 (Offset-Segment order) */
;	PROCESSES:	Get Address of Data Item
;			Return to C
;	OUTPUTS:	unsigned address[2];  /* Pointer to address of item
;						 (Offset-Segment order) */
;*******************************************************************************
;	DATE:		12 Dec 85
;	VERSION:	01.01
;	REMARKS:	This routine gets the data item's address for use by IPX/ECB.
;			The address is in offset-segment order!
;			C requires that DS, SS, SP, BP be saved for all models and
;			that ES be saved for S and P models.
;*******************************************************************************

	INCLUDE c.mac

	PSeg

	ProcDef IPXGetDataAddress, <<data,dptr>,<address,dptr>>
	PushDS
	PushSI
	PushDI
	LoadPtr di, data, es
	LoadPtr si, address, ds
	mov	[si], di
	mov	[si+2], es
	PopDI
	PopSI
	PopDS
	Return
	ProcEnd IPXGetDataAddress

	EndPS

	END

