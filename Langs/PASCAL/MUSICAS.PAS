program Musicas;

uses DOS,CRT;

type perdedor = array[1..8] of integer;
     fanfarra = array[1..8] of integer;
     erro     = array[1..2] of integer;

const perdnota : perdedor = (160,215,225,215,202,215,150,130);
      perdurat : perdedor = ( 20, 40, 40, 40,120,240,120,120);

      fanfnota : fanfarra = (152,203,203,181,203,32767,161,152);
      fandurat : fanfarra = ( 70, 35, 35, 70, 75,   90,70, 70);

      erronota : erro     = (181,255);
      errdurat : erro     = (100,150);

{-----------------------------------------------------------------------}
procedure MusicaPerdedor;
var i : integer;
begin
   for i:=1 to 8 do
   begin
      writeln('sound(',500-perdnota[i],') -- delay(',perdurat[i]*5,')');
      sound(500-perdnota[i]);
      delay(perdurat[i]*5);
      nosound;
   end;
end;
{-----------------------------------------------------------------------}
procedure MusicaFanfarra;
var i : integer;
begin
   for i:=1 to 8 do
   begin
      writeln('sound(',500-fanfnota[i],') -- delay(',fandurat[i]*5,')');
      sound(500-fanfnota[i]);
      delay(fandurat[i]*5);
      nosound;
      delay(20);
   end;
end;
{-----------------------------------------------------------------------}
procedure MusicaErro;
var i : integer;
begin
   for i:=1 to 2 do
   begin
      writeln('sound(',500-erronota[i],') -- delay(',errdurat[i]*5,')');
      sound(500-erronota[i]);
      delay(errdurat[i]*5);
      nosound;
   end;
end;
{-----------------------------------------------------------------------}
begin
   MusicaPerdedor;
   readln;
   MusicaFanfarra;
   readln;
   MusicaErro;
end.

