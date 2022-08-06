;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SETQUEID
;__________________________ SetCapturePrintQueue ____________________________
;
; Input:    BYTE serverNum	/* File server number
;	    BYTE device 	/* 0,1 or 2 for LPT1, 2, or 3		    */
;	    long queueID	/* Print queues object ID
;
; Output:   None.
;
; Comments:
;   This routine will set the server and queue ID for the selected
;   local printer for spooling
;_____________________________________________________________________________

	INCLUDE c.mac
	DOSExterns

	PSeg

	ProcDef SetCapturePrintQueue,<<device,byte>,<serverNum,byte>,<queueID,long>>

	mov	dl, serverNum
	mov	dh, device
	mov	cx, word ptr (queueID)
	; GJP correct for LongSwap done by C routine that got objectID
	xchg	ch,cl
	mov	bx, word ptr (queueID + 2)
	; GJP
	xchg	bh,bl

	mov	ax, 0b806h	; SetCapturePrintQueue function

	EnterDOS
	sub	ah,ah
	RetVal	word, reg, ax

	Return

	ProcEnd SetCapturePrintQueue

	EndPS

	END

