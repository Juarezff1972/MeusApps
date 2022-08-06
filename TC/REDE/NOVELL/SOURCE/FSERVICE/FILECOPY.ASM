;   (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	FILECOPY
	INCLUDE c.mac

;----------------------------------------------------------------------------
;  FileServerFileCopy - Copies directly from one file to another.
;
;  ccode = FileServerFileCopy(sourceFileHandle, destinationFileHandle,
;		sourceFileOffset, destinationFileOffset, numberOfBytesToCopy,
;		numberOfBytesCopied);
;
;  Input:   int   sourceFileHandle;
;	    int   destinationFileHandle;
;	    long  sourceFileOffset;
;	    long  destinationFileOffset;
;	    long  numberOfBytesToCopy;
;	    long *numberOfBytesCopied
;
;  Output:  0 if successful and the number of bytes copied
;_____________________________________________________________________________

	DOSExterns

	PSeg

	ProcDef FileServerFileCopy,<<srcHandle,word>,<destHandle,word>,<srcOffset,long>,<destOffset,long>,<count,long>,<bytesCopied,dptr>>
	PushES
	PushDI
	mov	ax, ss
	mov	es, ax
	lea	di, srcHandle
	mov	ah, 0f3h	;Shell function 243 file to file copy with
				;	DOS file handles
	EnterDOS
	or	al, al
	jnz	error_occurred
	PushDS
	LoadPtr di, bytesCopied, ds
	mov	[di], cx
	mov	[di + 2], dx
	PopDS
error_occurred:
	RetVal	byte, reg, al
	PopDI
	PopES
	Return
	ProcEnd FileServerFileCopy

	EndPS

	END

