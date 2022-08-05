;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This is a command-line cd player for DOS                 ;;
;; Type CDP at the prompt and then follow the instructions  ;;
;;                                                          ;;
;;  Note:  CDP assumes that drive D: is your cd drive       ;;
;;          if not change Device below (3=D:,4=E:,etc.)     ;;
;;  Also:  CDP assumes that MSCDEX or a compatible driver   ;;
;;          is already installed and activated.             ;;
;;                                                          ;;
;;    Version:  1.031                                       ;;
;;     Author:  Ben Lunt                                    ;;
;;       Date:  06/02/97                                    ;;
;;  Assembler:  MASM 5.10                                   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.MODEL small, basic
.386
.STACK 100h
.DATA

    StartIt    db  13,10,' Forever Young(R)  CD Player     Version 1.031'
               db  13,10,'   Copyright(C)  Forever Young Software  1990-1997    All rights reserved',13,10,10,0
    DeviceErr  db  13,10,' Device error.  ',13,10,0
    Devicebusy db  13,10,' Device busy.  '
               db  13,10,'  Press X to stop.  Any other exits.',0
    DeviceSize db  13,10,'       Total disk time:  ',0
    TotalTrx   db  13,10,'          Total tracks:  ',0
    DfltStr    db  13,10,' Audio channel default:  Normal Stereo',0
    RetdErr    db  13,10,' Returned error number:  ',0
    BusyO      db  13,10,' IOCTL_OUT returned busy.  ',0
    RetSErr    db  13,10,' SEEK returned error #:  ',0
    BusyS      db  13,10,' SEEK returned busy.  ',0
    RetPErr    db  13,10,' PLAY returned error #:  ',0
    BusyP      db  13,10,' PLAY returned busy.  ',0
    TrackNum   db  'Track: ',0
    CurTime    db  '  Time: ',0
    UserKeys   db  13,10,10
               db  13,10,'  Press <P> to Play.'
               db  13,10,'  Press <S> for Stop.'
               db  13,10,'  Press <n> for Next track.'
               db  13,10,'  Press <p> for Previos track.'
               db  13,10,'  Press <X> to Exit (without stoping).'
               db  13,10,'  Press <ESC> to Stop and Exit.'
               db  13,10,'  Press <E> to Eject Disk.'
               db  13,10,'  Press <C> to Close the tray.'
               db  13,10,10,0

    device     dw  00h
    i          dw  00h
    HSldout    dd  0000h
    busy       dw  00h
    min        dw  00h
    sec        dw  00h
    secrem     dd  0000h
    Strtsect   dd  0000h
    Nsect      dd  0000h
    CurTrk     db  00h
    LastTrk    db  00h
    FirstTrk   db  01h
    LastChar   db  'S'

    Reset      db  2                    ; cc
    Eject      db  0                    ; cc
    CloseIt    db  5                    ; cc
    StopPlayReq db 13,0,133             ; rhlen,subu,comc
               dw  00h                  ; status
               db  8 dup(0)             ; reserved
    ioctli     db  0,0,0                ; rhlen,subu,comc
               dw  00h                  ; status
               db  8 dup(0)             ; reserved
               db  00h                  ; mdb
               dd  0000h                ; *trandad
               dw  00h,00h              ; tranct,ssn
               dd  0000h                ; *volid
    ioctlo     db  0,0,0                ; rhlen,subu,comc
               dw  00h                  ; status
               db  8 dup(0)             ; reserved
               db  00h                  ; mdb
               dd  0000h                ; *trandad
               dw  00h,00h              ; tranct,ssn
               dd  0000h                ; *volid
    Qi         db  12                   ; cc
               db  00h                  ; ctladr
               db  00h                  ; tno
               db  00h                  ; point
               db  00h                  ; min
               db  00h                  ; sec
               db  00h                  ; frame
               db  00h                  ; zero
               db  00h                  ; amin
               db  00h                  ; asec
               db  00h                  ; aframe
    vs         db  8                    ; cc
               dd  0000h                ; volsize
    ddi        db  10,0,0,4 dup(0)      ; cc,lotrak,hitrak,ldouttrk[4]
    ati        db  11,0                 ; cc,trakno
               db  4 dup(0)             ; trakstrt[4]
               db  0                    ; tctl
    ai         db  3                    ; cc
               db  4 dup(2 dup(0))      ; z[4](icvc)
    SeekReq    db  0,0,0                ; rhlen,subu,comc
               dw  00h                  ; status
               db  8 dup(0)             ; reserved
               db  00h                  ; am
               dd  0000h                ; ta
               dw  00h                  ; str
               dd  0000h                ; ssn
    PlayReq    db  0,0,0                ; rhlen,subu,comc
               dw  00h                  ; status
               db  8 dup(0)             ; reserved
               db  00h                  ; am
               dd  0000h,0000h          ; Strsect,Nsect

