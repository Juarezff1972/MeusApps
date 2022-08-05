.model small, basic
include linka.inc

CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg
.186
           org  100h
start:     jmp  begin

   Old_IsrO     dw  00h                 ; old interrupt address
   Old_IsrS     dw  00h
   old_int_mask db  00h                 ; the old interrupt mask on the PIC
   constr       db  13,10,'Connecting... (press <esc> to quit)',36
   plus3        db  '+++'
   dialtone     db  'atdt'
   number       db  '555-5555',0Dh
   clsmdm       db  'at&f'
   ser_buffer   db  1024 dup(0)         ; serial input buffer
   ser_end      dw  -1                  ; indexes into receive buffer
   ser_start    dw  -1                  ;
   serial_lock  db  00h                 ; serial ISR semaphore so the buffer
                                        ; isn't altered while it is being written
                                        ; to by the ISR

begin:     mov  ax,cs                   ; free unused part of Mem Block
           mov  es,ax                   ;   for .COM file format
           mov  bx,4096                 ;
           mov  ah,4Ah                  ;
           int  21h                     ;
           push cs                      ; make sure ds = cs
           pop  ds                      ;

; Default baud rate = 9600
; Initialize port handlers (open com)
           mov  bx,SER_BAUD_9600
           mov  cx,SER_STOP_1 or SER_BITS_8 or SER_PARITY_NONE
           call open_serial

; open modem for commands
           mov  si,offset plus3
           mov  cx,03h
           call send_command

; ready modem for number
           mov  si,offset dialtone
           mov  cx,04h
           call send_command

; dial number
           mov  si,offset number
           mov  cx,09h
           call send_command

           mov  dx,offset constr
           mov  ah,09h
           int  21h

wloop:     mov  ah,01h                  ; try and get a char from local machine
           int  16h                     ;
           jz   short nokey             ;

           xor  ah,ah
           int  16h                     ; al = character from keybrd

;if the modem/host doesn't return the char then you will need the next line
;           int  29h

           call serial_write            ; send the character to other machine

           cmp  al,27
           je   short wloopd1
           cmp  al,13
           jne  short NotCR
           mov  al,10
;if the modem/host doesn't return the char then you will need the next line
;           int  29h
           call serial_write            ; send the character to other machine
NotCR:
nokey:     call serial_read             ; try and get a character from remote
           cmp  ax,0FFh
           ja   short wloop
           mov  ah,02
           mov  dl,al
           int  21h
           jmp  short wloop

wloopd1:   mov  si,offset clsmdm
           mov  cx,04h
           call send_command

; close serial port
           call close_serial

           mov  ax,4C00h
           int  21h

serial_isr proc far
           cli
           push ax
           push bx
           push dx                      ; lock out other functions so buffer
           mov  cs:serial_lock,01h      ;   doesn't get corrupted
           inc  cs:ser_end              ; wrap buffer index around
           cmp  cs:ser_end,1023
           jle  short NoWrap1
           mov  cs:ser_end,00h
NoWrap1:   mov  bx,offset ser_buffer    ; move character into buffer
           add  bx,cs:ser_end           ;
           mov  dx,COM_2 + SER_RBF      ; place character into next position
           in   al,dx                   ;
           mov  cs:[bx],al              ;
           mov  dx,20h                  ; restore PIC
           mov  al,20h                  ;
           out  dx,al                   ;
           mov  cs:serial_lock,00h      ; undo lock
           pop  dx
           pop  bx
           pop  ax
           sti
           iret
serial_isr endp

; this function reads a character from the circulating buffer
; and returns it to the caller
serial_read proc near uses bx dx
isrend:    mov  al,cs:serial_lock       ; wait for isr to end
           or   al,al
           jnz  short isrend
; is there a character ready in buffer?
           mov  ax,cs:ser_end
           cmp  ax,cs:ser_start
           je   short nochars
           inc  cs:ser_start            ; wrap buffer index around if needed
           cmp  cs:ser_start,1023
           jle  short NoWrap
           mov  cs:ser_start,00h
NoWrap:    mov  bx,offset cs:ser_buffer ; get the character out of buffer
           add  bx,cs:ser_start         ;
           mov  al,[bx]                 ;
           xor  ah,ah
           jmp  short sreadd
nochars:   mov  ax,0FFFFh               ; buffer was empty return -1
sreadd:    ret
serial_read endp

; this function writes a character to the transmit buffer, but first it
; waits for the transmit buffer to be empty.
serial_write proc near uses ax cx dx
           cli
           push ax
           mov  dx,COM_2 + SER_LSR
waitempty: in   al,dx
           and  al,20h
           or   al,al
           jz   short waitempty
           pop  ax
           mov  dx,COM_2 + SER_THR
           out  dx,al
           sti
           ret
serial_write endp

open_serial proc near
           cli
           mov  al,80h                  ; set the baud rate
           mov  dx,COM_2+SER_LCR        ;
           out  dx,al                   ;
           mov  al,bl
           mov  dx,COM_2+SER_DLL        ;
           out  dx,al                   ;
           xor  al,al
           mov  dx,COM_2+SER_DLH        ;
           out  dx,al                   ;
           mov  al,cl
           mov  dx,COM_2+SER_LCR        ; set the configuration for the port
           out  dx,al                   ;

           mov  dx,COM_2+SER_RBF        ; do a read just incase one already there
           in   al,dx                   ;

; enable the interrupt when char is ready to be received
           mov  al,0Bh
           mov  dx,COM_2+SER_MCR        ;
           out  dx,al                   ;

           mov  al,01h
           mov  dx,COM_2+SER_IER        ;
           out  dx,al                   ;

           cli                          ; get and save org int vector
           push es                      ;
           mov  al,INT_SER_PORT_1       ;
           mov  ah,35h                  ;
           int  21h                     ;
           mov  cs:Old_IsrO,bx          ;
           mov  cs:Old_IsrS,es          ;
           pop  es                      ; restore int. vec. to use
           mov  ah,25h                  ; set new interrupt vector
           mov  al,INT_SER_PORT_1       ;
           mov  dx,offset cs:serial_isr ;
           int  21h                     ; ds already equals cs

           mov  dx,21h                  ; enable interrupt on PIC
           in   al,dx                   ;
           mov  cs:old_int_mask,al      ;
           and  al,0F7h                 ;
           out  dx,al                   ;
           sti
           ret
open_serial endp

; this function closes the port, turning off interrupts and
; restoring the old interrupt vector
close_serial proc near uses ax bx cx dx

           cli
           mov  dx,COM_2 + SER_MCR      ;
           xor  al,al                   ; disable the interrupts
           out  dx,al                   ;
           mov  dx,COM_2 + SER_IER      ;
           out  dx,al                   ;
           mov  dx,21h
           mov  al,cs:old_int_mask
           out  dx,al
           sti

           mov  ah,25
           mov  al,INT_SER_PORT_1
           push ds
           mov  bx,cs:Old_IsrO
           mov  ds,cs:Old_IsrS
           int  21h
           pop  ds
           ret
close_serial endp

;  Send the buffer
send_command proc uses ax si
sendit:    lodsb
           call serial_write
           loop sendit
           ret
send_command endp

CodeSeg    ends
           end start
