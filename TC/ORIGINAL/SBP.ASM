;
; Reset routine for Soundblaster
;
; NOTE:   IO address and IRQ number are set in this file
;	  DMA channel is assumed to be channel 1
;
    .model large,C
    locals

public InitSB, IOaddr, IRQ

.data
IOaddr	dw	220h
IRQ	dw	5

.code
InitSB	proc
    mov al,1
    mov dx,IOaddr
    add dx,6
    out dx,al
    in	al,dx
    in	al,dx
    in	al,dx
    in	al,dx
    mov al,0
    out dx,al
    add dx,4
    mov cx,100
@@1:
    in al,dx
    cmp al,0AAh
    je @@2
    loop @@1
    mov  ax,1
    ret
@@2:
    mov ax,0
    ret
InitSB	endp
    end
