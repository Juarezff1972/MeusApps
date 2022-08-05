' This is a demo souce to show how to get a key
' press through the hardware.
' used in game programming.
' http://www.zekes.com/~blunt/index.html
'

DECLARE FUNCTION GetKeyH% ()

CLS
DO
  LOCATE 10, 10
  PRINT "Key = "; HEX$(GetKeyH%); "    "
LOOP UNTIL INKEY$ = CHR$(27)
END

