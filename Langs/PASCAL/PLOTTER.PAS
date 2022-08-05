program Plotador;
uses crt,Dos,printer;
var
  gPts,i      : integer;
  gGrade      : boolean;
  gRpX,gRpY   : array[1..30] of real;
  MaxX,MaxY   : integer;
{--------------------------------------------------------------------------}
Procedure Eixos;
var x,y : integer;
begin
   x:=0;
   y:=0;
   writeln(lst,'MA',MaxX div 2,',',90,';');
   writeln(lst,'DA',MaxX div 2,',',1710,';');
   writeln(lst,'MA',0,',',MaxY div 2,';');
   writeln(lst,'DA',MaxX,',',MaxY div 2,';');

     x:=321;
     repeat
       writeln(lst,'MA',trunc(x-1)*MaxX div 640,',',((200 div 2)-1)*MaxY div 200,';');
       writeln(lst,'DA',trunc(x-1)*MaxX div 640,',',((200 div 2)+1)*MaxY div 200,';');
       writeln(lst,'MA',trunc(640-(x))*MaxX div 640,',',((200 div 2)-1)*MaxY div 200,';');
       writeln(lst,'DA',trunc(640-(x))*MaxX div 640,',',((200 div 2)+1)*MaxY div 200,';');
       x:=x+(10);
     until x >= 640;

     y:=101;
     repeat
       writeln(lst,'MA',((640 div 2)-2)*MaxX div 640,',',trunc(Y-1)*MaxY div 200,';');
       writeln(lst,'DA',((640 div 2)+2)*MaxX div 640,',',trunc(y-1)*MaxY div 200,';');
       writeln(lst,'MA',((640 div 2)-2)*MaxX div 640,',',trunc(200-(Y))*MaxY div 200,';');
       writeln(lst,'DA',((640 div 2)+2)*MaxX div 640,',',trunc(200-(y))*MaxY div 200,';');
       y:=y+(2);
     until y >= 200-10;
end;
{--------------------------------------------------------------------------}
Procedure Coordenadas(x1,y1 : real; var x2,y2 : integer);
begin
  x2:=(10*(trunc(x1)*(640 div (640 div 2)))+(640 div 2))*MaxX div 200;
  y2:=(2*(-trunc(y1)*(200 div (200 div 2)))+(200 div 2))*MaxY div 200;
end;
{--------------------------------------------------------------------------}
Procedure Desenha;
var x,y : integer;
begin
  Coordenadas(gRpX[1],gRpY[1],x,y);
  writeln(lst,'MA',trunc(x),',',trunc(y),';');
  for i:=2 to gPts do
  begin
    Coordenadas(gRpX[i],gRpY[i],x,y);
    writeln(lst,'DA',trunc(x),',',trunc(y),';');
  end;
  coordenadas(gRpX[1],gRpY[1],x,y);
  writeln(lst,'DA',trunc(x),',',trunc(y),';');
  writeln(lst,'PU;HO;');
end;
{--------------------------------------------------------------------------}
Procedure Ler;
var f : text;
    a : real;
    n : string[8];
    s : string[1];
begin
  clrscr;
  write('Qual o nome? '); readln(n);
  writeln;
  writeln('Lendo ',n,'.pic,','...');
  writeln;
  assign(f,n+'.pic');
  reset(f);
  readln(f,a);  readln(f,a);
  readln(f,a);  readln(f,a);
  readln(f,a);  readln(f,a);
  readln(f,a); readln(f,a);
  readln(f,gPts); readln(f,a);
  for i:=1 to gPts do
  begin
    readln(f,gRpX[i]);
    readln(f,gRpY[i]);
    writeln('Ponto X: ',gRpX[i]:1:1,'              Ponto Y: ',gRpY[i]:1:1);
  end;
  close(f);
  writeln;
  writeln('Plotando...');
  writeln;
  write('Voce quer os eixos? ');  readln(S);
  if (S='s') or (S='S') then gGrade:=true else gGrade:=false;
  if gGrade=true then eixos;
  desenha;
end;
{--------------------------------------------------------------------------}
begin
MaxX:=2600;
MaxY:=1800;
gPts:=3;
gGrade:=true;
Ler;
end.
