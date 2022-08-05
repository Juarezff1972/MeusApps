; This is donated to public domain.  You may do with it what you want
;  as long as you don't make any money doing so.
;  I take no blame in the errors (if any) that this might do to your machine.
;  I hope this will help you learn more about MCB's
;  Thanks to Paul L. Olmstead and PCM magazine for helping me with this info.
;  I also published in PCM magazine  (Vol. XI #11  June/July 1994  Pgs. 24-5)
;  but after that issue the magazine no longer is in circulation.  (I hope
;  it wasn't something I did.  Ha Ha Ha!)
;  -PCM magazine is from TANDY (Radio Shack)-  All rights reserved.
;
;  Forever Young Software
;  19 Feb 1998 (revised)
;  MASM 5.10
;

.MODEL small, basic
.386
.stack
.data

     Hdr       db  13,10,'MCB    MCB       Block       Owner       Memory Block'
               db  13,10,'No.  Segment      Size      Segment      Usage'
               db  13,10,'--------------------------------------------------------------'
     CRLF      db  13,10,0
     Err       db  13,10,'There was an error',0
     ProgB     db  'Program:      ',0
     FreeB     db  'Free Block',0
     EnvrB     db  'Environment Block',0
     DataB     db  'Data Block',0
     TotlMS    db  '--------------------------------------------------------------',13,10
               db  '  Total Memory:   655,360',13,10,0
     FreeMS    db  '   Memory Free:  ',0
     UsedMS    db  '   Memory Used:  ',0
     HddnMS    db  ' Memory Hidden:  ',0
     TempLen   dw  00h
     Version   dw  00h
     MoreBlox  db  00h
     MNum      db  00h
     UsedM     dd  0000h
     FreeM     dd  0000h
     SysD      db  'SYSTEM DOS Ver ',0
     SysS      db  'SYSTEM',0
     PBuff     db  32 dup(0)

.code
start:     mov  ax,DGROUP               ; .STARTUP  (6.1)
           mov  ds,ax                   ;
           mov  bx,ss                   ;
           sub  bx,ax                   ;
           shl  bx,04                   ;
           mov  ss,ax                   ;
           add  sp,bx                   ;
           mov  bx,ds                   ; free unused part of Mem Block
           mov  ax,es                   ;
           sub  bx,ax                   ;
           mov  ax,sp                   ;
           shr  ax,04                   ;
           add  bx,ax                   ;
           mov  ah,4Ah                  ;
           int  21h                     ;
           push ds                      ; make sure es = ds
           pop  es

           mov  si,offset Hdr           ; print page header
           call prtstring               ;

           mov  ax,3000h                ; get MS-DOS version
           int  21h                     
           mov  Version,ax              ; ah = minor, al = major

           mov  MNum,00h                ; current block number
           mov  ax,5200h                ; find 1st MCB
           int  21h                     ; returns it in es:[bx-2]
           mov  ax,es:[bx-2]            ;
           mov  es,ax                   ; es now = 1st MCB segment
LoopIt:    xor  ah,ah                   ; print MCB Number
           mov  al,MNum                 ;
           cmp  al,09                   ; if num < 9 then print a space
           ja   short PrntMNum          ;  to keep aligned
           push ax                      ;
           mov  ah,02                   ;
           mov  dl,20h                  ;
           int  21h                     ;
           pop  ax                      ;
PrntMNum:  call prtdec                  ; print MCB number
           inc  MNum                    ; inc MCB number for next one

           mov  cx,04h                  ; print spaces
           call prtspcs                 ;

           mov  ax,es                   ; print MCB Segment address
           call prthex                  ;

           xor  eax,eax
           mov  ax,es:[0003h]           ; length in paragraphs
           mov  TempLen,ax              ; save for calculation to next
           shl  eax,04h                 ;
           call PrtDecLn                ; convert to ascii
           mov  cx,14                   ; skip spaces to keep aligned
           sub  cx,bx                   ;
           call prtspcs                 ;
           mov  si,offset PBuff         ; print LONG number
           call prtstring               ;

           mov  cx,05                   ; print spaces
           call prtspcs                 ;

           mov  ax,es:[0001h]           ; print Owner segment
           call prthex
           mov  cx,07                   ; print spaces
           call prtspcs                 ;

           cmp  MNum,01h                ; the first two MCB's are always the 
           jne  short NotFirst          ; the two SYSTEM MCB's
           mov  si,offset SysD          ;
           call prtstring               ;
           mov  ax,Version              ;
           xor  ah,ah                   ;
           call prtdec                  ;
           mov  dl,'.'                  ;
           mov  ah,02                   ;
           int  21h                     ;
           mov  ax,Version              ;
           shr  ax,08                   ;
           call prtdec                  ;
           mov  MoreBlox,01h            ;
           jmp  DoneChar                ;
NotFirst:  cmp  MNum,02h                ;
           jne  short NotSecond         ;
           mov  si,offset SysS          ;
           call prtstring               ;
           mov  MoreBlox,01h            ;
           jmp  DoneChar                ;

NotSecond: mov  bl,es:[0000h]           ; else 77 means that there are more
           cmp  bl,77                   ;
           jne  short Not77             ;
           mov  MoreBlox,01h            ;
           jmp  short IndexOK           ;
Not77:     cmp  bl,90                   ; 90 means last one
           je   short Is90              ;
           mov  ax,-1                   ;
           jmp  Error                   ;
Is90:      mov  MoreBlox,00h            ;
IndexOK:   or   ax,ax                   ; if Owner segment = 0 then is
           jne  short NFreeBl           ;   a free block
           mov  si,offset FreeB         ;
           call prtstring               ;
           xor  eax,eax                 ; keep track of Free Memory
           mov  ax,TempLen              ;
           shl  eax,04h                 ; mul by 16
           add  FreeM,eax               ;
           jmp  short DoneChar          ;
NFreeBl:   mov  bx,es                   ; if owner seg address
           inc  bx                      ;  is one more than this MCB's
           cmp  ax,bx                   ;  seg address, then is a program
           jne  short NotProg           ;
           mov  si,offset ProgB         ;
           call prtstring               ;
           xor  eax,eax                 ; keep track of used Memory
           mov  ax,TempLen              ;
           shl  eax,04h                 ;
           add  UsedM,eax               ;
           jmp  short GetN              ;
NotProg:   mov  bl,es:[0016]            ; if name = "COMSPEC" then envir.
           cmp  bl,'C'                  ;  block, else Data block
           jne  short IsData            ;
           mov  bl,es:[0022]            ; test for last C just to make sure
           cmp  bl,'C'                  ;
           jne  short IsData            ;
           mov  si,offset EnvrB         ;
           call prtstring               ;
           xor  eax,eax                 ; keep track of used Memory
           mov  ax,TempLen              ;
           shl  eax,04h                 ;
           add  UsedM,eax               ;
           jmp  short GetN              ;
IsData:    mov  si,offset DataB         ;
           call prtstring               ;
           xor  eax,eax                 ; keep track of used Memory
           mov  ax,TempLen              ;
           shl  eax,04h                 ;
           add  UsedM,eax               ;
           jmp  short DoneChar          ;

GetN:      mov  cx,8                    ; is program so get name
           mov  si,cx                   ;   is at most 8 chars long
GetIt:     mov  dl,es:[si]              ;
           or   dl,dl                   ;   not always a asciiz string
           jz   short DoneChar          ;
           mov  ah,02                   ;
           int  21h                     ;
           inc  si                      ;
           loop GetIt                   ;

DoneChar:  mov  si,offset CRLF          ;
           call prtstring               ;
           cmp  MoreBlox,00h            ; if more flag then go do more
           je   short DoneM             ;
           mov  ax,TempLen              ; Next MCB is at this MCB location
           inc  ax                      ;  plus the length of this MCB
           mov  dx,es                   ;
           add  dx,ax                   ;
           mov  es,dx                   ;
           jmp  LoopIt                  ;

DoneM:     mov  si,offset TotlMS        ; else print total mem:
           call prtstring               ;   not always 655,360  (640k)
           mov  si,offset UsedMS        ;   but suits our neads
           call prtstring               ;
           mov  eax,UsedM               ; print amount used
           call prtdecln                ;
           mov  cx,08                   ;
           sub  cx,bx                   ;
           call prtspcs                 ;
           mov  si,offset PBuff         ;
           call prtstring               ;
           mov  si,offset CRLF          ;
           call prtstring               ;
           mov  si,offset FreeMS        ; print free memory
           call prtstring               ;
           mov  eax,FreeM               ;
           call prtdecln                ;
           mov  cx,08                   ;
           sub  cx,bx                   ;
           call prtspcs                 ;
           mov  si,offset PBuff         ;
           call prtstring               ;
           mov  si,offset CRLF          ;
           call prtstring               ;
           mov  si,offset HddnMS        ; hidden memory is what we didn't
           call prtstring               ;  find.  Usally the device drivers
           mov  eax,655360              ;  are in this area
           sub  eax,UsedM               ;  Hidden = 655360 - used - free
           sub  eax,FreeM               ;
           jnc  short notneg            ; if (655360 - used - free) < 0 then
           xor  eax,eax                 ;  lets make eax = 0
notneg:    call prtdecln                ;
           mov  cx,08                   ;
           sub  cx,bx                   ;
           call prtspcs                 ;
           mov  si,offset PBuff         ;
           call prtstring               ;
           mov  si,offset CRLF          ;
           call prtstring               ;
           jmp  short Done              ;
Error:     mov  si,offset Err           ; if an error print this line
           call prtstring               ;
Done:      mov  ah,4Ch                  ; exit to DOS
           int  21h                     ; 

PrtHex     proc near uses ax cx dx      ; print a number in ascii hex format

           mov  cx,02h                  ; come back and do this once more
           mov  dx,ax
Loop1:     push dx                      ; save dl for second go 'round
           push dx
           shr  dh,4
           add  dh,48
           cmp  dh,58
           jb   short NotA
           add  dh,07                   ; make uc letter
NotA:      mov  dl,dh
           mov  ah,02
           int  21h
           pop  dx
           and  dh,00001111b
           add  dh,48
           cmp  dh,58
           jb   short NotA1
           add  dh,07                   ; make uc letter
NotA1:     mov  dl,dh
           mov  ah,02
           int  21h
           pop  dx
           mov  dh,dl
           loop Loop1
           ret
PrtHex     endp

Prtstring  proc near uses ax            ; print an asciiz string
Ps1:       mov  dl,[si]                 ; Get character
           inc  si                      ; Point to next one
           or   dl,dl                   ; End of string?
           jz   short ps2               ; Yes, so exit
           mov  ah,02h                  ; Output a character
           int  21h
           jmp  short Ps1               ; Keep doing it
Ps2:       ret
Prtstring  endp

PrtDec     proc uses ax cx dx           ; print a number in ascii dec format
           mov  cx,0FFFFh               ; Ending flag
           push cx
           mov  cx,10
PD1:       xor  dx,dx
           div  cx                      ; Divide by 10
           add  dl,30h                  ; Convert to ASCII
           push dx                      ; Store remainder
           or   ax,ax                   ; Are we done?
           jnz  short PD1               ; No, so continue
PD2:       pop  dx                      ; Character is now in DL
           cmp  dx,0FFFFh               ; Is it the ending flag?
           je   short PD3               ; Yes, so continue
           mov  ah,02h                  ; Output a character
           int  21h
           jmp  short PD2               ; Keep doing it
PD3:       ret
PrtDec     ENDP

PrtSpcs    proc near uses ax dx         ; print spaces (cx times)
PLoop:     mov  ah,02
           mov  dl,20h
           int  21h
           loop PLoop
           ret
PrtSpcs    endp

PrtDecLn   proc near uses eax ecx edx es 
                                        ; convert long integer to asciiz
           push ds
           pop  es
           mov  di,offset PBuff
           xor  bx,bx
           mov  cx,0FFFFh               ; Ending flag
           push cx
           or   eax,eax
           jnz  short Notzero
           mov  dx,0030h
           push dx
Notzero:   mov  ecx,10
PD1:       or   eax,eax
           jz   short PD2
           cmp  bh,03
           jb   short NoComma
           mov  dx,002Ch
           push dx
           xor  bh,bh
NoComma:   xor  edx,edx
           div  ecx                     ; Divide by 10
           add  dl,30h                  ; Convert to ASCII
           push dx                      ; Store remainder
           inc  bh
           jmp  short PD1               ; No, so continue
PD2:       pop  dx                      ; Character is now in DL
           cmp  dx,0FFFFh               ; Is it the ending flag?
           je   short PD3               ; Yes, so continue
           mov  al,dl
           stosb
           inc  bl
           jmp  short PD2               ; Keep doing it
PD3:       xor  al,al
           stosb
           xor  bh,bh
           ret
PrtDecLn   endp
           end  start