.CODE
start:     mov  ax,DGROUP               ; .STARTUP  (6.1)
           mov  ds,ax                   ;
           mov  bx,ss                   ;
           sub  bx,ax                   ;
           shl  bx,04h                  ;
           mov  ss,ax                   ;
           add  sp,bx                   ;
           push ds                      ; make sure es=ds
           pop  es                      ;
           mov  si,offset StartIt       ; print startup string
           call Prtstring               ;

           mov  device,03h              ; 3 = D:, 4 = E:, etc.
           call cdrck                   ; check device
           or   ax,ax                   ; if 0 then device error
           jnz  short DevOK             ;
           mov  si,offset DeviceErr     ; print device error string
           call Prtstring               ;
           jmp  Done                    ;

DevOK:     mov  bx,offset Qi            ; see if device busy
           mov  dx,11                   ;
           call Ioctl_in                ; return status in ax
           and  ax,0200h                ;
           jz   short NotBusy           ;
           mov  si,offset DeviceBusy    ; print busy string
           call Prtstring               ;
           xor  ah,ah                   ; get a keystroke (wait for it)
           int  16h                     ;
           cmp  al,'X'                  ; if X then
           je   short StopIt            ;    reset device and continue
           jmp  Done                    ; else 'we are done'
StopIt:    mov  bx,offset StopPlayReq   ; stop playing
           call cdrreq                  ;

NotBusy:   mov  bx,offset vs            ; get volume size
           mov  dx,05h                  ;
           call ioctl_in_ck             ;
           mov  si,offset vs            ; min = vs.volsize/4500
           mov  eax,[si+1]              ;
           push eax                     ;
           xor  edx,edx                 ;
           mov  ebx,4500                ;
           div  ebx                     ;
           mov  min,ax                  ;
           xor  edx,edx                 ;
           and  eax,0FFFFh              ;
           mul  ebx                     ;
           mov  edx,eax                 ;
           pop  eax                     ;
           sub  eax,edx                 ; sec = (vs.volsize-(min*4500))/75
           xor  edx,edx                 ;
           mov  ebx,75                  ;
           div  ebx                     ;
           mov  sec,ax                  ;

           mov  si,offset DeviceSize    ; print volume size string
           call prtstring               ;   and parameters
           mov  ax,min                  ;
           call prtdec                  ;
           mov  ah,02                   ;
           mov  dl,':'                  ;
           int  21h                     ;
           mov  ax,sec                  ;
           call prtdec                  ;

           mov  bx,offset ddi           ; setup for getting tracks info
           mov  dx,05h                  ;
           call ioctl_in_ck             ;
           mov  si,offset ddi           ;
           add  si,03h                  ;
           call HSSect                  ;
           mov  HSldout,eax             ;

           mov  si,offset TotalTrx      ; get total tracks
           call prtstring               ;
           mov  si,offset ddi           ;
           xor  ah,ah                   ;
           mov  al,[si+2]               ;
           call prtdec                  ;
           mov  LastTrk,al              ; last track #
           mov  al,[si+1]               ;
           mov  FirstTrk,al             ; first track #

           mov  si,offset ddi           ; get lotrk #
           xor  ah,ah                   ;
           mov  al,[si+1]               ;
           mov  si,offset ati           ; and put it in trk #
           mov  [si+1],al               ;

           mov  bx,offset ati           ;
           mov  dx,07h                  ;
           call ioctl_in_ck             ;
           mov  si,offset ati           ;
           inc  si                      ;
           inc  si                      ;
           call HSSect                  ;
           mov  Strtsect,eax            ;
           mov  edx,HSldout             ; Nsect = HSldout-Strtsect
           sub  edx,eax                 ;
           mov  Nsect,edx               ;

           mov  al,255                  ; make normal stereo
           mov  si,offset ai            ;
           mov  [si+2],al               ;
           mov  [si+4],al               ;
           xor  al,al                   ;
           mov  [si+1],al               ;
           inc  al                      ;
           mov  [si+3],al               ;
           mov  si,offset DfltStr       ;
           call prtstring               ;
           mov  bx,offset AI            ;
           mov  dx,09h                  ;
           call ioctl_out               ;

           mov  edx,Strtsect
           call seek

           mov  bx,offset Reset         ; reset the drive
           mov  dx,01h
           call ioctl_out
           xor  al,al                   ; reset time vars
           mov  bx,offset Qi
           mov  [bx+4],al
           mov  [bx+5],al
           mov  [bx+6],al

