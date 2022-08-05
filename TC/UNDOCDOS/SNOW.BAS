DECLARE FUNCTION NewOneX% ()

CONST NumFlks% = 300


SCREEN 12
RANDOMIZE TIMER
LOCATE 12, 10
COLOR 5
PRINT "This is a demo for a snow-fall effect in QB45"

LINE (10, 300)-(30, 330), 5
LINE (10, 301)-(30, 331), 5
LINE (35, 340)-(15, 370), 5
LINE (35, 341)-(15, 371), 5

CIRCLE (200, 300), 25, 1
CIRCLE (200, 300), 26, 1
CIRCLE (200, 300), 27, 1

LINE (350, 250)-(400, 300), 12, B

LINE (450, 310)-(480, 350), 13
LINE -(420, 350), 13
LINE -(450, 310), 13
LINE (419, 350)-(449, 310), 13

REDIM Snow%(NumFlks%, 2)   'number of snowflakes
FOR R% = 1 TO NumFlks%     'number of snowflakes
  Snow%(R%, 1) = NewOneX%
  StartY% = (RND * 479)
  IF StartY% > 479 THEN StartY% = 479
  IF StartY% < 0 THEN StartY% = 0
  Snow%(R%, 2) = StartY%
NEXT
DO
  FOR R% = 1 TO NumFlks%   'number of snowflakes
    Snow%(R%, 2) = Snow%(R%, 2) + 1
    IF POINT(Snow%(R%, 1), Snow%(R%, 2)) <> 0 THEN
      IF POINT(Snow%(R%, 1) + 1, Snow%(R%, 2)) = 0 AND Snow%(R%, 1) < 640 THEN
        Snow%(R%, 1) = Snow%(R%, 1) + 1
        PRESET (Snow%(R%, 1) - 1, Snow%(R%, 2) - 1)
        PSET (Snow%(R%, 1), Snow%(R%, 2)), 15
        GOTO NextLoop
      ELSE
        IF POINT(Snow%(R%, 1) - 1, Snow%(R%, 2)) = 0 AND Snow%(R%, 1) > 1 THEN
          Snow%(R%, 1) = Snow%(R%, 1) - 1
          PRESET (Snow%(R%, 1) + 1, Snow%(R%, 2) - 1)
          PSET (Snow%(R%, 1), Snow%(R%, 2)), 15
          GOTO NextLoop
        END IF
      END IF
      Snow%(R%, 2) = (RND * 10) + 25
      Snow%(R%, 1) = NewOneX%
      GOTO NextLoop
    END IF
    IF Snow%(R%, 2) > 479 THEN
      Snow%(R%, 2) = (RND * 10)
      Snow%(R%, 1) = NewOneX%
    ELSE
      PRESET (Snow%(R%, 1), Snow%(R%, 2) - 1)
      PSET (Snow%(R%, 1), Snow%(R%, 2)), 15
    END IF
NextLoop:
  NEXT
LOOP UNTIL INKEY$ <> ""

FUNCTION NewOneX%
  N% = (RND * 639)
  IF N% > 639 THEN N% = 639
  IF N% < 0 THEN N% = 0
  NewOneX% = N%
END FUNCTION
