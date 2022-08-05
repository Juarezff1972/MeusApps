unit Drawer;

interface

procedure Coords;
procedure RE;
procedure HO;
procedure PU;
procedure MA(u,v : integer);
procedure PD;
procedure DA(u,v : integer);
procedure comece;

implementation

uses graph;

var x,y,z,a : integer;
    pen     : boolean;
    gd,gm   : integer;

procedure Coords;
begin
   z:=trunc(getmaxx*(x/2500));
   a:=getmaxy-trunc(getmaxy*(y/1746));
end;

procedure RE;
begin
   moveto(0,0);
end;

procedure HO;
begin
   moveto(getmaxx,getmaxy);
end;

procedure PU;
begin
   pen:=false;
end;

procedure MA;
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

procedure DA;
begin
   x:=u;
   y:=v;
   coords;
   if pen=true then lineto(z,a);
end;

procedure comece;
begin
   gd:=detect;
   initgraph(gd,gm,'');
   setcolor(15);
   x:=0;
   y:=0;
   z:=0;
   a:=0;
end;

begin
end.