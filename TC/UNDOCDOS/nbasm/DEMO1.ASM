comment %
 Demo program assembled with the NewBasic++ assembler
 (C)opyright  1984-1999  Forever Young Software
 Benjamin David Lunt
 18 Jan 99 %

One    equ  01h                         ; test EQU
.external prtstring                     ; include the code for prtstring
.186                                    ; allow 186 intructions
.model tiny                             ; create COM file
.stack 64                               ; make the stack 64+ bytes
.code                                   ; start of code segment
           .start                       ; write startup code (set sp)
           mov  ah,One                  ; turn off cursor (test EQU)
           mov  ch,00100000b            ; bit number 5 (test BIN num)
           int  10h                     ;
           mov  ax,offset cs:msg1       ; string output (test offset cs:)
           push ax                      ; use library function (prtstring)
           call prtstring               ;
scrolls:   mov  dx,offset scrllmsg      ; scroll section
           mov  ah,09h                  ;
           int  21h                     ;
           mov  si,dx                   ; move scrllmsg into si from dx above
           mov  di,dx                   ; move desination into di
           lodsb                        ; move 1st letter -> al move si to 2nd letter (save letter)
           mov  cx,'M'                  ; do it 77 times (test ischar)
           rep
           movsb                        ; move from source to destination
           stosb                        ; store string byte into destination (saved letter)
           mov  ah,86h                  ; pause execution
           mov  cx,02h                  ;
           mov  dx,00h                  ; ** optimizer change to:  xor dx,dx **
           int  15h                     ;
           mov  ah,06h                  ; see if key pressed
           mov  dl,0FFh                 ;
           int  21h                     ;
           jz   short scrolls           ; loop if no key pressed (test 'short')
           call PrtThnx                 ; test calling proc and ret - endp
           mov  dx,offset @label        ; Move down a line (test unique name)
           mov  ah,09h                  ;
           int  21h                     ;
           cs:                          ; test cs: (same as mov  ah,cs:One1)
           mov  ah,One1                 ; turn on cursor (test mem ref)
           mov  ah,cs:One1              ; turn on cursor (test mem ref w/OvrR)
           mov  cs:One1,ah              ; just testing seg override
           push 0607h                   ; test the push of a const
           pop  cx                      ; cx = 0607h start = 6  finish = 7
           int  10h                     ;
.OPTON                                  ; optimiser on
           int  20h                     ; if optimize & .START used, then
                                        ;  will be changed to:  RET
.OPTOF                                  ; optimiser off


PrtThnx    proc near uses all           ; check the all keyword
           mov  dx,offset ThnkS
           mov  ah,09
           call PrtThnx1
           ret
           endp

PrtThnx1   proc near uses ax            ; check the nesting of uses
           int  21h
           ret
           endp

One1       db   01h
msg1       db   13,0Ah
           db    'ษ'
           dup   38,'อ'       ; test DUP
           db    'ป',13,10
           db   'บ              NewBasic++              บ',13,10
           db   'บ        Forever Young Software        บ',13,10
           db   'บ          Benjamin David Lunt         บ',13,10
           db   'บ                  ',157,'                   บ',13,10
           db   'บ         copyright  1984-1999         บ',13,10
           db   'บ           version 00.23.29           บ',13,10
           db   'ศออออออออออออออออออออออออออออออออออออออผ',13,10,0
scrllmsg   db   157
           db   '  NewBasic++  **  Forever Young Software  **  Benjamin David Lunt  **  1999 '
           db   157,13,36
@label     db   13,10,36     ; test unique name
ThnkS      db   13,10,10,"      Forever Young thanks you...",36
.end
