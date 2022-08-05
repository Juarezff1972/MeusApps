; ************************************************************************
; *  This is a small snip of code to see what the bit status of a
; *  specified word in memory.  I use it for checking the Keyboard
; *  word status.  (0040:0017h)
; *  It also contains a snip of code to change an unsigned word to an
; *  ascii binary string ready for output to this screen.
; *
; *  You may use this code as you would like.
; *  As always, If this code crashes you machine in anyway I am
; *  not held responsible.  Use at your own risk.
; *   Benjamin David Lunt
; *   Forever Young Software (r)
; *   (C)opyright 1984-1999
; *   All rights reserved.
; *    08 Dec 1998
; *    NBASM 00.23.xx
; *

.model tiny
.code
           org  100h
           call cursoroff
           mov  ah,02
           xor  bh,bh
           mov  dx,0502h
           int  10h
           mov  si,offset Bit1
           call prtstring
Loop1:     push ds
           mov  bx,Addr
           mov  ds,ASeg
           mov  ax,[bx]
           pop  ds
           push ax
           mov  ah,02
           xor  bh,bh
           mov  dx,0602h
           int  10h
           pop  dx
           call Num2Bin
           mov  si,offset Bit
           call prtstring
           mov  ah,01
           int  16h
           jz   short Loop1
           xor  ah,ah
           int  16h
           cmp  al,27
           jnz  short Loop1
Done:      call cursoron
           mov  ah,4Ch                  ; exit to DOS
           int  21h                     ; 

Num2Bin    proc near
           push ds
           pop  es
           mov  di,offset Bit
           mov  cx,16
DoIt:      mov  al,48
           shl  dx,1
           jnc  short Done1
           inc  al
Done1:     stosb
           loop DoIt
           xor  al,al
           stosb
           ret
Num2Bin    endp                         ; end of procedure

Prtstring  proc near uses ax si
Ps1:       mov  dl,[si]                 ; Get character
           inc  si                      ; Point to next one
           or   dl,dl                   ; End of string?
           jz   short ps2               ; Yes, so exit
           mov  ah,02h                  ; Output a character
           int  21h
           jmp  short Ps1               ; Keep doing it
Ps2:       ret
Prtstring  endp

CursorOff  proc near uses ax cx
           pushf
           mov  ah,01h                  ; turn off cursor
           mov  ch,20h                  ; bit number 5
           int  10h                     ;
           popf
           ret
CursorOff  endp

CursorOn   proc near uses ax cx
           mov  ah,01h                  ; turn on cursor
           mov  cx,0607h                ; start = 6   finish = 7
           int  10h                     ;
           ret
CursorOn   endp

Bit1       db  'FEDCBA9876543210',0
ASeg       dw  0040h
Addr       dw  0017h
Bit        dup 33,0

.end
