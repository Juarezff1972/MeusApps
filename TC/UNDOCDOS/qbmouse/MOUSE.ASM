;*************************************************************************
; MOUSE.LIB  You may use this lib as you would like as long as you don't
; make any money doing so.  This package is donated to public domain.
; If any damages come to your machine due to this package I am not reliable
; Enough of the legal stuff, you can do what you want with this.  If you
; make any money from this, please give me some credit, donations, or what
; ever.  This is so that you can learn all the mouse functions if you don't
; know them by now.  If you distribute this, please leave me some credit.
;   This was quickly thrown together, so if you find any errors, please
;    let me know.
;
;   QBMOUSE.LIB
;   Forever Young Software
;   Benjamin David Lunt
;   All rights reserved
;   MASM 5.1x
;   06/28/97
;
.MODEL medium, basic
.286
.stack
.code

;**** Reset/Initialize Mouse
; On Entry:
;   nothing
; On return:
;   If AX !0 then
;     BX = number of buttons
;
           public    ResetIni
ResetIni   proc far basic uses bp bx
           xor  ax,ax
           int  33h
           or   ax,ax 
           jz   short ResetD
           mov  ax,0FFFFh
ResetD:    ret
ResetIni   endp

;**** Show Mouse Cursor
; On Entry:
;   nothing
; On return:
;   nothing
; Use MouseOff to Hide the Mouse Cursor
;
           public    MouseOn
MouseOn    proc far basic uses bp ax
           mov  ax,01h
           int  33h
           ret
MouseOn    endp

;**** Hide Mouse Cursor
; On Entry:
;   nothing
; On return:
;   nothing
; Use MouseOn to Show the Mouse Cursor
;
           public    MouseOff
MouseOff   proc far basic uses bp ax
           mov  ax,02h
           int  33h
           ret
MouseOff   endp

;**** Get Mouse Status
; On Entry:
;   nothing
; On return:
;   BX:  Button status (left = bit 0, right = bit 1)
;   CX:  Horizontal cursor position
;   DX:  Vertical cursor position
;
           public    MouseStat
MouseStat  proc far basic uses bp bx cx dx, Horz:word, Vert:word
           mov  ax,03h
           int  33h
           push bx
           mov  bx,offset Horz
           mov  [bx],cx
           mov  bx,offset Vert
           mov  [bx],dx
           pop  ax
           xor  ah,ah
           ret
MouseStat  endp

;**** Set Mouse Cursor Position
; On Entry:
;   CX:  Horizontal cursor position wanted
;   DX:  Vertical cursor position wanted
; On return:
;   nothing
;
           public    MSetCurPos
MSetCurPos proc far basic uses bp ax bx cx dx, Horz:word, Vert:word
           mov  bx,offset Horz
           mov  cx,[bx]
           mov  bx,offset Vert
           mov  dx,[bx]
           mov  ax,04h
           int  33h
           ret
MSetCurPos endp

;**** Get Button Press Info
; On Entry:
;   BX:  Button desired (0 - left, 1 - right)
; On return:
;   AX:  Button status (left = bit 0, right = bit 1)
;   BX:  Number of times the button has been pressed since last call
;   CX:  Horizontal coordinate at last press
;   DX:  Vertical coordinate at last press
;
           public    BttnPInfo
BttnPInfo  proc far basic uses bp bx cx dx, NTms:word, Horz:word, Vert:word
           mov  ax,05h
           int  33h
           push ax
           mov  ax,bx
           mov  bx,offset NTms
           mov  [bx],ax
           mov  bx,offset Horz
           mov  [bx],cx
           mov  bx,offset Vert
           mov  [bx],dx
           pop  ax
           ret
BttnPInfo  endp

;**** Get Button Release Info
; On Entry:
;   BX:  Button desired (0 - left, 1 - right)
; On return:
;   AX:  Button status (left = bit 0, right = bit 1)
;   BX:  Number of times the button has been released since last call
;   CX:  Horizontal coordinate at last release
;   DX:  Vertical coordinate at last release
;
           public    BttnRInfo
BttnRInfo  proc far basic uses bp bx cx dx, NTms:word, Horz:word, Vert:word
           mov  ax,06h
           int  33h
           push ax
           mov  ax,bx
           mov  bx,offset NTms
           mov  [bx],ax
           mov  bx,offset Horz
           mov  [bx],cx
           mov  bx,offset Vert
           mov  [bx],dx
           pop  ax
           ret
BttnRInfo  endp

