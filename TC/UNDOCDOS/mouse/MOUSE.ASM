;*************************************************************************
; MOUSE.LIB  You may use this lib as you would like as long as you don't
; make any money doing so.  This package is donated to public domain.
; If any damages come to your machine due to this package I am not reliable
; Enough of the legal stuff, you can do what you want with this.  If you
; make any money from this, please give me some credit, donations, or what
; ever.  This is so that you can learn all the mouse functions if you don't
; know them by now.  If you distribute this, please leave me some credit.
;   MOUSE.LIB
;   Forever Young Software
;   Benjamin David Lunt
;   All rights reserved
;   MASM 5.1x
;   06/28/97
;
.MODEL small, basic
.386
.stack
.data

   BackCol   db  17h

.code
;**** Reset/Initialize Mouse
; On Entry:
;   nothing
; On return:
;   If AX !0 then
;     BX = number of buttons
;
ResetIni   proc near
           xor  ax,ax
           int  33h
           ret
ResetIni   endp

;**** Show Mouse Cursor
; On Entry:
;   nothing
; On return:
;   nothing
; Use MouseOff to Hide the Mouse Cursor
;
MouseOn    proc near uses ax
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
MouseOff   proc near uses ax
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
MouseStat  proc near uses ax
           mov  ax,03h
           int  33h
           ret
MouseStat  endp

;**** Set Mouse Cursor Position
; On Entry:
;   CX:  Horizontal cursor position wanted
;   DX:  Vertical cursor position wanted
; On return:
;   nothing
;
MSetCurPos proc near uses ax
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
BttnPInfo  proc near
           mov  ax,05h
           int  33h
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
BttnRInfo  proc near
           mov  ax,06h
           int  33h
           ret
BttnRInfo  endp

;**** Set Horizontal Boundries
; On Entry:
;   CX:  Minimum Horizontal position
;   DX:  Maximum Horizontal position
; On return:
;   nothing
;
SetHBndrs  proc near uses ax
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
SetVBndrs  proc near uses ax
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
SetGMask   proc near uses ax
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
SetTCursr  proc near uses ax
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
ReadMotion proc near uses ax
           mov  ax,0Bh
           int  33h
           ret
ReadMotion endp

;**** Set Interrupt Subroutine
; On Entry:
;   CX:  Interrupt mask - call your routine when: (can set more than one)
;     bit 0 - Change in cursor position
;     bit 1 - Left Button pressed
;     bit 2 - Left Button released
;     bit 3 - Right Button pressed
;     bit 4 - Right Button released
;   ES:DX  Sub Segment:offset (address)
; On return:
;  nothing
;  Note:  When the mouse interrupt sends control to your routine, it sets
;          the following registers as:
;            AX - Action indicator  (see CX settings on entry)
;            BX - Button state
;            CX - Horizontal cursor position
;            DX - Vertical cursor position
;            SI - Horizontal change
;            DI - Vertical change
;   Note:  see 'Set Alternate Sub' for a better routine
;
SetIntSub  proc near uses ax
           mov  ax,0Ch
           int  33h
           ret
SetIntSub  endp

;**** Endable Light-Pen Emulation
; On Entry:
;   nothing
; On return:
;   nothing
; Note:  This causes the mouse to behave as though it were a light pen
;          Both buttons press - Pen Down
;          At lease on button released - Pen Up
;
EnblPenEmu proc near uses ax
           mov  ax,0Dh
           int  33h
           ret
EnblPenEmu endp

;**** Disable Light-Pen Emulation
; On Entry:
;   nothing
; On return:
;   nothing
;
DisPenEmu  proc near uses ax
           mov  ax,0Eh
           int  33h
           ret
DisPenEmu  endp

;**** Set Mickey-to-Pixel Ratio  (Speed of mouse)
; On Entry:
;   CX:  Horizontal Ratio  (Move CX pixels per Mickey) (default = 8)
;   DX:  Vertical Ratio (Move DX pixels per Mickey) (default = 8)
; On return:
;   nothing
;
SetSpeed   proc near uses ax
           mov  ax,0Fh
           int  33h
           ret
SetSpeed   endp

