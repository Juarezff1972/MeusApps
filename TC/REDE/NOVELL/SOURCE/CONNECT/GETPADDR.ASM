;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETPADDR
	INCLUDE c.mac

;*____________________________ GetStationAddress _____________________________
;
; GetStationAddress(internetHost)
;
; Input:    BYTE internetHost[6];	/* Buffer where the station address
;					   will be returned		    */
;
; Output:   None.
;
; Comments:
;   This routine returns the station address of the network interface
;   card.  The station address is the 6 byte internetHost address.  This
;   call will not return the 4 byte internetNetwork address, which is also
;   needed to send packets on the internet.  The complete address of the
;   station can be obtained by calling GetConnectionNumber and then calling
;   GetInternetAddress.
;____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef GetStationAddress, <<nodeAddress, dptr>>
	mov	ah, 0eeh	;Function 238 (EEh) Get Physical Station
	EnterDOS
	PushDS
	PushDI
	xchg cl, ch
	xchg bl, bh
	xchg al, ah
	LoadPtr di, nodeAddress, ds
	mov	[di], cx
	mov	[di + 2], bx
	mov	[di + 4], ax
	PopDI
	PopDS
	Return
	ProcEnd GetStationAddress

	EndPS

	END

