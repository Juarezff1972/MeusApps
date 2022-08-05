; ************************************************************************
; *  This is a small snip of code to print a single precision, fixed
; *  floating point number to the screen. (fixed to 5 decimal places)
; *    Call with EAX holding the floating point number to print
; *
; *  You may use this code as you would like.
; *  As always, If this code crashes you machine in anyway I am
; *  not held responsible.  Use at your own risk.
; *   Benjamin David Lunt
; *   Forever Young Software (r)
; *   (C)opyright 1999
; *   All rights reserved.
; *    05/20/97
; *    MASM 5.1

PrtSngl    proc near uses eax bx cx edx si

           mov  edx,eax                 ; conver from eax --> dx:ax
           shr  edx,16
           and  eax,0FFFFh
           test dx,dx                   ; negative?
           jge  short p1_noneg
           push ax
           mov  al,45
           push dx
           mov  ah,02h
           mov  dl,al
           int  21h
           pop  dx
           pop  ax
           neg  dx                      ; make it positive
           neg  ax
           sbb  dx,00h
p1_noneg:  push dx                      ; save dx
           xor  bx,bx                   ; zero rem flag
           add  ax,ax                   ; shift out high bit
           adc  bx,00h                  ; bx = rem flag
           shr  ax,01h                  ; restore ax
           mov  cx,50000                ; ax = ax * 50000
           mul  cx
           shr  ax,15                   ; ax = ax / 32768
           shl  dx,01h
           or   ax,dx
           pop  dx                      ; restore dx
           push bx                      ; save bx, ax
           push ax
           xchg dx,ax                   ; integer part in ax
           xor  cx,cx                   ; zero cx
           mov  si,10                   ; si = 10
p1_dloop:  xor  dx,dx                   ; divide by 10
           div  si
           mov  bl,dl                   ; remainder in bl
           add  bl,30h                  ; convert to digit
           push bx                      ; push digit
           inc  cx
           test ax,ax                   ; loop back
           jnz  short p1_dloop
p1_ploop9: pop  ax                      ; pop digit
           push ax
           push dx
           mov  ah,02h
           mov  dl,al
           int  21h
           pop  dx
           pop  ax
           loop p1_ploop9               ; loop back
           mov  al,46
           mov  ah,02h
           mov  dl,al
           int  21h
           pop  ax                      ; restore low data
           pop  bx
           xor  dx,dx                   ; zero dx
           test bx,bx                   ; check for high part
           jz   short p1_nohigh
           add  ax,50000                ; add in 50000
           adc  dx,00h
p1_nohigh: mov  si,10                   ; si = 10
           mov  cx,5                    ; 5 digits
           jmp  short p1_skip1
p1_dloopb: xor  dx,dx                   ; zero dx
p1_skip1:  div  si                      ; divide by 10
           mov  bl,dl                   ; remainder in bl
           add  bl,30h                  ; convert to digit
           push bx                      ; push digit
           loop p1_dloopb               ; Loop back
           mov  cx,5                    ; 5 digits
p1_ploopb: pop  ax                      ; pop digit
           push ax
           push dx
           mov  ah,02h
           mov  dl,al
           int  21h
           pop  dx
           pop  ax
           loop p1_ploopb               ; loop back
           ret
PrtSngl    endp
