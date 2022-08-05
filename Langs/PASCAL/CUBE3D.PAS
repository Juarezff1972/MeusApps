Program Cube_3D;
uses graph,crt;

type
  coordenadas = array[1..21,0..2] of integer;

var
  x,y,z,sx,sy,xa,ya,za,sxa,sya,sxb,syb,sxs,sys,temptroca,d,r1 : real;
  sr1,sr2,sr3,cr1,cr2,cr3,mx,my,mz,telax,telay,c,rx,ry,r3,r2  : real;
  sx1,sy1,sx2,sy2,xres,yres,rotacaoX,vira,rotacaoY            : real;
  maxx,minx,maxy,miny,t1,t2,pl,cor                            : integer;
  ch                                                          : char;
  auxstr                                                      : string;

const
  arr : coordenadas =
	(( 30, 30, 30),(-30, 30, 30),(-30,-30, 30),( 30,-30, 30),
  	 ( 30, 30, 30),( 30, 30,-30),(-30, 30,-30),(-30,-30,-30),
	 ( 30,-30,-30),( 30, 30,-30),( 30,-30, 30),( 30,-30,-30),
	 (-30,-30, 30),(-30,-30,-30),(-30, 30, 30),(-30, 30,-30),
    (-30, 30, 30),( 30, 30,-30),( 30, 30, 30),(-30,-30,-30),(0,0,0));

{-------------------------------------------------------------------------}
procedure rotation;
begin
   sr1:=sin(r1); sr2:=sin(r2); sr3:=sin(r3);
   cr1:=cos(r1); cr2:=cos(r2); cr3:=cos(r3);
end;
{-------------------------------------------------------------------------}
procedure calc3d;
begin
   x:=-x;
   xa:=cr1*x-sr1*z;
   za:=sr1*x+cr1*z;
   x:=cr2*xa+sr2*y;
   ya:=cr2*y-sr2*xa;
   z:=cr3*za-sr3*ya;
   y:=sr3*za+cr3*ya;
   x:=x+mx;
   y:=y+my;
   z:=z+mz;
   sx:=d*x/z;
   sy:=d*y/z;
end;
{-------------------------------------------------------------------------}
procedure tela;
begin
   sx:=sx+399; sy:=sy+299; rx:=telax/799; ry:=telay/599; sx:=sx*rx;
   sy:=sy*ry;
end;
{-------------------------------------------------------------------------}
procedure viewport;
begin
   if (sxa>sxb) then
      begin
         temptroca:=sxa;
         sxa:=sxb; sxb:=temptroca;
         temptroca:=sya;
         sya:=syb;
         syb:=temptroca;
      end;
   if (sxa<minx) and (sxb<minx) then exit;
   if (sxa>maxx) and (sxb>maxx) then exit;
   if (sya<miny) and (syb<miny) then exit;
   if (sya>maxy) and (syb>maxy) then exit;
   if (sxa<minx) then
      begin
         begin
            c:=(syb-sya)/(sxb-sxa)*(sxb-minx);
            sxa:=minx;
            sya:=syb-c;
         end;
        if (sya<miny) and (syb<miny) then exit;
        if (sya>maxy) and (syb>maxy) then exit;
      end;
      if (sxb>maxx) then
      begin
         begin
            c:=(syb-sya)/(sxb-sxa)*(maxx-sxa);
            sxb:=maxx;
            syb:=sya-c;
         end;
        if (sya<miny) and (syb<miny) then exit;
        if (sya>maxy) and (syb>maxy) then exit;
      end;
      if (sya>syb) then
      begin
         temptroca:=sya;
         sya:=syb;
         syb:=temptroca;
         temptroca:=sxa;
         sxa:=sxb;
         sxb:=temptroca;
      end;
      if (sya<miny) then
      begin
         c:=(sxb-sxa)/(syb-sya)*(syb-miny);
         sxa:=sxb-c;
         sya:=miny;
      end;
      if (syb>maxy) then
      begin
         c:=(sxb-sxa)/(syb-sya)*(maxy-sya);
         sxb:=sxa-c;
         syb:=maxy;
      end;
end;
{-------------------------------------------------------------------------}
procedure GraphicsSetup;
var gd,gm : integer;
    mx,my : string;
