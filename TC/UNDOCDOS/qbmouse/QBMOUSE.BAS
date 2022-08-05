' This is a simple QB file to show how to use the QBMOUSE.LIB
'  To load in the INTERPRETER:
'    qb /l mouse qbmouse
'  To compile and link:
'    bc mouse;
'    link qbmouse,,,mouse.lib;

DECLARE FUNCTION ResetIni% ()
   '  RC = 0 then error, else RC = number of buttons
DECLARE FUNCTION MouseStat% (Horz%, Vert%)
   '  RC = 0 none, 1 left, 2 right, 3 both
   ' horz% = h pos
   ' vert% = v pos
DECLARE FUNCTION BttnPInfo% (NTms%, Horz%, Vert%)
   '  RC = 0 none, 1 left, 2 right, 3 both
   ' ntms% = number of times pressed since last call
   ' horz% = h pos (set)
   ' vert% = v pos
DECLARE FUNCTION BttnRInfo% (NTms%, Horz%, Vert%)
   '  RC = 0 none, 1 left, 2 right, 3 both
   ' ntms% = number of times released since last call
   ' horz% = h pos (set)
   ' vert% = v pos
DECLARE FUNCTION ResetSoft% ()
DECLARE FUNCTION GetLang% ()
   ' see set lang (sub)
DECLARE FUNCTION GetDrvVer% ()
   ' hi byte = major, lo byte = minor
DECLARE FUNCTION ResetHard% ()


DECLARE SUB MSetCurPos (Horz%, Vert%)
DECLARE SUB MouseOn
DECLARE SUB MouseOff
DECLARE SUB SetHBndrs (MinH%, MaxH%)
DECLARE SUB SetVBndrs (MinV%, MaxV%)
DECLARE SUB SetGMask (HorzH%, VertH%, MaskS%, MaskO%)
'  HorzH = H hot spot
'  VertH = V hot spot
'  MaskS = Segment of 64 byte mask (16 word)+(16 word)
'  MaskO = Segment of 64 byte mask
DECLARE SUB SetTCursr (CurSH%, BSnLn%, ESnLn%)
'  CurSH = 0 = software, 1 = hardware
'  BSnLn = Beginning Scan Line
'  ESnLn = Ending Scan Line
DECLARE SUB ReadMotion (HorzC%, VertC%)
'  HorzC = Horz change since last call
'  VertC = Vert change since last call
DECLARE SUB SetSpeed (Horz%, Vert%)
'  Horz = Horz Ratio (default = 8)
'  Vert = Vert Ratio
DECLARE SUB DblSpeed (ThrsHold%)
'  ThrsHold = Threshold value (default = 64)
DECLARE SUB SetMSnstvy (HorzS%, VertS%, ThrsHold%)
'  HorzS = horz Sesititvity
'  VertS = Vert Sesititvity
'  ThrsHold = Double Threshold value (default = 64)
DECLARE SUB GetMSnstvy (HorzS%, VertS%, ThrsHold%)
'  HorzS = horz Sesititvity
'  VertS = Vert Sesititvity
'  ThrsHold = Double Threshold value (default = 64)
DECLARE SUB SetIntRate (IRate%)
'  IRate =
'         0 = Turn off interrupts
'         1 = 30 interrupts per second (default)
'         2 = 50 interrupts per second
'         3 = 100 interrupts per second
'         4 = 200 interrupts per second
DECLARE SUB SetLang (Lang%)
'  Lang =
'        0 = English
'        1 = French
'        2 = Dutch
'        3 = German
'        4 = Swedish
'        5 = Finnish
'        6 = Spanish
'        7 = Portugues
'        8 = Italian
DECLARE SUB GetMaxVCds (Horz%, Vert%)
'  Horz = Maximum Horizontal coordinate
'  Vert = Maximum Vertical coordinate
DECLARE SUB GetVCords (HMax%, HMin%, VMax%, VMin%)
'  Hmax = Maximum Horizontal Coordinate
'  Hmin = Minimum Verticle Coordinate
'  Hmax = Maximum Horizontal coordinate
'  Hmin = Minimum Vertical coordinate

CLS
IF NOT ResetIni% THEN
  PRINT "Error while reseting mouse"
  END
END IF

PRINT "Mouse Language used:  ";
SELECT CASE GetLang%
  CASE 0
    PRINT "English"
  CASE 1
    PRINT "French"
  CASE 2
    PRINT "Dutch"
  CASE 3
    PRINT "German"
  CASE 4
    PRINT "Swedish"
  CASE 5
    PRINT "Finnish"
  CASE 6
    PRINT "Spanish"
  CASE 7
    PRINT "Portugues"
  CASE 8
    PRINT "Italian"
END SELECT

Ver% = GetDrvVer%
PRINT "Mouse driver version: ";
PRINT STR$((Ver% AND &HFF00) \ &H100); "."; STR$(Ver% AND &HFF)

PRINT "Setting Cursor Position to 50, 12"
MSetCurPos 50 * 8, 12 * 8

PRINT "Setting boundries at 30,5 and 70,20"
LOCATE 6, 31: PRINT "*"
LOCATE 6, 71: PRINT "*"
LOCATE 21, 31: PRINT "*"
LOCATE 21, 71: PRINT "*"

SetHBndrs 30 * 8, 70 * 8    ' remember to * 8 in text mode
SetVBndrs 5 * 8, 20 * 8    ' remember to * 8 in text mode

MouseOn

DO
  Buttons% = MouseStat%(Horz%, Vert%)
  LOCATE 10, 7
  PRINT "        Horz: "; Horz% \ 8; "   "  ' for text, we must devide by 8
  LOCATE 11, 7
  PRINT "        Vert: "; Vert% \ 8; "   "  ' for text, we must devide by 8
  LOCATE 12, 7
  PRINT " Left Button: ";
  IF (Buttons% AND &H1) = 1 THEN PRINT "Pressed" ELSE PRINT "         "
  LOCATE 13, 7
  PRINT "Right Button: ";
  IF (Buttons% AND &H2) = 2 THEN PRINT "Pressed" ELSE PRINT "         "
LOOP UNTIL INKEY$ <> ""

MouseOff
END

