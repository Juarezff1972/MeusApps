;       COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   
NAME	CONVHAN
	INCLUDE c.mac
	DOSExterns

	DSeg
	sendPacket		DB	'SENDPACK'
	sP2	    DW 0123h, 4567h
	EndDS

	PSeg

	ProcDef _ConvertHandle <<connID,word>,<accessMode,byte>,<netwareHandle,dptr>,<forkLength,long>,<DOSHandle,dptr>>
	PushES
	PushDS
	PushSI
	PushDI
	mov ax, connID
	mov sendPacket,al
	mov al,accessMode
	mov sendPacket+1,al
IF MODEL AND 1
	mov ax, seg DGroup
	mov es, ax
ELSE
	push ds
	pop es
ENDIF
	push ds
	LoadPtr si, netwareHandle, ds
	lea di, sendPacket+2
	mov cx, 6
	rep movsb
	pop ds
	les di, forkLength
	mov sP2, di
	mov ax, es
	mov sP2+2, ax
	lea si, sendPacket
	mov	ax, 0B400h
	EnterDOS			;Execute the shell function
	jc	fail
	LoadPtr si, DOSHandle, ds
	sub	ah,ah
	mov	ds:[si], ax
	RetVal word, imm, 0
	jmp	short done
fail:
	RetVal	word, reg, ax
done:
	PopDI
	PopSI
	PopDS
	PopES
	Return
	ProcEnd _ConvertHandle

	EndPS

	END

