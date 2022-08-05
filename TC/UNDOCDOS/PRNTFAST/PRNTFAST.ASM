.model small, basic
CodeSeg    segment
           assume cs:CodeSeg, ds:CodeSeg, es:CodeSeg
      
           org  100h
start:     mov  ax,cs                   ; free unused part of Mem Block
           mov  es,ax                   ;   for .COM file format
           mov  bx,4096                 ;
           mov  ah,4Ah                  ;
           int  21h                     ;
           mov  ah,01h                  ; turn off cursor
           mov  ch,20h                  ; bit number 5
           int  10h                     ;
           mov  dx,03D8h                ; enable 16 color background
           mov  al,09h                  ;   (disable blink mode)
           out  dx,al                   ;  (on a CGA)
           mov  ax,1003h                ; enable 16 color background
           xor  bx,bx                   ;   (disable blink mode)
           int  10h                     ;  (on a VGA)

           call GetVidSeg               ; return screen seg in bx
           mov  es,bx
           mov  si,offset msg1
           xor  di,di
           mov  ah,07h                  ; assume white on black
MainLoop:  lodsb
           or   al,al
           jnz  short PrntIt
           lodsb
           or   al,al                   ; if two zeros then done
           jz   short PrntDone
           xchg al,ah
           jmp  short MainLoop
PrntIt:    stosw
           jmp  short MainLoop

PrntDone:  mov  ah,01h                  ; turn on cursor
           mov  cx,0607h                ; start = 6   finish = 7
           int  10h                     ;

           xor  ah,ah                   ; wait for key press
           int  16h

           int  20h                     ; exit to DOS

GetVidSeg  proc near uses ax es
           mov  bx,0B000h               ; assume the mono screen segment
           xor  ax,ax                   ; zero out ax and sets ax for no cga
           mov  es,ax                   ; put the zero in es, to look at low memory
           mov  al,es:[0463h]           ; look at the video controller port address
           cmp  al,0B4h                 ; is it mono?
           jz   short all_done          ; yes, skip over adding 800h to video segment
           add  bx,800h                 ; no, adjust bx for a color monitor
All_done:  ret
GetVidSeg  endp

msg1       db   00h,07h
           db   'ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป'
           db   'บ       This is a demo program to show how to quickly print colored text.      บ'
           db   'วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ'
           db   'บ  This whole box should be printed white on black.                            บ'
           db   'ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ'
           db   80 dup(20h)
           db   0,70h
           db   'While this line should be black on white                                        '
           db   0,84h
           db   'How about Red on Grey?                                                          '
           db   0,1Eh
           db   'How about Yellow on Blue?                                                       '
           db   0,01h
           db   'You '
           db   0,02h
           db   'can '
           db   0,03h
           db   'even '
           db   0,04h
           db   'have '
           db   0,05h
           db   'each '
           db   0,06h
           db   'char '
           db   0,07h
           db   'different, '
           db   0,18h
           db   'even '
           db   0,29h
           db   'the '
           db   0,3Ah
           db   'background '
           db   0,4Bh
           db   'can '
           db   0,5Ch
           db   'be '
           db   0,6Dh
           db   'different.    '
           db   00h,07h
           db   80 dup(20h)
           db   'All you have to do is give a 4 bit background and a 4 bit foreground color.     '
           db   '  This means that you can have 16 background colors and 16 foreground colors.   '
           db   '  This gives you the combination of 256 different attributes.                   '
           db   '    That means you can have 256 different color combinations on the screen at   '
           db   '     one time.                                                                  '
           db   00h,07h
           db   80 dup(20h)
           db   ' The background color goes in the high order of AH while the foreground color   '
           db   '  goes in the lo order of AH.  Then write the char, then its attribute.         '
           db   80 dup(20h)
           db   00h,0F7h
           db   ' You try some different combinations.                                           '
           db   00h,07h
           db   80 dup(20h)
           db   00h,0Dh
           db   16,'  Forever Young Software   Benjamin David Lunt   (c)opyright 1997   08/10/97  ',17
           db   00h,07h
           db   'Press any key to exit...                                                        '
           db   00h,00h

CodeSeg    ends
           end  start

The text screen memory is setup with 4000 bytes for each 80 col x 25 row page.
This 4000 bytes allow 2000 chars to be displayed using two bytes each.
The HI order byte being the char and the LO order byte being the attribute.
The char can be any of the 256 ascii chars.
The attribute is divided up in to two 4 bit parts (nibbles).  The HI order
  is the background and the LO order is the foreground color.
If you specify a background color that has the HI bit set (1???) then it
will display only one of the first 7 colors and blink.  To use the other
7 colors, you must turn off the blink mode.
- On the CGA, we write to the video card directly:

           mov  dx,03D8h                ; enable 16 color background
           mov  al,09h                  ;   (disable blink mode)
           out  dx,al                   ;  (on a CGA)

- On the VGA we use interrupt 10h:

           mov  ax,1003h                ; enable 16 color background
           xor  bx,bx                   ;   (disable blink mode)
           int  10h                     ;  (on a VGA)

If you have any more questions, just send me an email message.

Forever Young Software
%Benjamin David Lunt
(c)opyright 1984-1997
08/10/97

blunt@zekes.com
http://www.zekes.com/~blunt/index.html
