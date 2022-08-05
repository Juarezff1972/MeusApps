' this is a demo on how to do menus

DECLARE FUNCTION MakeMenu% (X%, Y%, Items$())

DIM Items$(10)
Items$(1) = "New"
Items$(2) = "Open"
Items$(3) = "Save"
Items$(4) = "Save as"
Items$(5) = "Close"
Items$(6) = " "
Items$(7) = "Print"
Items$(8) = "Goto DOS Shell"
Items$(9) = "Exit"
Items$(10) = ""

CLS
ItemNum% = MakeMenu%(5, 2, Items$())
LOCATE 19
PRINT "Returned Item number: "; ItemNum%, Items$(ItemNum%)

FUNCTION MakeMenu% (X%, Y%, Items$())
  Cnt% = 0
  Longest% = 0
  DO
    Cnt% = Cnt% + 1
    IF LEN(Items$(Cnt%)) > Longest% THEN Longest% = LEN(Items$(Cnt%))
  LOOP UNTIL LEN(Items$(Cnt%)) = 0
  Cnt% = Cnt% - 1

  COLOR 7, 0
  LOCATE Y%, X%
  PRINT "É"; STRING$(Longest% + 2, "Í"); "»"
  FOR R% = 1 TO Cnt%
    LOCATE , X%
    IF Items$(R%) = " " THEN
    PRINT "Ç"; STRING$(Longest% + 2, "Ä"); "¶"
    ELSE PRINT "º "; Items$(R%); SPACE$(Longest% - LEN(Items$(R%))); " º"
    END IF
  NEXT R%
  LOCATE , X%
  PRINT "È"; STRING$(Longest% + 2, "Í"); "¼"

  ItemLine% = 1
  WHILE Items$(ItemLine%) = " "
    ItemLine% = ItemLine% + 1
  WEND
  DO
    LOCATE Y% + ItemLine%, X% + 2
    COLOR 0, 7
    PRINT Items$(ItemLine%); SPACE$(Longest% - LEN(Items$(ItemLine%)))
    V$ = INPUT$(1)
    LOCATE Y% + ItemLine%, X% + 2
    COLOR 7, 0
    PRINT Items$(ItemLine%); SPACE$(Longest% - LEN(Items$(ItemLine%)))
    PrevLine% = ItemLine%
    SELECT CASE V$
      CASE "8"
        IF ItemLine% > 1 THEN
          DO
            ItemLine% = ItemLine% - 1
            IF ItemLine% < 1 THEN ItemLine% = PrevLine%: EXIT DO
          LOOP UNTIL Items$(ItemLine%) <> " "
        END IF
      CASE "2"
        IF ItemLine% < Cnt% THEN
          DO
            ItemLine% = ItemLine% + 1
            IF ItemLine% > Cnt% THEN ItemLine% = PrevLine%: EXIT DO
          LOOP UNTIL Items$(ItemLine%) <> " "
        END IF
      CASE CHR$(13)
        MakeMenu% = ItemLine%
        EXIT FUNCTION
    END SELECT
  LOOP
END FUNCTION

