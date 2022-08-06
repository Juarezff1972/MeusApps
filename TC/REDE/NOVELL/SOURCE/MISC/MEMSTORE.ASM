;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	MEMSTORE
	INCLUDE c.mac

	PSeg

	ProcDef MemStore, <<destinationSeg, word>, <destinationOffset, word>, <source, dptr>, <length, word>>
	pushds
	pushes
	pushdi
	pushsi
	mov	es, destinationSeg
	mov	di, destinationOffset
	LoadPtr si, source, ds
	mov	cx,length
	cld
	rep movsb
	popsi
	popdi
	popes
	popds
	Return
	ProcEnd MemStore
	EndPS
	END


