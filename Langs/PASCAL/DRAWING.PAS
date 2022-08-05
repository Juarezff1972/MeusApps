program Drawing;

uses graph;

var x,y,z,a : integer;
    pen     : boolean;
    gd,gm   : integer;

procedure Coords;
begin
   z:=trunc(639*(x/2500));
   a:=trunc(299*(y/1746));
end;

procedure RE;
begin
   moveto(640,400);
end;

procedure HO;
begin
   moveto(0,0);
end;

procedure PU;
begin
   pen:=false;
end;

procedure MA(u,v : integer);
begin
   x:=u;
   y:=v;
   coords;
   moveto(z,a);
end;

procedure PD;
begin
   pen:=true;
end;

procedure DA(u,v : integer);
begin
   x:=u;
   y:=v;
   coords;
   if pen=true then lineto(z,a);
end;

begin
   gd:=detect;
   initgraph(gd,gm,'');
   setcolor(15);
   x:=0;
   y:=0;
   z:=0;
   a:=0;
   RE;
   HO;
   PU;
   PU;
   MA(0,1746);
   PD;
   DA(2500,1746);
   DA(2500,0);
   DA(0,0);
   DA(0,1746);
   PU;
   MA(0,0);
end.