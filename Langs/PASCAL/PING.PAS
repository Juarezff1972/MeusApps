uses crt,dos;
type t=array[1..2,1..1000] of byte;
const num=7;
var op : char;
    k : integer;
    CaracAtrib:t;
    collin:t;
    xy:t;

Procedure Le_Caracter(n:integer);
var reg : registers;
begin
  reg.ah:=8;
  reg.bh:=0;
  intr($10,reg);
  caracatrib[1,n]:=reg.al;
  caracatrib[2,n]:=reg.ah
end;

procedure CursorSize(inicio,fim:byte);
var reg:registers;
begin
  reg.ch:=inicio;
  reg.cl:=fim;
  reg.ah:=1;
  intr($10,reg);
end;

procedure Escreve_Caracter(carac,atrib:byte);
var reg:registers;
begin
  reg.al:=carac;
  reg.bh:=0;
  reg.bl:=atrib;
  reg.ah:=9;
  reg.cx:=1;
  intr($10,reg)
end;

Procedure Partida;
var a,b:byte;
begin
  op:=#0;
  a:=WhereX;
  b:=WhereY;
  CursorSize(32,0);
  Randomize;
  for k:=1 to num do begin
                     xy[1,k]:=random(2)+1;
                     xy[2,k]:=random(2)+1;
                     collin[1,k]:=random(80)+1;
                     collin[2,k]:=random(24)+1;
                   end;
  Repeat
    for k:=1 to num do begin
                       GotoXY(Collin[1,k],colLin[2,k]);
                       Le_Caracter(k);
                       Escreve_Caracter(7,caracatrib[2,k])
                     end;
    delay(50);
    for k:=num downto 1 do begin
                       gotoxy(Collin[1,k],collin[2,k]);
                       Escreve_caracter(caracatrib[1,k],caracatrib[2,k]);
                       inc(collin[1,k],xy[1,k]);
                       inc(collin[2,k],xy[2,k])
                     end;
    for k:=1 to num do
     begin
       if Collin[1,k]>80 then begin
                                Collin[1,k]:=80;
                                xy[1,k]:=-(random(2)+1)
                              end;
       if Collin[1,k]<1 then begin
                               Collin[1,k]:=1;
                               xy[1,k]:=random(2)+1
                             end;
       if colLin[2,k]<1 then begin
                               colLin[2,k]:=1;
                               xy[2,k]:=random(2)+1
                             end;
       if colLin[2,k]>24 then begin
                               colLin[2,k]:=24;
                               xy[2,k]:=-(random(2)+1)
                              end
     end;
    if KeyPressed then op:=Readkey;
  Until op=#222;
  GotoXY(a,b);
  CursorSize(6,7)
end;

begin
  partida
end.