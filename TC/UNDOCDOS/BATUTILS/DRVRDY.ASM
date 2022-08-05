; utility to check if disk drive is ready
;
.model tiny
.code
           mov  al,0FFh                 ; Assume error
           cmp  byte [0080h],00h
           je   short Quit
           mov  dl,[0082h]
           and  dl,11011111b            ; make uppercase
           cmp  dl,'A'
           jb   short Quit
           cmp  dl,'B'
           ja   short Quit
           sub  dl,'A'                  ; 00h = a:, 01h = b:
           mov  ah,04h                  ; verify disk sectors
           mov  al,01h                  ; amount to verify
           mov  ch,01h                  ; high order cylinder number (8 bits)
           mov  cl,01h                  ; sector number in bits 0-5
           xor  dh,dh                   ;    remaining 6-7 bits are low order for ch
           int  13h                     ; dh is the head number
           mov  al,0FFh                 ; assume error (0FFh)
           jc   short Quit              ;
           xor  al,al                   ; else no error (00h)
Quit:      .exit                        ; exit with RC

.end

