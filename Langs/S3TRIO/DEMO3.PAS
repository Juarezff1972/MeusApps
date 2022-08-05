uses Crt, Graph;

var
   GraphDriver, GraphMode: Integer;
   ErrorCode: Integer;

begin
   GraphDriver := InstallUserDriver('S3TRIO64',NIL);
   GraphMode := 0;
   InitGraph(GraphDriver, GraphMode, '');
   ErrorCode := GraphResult;
   if (ErrorCode <> grOK) then
     begin
        Writeln('Graphics error: ', GraphErrorMsg(ErrorCode));
        Writeln('Program aborted.');
        Halt(1);
     end;

   SetActivePage(0);
   SetColor(12);
   Line(GetMaxX div 2, 0, GetMaxX div 2, GetmaxY);
   OutTextXY(GetMaxX div 2, GetMaxY div 2, 'Page 0');
   Rectangle(0,0,GetMaxX,GetMaxY);

   SetActivePage(1);
   SetColor(14);
   Line(GetMaxX,0,0,GetMaxY);
   OutTextXY(GetMaxX div 2, GetMaxY div 2, 'Page 1');
   Rectangle(0,0,GetMaxX,GetMaxY);

   SetActivePage(2);
   SetColor(15);
   Line(0,0,GetMaxX,GetMaxY);
   OutTextXY(GetMaxX div 2, GetMaxY div 2, 'Page 2');
   Rectangle(0,0,GetMaxX,GetMaxY);

   repeat
      SetVisualPage(0);
      Delay(800);
      SetVisualPage(1);
      Delay(800);
      SetVisualPage(2);
      Delay(800);
   until KeyPressed;

   ReadLn;
   CloseGraph;
end.
