;     title:  EXEHDR   An EXE Header viewer
;   version:  01.00b
;    author:  Forever Young Software(r)
;             Benjamin David Lunt
;      date:  10/05/97
; assembler:  MASM 5.10

.MODEL small, basic
.386
.stack
.data

   StartIt   db  13,10,'EXE header viewer           Version   01.00b'
             db  13,10,'Forever Young Software(r)   (C)opyright 1997',13,10,0
   Usage     db  13,10,' Usage:'
             db  13,10,'  EXEHDR  source.exe'
   CRLF      db  13,10,0

   HdrID     dw  00h     ; ID signature ('MZ') ******** EXE header *********
   excess    dw  00h     ; Image size mod 512 (bytes on last page)
   pages     dw  00h     ; # 512-byte pages in image
   reloct    dw  00h     ; count of relocateion table entries
   hdrsize   dw  00h     ; size of header, in paragraphs
   minmem    dw  00h     ; min required mem
   maxmem    dw  00h     ; max required mem
   stacks    dw  00h     ; stack seg offset in load module
   stackp    dw  00h     ; initial value of sp
   cksum     dw  00h     ; file checksum
   ipointer  dw  00h     ; initial value of IP
   codes     dw  00h     ; cs offset in load module
   relostart dw  00h     ; offset of first reloc item
   ovlnum    dw  00h     ; overlay number

   InfoStr   db  13,10,'    File Information          (in Hex)',0
   InfoHdrSz db  13,10,' Header size:                      ',0
   InfoPISz  db  13,10,' Code size:                    ',0
   InfoMLSz  db  13,10,' Min Load size:                ',0
   InfoMnAll db  13,10,' Min allocation (para):            ',0
   InfoMxAll db  13,10,' Max allocation (para):            ',0
   InfoICSIP db  13,10,' Initial CS:IP:               ',0
   InfoISSSP db  13,10,' Initial SS:SP:               ',0
   InfoRlCnt db  13,10,' Relocation count:                 ',0
   InfoRlStr db  13,10,' Relo table start:                 ',0
   InfoECSum db  13,10,' EXE file checksum:                ',0
   InfoOvlN  db  13,10,' Overlay number:                   ',0

   F1OErr    db  13,10,'Error opening exe file',0
   EBadRead  db  13,10,'Error reading exe file',0
   EBadSig   db  13,10,'Invalid EXE file signature',0

   Hex       db  '0123456789ABCDEF'
   
   File1     db  32 dup(0),0
   Handle1   dw  00h
   CodeStart dd  0000h
   CodeSize  dd  0000h


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
           mov  ah,4Ah                  ;
           int  21h                     ;

           mov  si,offset StartIt       ; print startup string
           call prtstring               ;

           mov  ah,62h                  ; get PSP segment
           int  21h                     ;
           mov  es,bx                   ;
           mov  cl,es:[80h]             ; get Command Line Length
           or   cl,cl                   ; if 0 then error (usage)
           jz   short File1Err          ;
           xor  ch,ch                   ;
           mov  si,82h                  ; start with next line (skip space)
           dec  cx                      ;
           mov  di,offset File1         ; put in FILE1
           push ds                      ;
           pop  es                      ;
           push ds                      ;
           mov  ds,bx                   ;
           rep  movsb                   ;
           xor  al,al                   ; make sure asciiz
           stosb                        ;
           pop  ds                      ;
           jmp  short File1Ok           ;
File1Err:  mov  si,offset Usage         ; print usage string and exit
           call prtstring               ;
           xor  al,al                   ; ERRORLEVEL = 1  --> no source file
           inc  al                      ; .
           jmp  short Done              ;

File1Ok:   push ds                      ; make sure es = ds
           pop  es                      ;
           mov  ax,3D00h                ; open file1
           mov  dx,offset File1         ;
           int  21h                     ;
           jnc  short F1Opend           ; if no error
           mov  si,offset F1OErr
           call prtstring
           mov  al,02h                  ; ERRORLEVEL = 2  --> error opening File1
           jmp  short Done
F1Opend:   mov  Handle1,ax
           call ReadHdr
           or   al,al                   ; was there an error ?
           jnz  short Done
           call DispInfo
           mov  ah,3Eh                  ; close source file
           mov  bx,Handle1
           int  21h
           xor  al,al                   ; ERRORLEVEL = 0  --> no error
