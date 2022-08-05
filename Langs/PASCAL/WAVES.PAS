program waves;

uses graph,crt;

type matriz = array[1..400] of integer;

var x0,y0,x1,y1,j,q,i : integer;
    px,py,pz,p5,a,xf,yf,zf,x,y,z : real;
    gd,gm : integer;
    ma,mi : matriz;
    cont  : boolean;


procedure dplot(x0,y0,x1,y1:integer);

var n1,d1,d2,d3,s1,s2,a1,a2 : real;

label l1,l2,l3,l4;

begin
   d1:=x1-x0;
   d2:=y1-y0;
   s1:=0;
   s2:=1;
   a1:=1;
   a2:=0;
   if d1 >= 0 then goto l1;
   a1:=-1;d1:=-d1;
l1:if d2 >= 0 then goto l2;
   d2:=-d2;
   s2:=-1;
l2:if d1 >= d2 then goto l3;
   n1:=d1;
   d1:=d2;
   d2:=n1;
   s1:=a1;
   a1:=0;
   a2:=s2;
   s2:=0;
l3:d3:=d1 / 2;
   n1:=1;
l4:putpixel(x0,y0,15);
   if (d1-n1)< 0 then exit;
   x0:=trunc(x0+a1);
   y0:=trunc(y0+a2);
   d3:=d3+d2;
   n1:=n1+1;
   if (d1-d3)> 0 then goto l4;
   d3:=d3-d1;
   x0:=trunc(x0+s1);
   y0:=trunc(y0+s2);
   goto l4;
end;

begin
   x0:=0;
   y0:=0;
   x1:=0;
   y1:=0;
   gd:=CGA; gm:=CGAhi;
   initgraph(gd,gm,'');
   cleardevice;
   px:=250;
   py:=150;
   pz:=245;
   p5:=0.5;
   a:=2*pi;
   restorecrtmode;
   write('XMAX= '); readln(xf);
   write('YMAX= '); readln(yf);
   write('ZMAX ='); readln(zf);
   setgraphmode(gm);
   for j:=0 to 400 do
     begin
        ma[j]:=0;
        mi[j]:=398;
     end;
   j:=0;
   repeat
     x:=0;
     y:=j*yf/py;
     z:=sin(x-a)*sin(y-a)+1;
     z:=z*pz/zf;
     y0:=trunc(j+trunc(z+p5));
     x0:=trunc(j);
     for i:=0 to 250 do
       begin
          q:=trunc(i+j);
          x:=i*xf/px;
          x1:=trunc(q);
          z:=sin(x-a)*sin(y-a)+1;
          z:=z*pz/zf;
          y1:=trunc(j+z+p5);
          cont := true;
          if (y1 > ma[q]) and (y1 > mi[q]) then
          begin
             x0:=x1;
             y0:=y1;
             cont:=false;
          end;
          if y1 > ma[q] then ma[q]:=y1;
          if y1 < mi[q] then mi[q]:=y1;
          if cont=true then dplot(x0,y0,x1,y1);
       end;
     j:=j+2;
   until j=150;
   readln;
   closegraph;
end.