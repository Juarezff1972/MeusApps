program Demo4;    { Demo program for S3TRIO64.BGI driver }

uses
   Graph;

const
   { Enhanced mixes for SetWriteMode }
   {--COPY------------}
   CopyPut        = 0;
   NotCopyPut     = 4;
   {--XOR-------------}
   XorPut         = 1;
   NotXorPut      = 5;
   {--OR--------------}
   OrPut          = 2;
   OrNotPut       = 6;
   NotOrPut       = 7;
   NotOrNotPut    = 8;
   {--AND-------------}
   AndPut         = 3;
   AndNotPut      = 9;
   NotAndPut      = 10;
   NotAndNotPut   = 11;
   {--SPECIAL---------}
   CurrentAsIsPut = 12;
   NotCurrentPut  = 13;
   LogicalZeroPut = 14;
   LogicalOnePut  = 15;

var
   GraphDriver, GraphMode: Integer;
   ErrorCode: Integer;

begin
   GraphDriver := InstallUserDriver('S3TRIO64', NIL);
   GraphMode := 0; { <-- select desired mode 0..4 }
   InitGraph(GraphDriver, GraphMode, '');
   ErrorCode := GraphResult;
   if (ErrorCode <> grOK) then
     begin
        Writeln('Graphics error: ', GraphErrorMsg(ErrorCode));
        Writeln('Program aborted.');
        Halt;
     end;

   SetWriteMode(OrPut);

   SetColor(Red);
   SetFillStyle(SolidFill, Red);
   FillEllipse(150, 150, 150, 150);
   SetColor(Blue);
   SetFillStyle(SolidFill, Blue);
   FillEllipse(350, 150, 150, 150);
   SetColor(Green);
   SetFillStyle(SolidFill, Green);
   FillEllipse(250, 350, 150, 150);
   SetWriteMode(CopyPut);
   SetColor(White);
   OutTextXY(10, 10, 'Draw some objects!');
   readln;
   OutTextXY(10, 20, 'Now, remove objects!');
   readln;

   SetWriteMode(AndNotPut);
   SetColor(Red);
   SetFillStyle(SolidFill, Red);
   FillEllipse(150, 150, 150, 150);
   readln;

   SetColor(Green);
   SetFillStyle(SolidFill, Green);
   FillEllipse(250, 350, 150, 150);
   readln;

   SetColor(Blue);
   SetFillStyle(SolidFill, Blue);
   FillEllipse(350, 150, 150, 150);

   SetWriteMode(CopyPut);
   SetColor(White);
   OutTextXY(10, 30, 'Removed!');

   readln;
   CloseGraph;
end.