PrtUKeys:  call TurnCOff                ; turn cursor off
           mov  si,offset UserKeys      ; print user keys
           call prtstring               ;

GetChar:   mov  ah,01h
           int  16h
           jz   NoChar
           xor  ah,ah
           int  16h
           cmp  al,27
           je   IsESC
           cmp  al,'X'
           je   Done
           cmp  al,'C'
           je   IsC
           cmp  al,'p'
           je   short IsPr
           cmp  al,'n'
           je   short IsN
           cmp  al,'S'
           je   short IsS
           cmp  al,'E'
           je   IsE
           cmp  al,'P'
           jne  NoChar

           mov  edx,Strtsect            ; Play track
           mov  ebx,Nsect               ;
           call Play                    ;
           mov  LastChar,20h            ;
           jmp  NoChar                  ;

IsS:       mov  bx,offset StopPlayReq   ; stop playing
           call cdrreq                  ;
           mov  al,FirstTrk
           call Jumpnext                ; jump to track
           xor  al,al                   ; reset time vars
           mov  bx,offset Qi            ;
           mov  [bx+4],al               ;
           mov  [bx+5],al               ;
           mov  [bx+6],al               ;
           mov  LastChar,'S'            ;
           jmp  short NoChar            ;

IsN:       mov  al,CurTrk               ; goto next track
           inc  al                      ;
           cmp  al,LastTrk              ;
           jbe  short DoJump            ;
           mov  al,FirstTrk             ;
DoJump:    call Jumpnext                ; jump to track
           mov  bx,offset StopPlayReq   ; stop playing
           call cdrreq                  ;
           mov  edx,Strtsect            ; Play
           mov  ebx,Nsect               ;
           call Play                    ;
           mov  LastChar,20h            ;
           jmp  short NoChar            ;

IsPr:      mov  al,CurTrk               ; Previous
           dec  al                      ;
           cmp  al,FirstTrk             ;
           jae  short DoJump1           ;
           mov  al,LastTrk              ;
DoJump1:   call Jumpnext                ; jump to track
           mov  bx,offset StopPlayReq   ; stop playing
           call cdrreq                  ;
           mov  edx,Strtsect            ; Play
           mov  ebx,Nsect               ;
           call Play                    ;
           mov  LastChar,20h            ;
           jmp  short NoChar            ;

IsESC:     mov  bx,offset StopPlayReq   ; stop playing
           call cdrreq                  ;
           jmp  Done                    ; and exit to DOS

IsE:       mov  bx,offset StopPlayReq   ; stop playing and eject
           call cdrreq                  ;
           mov  bx,offset Eject         ;
           mov  dx,01h                  ;
           call ioctl_out               ;
           jmp  short NoChar            ;

IsC:       mov  bx,offset CloseIt       ; Close the tray
           mov  dx,01h                  ;
           call ioctl_out               ;

NoChar:    cmp  LastChar,'S'            ;
           je   short WasS              ;
           mov  bx,offset Qi            ;
           mov  dx,11                   ;
           call Ioctl_in                ; return status in ax
