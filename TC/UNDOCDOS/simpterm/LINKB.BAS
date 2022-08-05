' A 'simple' terminal program
' By Ben Lunt
' Forever Young Software
' Version 1.0
' 02 Jan 1998


ON COM(2) GOSUB DISPLAY

Temp$ = ""
A$ = ""

OPEN "COM2: 9600,n,8,1,BIN,RB32767" FOR RANDOM AS #1
  COM(2) ON
 
  PRINT #1, "+++atdt555-5555" + CHR$(13)

  A$ = INKEY$
  WHILE (A$ <> CHR$(27))
    PRINT #1, A$;
    A$ = INKEY$
  WEND
 
  PRINT #1, "atz" + CHR$(13)
 
CLOSE #1
COM(2) OFF
END

DISPLAY:
  Temp$ = INPUT$(LOC(1), #1)
  PRINT Temp$;
RETURN

