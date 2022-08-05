;********************************************************
; This is the example to call a proc from MOUSE.LIB
;  masm new;                   (MASM 5.1x)
;  link new,,,mouse.lib;
;
;
.MODEL small, basic
.386
.stack
.data

  HorizHS      dw     8
  VertHS       dw     8
  OurMask      dw     1111111111111111b  ; This is the background
               dw     1111111111111111b  ;  if it is a 1 then
               dw     1111111111111111b  ;   use current backgound
               dw     1111111111111111b  ;     (transparent)
               dw     1111111111111111b  ;  if it is a 0 then make the
               dw     1111111111111111b  ;    backgound black
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b
               dw     1111111111111111b

               dw     0000000101000000b  ; This is the actual cursor
               dw     0000000101000000b  ;  if it is a 1 then print white
               dw     0001100101001100b  ;  if it is a 0 then print black
               dw     0001010101010100b
               dw     0000101101101000b
               dw     0000010101010000b
               dw     0011111000111100b
               dw     0100000000000010b
               dw     0011111000111100b
               dw     0000010101010000b
               dw     0000101101101000b
               dw     0001010101010100b
               dw     0001100101001100b
               dw     0000000101000000b
               dw     0000000101000000b
               dw     0000000010000000b

.code
           extrn MouseOn:proc
           extrn MouseOff:proc
           extrn SetGMask:proc
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
           push ds
           pop  es

           mov  ax,0012h                ; set screen to mode 12 (VGA)
           int  10h

           mov  bx,HorizHS              ; Horz Hot Spot
           mov  cx,VertHS               ; Vert Hot Spot
           mov  dx,offset OurMask       ; Our Mask (2 - 16x16)
           call SetGMask                ; change it

           call MouseOn                 ; show mouse cursor

           xor  ah,ah                   ; wait for key press
           int  16h                     ;

           call MouseOff                ; hide mouse cursor

           mov  ax,0003h                ; text mode
           int  10h

Done:      mov  ah,4Ch                  ; exit to DOS
           int  21h                     ; 
           end  start