WasS:      mov  si,offset TrackNum      ;
           call prtstring               ;
           xor  ah,ah                   ;
           mov  dl,[bx+2]               ;
           mov  al,dl                   ;
           and  dl,0Fh                  ;
           and  al,0F0h                 ;
           mov  cl,04h                  ;
           shr  al,cl                   ;
           mov  cl,10                   ;
           mul  cl                      ;
           add  dl,al                   ;
           mov  al,dl                   ;
           xor  ah,ah                   ;
           call prtdec                  ;
           mov  CurTrk,al               ;
           mov  si,offset CurTime       ;
           call prtstring               ;
           mov  al,[bx+4]               ;
           cmp  al,10                   ;
           jae  short Prt1              ;
           push ax                      ;
           mov  ah,02h                  ;
           mov  dl,48                   ;
           int  21h                     ;
           pop  ax                      ;
Prt1:      call prtdec                  ;
           mov  ah,02                   ;
           mov  dl,':'                  ;
           int  21h                     ;
           xor  ah,ah                   ;
           mov  al,[bx+5]               ;
           cmp  al,10                   ;
           jae  short Prt2              ;
           push ax                      ;
           mov  ah,02h                  ;
           mov  dl,48                   ;
           int  21h                     ;
           pop  ax                      ;
Prt2:      call prtdec                  ;
           mov  ah,02                   ;
           mov  dl,':'                  ;
           int  21h                     ;
           xor  ah,ah                   ;
           mov  al,[bx+6]               ;
           cmp  al,10                   ;
           jae  short Prt3              ;
           push ax                      ;
           mov  ah,02h                  ;
           mov  dl,48                   ;
           int  21h                     ;
           pop  ax                      ;
Prt3:      call prtdec                  ;
           mov  ah,02                   ; clean up end of line printed
           mov  dl,20h                  ;
           int  21h                     ;
           mov  ah,02                   ; clean up end of line printed
           mov  dl,20h                  ;
           int  21h                     ;
           mov  ah,02                   ;
           mov  dl,13                   ;
           int  21h                     ;
           jmp  GetChar                 ;

Done:      call TurnCOn                 ; turn cursor on
           mov  ah,4Ch                  ; Exit program, close files
           int  21h                     ; and return to DOS


Ioctl_in   proc near uses si bx dx

           mov  si,offset ioctli        ;
           xor  eax,eax                 ;
           mov  [si+13],al              ; ioc.mdb = 0
           mov  [si+20],al              ; ioc.ssn = 0
           mov  [si+22],eax             ; volid=NULL
           mov  al,13
           mov  [si],al                 ; rhlen
           mov  al,03
           mov  [si+2],al               ; comc
           xor  ax,ax
           mov  [si+3],ax               ; status
           mov  [si+14],bx              ; trandad
           mov  ax,ds                   ;
           mov  [si+16],ax              ; data seg
           mov  [si+18],dx              ; tranct
           mov  bx,offset ioctli
           call cdrreq
           mov  ax,[si+3]               ; status
           ret
Ioctl_in   endp

ioctl_in_ck proc near uses ax bx dx

           call Ioctl_in                ; return status in ax
           push ax
           and  ax,8000h
           je   short NoErr
           mov  si,offset DeviceErr
           call prtstring
NoErr:     pop  ax
           and  ax,20h
           je   short NotbusyI
           mov  si,offset DeviceBusy
           call prtstring
NotbusyI:  ret
ioctl_in_ck endp

ioctl_out  proc near

           mov  si,offset ioctlo        ;
           xor  eax,eax                 ;
           mov  [si+13],al              ; ioc.mdb = 0
           mov  [si+20],al              ; ioc.ssn = 0
           mov  [si+22],eax             ; volid=NULL
           mov  al,13
           mov  [si],al                 ; rhlen
           mov  al,12
           mov  [si+2],al               ; comc
           xor  ax,ax
           mov  [si+3],ax               ; status
           mov  [si+14],bx              ; trandad
           mov  ax,ds                   ;
           mov  [si+16],ax              ; data seg
           mov  [si+18],dx              ; tranct
           mov  bx,offset ioctlo
           call cdrreq
           mov  dx,[si+3]               ; status
           mov  ax,dx
           and  ax,8000h
           jz   short NoRetErr
           mov  si,offset RetdErr
           call prtstring
           mov  ax,dx
           and  ax,0FFh
           call prtdec
