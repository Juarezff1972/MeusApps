comment %
 Demo program assembled with the NewBasic++ assembler
 (http://www.zekes.com/newbasic.html)  to locate and
 print the master environment.
 (C)opyright  1984-1998  Forever Young Software
 Benjamin David Lunt
 http://www.zekes.com/~blunt/index.html
 blunt@zekes.com
 20 Sept 98 %

.186                                    ; allow 186 intructions
.model tiny                             ; create COM file
.code                                   ; start of code segment
           mov  ah,52h                  ; (undocumented) get 1st MCB 
           int  21h                     ; .
           mov  ax,es:[bx-2]            ; .
           mov  es,ax                   ; .

           add  ax,es:[0003h]           ; add length of 1st block
           inc  ax                      ;  (paragraphs)
           inc  ax                      ;  +2 to get command psp
           mov  es,ax                   ; and put in es

           mov  ax,es:[002Ch]           ; 2Ch = offset in PSP to environment
           or   ax,ax                   ; if = 0, 
           jnz  short validoff          ;  then must be DOS 3.x or below
           mov  ax,es                   ; dec  es
           dec  ax                      ;
           mov  es,ax                   ;
           add  ax,es:[0003h]           ; get address
           inc  ax                      ;

validoff:  dec  ax                      ; mov one paragraph back
           mov  es,ax                   ; and put in es
           mov  cx,es:[0003h]           ; cx = size of environment
           shl  cx,04                   ;   (in bytes)

           add  ax,04                   ; for Windoze9x DOS session
           ;add  ax,02                   ; for True DOS 7.x
           ;inc  ax                      ; for DOS 4.01 and less

           mov  ds,ax                   ; make ds:si point to first
           xor  si,si                   ;  envirnment var

PrtLoop:   mov  al,0Dh                  ; print CRLF
           int  29h                     ;
           mov  al,0Ah                  ;
           int  29h                     ;
           mov  al,ds:[si]              ; if null then we're done
           or   al,al                   ;
           jz   short Done              ;
PrtLoop1:  lodsb                        ; get the char to print
           or   al,al                   ; if null then end of string
           jz   short PrtLoop           ;
           int  29h                     ; else print it
           loop PrtLoop1                ; loop to next one
; if for some reason we don't find a double null string
;  we will only go cx times with the loop instruction above

Done:      ret
.end
