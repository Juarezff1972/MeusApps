uses
  Graph;
var
  Driver, Mode : Integer;
  TestFont : array[1..6] of Integer;
begin
  TestFont[1] := InstallUserFont('BOLD');
  TestFont[2] := InstallUserFont('EURO');
  TestFont[3] := InstallUserFont('LCOM');
  TestFont[4] := InstallUserFont('SCRI');
  TestFont[5] := InstallUserFont('SIMP');
  TestFont[6] := InstallUserFont('TSCR');
  if GraphResult <> grOk then
  begin
    WriteLn('Error installing TestFont' +
            '(using DefaultFont)');
    ReadLn;
  end;
  Driver := Detect;
  Mode := 0;
  InitGraph(Driver, Mode, '');
  if GraphResult <> grOk then Halt(1);
  SetTextStyle(TestFont[1], HorizDir, 2);
  OutTextXY(1,1,'Boldface Font...(2)');
  SetTextStyle(TestFont[2], HorizDir, 2);
  OutTextXY(1,30,'European Font...(2)');
  SetTextStyle(TestFont[3], HorizDir, 4);
  OutTextXY(1,65,'LCom Font...(4)');
  SetTextStyle(TestFont[4], HorizDir, 4);
  OutTextXY(1,95,'Script Font...(4)');
  SetTextStyle(TestFont[5], HorizDir, 4);
  OutTextXY(1,135,'Simplex Font...(4)');
  SetTextStyle(TestFont[6], HorizDir, 4);
  OutTextXY(1,165,'Text Script Font...(4)');
  ReadLn;
  Closegraph;
end.


