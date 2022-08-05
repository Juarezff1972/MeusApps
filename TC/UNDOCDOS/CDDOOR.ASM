; this utilitie opens/closes the CD door.  It doesn't print anything
; to the screen so that it can be 'cleanly' used in a batch file.
;
; util to open/close the CD door.
; CDDOOR +D  (or -D)
;  where + denotes open and - denotes close
;  where D is the drive letter (A - Z) (upper case)

; assembled with NBASM
; http://www.zekes.com/~blunt/newbasic.htm

.model tiny
.code
           mov  al,[0082h]              ; get first char on command line
           cmp  al,'+'                  ; + opens the door
           jne  short NotOpenIt
           mov  byte Job,00h            ; open the door
           jmp  short GetDrive
NotOpenIt: cmp  al,'-'                  ; - closes the door
           jne  short NotBuzyo          ; invalid job
           mov  byte Job,05h            ; close the door
           jmp  short GetDrive

GetDrive:  mov  al,[0083h]              ; get second char on command line
           cmp  al,'A'                  ; if it is 'D' or less then
           jb   short JustDoIt          ;  just assume 'D'
           cmp  al,'Z'                  ; if it is 'Z' or more then
           ja   short JustDoIt          ;  just assume 'D'
           sub  al,'A'                  ; D = 3, E = 4, ...
           xor  ah,ah                   ;
           mov  device,ax               ; put in device

JustDoIt:  mov  bx,offset ioctlo        ; Do the I/O
           mov  [bx+16],ds              ; data seg
           mov  cx,device               ;
           mov  ax,1510h                ;
           int  2Fh                     ;
           mov  bx,offset ioctlo        ;
           mov  dx,[bx+3]               ; status
           xor  ax,ax                   ; assume no error
           test dx,8000h                ;
           jz   short NoRetErr          ;
           mov  al,02h                  ; other error
NoRetErr:  test dx,20h                  ;
           jz   short NotBuzyo          ;
           mov  al,01h                  ; busy error

NotBuzyo:  mov  ah,4Ch                  ; Exit program, close files
           int  21h                     ; and return to DOS

device     dw  03h                      ; 3 = D:, 4 = E:, etc.
Job        db  0                        ; cc   (00 = Open it, 05 = close it)
ioctlo     db  13,0,12                  ; rhlen,subu,comc
           dw  00h                      ; status
           dup 8,0                      ; reserved
           db  0                        ; mdb
           dw  offset Job               ; *trandad (off)
           dw  00h                      ; *trandad (seg)
           dw  01h,00h                  ; tranct,ssn
           dw  00h,00h                  ; *volid
.end
 
