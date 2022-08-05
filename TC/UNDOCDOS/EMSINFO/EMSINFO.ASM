;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This is an EMS memory tester                             ;;
;; Type EMSINFO at the prompt                               ;;
;;                                                          ;;
;;    Version:  1.00                                        ;;
;;     Author:  Ben Lunt                                    ;;
;;       Date:  04/14/97                                    ;;
;;  Assembler:  MASM 5.10                                   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Dosseg
.model small, basic
.386
.stack
.data

StartIt     db  13,10,' Forever Young(R)  EMS Info      Version 1.00'
            db  13,10,'   Copyright(C)  Forever Young Software  1990-1997    All rights reserved',13,10,10,0
InMsg       db      13,10,'The expanded memory manager is installed.',0
NoneMsg     db      13,10,'No expanded memory manager detected!',0
Version     db      13,10,'EMS version '
Major       db      00,'.'
Minor       db      00,'.',00h
PageMsg     db      13,10,'   The page frame address:  ',0
TotalMsg    db      13,10,'    Total expanded memory:  ',0
FreeMsg     db      13,10,'Expanded memory available:  ',0

EMSName     db      'EMMXXXX0',0
EMPhandle   dw      0000h
FillMsg     db      13,10,10,'-------Performing the test---------'
            db      13,10,'  Filling expanded memory pages',0
ReadMsg     db      13,10,'  Reading expanded memory pages',13,10,0
TestMsg     db      'This is a test message 36blk#:'
Count       db      '   1',13,0
EMsg1       db      13,10,'Could not allocate memory requested',0
EMsg2       db      13,10,'Error mapping memory',0
EMsg3       db      13,10,'Could not determine page frame',0
EMsg4       db      13,10,'Error releasing EMS handle',0