;**** Conditional Off
; On Entry:
;   CX:  Left coordinate
;   DX:  Top coordinate
;   SI:  Right coordinate
;   DI:  Bottom coordinate
; On return:
;   nothing
; Note: When set, the mouse cursor is off when ever in this 'rectangle'
;
ConOff     proc near uses ax
           mov  ax,10h
           int  33h
           ret
ConOff     endp

;**** Set Double Speed Threshold
; On Entry:
;   DX:  Threshold value  (Mickeys per second) (default-64)
; On return:
;   nothing
;
DblSpeed   proc near uses ax
           mov  ax,13h
           int  33h
           ret
DblSpeed   endp

;**** Swap Interrupt Subs
; On Entry:
;   CX:  interrupt Mask
;   ES:DX:  Sub Segment:offset (Address)
; On return:
;   CX:  Old interrupt Mask
;   ES:DX:  Old Sub Segment:offset (Address)
;  See 'Set Interrupt sub' for more info
;
SwapInt    proc near uses ax
           mov  ax,14h
           int  33h
           ret
SwapInt    endp

;**** Get Mouse Driver Storage Needs
; On Entry:
;   nothing
; On return:
;   BX: Memory Storage needed
;
DrvSNeeds  proc near uses ax
           mov  ax,15h
           int  33h
           ret
DrvSNeeds  endp

;**** Save Mouse driver state
; On Entry:
;   ES:BX  Segment:Offset address
; On return:
;   nothing
;
SaveDState proc near uses ax
           mov  ax,16h
           int  33h
           ret
SaveDState endp

;**** Restore Mouse driver state
; On Entry:
;   ES:BX  Segment:Offset address of data stored with SaveDState
; On return:
;   nothing
;
RstrDState proc near uses ax
           mov  ax,17h
           int  33h
           ret
RstrDState endp

;**** Set Alternate Subroutine
; On Entry:
;   CX:  Interrupt mask - call your routine when: (can set more than one)
;     bit 0 - Change in cursor position
;     bit 1 - Left Button pressed
;     bit 2 - Left Button released
;     bit 3 - Right Button pressed
;     bit 4 - Right Button released
;     bit 5 - Shift key pressed while a button was pressed
;     bit 6 - Ctrl key pressed while a button was pressed
;     bit 7 - Alt key pressed while a button was pressed
;   ES:DX  Sub Segment:offset (address)
; On return:
;  Carry set
;  Note:  When the mouse interrupt sends control to your routine, it sets
;          the following registers as:
;            AX - Action indicator (see CX settings on entry (bits 0-4 only))
;            BX - Button state
;            CX - Horizontal cursor position
;            DX - Vertical cursor position
;            SI - Horizontal change
;            DI - Vertical change
;
SetAIntSub proc near uses ax
           clc
           mov  ax,18h
           int  33h
           ret
SetAIntSub endp

;**** get Alternate Interrupt Address
; On Entry:
;   CX  Interrupt mask to find
; On return:
;   AX is -1 if error, not -1 if successful
;   BX  Segment Address (if AX <> -1)
;   CX  Interrupt mask
;   DX  Offset Address (if AX <> -1)
;   ES  Old Segment Address (if AX <> -1)
;
GetAltAddr proc near
           mov  ax,19h
           int  33h
           ret
GetAltAddr endp

;**** Set Mouse Sensitivity
; On Entry:
;   BX  Horizontal Sensitivity
;   CX  Vertical Sensitivity
;   DX  Double Speed Treshold
; On return:
;   nothing
;
SetMSnstvy proc near uses ax
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
GetMSnstvy proc near uses ax
           mov  ax,1Bh
           int  33h
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
SetIntRate proc near uses ax
           mov  ax,1Ch
           int  33h
           ret
SetIntRate endp

;**** Set CRT Page Number
; On Entry:
;   BX  CTR page (video page)
; On return:
;   nothing
;
SetCRTpage proc near uses ax
           mov  ax,1Dh
           int  33h
           ret
SetCRTpage endp

;**** Get CRT Page Number
; On Entry:
;   nothing
; On return:
;   BX  CTR page (video page)
;
GetCRTpage proc near uses ax
           mov  ax,1Eh
           int  33h
           ret
GetCRTpage endp

