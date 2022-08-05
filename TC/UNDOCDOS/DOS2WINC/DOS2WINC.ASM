Comment |************************************************************
* This is a TSR that when activated and the CTRL-ALT-C              *
* key combonation is pressed, it will send what ever text is on the *
* screen (memory at 0B800h) to the Windoze Clipboard.               *
* Disclaimer:  *** USE at your OWN Risk ***                         *
*                                                                   *
* WARNING: -Does not work well with DOSKEY installed,               *
*            or any other TSR that 'interrupts' the                 *
*            keyboard interrupt.                                    *
*          -Also doesn't work that wonderfully anyway.              *
*            Depends on what TSR's are loaded in what               *
*            order.                                                 *
*          -If you know why it isn't working like it                *
*            should, please let me know at:                         *
*                 blunt@zekes.com                                   *
*                                                                   *
* Assembled with NBASM (*beta version*)                             *
*  Get NBASM at:  http://www.zekes.com/~blunt/newbasic.html         *
* Forever Young Software                                            *
* http://www.zekes.com/~blunt/index.html                            *
* blunt@zekes.com                                                   *
* 25 Oct 1998                                                       *
* Version 0.90b                                                     *
*                                                                   *
*********************************************************************|

.MODEL tiny
.186
.code

start:     jmp  install

comment |
  We could put the Copyright Notice in the install part of the code
   so that when we are a TSR, it would make for a smaller memory
   allocation, but this way if someone is disassembling Int 09h,
   they see our copyright notice. |

NewInt9:   jmp  short NewInt9S  ; skip data area

ID         db  'FY'      ; ID word to see if we are already here
Copyright  db  13,10,'DOS Screen to Windoze Clipboard    v0.90b'
           db  13,10,'Copyright  1984-1998  Forever Young Software'
           db  13,10,36
Old9Adr    dw  00h          ; these remember the original Int 9 address
Old9Seg    dw  00h          ; they must be in the code segment
OurInFlag  db  00h

NewInt9S:  pusha                        ; save the registers we'll be using
           pushf                        ; and the flags
           push bp                      ; and to be safe

           in   al,60h                  ; get key 'make' scan code

           pushf                        ; simulate INT
           cli                          ; 
           callf cs:Old9Adr             ; call BIOS
           sti                          ;

           cmp  al,2Eh                  ; is it the 'C' key?
           je   short IsC
           jmp  Continue                ; no, continue on to BIOS interrupt 9
IsC:       mov  ax,0040h                ; get the shift status at 0040:0017h
           mov  es,ax                   ;
           mov  al,es:[0017h]           ;
           and  al,00001100b            ;
           cmp  al,00001100b            ; is Ctrl+Alt pressed
           je   short IsCAkeys
           jmp  Continue                ; no, continue on to BIOS interrupt 9
IsCAkeys:  cmp  byte cs:OurInFlag,00h   ; already doing it?
           je   short IsOktoDo          ;
           jmp  Continue                ;
IsOktoDo:  mov  byte cs:OurInFlag,01h   ; set 'already doing it' flag
           in   al,61h                  ; send an acknowledge to keyboard
           mov  ah,al                   ; otherwise the keystroke set
           or   al,80h                  ; will still be hanging around the
           out  61h,al                  ; next time a program goes to get a key
           mov  al,ah
           out  61h,al
           mov  al,20h                  ; indicate end of interrupt to 8259
           out  20h,al                  ; interrupt controller chip

           mov  ax,1700h                ; check clipboard for usage
           int  2Fh                     ;
           cmp  ax,1700h                ; if 1700h then error
           je   short Continue

           mov  ax,1701h                ; open clipboard
           int  2Fh

           push ds
           push cs
           pop  es
           mov  cx,25
           xor  si,si
           mov  di,offset cs:Install
           mov  ax,0B800h
           mov  ds,ax
GetScrL1:  push cx
           mov  cl,80        ; we know that ch = 0 from mov cx,25 above
GetScrL2:  lodsw
           stosb
           loop GetScrL2
           mov  ax,0A0Dh                ; put a CRLF at end of line
           stosw
           pop  cx
           loop GetScrL1
           pop  ds

           mov  ax,1703h                ; put screen 03h into clipboard
           push cs
           pop  es
           mov  bx,offset cs:Install
           mov  dx,01h
           mov  cx,2050                 ; (25x80) + 25 CRLF's
           xor  si,si
           int  2Fh

           mov  ax,1708h                ; close clipboard
           int  2Fh

           ; print smiley face in upper right corner
           mov  ax,0B800h               ;
           mov  es,ax
           mov  bx,es:[009Eh]
           mov  word es:[009Eh],1701h   ; make print smiley white on blue

           ; pause for a few miliseconds
           mov  cx,06
           mov  dx,03DAh
delay_l1:  in   al,dx
           test al,08h
           jne  short delay_l1
delay_l2:  in   al,dx
           test al,08h
           je   short delay_l2
           loop delay_l1

           ; restore char that was originally there
           mov  es:[009Eh],bx

           mov  byte cs:OurInFlag,00h   ; clr 'already doing it' flag

Continue:  pop  bp                      ; restore save registers and flags
           popf                         ; 
           popa                         ; 
           iret                         ; interrupt ret

; This is the install part.  It gets wiped out by our buffer used above.
; After TSR'ed, we no longer need this code.
;
Install:   mov  dx,offset Copyright     ; print our copyright string
           mov  ah,09
           int  21h

           mov  ax,[002Ch]              ; free environment string
           mov  es,ax
           mov  ah,49h
           int  21h

           cli
           mov  ax,3509h                ; ask DOS for the existing Int 9 vector address
           int  21h                     ; DOS returns the segment:address in ES:BX
           mov  cs:Old9Adr,bx           ; save it locally
           mov  cs:Old9Seg,es
           sti

           ; make sure we are not already installed
           mov  ax,es:[bx+2]            ; get ID
           cmp  ax,5946h
           jne  short NotAlrdy
           mov  dx,offset AlrdS
           mov  ah,09h
           int  21h
           mov  ax,4C00h                ; exit completely (no TSR)
           int  21h

NotAlrdy:  cli
           mov  ax,2509h                ; point Interrupt 9 to our own handler
           mov  dx,offset NewInt9
           push cs                      ; copy CS into DS
           pop  ds
           int  21h
           sti
      
           ; we could put all of this on one line, but for demonstrational
           ; purposes, we will have it on multiple lines, using multiple
           ; instructions.  (the memory gets used anyway, so size isn't a
           ; problem here.)
           mov  dx,offset install       ; save all TSR code
           sub  dx,offset start
           add  dx,256                  ; + PSP  (100h bytes)
           add  dx,2050                 ; lets add 2050 bytes for the buffer
           shr  dx,04h                  ;
           inc  dx                      ; 1 more para for safety
           mov  ax,3100h                ; exit to DOS but stay resident
           int  21h

AlrdS      db  13,10,'Already installed...',13,10,36

.end start
