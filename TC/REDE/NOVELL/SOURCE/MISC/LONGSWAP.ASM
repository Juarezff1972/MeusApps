;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	LONGSWAP
	INCLUDE c.mac

	PSeg

;****************************************************************************
;			  ----- LongSwap -----
;  Example:   swappedLong = LongSwap(dword);
;
;  Inputs:    long  dword	      Long word who's bytes we'll swap
;  Outputs:   long  swappedLong       Long with its bytes swapped
;
;  Comments:  This function swaps the bytes of a long (4 bytes).
;*****************************************************************************

	ProcDef LongSwap,<<longValue,long>>
	mov	bx, word ptr (longValue + 2)
	xchg	bl, bh
	mov	ax, word ptr (longValue)
	xchg	al, ah
	RetVal	long, reg, bx, ax
	Return
	ProcEnd LongSwap

	EndPS
	END
