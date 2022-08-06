;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SCAPPRNJ
    INCLUDE c.mac
    DOSExterns

    PSEG

    ProcDef SetCapturePrintJob <<lptdev, byte>,<jobnum, word>,<hndl, dptr>>
    pushds
    pushdi
    pushsi

    mov     ax, 0b807h
    LoadPtr bx, hndl, ds
    mov     si, ds:[bx]      ;capture file handle- hi word to low word
    mov     di, ds:[bx+2]
    mov     cx, ds:[bx+4]
    mov     dh, lptdev
    mov     bx, jobnum
    EnterDOS
    sub     ah,ah	    ;clear high byte
    RetVal  word, reg, ax

    popsi
    popdi
    popds
    Return

    ProcEnd SetCapturePrintJob

    EndPS

    End
