program BgiLink;

uses Graph,     { library of graphics routines }
     BGIDriv,   { all the BGI drivers }
     BGIFont,   { all the BGI fonts }
     BGIFont2;  { Extra BGI fonts }
var
  GraphDriver, GraphMode, Error : integer;

procedure Abort(Msg : string);
begin
  Writeln(Msg, ': ', GraphErrorMsg(GraphResult));
  Halt(1);
end;

begin

  { Register all the drivers }
  if RegisterBGIdriver(@CGADriverProc) < 0 then
    Abort('CGA');
  if RegisterBGIdriver(@EGAVGADriverProc) < 0 then
    Abort('EGA/VGA');
  if RegisterBGIdriver(@HercDriverProc) < 0 then
    Abort('Herc');
  if RegisterBGIdriver(@ATTDriverProc) < 0 then
    Abort('AT&T');
  if RegisterBGIdriver(@PC3270DriverProc) < 0 then
    Abort('PC 3270');
  

  { Register all the fonts }
  if RegisterBGIfont(@GothicFontProc) < 0 then
    Abort('Gothic');
  if RegisterBGIfont(@SansSerifFontProc) < 0 then
    Abort('SansSerif');
  if RegisterBGIfont(@SmallFontProc) < 0 then
    Abort('Small');
  if RegisterBGIfont(@TriplexFontProc) < 0 then
    Abort('Triplex');
  if RegisterBGIfont(@BoldFontProc) < 0 then
    Abort('Bold');
  if RegisterBGIfont(@EuropeanFontProc) < 0 then
    Abort('European');
  if RegisterBGIfont(@LComFontProc) < 0 then
    Abort('LCOM');
  if RegisterBGIfont(@ScriptFontProc) < 0 then
    Abort('Script');
  if RegisterBGIfont(@SimplexFontProc) < 0 then
    Abort('Simplex');
  if RegisterBGIfont(@ItalicFontProc) < 0 then
    Abort('Italic');

  GraphDriver := Detect;                  { autodetect the hardware }
  InitGraph(GraphDriver, GraphMode, '');  { activate graphics }
  if GraphResult <> grOk then             { any errors? }
  begin
    Writeln('Graphics init error: ', GraphErrorMsg(GraphDriver));
    Halt(1);
  end;

  MoveTo(5, 5);
  OutText('Drivers and fonts were ');
  MoveTo(5, 20);
  SetTextStyle(GothicFont, HorizDir, 4);
  OutText('Built ');
  SetTextStyle(SmallFont, HorizDir, 4);
  OutText('into ');
  SetTextStyle(TriplexFont, HorizDir, 4);
  OutText('EXE ');
  SetTextStyle(SansSerifFont, HorizDir, 4);
  OutText('file!');
  Rectangle(0, 0, GetX, GetY + TextHeight('file!') + 1);
  moveto(5,60);
  SetTextStyle(BoldFaceFont, HorizDir, 4);  OutText('Extra ');
  SetTextStyle(EuropeanFont, HorizDir, 4);  OutText('fonts ');
  SetTextStyle(LComFont, HorizDir, 4);  OutText('are ');
  SetTextStyle(ScriptFont, HorizDir, 4);  OutText('also ');
  SetTextStyle(SimplexFont, HorizDir, 4);  OutText('allowed! ');
  moveto(5,120);
  SetTextStyle(ItalicFont, HorizDir, 2);  OutText('Anything you want!');
  Readln;
  CloseGraph;
end.
