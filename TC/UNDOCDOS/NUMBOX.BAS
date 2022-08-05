'   Program Name    *******   NumBoxB.Bas    *******

DEFINT A-Z

1 CLS
COLOR 13
LOCATE 10
LINE INPUT "How many rows? (Must be odd (3-19) "; Row$
Row = VAL(Row$)
IF (Row MOD 2 = 0) OR Row < 3 OR Row > 19 THEN
  PRINT "That is an incorrect number...."
  V$ = INPUT$(1)
  GOTO 1
END IF

CLS
Tot = 0
TRow = Row
Tcol = Row
Col = (Row \ 2) + 1    '   '\' = div (integer)
Row = 1
DIM Arr(TRow + 1, Tcol + 1)
Arr(Col, Row) = 1
FOR R = 2 TO TRow * Tcol
  Col = Col + 1
  Row = Row - 1
  IF (Col > Tcol) AND (Row < 1) THEN
    Col = Col - 1
    Row = Row + 2
    Arr(Col, Row) = R
    GOTO 2
  END IF
  IF Col > Tcol THEN Col = Col - Tcol
  IF Row < 1 THEN Row = TRow
  IF Arr(Col, Row) = 0 THEN
    Arr(Col, Row) = R
  ELSE
    Col = Col - 1
    Row = Row + 2
    Arr(Col, Row) = R
  END IF
2 NEXT R
Last = R - 1
PRINT : PRINT

FOR T = 1 TO Tcol
  FOR R = 1 TO TRow
    PRINT USING " ###"; Arr(R, T);
  NEXT R
  Tot = Tot + Arr(1, T)
  PRINT
NEXT T
PRINT : PRINT
PRINT "Using numbers 1 -"; Last
PRINT " Total = "; Tot