;**** Disable Mouse Driver
; On Entry:
;   nothing
; On return:
;   AX = -1 if error, <> -1 if successful
;   ES:BX  Segment:Offset Address of Current driver
;
DisableDrv proc near
           mov  ax,1Fh
           int  33h
           ret
DisableDrv endp

;**** Endable Mouse Driver
; On Entry:
;   nothing
; On return:
;   nothing
;
EnableDrv  proc near uses ax
           mov  ax,20h
           int  33h
           ret
EnableDrv  endp

;**** Software reset
; On Entry:
;   nothing
; On return:
;   AX = 0FFFFh if successful, 21h if not (unchanged)
;   BX = 02h
;
ResetSoft  proc near
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
SetLang    proc near uses ax
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
GetLang    proc near uses ax
           mov  ax,23h
           int  33h
           ret
GetLang    endp

;**** Get Driver Version Info
; On Entry:
;   nothing
; On return:
;   BH = Major version number
;   BL = Minor version number
;   CH = Mouse Type
;      1-Bus, 2-Serial, 3-InPort, 4-PS/2, 5-Hewlett-Packard
;   CL = IRQ Number
;
GetDrvVer  proc near uses ax
           mov  ax,24h
           int  33h
           ret
GetDrvVer  endp

;**** Get General Driver Info
; On Entry:
;   nothing
; On return:
;   AX = Driver info
;     bits  FEDC (B-0 not used)
;           0                 - Driver was loaded as a COM file
;           1                 - Driver was loaded as a SYS file
;            0                - Non-Intergrated Mouse driver
;            1                - Intergrated Mouse Display driver
;             1               - Graphics cursor active
;             0               - Graphics not cursor active
;              0              - Software text cursor active
;              1              - Hardware text cursor active
;
GetDrvInfo proc near
           mov  ax,25h
           int  33h
           ret
GetDrvInfo endp

;**** Get Maximum Virtual Coordinates
; On Entry:
;   nothing
; On return:
;   BX = Mouse Driver status
;   CX = Maximum Horizontal coordinate
;   DX = Maximum Vertical coordinate
;
GetMaxVCds proc near uses ax
           mov  ax,26h
           int  33h
           ret
GetMaxVCds endp

;**** Get Cursor Masks and Mickey Counts
; On Entry:
;   nothing
; On return:
;   AX = Screen Mask or beginning scan line
;   BX = Cursor Mask or ending scan line
;   CX = Horizontal mickey count
;   DX = Vertical mickey count
;
GetCMsMCnt proc near
           mov  ax,27h
           int  33h
           ret
GetCMsMCnt endp

;**** Set Video mode
; On Entry:
;   CX = Video mode
;   DX = Font size
; On return:
;   CX = 0 if successful
;
SetVidMd   proc near uses ax
           mov  ax,28h
           int  33h
           ret
SetVidMd   endp

;**** Get supported Video modes
; On Entry:
;   CX = Search flag
; On return:
;   BX = Description segment
;   CX = Mode number
;   DX = Description offset
;
GetSVidM   proc near uses ax
           mov  ax,29h
           int  33h
           ret
GetSVidM   endp

;**** Get Cursor Hot Spot
; On Entry:
;   nothing
; On return:
;   AX = Display flag
;   BX = Horizontal Hot Spot
;   CX = Horizontal Hot Spot
;   DX = Mouse type
;
GetCurHSpt proc near
           mov  ax,2Ah
           int  33h
           ret
GetCurHSpt endp

;**** Set Acceleration Curves
; On Entry:
;   AX = 0 if successful, <> 0 if error
;   BX = Curv Number
;   ES:SI Segment:Offset of Buffer  (324 byte buffer)
;    Offset 00h  01h byte   Curve 1 counts and factors
;           01h  01h byte   Curve 2 counts and factors
;           02h  01h byte   Curve 3 counts and factors
;           03h  01h byte   Curve 4 counts and factors
;           04h  20h bytes  Curve 1 mouse-count threshold array
;           24h  20h bytes  Curve 2 mouse-count threshold array
;           44h  20h bytes  Curve 3 mouse-count threshold array
;           64h  20h bytes  Curve 4 mouse-count threshold array
;           84h  20h bytes  Curve 1 scale factor array
;           A4h  20h bytes  Curve 2 scale factor array
;           C4h  20h bytes  Curve 3 scale factor array
;           E4h  20h bytes  Curve 4 scale factor array
;          104h  10h bytes  Curve 1 name (right padded ascii)
;          114h  10h bytes  Curve 2 name (right padded ascii)
;          124h  10h bytes  Curve 3 name (right padded ascii)
;          134h  10h bytes  Curve 4 name (right padded ascii)
; On return:
;   nothing
;
SetAccCrvs proc near
           mov  ax,2Bh
           int  33h
           ret
