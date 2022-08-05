;  WipeIt   is a file deletion routine that first replaces all bytes in the
;           file with a predefined byte.  This makes it harder for someone
;           to see what was in that file.
;           It would not be to difficult to make this routine loop 2 or 3
;           times and replace each byte with a different byte on each loop.
;           This makes for higher security.
;
; WipeIt.com  v1.01
; Forever Young Software
; (C)opyright 1997
; Benjamin David Lunt
; All Rights Reserved
; 06/12/97
; MASM 5.10
;   masm wipeit;
;   link wipeit;
;   exe2bin wipeit wipeit.com
;         (if you don't have exe2bin or a compatible util, see 'MISC FILES'
;          on my web site for exe2com.  It does the same as exe2bin, but
;          has better error checking and error display.)
;
CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg
.386
           org  100h
start:     mov  ax,cs                   ; free unused part of Mem Block
           mov  es,ax                   ;   for .COM file format
           mov  bx,4096                 ;
           mov  ah,4Ah                  ;
           int  21h                     ;
           mov  ah,48h                  ; allocate a full segment
           mov  bx,4096                 ;
           int  21h                     ;
           jc   NoMemory                ;
           mov  ZeroSeg,ax              ; save seg address
           mov  es,ax                   ;
           mov  cx,16384                ;
           mov  eax,59465946h           ; and put 'FY' in segment
           xor  di,di                   ;
           rep  stosd                   ;

           push ds                      ; make sure es=ds
           pop  es                      ;
           push ds                      ; save ds
           mov  ah,62h                  ; get segment address of PSP
           int  21h                     ;
           push bx                      ;
           pop  ds                      ; and put in ds
           mov  cl,ds:[0080h]           ; get len of command line
           xor  ch,ch                   ;
           or   cx,cx                   ; if zero then no filename specified
           jz   NoFileN                 ;
           mov  di,offset es:CmmdLine   ; put it in our filename buffer
           mov  si,0082h                ;
CmmdL:     lodsb                        ; I don't rely on the amount at 0080h
           cmp  al,13                   ; this is more reliable
           je   short CmmdDone          ;
           stosb                        ;
           loop CmmdL                   ;
CmmdDone:  pop  ds                      ; restore ds
           mov  ax,2400h                ; make asciiz string and eos at end
           stosw                        ;  of asciiz for string output
           mov  dx,offset Msg1          ; string output
           mov  ah,09h                  ;    Msg1 and CmmdLine
           int  21h                     ;
           xor  ah,ah                   ;
           int  16h                     ;
           cmp  ax,1559h                ; if not 'Y' then abort
           jne  short Aborted           ;
           mov  ax,3D01h                ; open file for write access
           mov  dx,offset CmmdLine      ;
           int  21h                     ;
           jc   short FileErr           ; if carry then file/path not found?
           mov  Handle,ax               ; else save handle
           mov  bx,ax                   ; and put it in bx
           mov  ax,4202h                ; place file pointer at end of file
           xor  cx,cx                   ;
           xor  dx,dx                   ;
           int  21h                     ; returns file len in dx:ax
           shl  edx,16                  ; put dx in hi of edx, clr lo word
           and  eax,0FFFFh              ; clr hi word of eax
           or   eax,edx                 ; put file size in eax
           push eax                     ; save file size
           mov  ax,4200h                ; replace file pointer at beginning
           xor  cx,cx                   ;  of file
           xor  dx,dx                   ;
           int  21h                     ; returns file len in dx:ax
           pop  eax                     ; restore file size
           xor  dx,dx                   ; offset in buffer
           mov  bx,Handle               ; handle to use
           push ds
           mov  ds,ZeroSeg
DoItL:     or   eax,eax                 ; if bytes left to clr = 0 then done
           jz   short DoItDn            ;
           cmp  eax,0FFFFh              ; if size  <=  0FFFFh then
           jbe  short LastTime          ;   just put in ecx
           mov  ecx,0FFFFh              ; else put 0FFFFh in ecx
           jmp  short WrtIt             ;
LastTime:  mov  ecx,eax                 ;
WrtIt:     sub  eax,ecx                 ; sub len by ecx
           push eax                     ; save len
           mov  ah,40h                  ; write the 'string' (FYFYFYFY....)
           int  21h                     ;  .
           pop  eax                     ; restore len
           jmp  short DoItL             ; go to top of loop
DoItDn:    pop  ds                      ; restore the data segment
           mov  bx,Handle               ; close the file
           mov  ah,3Eh                  ;
           int  21h                     ;
           mov  dx,offset CmmdLine      ; the filename of file to delete
           mov  ah,41h                  ; delete it
           int  21h                     ;
           jmp  short Done              ; finished (exit normaly)
NoMemory:  mov  dx,offset NoMemS        ; string output
           jmp  short PrntIt            ;
FileErr:   mov  dx,offset FireErrS      ; string output
           jmp  short PrntIt            ;
Aborted:   mov  dx,offset AbortedS      ; string output
           jmp  short PrntIt            ;
NoFileN:   pop  ds                      ; restore ds from above
           mov  dx,offset NoFlErr       ; string output
PrntIt:    mov  ah,09h                  ;
           int  21h                     ;
Done:      int  20h                     ; exit to DOS
                                        ;  (uses less code then service 4Ch)
Handle     dw   00h
ZeroSeg    dw   00h
NoFlErr    db   13,10,'No file specified.',36
AbortedS   db   13,10,' Aborted by user.  File not wiped out...',36
FireErrS   db   13,10,'  *Aborted*  File Error...',36
NoMemS     db   13,10,'  *Aborted*  Out of Memory...',36
Msg1       db   13,10,'   Wipe out the file (Y/N):  '      ; no 36 needed
CmmdLine   db   00h   ; we don't need to assign this any thing longer than
                      ; just one byte.  In the startup code, we allowed
                      ; for the whole segment to be in use for our code

CodeSeg    ends
           end  start


This file is donated to the public domain as freeware.  You may do what you
want with it, but please leave me the credit if you distribute it.  If this
routine in anyway destroys you system, I take no liability for it (sorry
about the laywer stuff).  If you have any problems or questions, please
let me know.

Benjamin David Lunt
blunt@zekes.com
http://www.zekes.com/~blunt/index.html

All Rights Reserved

History:
  wipeit.com  v1.00  *release version*

  wipeit.com  v1.01  A simple bug.  I didn't restore the stack when jump
                       to 'No filename specified'.  (added pop ds)
