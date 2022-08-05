program Demo;    { Demo program for S3TRIO64.BGI driver }

uses
   Graph, CRT;

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
   s: String;

{----------------------------------------------------------------------------}

procedure WaitForKey;
var
   ch: Char;
begin
   ch := ReadKey;
   if (ch=#27) then
     begin
        CloseGraph;
        Halt;
     end;
end;

{----------------------------------------------------------------------------}

procedure Demo_SetWriteMode;
begin
   ClearViewPort;

   SetWriteMode(CopyPut);
   SetColor(LightRed);
   SetFillStyle(SolidFill, LightRed);
   FillEllipse(150, 150, 150, 150);
   SetColor(LightBlue);
   SetFillStyle(SolidFill, LightBlue);
   FillEllipse(350, 150, 150, 150);
   SetColor(LightGreen);
   SetFillStyle(SolidFill, LightGreen);
   FillEllipse(250, 350, 150, 150);
   SetWriteMode(CopyPut);
   SetColor(White);
   OutTextXY(10, 10, 'CopyPut');
   WaitForKey;

   SetWriteMode(OrPut);
   SetColor(LightRed);
   SetFillStyle(SolidFill, LightRed);
   FillEllipse(150, 150, 150, 150);
   SetColor(LightGreen);
   SetFillStyle(SolidFill, LightGreen);
   FillEllipse(250, 350, 150, 150);
   SetColor(LightBlue);
   SetFillStyle(SolidFill, LightBlue);
   FillEllipse(350, 150, 150, 150);
   SetWriteMode(CopyPut);
   SetColor(White);
   OutTextXY(10, 20, 'OrPut');
   WaitForKey;
end;

{----------------------------------------------------------------------------}

procedure Demo_Play;
begin
   ClearViewPort;
   SetWriteMode(CopyPut);

   repeat
      PutPixel(Random(GetMaxX) , Random(GetMaxY), Random(13)+1);
   until KeyPressed;
   WaitForKey;

   ClearViewPort;
   repeat
      SetColor(Random(13)+1);
      Line(Random(GetMaxX), Random(GetMaxY),
           Random(GetMaxX), Random(GetMaxY));
   until KeyPressed;
   WaitForKey;

   ClearViewPort;
   repeat
      SetColor(Random(13)+1);
      SetFillStyle(SolidFill, Random(13)+1);
      Bar(Random(GetMaxX), Random(GetMaxY),
          Random(GetMaxX), Random(GetMaxY));
   until KeyPressed;
   WaitForKey;

   ClearViewPort;
   repeat
      SetColor(Random(13)+1);
      SetFillStyle(SolidFill, Random(13)+1);
      FillEllipse(Random(GetMaxX), Random(GetMaxY), 50, 50);
   until KeyPressed;
   WaitForKey;
end;

{----------------------------------------------------------------------------}

begin
   ClrScr;
   repeat
      WriteLn;
      WriteLn('Available video modes');
      WriteLn;
      WriteLn('0) 640x480x256');
      WriteLn('1) 800x600x256');
      WriteLn('2) 1024x768x256');
      WriteLn('3) 1280x1024x256');
      WriteLn('4) 1600x1200x256');
      WriteLn('Q) Quit');
      WriteLn;
      Write('Select mode: ');
      ReadLn(s);
   until (s[1] in ['0','1','2','3','4','q','Q']);

   if ((s[1]='q') or (s[1]='Q')) then
     Halt;

   GraphMode := Ord(s[1])-48;

   GraphDriver := InstallUserDriver('S3TRIO64', NIL);
   (*GraphMode := 0; { <-- select desired mode 0..4 }*)
   InitGraph(GraphDriver, GraphMode, '');
   ErrorCode := GraphResult;
   if (ErrorCode <> grOK) then
     begin
        Writeln('Graphics error: ', GraphErrorMsg(ErrorCode));
        Writeln('Program aborted.');
        Halt(1);
     end;

   Demo_SetWriteMode;
   Demo_Play;

   CloseGraph;
end.