SetAccCrvs endp

;**** Get Acceleration Curves
; On Entry:
;   nothing
; On return:
;   AX = 0 if successful, <> 0 if error
;   BX = Curv Number
;   ES:SI Segment:Offset of Buffer  (324 byte buffer)
;     See Set Acceleration Curves
;
GetAccCrvs proc near
           mov  ax,2Ch
           int  33h
           ret
GetAccCrvs endp

;**** Set/Get Active Curve
; On Entry:
;   BX = Action Flag
; On return:
;   AX = 0 if successful, <> 0 if error
;   ES:SI Segment:Offset Curve name string offset
;
SGActvACrv proc near
           mov  ax,2Dh
           int  33h
           ret
SGActvACrv endp

;**** Mouse Hardware Reset
; On Entry:
;   nothing
; On return:
;   AX = 0FFFFh if successful, <> 0 if error
;
ResetHard  proc near
           mov  ax,2Fh
           int  33h
           ret
ResetHard  endp

;**** Set or Get BallPoint Information
; On Entry:
;   BX = Rotation angle
;   CX = Command
;       0 = get information
;   non 0 = set information
; On return:
;   AX = Button state if successful, <> 0FFFFh if error
;   BX = Rotation angle
;   CX = Buttons masks
;
SGBallInfo proc near
           mov  ax,30h
           int  33h
           ret
SGBallInfo endp

;**** Get Virtual Coordinates
; On Entry:
;   nothing
; On return:
;   AX = Minimum Horizontal Coordinate
;   BX = Minimum Vertical Coordinate
;   CX = Maximum Horizontal Coordinate
;   DX = Maximum Vertical Coordinate
;
GetVCords  proc near
           mov  ax,31h
           int  33h
           ret
GetVCords  endp

;**** Get Active Advanced Functions
; On Entry:
;   nothing
; On return:
;   AX = Function Flags
;  Bits FEDCBA9876543210
;       x                Is Function 34h available
;        x               Is Function 33h available
;         x              Is Function 33h available
;          ...........     (32h - 27h)
;                     x  Is Function 26h available
;                      x Is Function 25h available
;
GetAAFuncs proc near
           mov  ax,32h
           int  33h
           ret
GetAAFuncs endp

;**** Get Switch Settings
; On Entry:
;   CX = Buffer length
;   ES:DX = Buffer segment:offset
; On return:
;   CX = Bytes in buffer
;    Offset 00h  01 byte   Mouse type and port
;    Offset 01h  01 byte   Language
;    Offset 02h  01 byte   Horizontal sensitivity
;    Offset 03h  01 byte   Vertical sensitivity
;    Offset 04h  01 byte   Double speed threshold
;    Offset 05h  01 byte   ballistic curve
;    Offset 06h  01 byte   Interrupt rate
;    Offset 07h  01 byte   Cursor mask
;    Offset 08h  01 byte   Laptop adjustment
;    Offset 09h  01 byte   Memory type
;    Offset 0Ah  01 byte   Super VGA flag
;    Offset 0Bh  02 byte   Rotation angle
;    Offset 0Dh  01 byte   Primary button
;    Offset 0Eh  01 byte   Secondary button
;    Offset 0Fh  01 byte   Click lock enabled
;    Offset 10h 324 byte   Acceleration curves tables
;
GetSwitch  proc near
           mov  ax,33h
           int  33h
           ret
GetSwitch  endp

;**** Get MOUSE.INI
; On Entry:
;   nothing
; On return:
;   ES:DX = Buffer Segment:Offset
;
GetMINI    proc near uses ax
           mov  ax,34h
           int  33h
           ret
GetMINI    endp
           end
