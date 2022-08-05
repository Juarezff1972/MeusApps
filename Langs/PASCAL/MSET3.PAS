program Mandelbrot_Set;

uses graph,crt;

type color = array[0..7] of integer;

var i,y,x,gd,gm       : integer;
    a,b,c,d,p,t,r,j,q : real;
    maior,igual       : integer;
    ko                : color;

begin
   for i:=0 to 3 do ko[i]:=i;
   for i:=4 to 7 do ko[i]:=i-4;
   gd:=cga;
   gm:=cgac0;
   initgraph(gd,gm,'');
   cleardevice;
   for y:=0 to 200 do {96}
      for x:=320 downto 0 do {279}
         begin
            a:=0;
            b:=0;
            c:=(x-160)/80;{-0.5;} {140}
            d:=(y-96)/48;      {96}
            i:=-1;
            repeat
                  i:=i+1;
                  q:=a*a-b*b+c;
                  b:=2*a*b+d;
                  a:=q;
                  maior:=0;
                  igual:=0;
                  if (i>3) then maior:=1;
                  if (i=7) then igual:=1;
                  p:=i+maior-igual*4;
                  putpixel(x,y,ko[trunc(p)]);
                  putpixel(x,192-y,ko[trunc(p)]);
                  putpixel(320-x,y,ko[trunc(p)]);
                  putpixel(320-x,192-y,ko[trunc(p)]);
                  t:=a*a;
                  r:=b*b;
                  j:=r+t;
                  maior:=0;
                  if (j>4) then maior:=1;
                  i:=i+(maior)*7;
            until i >= 7;
         end;
   repeat until keypressed;
   closegraph;
end.
