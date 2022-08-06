;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	GETVINFO
	INCLUDE c.mac

;_________________________ GetVolumeInfoWithNumber _____________________________
;
; ccode = GetVolumeInfoWithNumber(volumeNumber, volumeName, totalBlocks,
;				  sectorsPerBlock, availableBlocks,
;				  totalDirectorySlots, availableDirectorySlots,
;				  volumeIsRemovable);
;
;  BYTE volumeNumber;		  The volume number.
;  char volumeName[16]; 	  Receives the volume name that corresponds to
;				  the volume number.
;  WORD *totalBlocks;		  Receives the total number of blocks on the
;				  volume.
;  WORD *sectorsPerBlock;	  Receives the number of sectors per block.
;  WORD *availableBlocks;	  Receives the number of unused blocks on the
;				  volume.
;  WORD *totalDirectorySlots;	  Receives the total number of directory slots
;				  on the volume.
;  WORD *availableDirectorySlots; Receives the number of unused directory slots.
;  WORD *volumeIsRemovable;	  Receives a flag indicating whether or not the
;				  volume is removable or not.  0 = non-removable.
;
;
; Output:		     0	--  SUCCESSFUL
;	    Network Error Code	--  UNSUCCESSFUL
;
; Comments:  This routine returns information about the volume whose volume
;	     number is given.
;------------------------------------------------------------------------------

replyBufferSize     EQU     28

	DOSExterns

	PSeg

ProcDef GetVolumeInfoWithNumber,<<num,byte>,<nam,dptr>,<tlBk,dptr>,<scCt,dptr>,<avBk,dptr>,<tlSl,dptr>,<avSl,dptr>,<rmVol,dptr>>
	sub	sp, replyBufferSize  ;** 11/28/88
	pushes		    ;** 11/28/88
	pushsi
	pushdi
	mov	dl, num
	mov	ah, 0dah
	push	ss	    ;** 11/28/88
	pop	es
	lea	di, [BP-replyBufferSize] ;** 11/28/88
	EnterDOS
	xor	ah, ah
	or	al, al
	jz	NoErrorOccurred
	jmp	ErrorOccurred
NoErrorOccurred:
	lea	di, [BP-replyBufferSize] ;** 11/28/88
	PushDS
	;
	; Copy volume name
	;
	LoadPtr si, nam, ds
	ChkNull ax, si, ds
	jz	L$CopyTotalBlocks
	xor	bx, bx
L$1:
	IF Model AND 1
	mov	al, es:[bx+di+10]
	ELSE
	mov	al, [bx+di+10]
	ENDIF
	mov	[si+bx], al
	inc	bx
	test	al, al
	jnz	L$1
	;
	; Copy total blocks
	;
L$CopyTotalBlocks:
	LoadPtr si, tlBk, ds
	ChkNull ax, si, ds
	jz	L$CopySectorsPerBlock
	IF Model AND 1
	mov	ax, es:[di+2]
	ELSE
	mov	ax, [di+2]
	ENDIF
	xchg	ah, al
	mov	[si], ax
	;
	; Copy sectors per block
	;
L$CopySectorsPerBlock:
	LoadPtr si, scCt, ds
	ChkNull ax, si, ds
	jz	L$CopyAvailableBlocks
	IF Model AND 1
	mov	ax, es:[di]
	ELSE
	mov	ax, [di]
	ENDIF
	xchg	ah, al
	mov	[si], ax
	;
	; Copy available blocks
	;
L$CopyAvailableBlocks:
	LoadPtr si, avBk, ds
	ChkNull ax, si, ds
	jz	L$CopyTotalSlots
	IF Model AND 1
	mov	ax, es:[di+4]
	ELSE
	mov	ax, [di+4]
	ENDIF
	xchg	ah, al
	mov	[si], ax
	;
	; Copy Total Directory slots
	;
L$CopyTotalSlots:
	LoadPtr si, tlSl, ds
	ChkNull ax, si, ds
	jz	L$CopyAvailableSlots
	IF Model AND 1
	mov	ax, es:[di+6]
	ELSE
	mov	ax, [di+6]
	ENDIF
	xchg	ah, al
	mov	[si], ax
	;
	; Copy available directory slots.
	;
L$CopyAvailableSlots:
	LoadPtr si, avSl, ds
	ChkNull ax, si, ds
	jz	L$CopyRemovableFlag
	IF Model AND 1
	mov	ax, es:[di+8]
	ELSE
	mov	ax, [di+8]
	ENDIF
	xchg	ah, al
	mov	[si], ax
	;
	; Copy removable volume flag
	;
L$CopyRemovableFlag:
	LoadPtr si, rmVol, ds
	ChkNull ax, si, ds
	jz	done
	IF Model AND 1
	mov	ax, es:[di + 26]
	ELSE
	mov	ax, [di+26]
	ENDIF
	xchg	ah, al
	mov	[si], ax
done:
	xor	ax, ax
	PopDS
ErrorOccurred:
	RetVal	word, reg, ax
	popdi
	popsi
	popes		    ;** 11/28/88
	add	sp, replyBufferSize  ;** 11/28/88
	Return
ProcEnd GetVolumeInfoWithNumber

	EndPS
	END

