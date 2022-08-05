.MODEL small, basic
.386
.stack

  PgPort     equ 83h
  AddPort    equ 02h
  LenPort    equ 03h
  ModeReg    equ 49h
  Channel    equ 01h
  BasePort   equ 220h
  Freq       equ 11000

.data

   StartIt   db  13,10,'PlayWav   A utility for Playing WAV files    Version  1.02b'
             db  13,10,'Forever Young Software(r)   (C)opyright 1984-1998',13,10,36
   File1     db  'prop.wav',0
   FileErr   db  13,10,'Error opening file...',36
   ResetOKS  db  13,10,'Reset DSP was successful.',36
   ResetErrS db  13,10,'Error Reseting DSP...',36
   AnyKey    db  13,10,'Press a key to exit.',36

   Length1   dw  00h
   MemLoc    dd  0000h
   Page1     db  00h

.data?
   Buffer    db  51200 dup(?)      ; 50k buffer
   DataA     equ $-Buffer

.code
start:     mov  ax,DGROUP               ; .STARTUP  (6.1)
           mov  ds,ax                   ;
           mov  bx,ss                   ;
           sub  bx,ax                   ;
           shl  bx,04                   ;
           mov  ss,ax                   ;
           add  sp,bx                   ;
           mov  bx,ds                   ; Free unused blocks
           mov  ax,es                   ;   for .EXE file format
           sub  bx,ax                   ;
           mov  ax,sp                   ;
           shr  ax,04                   ;
           add  bx,ax                   ;
           add  bx,(DataA shr 4) + 1    ; size of .DATA? (in para's)
           mov  ah,4Ah                  ;
           int  21h                     ;
           push ds                      ;
           pop  es                      ;

           mov  dx,offset StartIt
           mov  ah,09
           int  21h

           mov  di,offset Buffer        ; clear buffer contents
           mov  cx,DataA                ;
           xor  al,al                   ;
           rep  stosb                   ;

           mov  ax,3D00h                ; open file
           mov  dx,offset File1         ;
           int  21h                     ;
           jnc  short File1OK           ; if no error then cont.
           mov  dx,offset FileErr       ;   else print error msg and exit
           mov  ah,09                   ;
           int  21h                     ;
           jmp  short Done              ;
File1OK:   mov  bx,ax                   ; put handle in bx
           mov  cx,43                   ; read it
           mov  dx,offset Buffer        ; save into buffer
           mov  ah,3Fh                  ; 
           int  21h                     ;
           mov  cx,51200                ; read it
           mov  dx,offset Buffer        ; save into buffer
           mov  ah,3Fh                  ; 
           int  21h                     ;
           mov  Length1,ax              ; save len of file
           mov  ah,3Eh                  ; close the file
           int  21h                     ;

           call ResetDSP
           or   ax,ax
           jz   short RstOK
           mov  dx,offset ResetErrS
           jmp  short RstOK1
RstOK:     mov  dx,offset ResetOKS
RstOK1:    mov  ah,09
           int  21h

           mov  al,0DDh                 ; Left = 13, Right = 13 (15-highest)
           call MstrVol                 ; L = Hi Nibble, R = Lo Nibble

           mov  al,0D1h                 ; turn speaker(s) on
           call WriteDSP

           xor  eax,eax
           xor  edx,edx
           mov  ax,seg Buffer
           mov  dx,offset Buffer
           shl  eax,04h
           add  eax,edx
           mov  MemLoc,eax
           call DMAPlay

           mov  dx,offset AnyKey
           mov  ah,09
           int  21h
           xor  ah,ah
           int  16h

Done:      mov  ah,4Ch                  ; exit to DOS
           int  21h                     ; 


DMAPlay    proc near uses eax ebx edx

           dec  Length1
           mov  Page1,00h

           mov  al,Channel + 4
           mov  dx,0Ah
           out  dx,al
           xor  al,al
           mov  dx,0Ch
           out  dx,al
           mov  al,ModeReg
           mov  dx,0Bh
           out  dx,al
           mov  eax,MemLoc
           mov  dx,AddPort
           out  dx,al
           xchg al,ah
           out  dx,al
           mov  eax,MemLoc
           mov  edx,eax
           and  eax,65536
           jz   short MemLocN1
           inc  Page1
MemLocN1:  mov  eax,edx
           and  eax,131072
           jz   short MemLocN2
           inc  Page1
           inc  Page1
MemLocN2:  mov  eax,edx
           and  eax,262144
           jz   short MemLocN3
           add  Page1,04
MemLocN3:  mov  eax,edx
           and  eax,524288
           jz   short MemLocN4
           add  Page1,08
MemLocN4:  mov  dx,PgPort
           mov  al,Page1
           out  dx,al
           mov  dx,LenPort
           mov  ax,Length1
           out  dx,al
           xchg al,ah
           out  dx,al
           mov  dx,0Ah
           mov  al,Channel
           out  dx,al

           mov  al,40h
           call WriteDSP
           xor  edx,edx
           mov  eax,1000000
           mov  ebx,Freq
           div  ebx
           mov  ebx,eax
           mov  eax,256
           sub  eax,ebx
           call WriteDSP
           mov  al,14h
           call WriteDSP
           mov  ax,Length1
           call WriteDSP
           xchg al,ah
           call WriteDSP

           ret
DMAPlay    endp

MstrVol    proc near uses ax dx
           push ax
           mov  dx,BasePort+04
           mov  al,22h
           out  dx,al
           pop  ax
           inc  dx
           out  dx,al
           ret
MstrVol    endp

ResetDSP   proc near uses cx dx

           mov  dx,BasePort+06
           mov  al,01
           out  dx,al
           mov  cx,50
WaitIt1:   in   al,dx
           loop WaitIt1
           xor  al,al
           out  dx,al
           mov  cx,50
WaitIt2:   in   al,dx
           loop WaitIt2
           mov  ah,0FFh                 ; part of Return Code
           mov  dx,BasePort+06
           in   al,dx
           and  al,80h
           cmp  al,80h
           jne  short ResetErr
           mov  dx,BasePort+10
           in   al,dx
           cmp  al,0AAh
           jne  short ResetErr
ResetOK:   xor  ax,ax                   ; return ax = 0 if reset ok
ResetErr:  ret
ResetDSP   endp

WriteDSP   proc near uses ax dx
           push ax
           mov  dx,BasePort+12
WaitIt:    in   al,dx
           and  al,80h
           jnz  short WaitIt
           pop  ax
           out  dx,al
           ret
WriteDSP   endp
           end  start