.code
start:     mov  ax,DGROUP               ; .STARTUP  (6.1)
           mov  ds,ax                   ;
           mov  bx,ss                   ;
           sub  bx,ax                   ;
           shl  bx,04                   ;
           mov  ss,ax                   ;
           add  sp,bx                   ;
           mov  bx,ds                   ; allow mem alloc.
           mov  ax,es                   ;
           sub  bx,ax                   ;
           mov  ax,sp                   ;
           shr  ax,04                   ;
           add  bx,ax                   ;
           add  bx,68                   ; size of .DATA? (in para's)
           mov  ah,4Ah                  ;
           int  21h                     ;

           mov  ah,01h                  ; turn off cursor
           mov  ch,20h                  ; bit number 5
           int  10h                     ;
           mov  si,offset StartIt       ; print start string
           call prtstring               ;
           call ChkEMS                  ; see if EMS is present
           jnc  short ItsThere
           mov  si,offset NoneMsg
           call prtstring
           jmp  AllDone

ItsThere:  mov  si,offset InMsg
           call prtstring

           mov  ah,46h                  ; get EMS version
           int  67h
           mov  ah,al
           and  ah,00001111b            ; minor version
           shr  al,04h                  ; major version
           or   ax,3030h                ; make ascii
           mov  Major,al
           mov  Minor,ah
           mov  si,offset Version
           call prtstring

           mov  si,offset PageMsg       ; page frame message
           call prtstring
           mov  ah,41h                  ; get page frame address
           int  67h
           mov  ax,bx                   ; This is the page frame base
           call prthex

           mov  si,offset TotalMsg      ; Total memory message
           call prtstring
           mov  ah,42h
           int  67h
           xor  eax,eax
           mov  ax,dx
           xor  edx,edx
           mov  ecx,4000h
           mul  ecx
           call prtdec

           mov  si,offset FreeMsg       ; available memory message
           call prtstring
           xor  eax,eax
           mov  ax,bx
           mov  ebx,4000h
           mul  ebx
           call prtdec

           mov  ah,43h                  ; allocate pages
           mov  bx,02h                  ; get 2 pages (32K)
           int  67h
           or   ah,ah                   ; was there an error?
           jne  short Error1            ;
           mov  EMPhandle,dx            ; no, save handle

           mov  ax,4400h                ; map memory - first EMS page
           xor  bx,bx                   ; want this part of my memory
           mov  dx,EMPhandle            ; assigned to this handle
           int  67h
           or   ah,ah                   ; was there an error?
           jnz  short Error2            ;
           mov  ax,4401h                ; map memory
           mov  bx,01h
           int  67h
           or   ah,ah                   ; was there an error?
           jnz  short Error2            ; yes, so handle

           mov  ah,41h                  ; get page frame address
           int  67h
           or   ah,ah                   ; was there an error?
           jnz  short Error3            ; yes, so handle
           mov  es,bx                   ; make page frame addressable

           mov  si,offset FillMsg
           call prtstring
           mov  cx,910                  ; 36-character message fits this
           cld                          ;   many times in 32K (32768 bytes)
           xor  di,di                   ; start of page buffer
InLoop:    push cx
           mov  si,offset TestMsg
           mov  cx,09                   ; length of message (36/4)
           rep  movsd
           call IncIt                   ; inc the counter in the 
           pop  cx                      ;   test message
           loop InLoop
           mov  al,0FFh                 ; put ending flag
           stosb                        ;

           mov  si,offset ReadMsg       ;
           call prtstring               ;
           xor  si,si                   ;
           push ds                      ; set up addressing
           push es                      ;
           pop  ds                      ;
OutLoop:   call prtstring               ; print back the test message
           cmp  byte ptr [si],0FFh      ;  that was stuffed in to
           jne  short OutLoop           ;   the memory
           pop  ds                      ;

           mov  ah,45h                  ; release EMS handle
           mov  dx,EMPhandle            ; this handle
           int  67h                     ;
           or   ah,ah                   ; was there an error
           je   short AllDone
           mov  si,offset EMsg4
           jmp  short PrntErr
Error1:    mov  si,offset EMsg1
           jmp  short PrntErr
Error2:    mov  si,offset EMsg2
           jmp  short PrntErr
Error3:    mov  si,offset EMsg3
PrntErr:   call prtstring
AllDone:   mov  ah,01h                  ; turn on cursor
           mov  cx,0607h                ; start = 6   finish = 7
           int  10h                     ;
           mov  ah,4Ch                  ; exit
           int  21h

ChkEMS     proc near uses ax bx dx es

           mov  dx,offset EMSName       ; device driver name
           mov  ax,3D00h                ; open device-access/file sharing mode
           int  21h
           jc   short NotThere2
           mov  bx,ax                   ; put handle in proper place
           mov  ax,4407h                ; IOCTL  -  get output status
           int  21h
           jc   short NotThere1
           cmp  al,0FFh
           jne  short NotThere1
           mov  ah,3Eh                  ; close device
           int  21h
           clc                          ; set for no error
           jnc  short Done
NotThere1: mov  ah,3Eh                  ; close device
           int  21h
NotThere2: stc
Done:      ret
ChkEMS     endp

IncIt      proc near uses si
           mov  si,offset Count
           add  si,03h
IncIt1:    inc  byte ptr [si]
           cmp  byte ptr [si],58        ; out of number range?
           jne  short IncIt2            ; no, so continue
           mov  byte ptr [si],48        ; reset to zero
           dec  si
           cmp  byte ptr [si],32        ; filled in yet?
           jne  short IncIt1
           mov  byte ptr [si],48        ; reset to zero
           jmp  short IncIt1
IncIt2:    ret
IncIt      endp

prtstring  proc near uses ax
Ps1:       mov  dl,[si]                 ; Get character
           inc  si                      ; Point to next one
           or   dl,dl                   ; End of string?
           jz   short ps2               ; Yes, so exit
           mov  ah,02h                  ; Output a character
           int  21h
           jmp  short Ps1               ; Keep doing it
Ps2:       ret
prtstring  endp

prtdec     proc near uses eax ecx edx bx di

           xor  bx,bx                   ; number of chars to print
           mov  cx,0FFFFh               ; Ending flag
           push cx
           mov  ecx,10
PD1:       xor  edx,edx
           div  ecx                     ; Divide by 10
           add  dl,30h                  ; Convert to ASCII
           push dx                      ; Store remainder
           or   eax,eax                 ; Are we done?
           jnz  short PD1               ; No, so continue
PD2:       pop  dx                      ; Character is now in DL
           cmp  dx,0FFFFh               ; Is it the ending flag?
           je   short PD3               ; Yes, so continue
           mov  ah,02
           int  21h
           inc  bx
           jmp  short PD2               ; Keep doing it
PD3:       ret
prtdec     endp

prthex     proc near uses ax dx
           mov  dx,ax
           push dx
           push dx
           shr  dh,04
           add  dh,48
           cmp  dh,58
           jb   short NotA
           add  dh,07                   ; skip 'garbage'
NotA:      mov  dl,dh
           mov  ah,02
           int  21h
           pop  dx
           shl  dh,04
           shr  dh,04
           add  dh,48
           cmp  dh,58
           jb   short NotA1
           add  dh,07                   ; skip 'garbage'
NotA1:     mov  dl,dh
           mov  ah,02
           int  21h
           pop  dx

           push dx
           shr  dl,04
           add  dl,48
           cmp  dl,58
           jb   short NotA2
           add  dl,07                   ; skip 'garbage'
NotA2:     mov  ah,02
           int  21h
           pop  dx
           shl  dl,04
           shr  dl,04
           add  dl,48
           cmp  dl,58
           jb   short NotA3
           add  dl,07                   ; skip 'garbage'
NotA3:     mov  ah,02
           int  21h
           mov  dl,'h'
           mov  ah,02
           int  21h
           ret
prthex     endp
           end

