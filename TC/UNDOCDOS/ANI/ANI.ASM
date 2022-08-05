.MODEL small, basic
.386
.stack
.data

    FileName   db  'backgrnd.dat',0
    FileName1  db  'ani.dat',0
    VidMode    db  00h                  ; saved video mode
    SavedP     dw  00h                  ; saved pos. to restore to pos.

.data?
    OldPalette db  768 dup(?)
    OurPalette db  768 dup(?)
    AniDat     db  1920 dup(?)
    Temp       db  1920 dup(?)

.code
start:     mov  ax,DGROUP               ; .STARTUP  (6.1)
           mov  ds,ax                   ;
           mov  bx,ss                   ;
           sub  bx,ax                   ;
           shl  bx,04                   ;
           mov  ss,ax                   ;
           add  sp,bx                   ;
           push ds
           pop  es

           mov  ah,0Fh
           int  10h
           mov  VidMode,al

           mov  ax,0013h                ; change screen to 320x200x256 (13h)
           int  10h                     ;

           call GetScrn                 ; get and print screen
           mov  ax,3D00h                ; open ani file (data to animate)
           mov  dx,offset FileName1     ;
           int  21h                     ;
           mov  bx,ax                   ;
           mov  ah,3Fh                  ; read in pixel data
           mov  cx,1920                 ; (32 x 60)
           mov  dx,offset AniDat        ;
           int  21h                     ;
           mov  ah,3Eh                  ; close file
           int  21h                     ;

           mov  ax,0A000h               ; screen memory
           mov  es,ax                   ;
           xor  di,di                   ; start with (0,0)
           mov  cx,10h                  ; do 16 times
AniLoop1:  push cx                      ;
           push di
           call SaveIt                  ; call save part of background
           mov  si,offset AniDat        ; AniDat holds bitmap to place
           mov  cx,32                   ; 32 pixels in height
AniLoop2:  push cx                      ;
           push di                      ;
           mov  cx,60                   ; 60 pixels in width
AniLoop3:  lodsb
           or   al,al                   ; if zero, then is a mask
           jz   SkipPxl
           mov  es:[di],al              ; stosb
SkipPxl:   inc  di
           loop AniLoop3
           pop  di
           pop  cx
           add  di,320                  ; go to next line pos
           loop AniLoop2                ; repeat all of image
           pop  di                      ;
           mov  eax,1000000             ; pause for 1 sec
           call PauseIt                 ;
           call RstrIt                  ; restore backgrond
           add  di,3600                 ; next place to put image
           pop  cx
           loop AniLoop1                ; do again

Done:      call RstrPltte               ; restore the origonal palette
           xor  ah,ah
           mov  al,VidMode              ; set screen to text
           int  10h
           mov  ah,4Ch                  ; exit to DOS
           int  21h                     ; 


GetScrn    proc near uses ax bx cx dx es

           mov  ax,1017h                ; get and save the origonal palette
           mov  dx,offset OldPalette    ;
           xor  bx,bx                   ;
           mov  cx,256                  ;
           int  10h                     ;

           mov  ax,3D00h                ; open file
           mov  dx,offset FileName      ;
           int  21h                     ;
           mov  bx,ax                   ; handle

           mov  ah,3Fh                  ; read in our palette
           mov  cx,768                  ;
           mov  dx,offset OurPalette    ;
           int  21h                     ;

           push bx                      ; save our handle
           mov  ax,1012h                ; use our palette
           mov  dx,offset OurPalette    ;
           xor  bx,bx                   ;
           mov  cx,256                  ;
           int  10h                     ;
           pop  bx                      ; restore our handle

           push ds                      ;
           mov  ax,0A000h
           mov  ds,ax
           mov  ah,3Fh                  ; read in pixel data
           mov  cx,64000                ;  and put it in 0A000h
           xor  dx,dx                   ;  with one command
           int  21h                     ;
           pop  ds                      ;

           mov  ah,3Eh                  ; close file
           int  21h                     ;

GPDone:    ret
GetScrn    endp

RstrPltte  proc near uses ax bx cx dx es

           push ds
           pop  es
           mov  ax,1012h                ; restore palette
           mov  dx,offset OldPalette    ;
           xor  bx,bx                   ;
           mov  cx,256                  ;
           int  10h                     ;
           ret
RstrPltte  endp

pauseit    proc near uses ecx edx
           mov  ecx,eax
           mov  edx,eax
           shr  ecx,16
           and  edx,0FFFFh
           mov  ah,86h
           int  15h
           ret
pauseit    endp

SaveIt     proc near uses cx si di ds es
           mov  si,di
           mov  SavedP,si
           mov  di,offset Temp
           push es                      ; swap es and ds
           push ds
           pop  es
           pop  ds
           mov  cx,32
SLoop1:    push cx
           push si
           mov  cx,60
           rep  movsb
           pop  si
           add  si,320
           pop  cx
           loop SLoop1
           ret
SaveIt     endp

RstrIt     proc near uses cx si di

           mov  di,SavedP
           mov  si,offset Temp
           mov  cx,32
SLoop1:    push cx
           push di
           mov  cx,60
           rep  movsb
           pop  di
           add  di,320
           pop  cx
           loop SLoop1
           ret
RstrIt     endp
           end  start