NoRetErr:  mov  ax,dx
           and  ax,20h
           jz   short NotBuzyo
           mov  si,offset BusyO
           call prtstring
NotBuzyo:  mov  ax,dx
           ret
ioctl_out  endp

cdrck      proc near uses bx cx
           xor  bx,bx                   ; check device
           mov  cx,device
           mov  ax,150Bh
           int  2Fh
           cmp  bx,0ADADh
           je   short cdrckDone
           xor  ax,ax
cdrckDone: ret
cdrck      endp

cdrreq     proc near uses si bx

           push ds
           pop  es
           mov  cx,device
           mov  ax,1510h
           int  2Fh
           ret
cdrreq     endp

HSSect     proc near uses bx ecx edx

           xor  eax,eax
           xor  edx,edx
           xor  bh,bh
           mov  dl,[si]                 ; frame
           mov  bl,[si+1]               ; sec
           mov  al,[si+2]               ; min
           mov  cx,60
           mul  cx
           add  ax,bx
           mov  ecx,75
           mul  ecx
           add  eax,edx
           sub  eax,150          
           ret
HSSect     endp

Seek       proc near

           xor  eax,eax
           mov  si,offset SeekReq
           mov  [si+13],al
           mov  [si+14],eax
           mov  [si+18],ax
           mov  [si+20],edx
           mov  al,13
           mov  [si],al
           mov  al,131
           mov  [si+2],al
           xor  ax,ax
           mov  [si+3],ax
           mov  bx,offset SeekReq
           call cdrreq
           mov  dx,[si+3]               ; status
           mov  ax,dx
           and  ax,8000h
           jz   short NoRetErr1
           mov  si,offset RetSErr
           call prtstring
           mov  ax,dx
           and  ax,0FFh
           call prtdec
NoRetErr1: mov  ax,dx
           and  ax,20h
           jz   short NotBuzyo1
           mov  si,offset BusyS
           call prtstring
NotBuzyo1: ret
Seek       endp

Play       proc near

           xor  al,al
           mov  si,offset PlayReq
           mov  [si+13],al
           mov  [si+14],edx
           mov  [si+18],ebx
           mov  al,13
           mov  [si],al
           mov  al,132
           mov  [si+2],al
           xor  ax,ax
           mov  [si+3],ax
           mov  bx,offset PlayReq
           call cdrreq
           mov  dx,[si+3]               ; status
           mov  ax,dx
           and  ax,8000h
           jz   short NoRetErr2
           mov  si,offset RetPErr
           call prtstring
           mov  ax,dx
           and  ax,0FFh
           call prtdec
NoRetErr2: mov  ax,dx
           and  ax,20h
           jz   short NotBuzyo2
           mov  si,offset BusyP
           call prtstring
NotBuzyo2: ret
Play       endp

JumpNext   proc near

           mov  si,offset ati           ; and put it in trk #
           mov  [si+1],al               ;
           mov  bx,offset ati
           mov  dx,07h
           call ioctl_in_ck
           mov  si,offset ati
           inc  si
           inc  si
           call HSSect
           mov  Strtsect,eax
           mov  edx,HSldout             ; Nsect = HSldout-Strtsect
           sub  edx,eax                 ;
           mov  Nsect,edx               ;
           ret
JumpNext   endp

TurnCOff   proc near
           mov  ah,01h                  ; turn off cursor
           mov  ch,20h                  ; bit number 5
           int  10h                     ;
           ret
TurnCOff   endp

TurnCOn    proc near
           mov  ah,01h                  ; turn on cursor
           mov  cx,0607h                ; start = 6   finish = 7
           int  10h                     ;
           ret
TurnCOn    endp

Prtstring  proc near uses ax
Ps1:       mov  dl,[si]                 ; Get character
           inc  si                      ; Point to next one
           or   dl,dl                   ; End of string?
           jz   short ps2               ; Yes, so exit
           mov  ah,02h                  ; Output a character
           int  21h
           jmp  short Ps1               ; Keep doing it
Ps2:       ret
Prtstring  endp

PrtDec     proc uses ax cx dx
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
           end  start
