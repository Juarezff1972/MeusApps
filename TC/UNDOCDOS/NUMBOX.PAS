(*   Program Name    *******   NumBoxP.Pas    *******   *)

Program Box;

(*  This program will 'put' numbers in the 'EQUAL BOX'           *)
(*  You may put any ODD number of Rows or Columns less than 21   *)

Var   Rows, ToNum, Cnt, R, C, T, TRows, Total:  Integer;
      Table : Array [1..21, 1..21] of Integer;
      Half : Real;

Begin
  Writeln ('**********************');
  Writeln ('*                    *');
  Writeln ('*       BOXS         *');
  Writeln ('*        by          *');
  Writeln ('*    Forever Young   *');
  Writeln ('*      Software      *');
  Writeln ('*        by          *');
  Writeln ('*      Ben Lunt      *');
  Writeln ('*                    *');
  Writeln ('**********************');
  Writeln;
  Writeln;
  Repeat
    Write ('Please enter the number on Rows: ');
    Readln (Rows);
  Until (Rows > 2) and (Rows < 22) and ((Rows mod 2) <> 0);
  Writeln;

(*  This part does the calculations *)
  For R:= 1 to Rows DO
    For C:= 1 to Rows DO
      Table[R,C]:= 0;
  R:= 1;
  TRows:= Rows*Rows;
  Half:= (Rows / 2) + (1 / 2);
  C:= Trunc(Half);
  ToNum:= 1;
  Table[R,C]:= ToNum;
  For ToNum:= 2 to TRows DO
    Begin
      C:= C + 1;
      R:= R - 1;
      IF ((C > Rows) AND (R = 0)) THEN
        Begin
          C:= C - 1;
          R:= R + 2;
        end;
      IF (C > Rows) THEN
        C:= C - Rows;
      IF (R = 0) THEN
        R:= R + Rows;
      IF (Table[R, C] > 0) THEN
        Begin
          C:= C - 1;
          R:= R + 2;
        end;
      Table[R,C]:= ToNum;
    end;

(*  This part prints the numbers  *)
  Total:= 0;
  For R:= 1 to Rows DO
    Begin
      For C:=1 to Rows DO
       Write (Table[R,C], ' ');
       Writeln;
    end;
  C:= 1;
  For R:= 1 to Rows DO
    Total :=Total + Table[R,C];
  Writeln;
  Writeln;
  Writeln ('The Total is ', Total);
  Readln;
End.