Done:      mov  ah,4Ch                  ; exit to DOS
           int  21h                     ;


ReadHdr    proc near uses bx cx edx si

           mov  ah,3Fh                  ; read in the header
           mov  dx,offset HdrID         ;
           mov  bx,Handle1              ;
           mov  cx,28                   ;
           int  21h                     ;
           jnc  short NoReadErr         ;
           mov  si,offset EBadRead      ; if error then print error and exit
           call prtstring               ;
           mov  al,04h                  ; ERRORLEVEL = 4  --> read error
           jmp  ReadHdrD
NoReadErr: mov  si,offset HdrID
           lodsw
           cmp  ax,5A4Dh                ; MZ in 'little endian' format
           je   short IDOK
           mov  si,offset EBadSig       ; if error then print error and exit
           call prtstring               ;
           mov  al,05h                  ; ERRORLEVEL = 4  --> bad ID
           jmp  ReadHdrD
IDOK:      xor  eax,eax
           mov  ax,hdrsize
           shl  eax,04h
           mov  CodeStart,eax
           push eax                     ; save codestart for later
           xor  eax,eax
           mov  ax,pages
           dec  ax
           mov  ebx,512
           mul  ebx
           xor  edx,edx
           mov  dx,excess               ; if bytes on last page is 0
           or   dx,dx                   ; then add another 512 byte page
           jnz  short AddExc            ; else
           mov  dx,512                  ;   add excess
AddExc:    add  eax,edx                 ;
           pop  edx                     ; restore codestart in edx
           sub  eax,edx
           mov  CodeSize,eax
           xor  al,al
ReadHdrD:  ret
ReadHdr    endp

DispInfo   proc near uses eax dx si

           mov  si,offset InfoStr
           call prtstring
           mov  si,offset InfoHdrSz
           call prtstring
           mov  ax,hdrsize
           call prthex
           mov  si,offset InfoPISz
           call prtstring
           mov  eax,codesize
           push eax                     ; print a long hex
           shr  eax,16                  ;
           call prthex                  ;
           pop  eax                     ;
           call prthex                  ;
           mov  si,offset InfoMLSz
           call prtstring
           xor  eax,eax
           mov  ax,minmem
           shl  eax,04
           add  eax,codesize
           push eax                     ; print a long hex
           shr  eax,16                  ;
           call prthex                  ;
           pop  eax                     ;
           call prthex                  ;
           mov  si,offset InfoMnAll
           call prtstring
           mov  ax,minmem
           call prthex
           mov  si,offset InfoMxAll
           call prtstring
           mov  ax,maxmem
           call prthex
           mov  si,offset InfoICSIP
           call prtstring
           mov  ax,codes
           call prthex
           mov  ah,02
           mov  dl,58
           int  21h
           mov  ax,ipointer
           call prthex
           mov  si,offset InfoISSSP
           call prtstring
           mov  ax,stacks
           call prthex
           mov  ah,02
           mov  dl,58
           int  21h
           mov  ax,stackp
           call prthex
           mov  si,offset InfoRlCnt
           call prtstring
           mov  ax,reloct
           call prthex
           mov  si,offset InfoRlStr
           call prtstring
           mov  ax,relostart
           call prthex
           mov  si,offset InfoECSum
           call prtstring
           mov  ax,cksum
           call prthex
           mov  si,offset InfoOvlN
           call prtstring
           mov  ax,ovlnum
           call prthex
           mov  si,offset CRLF
           call prtstring
           ret
DispInfo   endp

prtstring  proc near uses ax dx si
ps1:       mov  dl,[si]                 ; Get character
           inc  si                      ; Point to next one
           or   dl,dl                   ; End of string?
           jz   short ps2               ; Yes, so exit
           mov  ah,02h                  ; Output a character
           int  21h
           jmp  short ps1               ; Keep doing it
ps2:       ret
prtstring  endp

PrtHex     proc near uses ax bx cx
           mov  bx,offset Hex
           mov  cx,04h
HexLoop:   push ax
           mov  al,ah
           shr  al,04h
           xlatb
           mov  dl,al
           mov  ah,02
           int  21h
           pop  ax
           shl  ax,04h
           loop HexLoop
           ret
PrtHex     endp
           end  start