;**** Set Horizontal Boundries
; On Entry:
;   CX:  Minimum Horizontal position
;   DX:  Maximum Horizontal position
; On return:
;   nothing
;
           public    SetHBndrs
SetHBndrs  proc far basic uses bp ax bx cx dx, MinH:word, MaxH:word
           mov  bx,offset MinH
           mov  cx,[bx]
           mov  bx,offset MaxH
           mov  dx,[bx]
           mov  ax,07h
           int  33h
           ret
SetHBndrs  endp

;**** Set Vertical Boundries
; On Entry:
;   CX:  Minimum Vertical position
;   DX:  Maximum Vertical position
; On return:
;   nothing
;
           public    SetVBndrs
SetVBndrs  proc far basic uses bp ax bx cx dx, MinV:word, MaxV:word
           mov  bx,offset MinV
           mov  cx,[bx]
           mov  bx,offset MaxV
           mov  dx,[bx]
           mov  ax,08h
           int  33h
           ret
SetVBndrs  endp

;**** Set Graphics Cursor Block
; On Entry:
;   BX:  Horizontal Hot Spot
;   CX:  Vertical Hot Spot
;   ES:DX  Segment:Offset (Address) of Mask
;    Mask should be two consecutive bitmaps of 16 words
;      ie:  16 words for screen mask
;           16 words for cursor mask
; On return:
;   nothing
;
           public    SetGMask
SetGMask   proc far basic uses bp ax bx cx dx es, HorzH:word, VertH:word, MaskS:word, MaskO:word
           mov  bx,offset HorzH
           mov  ax,[bx]
           push ax
           mov  bx,offset VertH
           mov  cx,[bx]
           mov  bx,offset MaskS
           mov  ax,[bx]
           mov  es,ax
           mov  bx,offset MaskO
           mov  dx,[bx]
           pop  bx
           mov  ax,09h
           int  33h
           ret
SetGMask   endp

;**** Set Text Cursor
; On Entry:
;   BX:  Cursor wanted (0-Software/1-Hardware)
;   CX:  Begining scan line/Screen mask
;   DX:  Ending scan line/Cursor mask
; On return:
;   nothing
;
           public    SetTCursr
SetTCursr  proc far basic uses bp ax bx cx dx, CurSH:word, BSnLn:word, ESnLn:word
           mov  bx,offset CurSH
           mov  ax,[bx]
           push ax
           mov  bx,offset BSnLn
           mov  cx,[bx]
           mov  bx,offset ESnLn
           mov  dx,[bx]
           pop  bx
           mov  ax,0Ah
           int  33h
           ret
SetTCursr  endp

;**** Read Motion Counters
; On Entry:
;   nothing
; On return:
;   CX:  Horizontal Change since last call
;   DX:  Vertical Change since last call
;
           public    ReadMotion
ReadMotion proc far basic uses bp ax bx cx dx, HorzC:word, VertC:word
           mov  ax,0Bh
           int  33h
           mov  bx,offset HorzC
           mov  [bx],cx
           mov  bx,offset VertC
           mov  [bx],dx
           ret
ReadMotion endp

;**** Set Mickey-to-Pixel Ratio  (Speed of mouse)
; On Entry:
;   CX:  Horizontal Ratio  (Move CX pixels per Mickey) (default = 8)
;   DX:  Vertical Ratio (Move DX pixels per Mickey) (default = 8)
; On return:
;   nothing
;
           public    SetSpeed
SetSpeed   proc far basic uses bp ax bx cx dx, Horz:word, Vert:word
           mov  bx,offset Horz
           mov  cx,[bx]
           mov  bx,offset Vert
           mov  dx,[bx]
           mov  ax,0Fh
           int  33h
           ret
SetSpeed   endp

;**** Set Double Speed Threshold
; On Entry:
;   DX:  Threshold value  (Mickeys per second) (default-64)
; On return:
;   nothing
;
           public    DblSpeed
DblSpeed   proc far basic uses bp ax bx cx dx, ThrsHold:word
           mov  bx,offset ThrsHold
           mov  dx,[bx]
           mov  ax,13h
           int  33h
           ret
DblSpeed   endp

;**** Set Mouse Sensitivity
; On Entry:
;   BX  Horizontal Sensitivity
;   CX  Vertical Sensitivity
;   DX  Double Speed Treshold
; On return:
;   nothing
;
           public    SetMSnstvy
SetMSnstvy proc far basic uses bp ax bx cx dx, HorzS:word, VertS:word, DThrsHold:word
           mov  bx,offset HorzS
           mov  ax,[bx]
           push ax
           mov  bx,offset VertS
           mov  cx,[bx]
           mov  bx,offset DThrsHold
           mov  dx,[bx]
           pop  bx
           mov  ax,1Ah
           int  33h
           ret
