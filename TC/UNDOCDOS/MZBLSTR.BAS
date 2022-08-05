'   Program Name    *******   Mzblstr.Bas    *******

DEFINT A-Z

CLS
SCREEN 0
CLS

COLOR 29
LOCATE 8, 34
PRINT "Maze Blaster"
COLOR 13
LOCATE 10, 39
PRINT "by"
LOCATE , 29
PRINT "Forever Young Software"
LOCATE , 36
PRINT "Ben Lunt"
SLEEP 5
WHILE INKEY$ <> "": WEND
CLS
COLOR 9
LOCATE 5
PRINT "Please enter your maze using the asteris ('*') as the walls.  Please"
PRINT "make sure that you include the START ('S') and the TARGET ('T')."
DO
  LOCATE 8
  PRINT "Please enter the height of the maze (5 - 20)       "
  LOCATE 8, 46
  LINE INPUT H$
LOOP UNTIL VAL(H$) < 21 AND VAL(H$) > 4
H = VAL(H$)
DO
  LOCATE 10
  PRINT "Please enter the width of the maze (5 - 75)       "
  LOCATE 10, 45
  LINE INPUT W$
LOOP UNTIL VAL(W$) < 76 AND VAL(W$) > 4
W = VAL(W$)

Rc = W * H
DIM M$(Rc), Sk(Rc)
Hh = 0
Cc$ = "+"

CLS
LOCATE 1
PRINT "Please enter your maze:"
LOCATE 2
FOR I = 1 TO H
  LINE INPUT R$
  IF LEN(R$) > 0 THEN
    FOR Rt = 1 TO LEN(R$)
      IF MID$(R$, Rt, 1) = "s" THEN MID$(R$, Rt, 1) = "S"
      IF MID$(R$, Rt, 1) = "t" THEN MID$(R$, Rt, 1) = "T"
    NEXT Rt
    IF LEFT$(R$, 1) = "*" THEN
      R$ = R$ + SPACE$(W - 1)
      FOR J = 1 TO W
        Hh = Hh + 1
        M$(Hh) = MID$(R$, J, 1)
      NEXT
    ELSE
      PRINT "Maze must be enclosed by '*'"
      GOTO Endd
    END IF
  ELSE
    PRINT "Maze must be enclosed by '*'"
    GOTO Endd
  END IF
NEXT
FOR I = 1 TO W
  M$(I) = "*"
  M$((H - 1) * W + I) = "*"
NEXT
FOR I = 1 TO H
  M$((I - 1) * W + 1) = "*"
  M$((I - 1) * W + W) = "*"
NEXT
PRINT
PRINT "My Solution:"
PRINT
FOR I = 1 TO Rc
  IF M$(I) = "T" THEN GOTO Nextt
NEXT
PRINT "No TARGET"
GOTO Endd

Nextt:
T = I
FOR I = 1 TO Rc
  IF M$(I) = "S" THEN GOTO Next1
NEXT
PRINT "No START"
GOTO Endd

Next1:
S = I
FOR I = 1 TO Rc
  Sk(I) = 0
NEXT
K = 0
L = 1
I = T
DO
  IF M$(I - W) = "S" THEN
    I = I - W
    J = I + W
    GOTO Writ
  END IF
  IF M$(I - 1) = "S" THEN
    I = I - 1
    J = I + 1
    GOTO Writ
  END IF
  IF M$(I + W) = "S" THEN
    I = I + W
    J = I - W
    GOTO Writ
  END IF
  IF M$(I + 1) = "S" THEN
    I = I + 1
    J = I - 1
    GOTO Writ
  END IF
  IF M$(I - W) = " " THEN
    M$(I - W) = "1"
    Sk(L) = I - W
    L = L + 1
  END IF
  IF M$(I - 1) = " " THEN
    M$(I - 1) = "2"
    Sk(L) = I - 1
    L = L + 1
  END IF
  IF M$(I + W) = " " THEN
    M$(I + W) = "3"
    Sk(L) = I + W
    L = L + 1
  END IF
  IF M$(I + 1) = " " THEN
    M$(I + 1) = "4"
    Sk(L) = I + 1
    L = L + 1
  END IF
  K = K + 1
  I = Sk(K)
LOOP UNTIL I <= 0
IF Edo = 1 THEN
  PRINT "I can't get to the TARGET"
  GOTO Endd
END IF

Writ:
M$(I) = Cc$
I = J
IF M$(I) = "1" THEN J = I + W: GOTO Writ
IF M$(I) = "2" THEN J = I + 1: GOTO Writ
IF M$(I) = "3" THEN J = I - W: GOTO Writ
IF M$(I) = "4" THEN J = I - 1: GOTO Writ
M$(T) = "T"
M$(S) = "S"
FOR I = 1 TO Rc
  IF M$(I) = "1" OR M$(I) = "2" OR M$(I) = "3" OR M$(I) = "4" THEN M$(I) = " "
NEXT
FOR U = 1 TO H
  FOR V = 1 TO W
    PRINT M$((U - 1) * W + V);
  NEXT
  PRINT
NEXT

Endd:
V$ = INPUT$(1)
PRINT "Press a key to continue..."
COLOR 7, 0
CLS
END

