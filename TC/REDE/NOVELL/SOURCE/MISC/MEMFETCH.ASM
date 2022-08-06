;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	MEMFETCH
	INCLUDE c.mac

	PSeg

	ProcDef MemFetch, <<sourceSeg, word>, <sourceOffset, word>, <destination, dptr>, <length, word>>
	pushsi
	pushdi
	push	ds	  ;force push because ds get clobbered in any memory model
	pushes
	LoadPtr di, destination, es
	mov	ds, sourceSeg
	mov	si, sourceOffset
	mov	cx,length
	cld
	rep movsb
	popes
	pop	ds	  ;force pop because ds get clobbered in any memory model
	popdi
	popsi
	Return
	ProcEnd MemFetch
	EndPS
	END