SetMSnstvy endp

;**** Get Mouse Sensitivity
; On Entry:
;   nothing
; On return:
;   BX  Horizontal Sensitivity
;   CX  Vertical Sensitivity
;   DX  Double Speed Treshold
;
           public    GetMSnstvy
GetMSnstvy proc far basic uses bp ax bx cx dx, HorzS:word, VertS:word, DThrsHold:word
           mov  ax,1Bh
           int  33h
           mov  ax,bx
           mov  bx,offset HorzS
           mov  [bx],ax
           mov  bx,offset VertS
           mov  [bx],cx
           mov  bx,offset DThrsHold
           mov  [bx],dx
           ret
GetMSnstvy endp

;**** Set Interrupt Rate
; On Entry:
;   BX  Interrupts per second (0-4)
;         0 = Turn off interrupts
;         1 = 30 interrupts per second (default)
;         2 = 50 interrupts per second
;         3 = 100 interrupts per second
;         4 = 200 interrupts per second
; On return:
;   nothing
;
           public    SetIntRate
SetIntRate proc far basic uses bp ax bx cx dx, IRate:word
           mov  bx,offset IRate
           mov  ax,[bx]
           mov  bx,ax
           mov  ax,1Ch
           int  33h
           ret
SetIntRate endp

;**** Software reset
; On Entry:
;   nothing
; On return:
;   AX = 0FFFFh if successful, 21h if not (unchanged)
;
           public    ResetSoft
ResetSoft  proc far basic uses bp bx cx dx
           mov  ax,21h
           int  33h
           ret
ResetSoft  endp

;**** Set Language for messages
; On Entry:
;   BX = Language number
;        0 = English
;        1 = French
;        2 = Dutch
;        3 = German
;        4 = Swedish
;        5 = Finnish
;        6 = Spanish
;        7 = Portugues
;        8 = Italian
; On return:
;   nothing
;
           public    SetLang
SetLang    proc far basic uses bp ax bx cx dx, MLang:word
           mov  bx,offset MLang
           mov  ax,[bx]
           mov  bx,ax
           mov  ax,22h
           int  33h
           ret
SetLang    endp

;**** Get Language
; On Entry:
;   nothing
; On return:
;   BX = language number
;     (see Set Language)
;
           public    GetLang
GetLang    proc far basic uses bp bx cx dx
           mov  ax,23h
           int  33h
           mov  ax,bx
           ret
GetLang    endp

;**** Get Driver Version Info
; On Entry:
;   nothing
; On return:
;   BH = Major version number
;   BL = Minor version number
;
           public    GetDrvVer
GetDrvVer  proc far basic uses bp bx cx dx
           mov  ax,24h
           int  33h
           mov  ax,bx
           ret
GetDrvVer  endp

;**** Get Maximum Virtual Coordinates
; On Entry:
;   nothing
; On return:
;   BX = Mouse Driver status
;   CX = Maximum Horizontal coordinate
;   DX = Maximum Vertical coordinate
;
           public    GetMaxVCds
GetMaxVCds proc far basic uses bp ax bx cx dx, MHorz:word, MVert:word
           mov  ax,26h
           int  33h
           mov  bx,offset MHorz
           mov  [bx],cx
           mov  bx,offset MVert
           mov  [bx],dx
           ret
GetMaxVCds endp

;**** Mouse Hardware Reset
; On Entry:
;   nothing
; On return:
;   AX = 0FFFFh if successful, <> 0 if error
;
           public    ResetHard
ResetHard  proc far basic uses bp bx cx dx
           mov  ax,2Fh
           int  33h
           ret
ResetHard  endp

;**** Get Virtual Coordinates
; On Entry:
;   nothing
; On return:
;   AX = Minimum Horizontal Coordinate
;   BX = Minimum Vertical Coordinate
;   CX = Maximum Horizontal Coordinate
;   DX = Maximum Vertical Coordinate
;
           public    GetVCords
GetVCords  proc far basic uses bp ax bx cx dx, HMax:word, HMin:word, VMax:word, VMin:word
           mov  ax,31h
           int  33h
           push ax
           mov  ax,bx
           mov  bx,offset VMin
           mov  [bx],ax
           pop  ax
           mov  bx,offset HMin
           mov  [bx],ax
           mov  bx,offset HMax
           mov  [bx],cx
           mov  bx,offset VMax
           mov  [bx],dx
           ret
GetVCords  endp
           end
