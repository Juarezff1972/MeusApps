Program Ambulante;
uses Graph,Crt;
var GD,GM,D : integer;
    X,A     : real;
{---------------------------------------------------------------------}
procedure FrameA;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+163),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+144),143);
         lineto(trunc(x+159),146);
         lineto(trunc(x+162),122);
         lineto(trunc(x+170),100);
         lineto(trunc(x+173),125);
         lineto(trunc(x+183),147);
         lineto(trunc(x+173),150);
         lineto(trunc(x+170),150);
         moveto(trunc(x+157),108);
         lineto(trunc(x+163),92);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+173),90);
         lineto(trunc(x+175),106);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
procedure FrameB;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+163),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+144),144);
         lineto(trunc(x+154),146);
         lineto(trunc(x+161),122);
         lineto(trunc(x+170),100);
         lineto(trunc(x+176),125);
         lineto(trunc(x+186),146);
         lineto(trunc(x+177),150);
         lineto(trunc(x+172),150);
         moveto(trunc(x+153),107);
         lineto(trunc(x+162),93);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+175),90);
         lineto(trunc(x+178),107);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
procedure FrameC;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+163),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+145),146);
         lineto(trunc(x+159),148);
         lineto(trunc(x+164),125);
         lineto(trunc(x+170),100);
         lineto(trunc(x+175),124);
         lineto(trunc(x+183),146);
         lineto(trunc(x+176),150);
         lineto(trunc(x+172),150);
         moveto(trunc(x+158),107);
         lineto(trunc(x+163),93);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+173),89);
         lineto(trunc(x+174),105);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
procedure FrameD;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+161),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+153),149);
         lineto(trunc(x+168),150);
         lineto(trunc(x+168),123);
         lineto(trunc(x+170),100);
         lineto(trunc(x+173),123);
         lineto(trunc(x+183),144);
         lineto(trunc(x+170),147);
         moveto(trunc(x+162),109);
         lineto(trunc(x+165),91);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+172),89);
         lineto(trunc(x+175),106);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
procedure FrameE;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+161),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+155),150);
         lineto(trunc(x+170),150);
         lineto(trunc(x+168),122);
         lineto(trunc(x+170),100);
         lineto(trunc(x+169),122);
         lineto(trunc(x+176),143);
         lineto(trunc(x+160),146);
         moveto(trunc(x+166),108);
         lineto(trunc(x+167),90);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+170),90);
         lineto(trunc(x+171),108);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
procedure FrameF;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+161),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+158),144);
         lineto(trunc(x+172),142);
         lineto(trunc(x+162),122);
         lineto(trunc(x+170),100);
         lineto(trunc(x+170),122);
         lineto(trunc(x+175),150);
         lineto(trunc(x+162),150);
         moveto(trunc(x+162),108);
         lineto(trunc(x+167),91);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+171),89);
         lineto(trunc(x+174),106);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
procedure FrameG;
var I : integer;
begin
   for i:=1 downto 0 do
      begin
         setcolor(15*i);
         circle(trunc(x+162),59,8);
         moveto(trunc(x+165),66);
         lineto(trunc(x+167),72);
         lineto(trunc(x+170),100);
         moveto(trunc(x+148),143);
         lineto(trunc(x+161),145);
         lineto(trunc(x+163),122);
         lineto(trunc(x+170),100);
         lineto(trunc(x+172),124);
         lineto(trunc(x+178),148);
         lineto(trunc(x+163),150);
         moveto(trunc(x+158),110);
         lineto(trunc(x+164),92);
         lineto(trunc(x+164),72);
         lineto(trunc(x+168),71);
         lineto(trunc(x+172),89);
         lineto(trunc(x+175),107);
         delay(d*i);
      end;
end;
{---------------------------------------------------------------------}
begin
   gd:=CGA;
   gm:=CGAc0;
   clrscr;
   x:=150;
   a:=2;
   write('Velocidade: (1-1000) ');
   readln(d);
   d:=1000-d;
   initgraph(gd,gm,'');
   outtextxy(5,155,'Humano Andante - Uma Animacao em Pascal');
   setcolor(15);
   line(0,151,319,151);
   repeat
      FrameA;
      x:=x-a;
      FrameB;
      x:=x-a;
      FrameC;
      x:=x-a;
      FrameD;
      x:=x-a;
      FrameE;
      x:=x-a;
      FrameF;
      x:=x-a;
      FrameG;
      x:=x-a;
      if x=-200 then x:=150;
   until keypressed;
   closegraph;
end.
