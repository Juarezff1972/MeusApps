;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	INTSWAP
	INCLUDE c.mac

	PSeg

;****************************************************************************
;			    ----- IntSwap -----
;
;  Example:   SwappedInt = IntSwap(integer);
;
;  Inputs:    int integer;	   The integer who's bytes we'll swap
;  Outputs:   int SwappedInt;	   The swapped integer
;
;  Comments:  This function swaps the bytes of a WORD.
;****************************************************************************

	ProcDef IntSwap, <<integer,word>>
	mov	ax, integer
	xchg	al, ah
	RetVal	word, reg, ax
	Return
	ProcEnd IntSwap

	EndPS
	END
