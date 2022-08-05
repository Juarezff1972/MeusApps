program murphy;
{$M 8192,0,0}
uses dos,crt;
var pont : pointer;
{--------------------------------------------------------------------------}
procedure residente;
interrupt;
var pos       : byte;
	 a,b   : ^word;
	 c,d   : ^byte;
	 x,y : byte;
begin
  pos:=random(20);
  a:=ptr($40,$1a);
  b:=ptr($40,$1c);
  c:=ptr($40,a^);
  d:=ptr($40,a^+1);
  if pos=1 then d^:=72;
  if pos=2 then d^:=80;
  if pos=3 then d^:=75;
  if pos=4 then d^:=77;
  if (pos>0) and (pos<5) then
  begin
	 c^:=224;
  end;
  x:=wherex;
  y:=wherey;
  gotoxy(10,10);write(c^,'    ',d^,'      ');
  gotoxy(x,y);
end;
{--------------------------------------------------------------------------}
begin
  randomize;
  getintvec($1c,pont);
  setintvec($1c,@residente);
  keep(765);
end.