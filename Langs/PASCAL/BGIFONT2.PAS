{ Copyright (c) 1985, 1990 by Borland International, Inc. }
unit BGIFont2;
{ Sample unit to accompany BGILINK.PAS. This unit links all the BGI graphics
  fonts into a single TPU file. This makes it easy to incorporate the font
  files directly into an .EXE file. See BGILINK.PAS for more information.
}
interface
uses graph;
var boldfacefont,europeanfont,lcomfont,
    scriptfont,simplexfont,italicfont : integer;
procedure BoldFontProc;
procedure EuropeanFontProc;
procedure LComFontProc;
procedure ScriptFontProc;
procedure SimplexFontProc;
procedure ItalicFontProc;
implementation
procedure BoldFontProc; external;
{$L BOLD.OBJ }
procedure EuropeanFontProc; external;
{$L EURO.OBJ }
procedure LComFontProc; external;
{$L LCOM.OBJ }
procedure ScriptFontProc; external;
{$L SCRI.OBJ }
procedure SimplexFontProc; external;
{$L SIMP.OBJ }
procedure ItalicFontProc; external;
{$L TSCR.OBJ }
begin
   boldfacefont:=InstallUserFont('Bold');
   europeanfont:=InstallUserFont('Euro');
   lcomfont:=InstallUserFont('LCom');
   scriptfont:=InstallUserFont('Scri');
   simplexfont:=InstallUserFont('Simp');
   italicfont:=InstallUserFont('tscr');
end.