begin
   gd:=detect;
   initgraph(gd,gm,'c:\langs\egavga.bgi');
      case gd of
      cga    : gm:=cgac3;
      mcga   : gm:=mcgac3;
      vga    : gm:=vgahi;
      ega    : gm:=egahi;
      att400 : gm:=att400c3;
   end;
   setgraphmode(gm);
   xres:=getmaxx+1;
   yres:=getmaxy+1;
   minx:=1;
   miny:=1;
   maxx:=getmaxx-2;
   maxy:=getmaxy-2;
   telax:=getmaxx;
   telay:=getmaxy;
   setcolor(7);
   str(yres:1:0,my);
   str(xres:1:0,mx);
   outtextxy(0,0,mx+' x '+my+' Graphics Mode');
end;
{-------------------------------------------------------------------------}
procedure Coords;
begin
   sx:=sx*(xres/640);
   sy:=sy*(yres/480);
end;
{-------------------------------------------------------------------------}
begin
   x:=0; y:=0; z:=0; sx:=0; sy:=0; xa:=0; ya:=0; za:=0;
   sxa:=0; sxb:=0; sya:=0; syb:=0; sxs:=0; sys:=0; temptroca:=0;
   d:=1200; r1:=0; r2:=0; r3:=0;
   sr1:=0; sr2:=0; sr3:=0; cr1:=0; cr2:=0; cr3:=0;
   mx:=0; my:=0; mz:=-350; maxx:=638; minx:=1; maxy:=198; miny:=1;
   telax:=639; telay:=199; c:=0; rx:=0; ry:=0; t1:=0; t2:=0; pl:=0;
   rotacaoX:=0; vira:=0; rotacaoY:=0;
   GraphicsSetup;
   cor:=1;
 repeat
   cor:=1;
   if ((ch='j')or(ch='k')or(ch='m')or
       (ch='i')or(ch='m')or(ch=']')or(ch='[')) then cor:=0;
 repeat
   pl:=1;
   Setcolor(7); sx:=0; sy:=24; coords; sx1:=sx; sy1:=sy;
   sx:=638; sy:=455; coords; sx2:=sx; sy2:=sy;
   rectangle(trunc(sx1),trunc(sy1),trunc(sx2),trunc(sy2));
   rotation;
   for t2:=1 to 4 do
      begin
         x:=arr[pl,0]; y:=arr[pl,1]; z:=arr[pl,2];
         calc3d;
         tela;
         sxa:=sx;
         sya:=sy;
         for t1:=1 to 4 do
            begin
               str(pl,auxstr);
               auxstr:='/PL='+auxstr;
               setcolor(cor*14);
               outtextxy(trunc(sxa),trunc(sya+(5*pl)),auxstr);
               pl:=pl+1;
               setcolor(cor*7);
               if pl=1 then setcolor(0);
               x:=arr[pl,0]; y:=arr[pl,1]; z:=arr[pl,2];
               calc3d;
               tela;
               sxs:=sx; sys:=sy; sxb:=sx; syb:=sy;
               viewport;

               if (sxa>sxb)and(sya>syb) then setcolor(cor*14);  {14}
               if (sxa>sxb)and(sya<syb) then setcolor(cor*13);  {13}
               if (sxa>sxb)and(sya=syb) then setcolor(cor*8);   {18}

               if (sxa<sxb)and(sya>syb) then setcolor(cor*12);  {12}
               if (sxa<sxb)and(sya<syb) then setcolor(cor*11);  {11}
               if (sxa<sxb)and(sya=syb) then setcolor(cor*6);   { 6}

               if (sxa=sxb)and(sya>syb) then setcolor(cor*10);  {10}
               if (sxa=sxb)and(sya<syb) then setcolor(cor*9);   { 9}
               if (sxa=sxb)and(sya=syb) then setcolor(cor*5);   { 5}

               moveto(trunc(sxa),trunc(sya));
               lineto(trunc(sxb),trunc(syb));

               sxa:=sxs;
               sya:=sys;
            end;
         pl:=pl+1;
      end;
   rotacaoX:=0;
   vira:=0;
   rotacaoY:=0;
   cor:=cor+1;
   if ch='j' then rotacaoX:=-0.05;
   if ch='k' then rotacaoX:=+0.05;
   if ch='i' then rotacaoY:=+0.05;
   if ch='m' then rotacaoY:=-0.05;
   if ch='[' then vira:=+0.05;
   if ch=']' then vira:=-0.05;
   ch:=#00;
   if r1>(2*pi) then r1:=r1-(2*pi);
   if r2>(2*pi) then r2:=r2-(2*pi);
   if r3>(2*pi) then r3:=r3-(2*pi);
   r1:=r1+rotacaoX;
   r2:=r2+vira;
   r3:=r3+rotacaoY;
  until cor=2;
   if cor>1 then cor:=1;
   ch:=readkey;
 until ch=#27;
   closegraph;
end.
