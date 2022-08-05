program desenho;

uses crt,graph;{,printer;}

type gg = array[1..100,1..100] of real;

var
   largura,altura,va,xfal,zfal,inicio,xvelho,xnovo : real;
   xbase,x,zvelho,znovo,zbase,z,c,y                : real;
   g : gg;
   tamanho,prof,a,b,zz,xx,graphdriver,graphmode    : integer;

function sign(x : real) : integer;
begin
   if x = 0 then sign := 0;
   if x > 0 then sign := 1;
   if x < 0 then sign := -1;
end;
begin
   for xx := 1 to 100 do
      for zz := 1 to 100 do
         g[xx,zz] := 0;

   largura := 280; altura := 190; va := -1;

   xfal := 5; zfal := 3;

   tamanho := trunc(largura/xfal/2);

   prof := trunc(altura/zfal/3);

   graphdriver := detect;
   detectgraph(graphdriver,graphmode);
   initgraph(graphdriver,graphmode,'');
   cleardevice;
   outtext('Aguarde um minuto, por favor.');
   outtext(' Estou pensando');

   inicio := 5;
{   writeln(lst,'HO;PU');}

   for a := -trunc(prof/2) to trunc(prof/2) do
      for b := -trunc(tamanho/2) to trunc(tamanho/2) do
         begin
            x := a*18/tamanho; z := b*18/prof;
            c := x*x+z*z+0.00001;
            y := (sign(trunc(13/c)))/3+(sign(trunc(35/c)))/15;
            G[trunc(b+tamanho/2),trunc(a+prof/2)] := y*va*altura;
         end;

   cleardevice;

   for zz := 1 to prof do
      begin
         xbase := xfal*zz;
         zbase := altura/2+zz*zfal+inicio*va;
         xvelho := xbase + xfal;
         zvelho := 191-zbase+zfal+g[1,zz];
         for xx := 1 to tamanho do
            begin
               xnovo := xbase+xx*xfal;
               znovo := 191-zbase+xx*zfal+g[xx,zz];
               line(2*trunc(xvelho),trunc(zvelho),2*trunc(xnovo),trunc(znovo));
{               writeln(lst,'MA',5*2*trunc(xvelho),',',5*trunc(zvelho),';');
               writeln(lst,'PD;DA',5*2*trunc(xnovo),',',5*trunc(znovo),';');}
               zvelho := znovo; xvelho := xnovo;
            end;
{            writeln(lst,'PU');}
      end;

   readln;

   for xx := 1 to tamanho do
      begin
         xbase := xfal*xx+prof*xfal;
         zbase := altura/2-xx*zfal+prof*zfal+inicio*va;
         zvelho := 191-zbase+zfal+g[xx,prof-1];
         xvelho := xbase-xfal;
         for zz := 0 to prof-1 do
         begin
            xnovo := xbase-zz*xfal;
            znovo := 191-zbase+zz*zfal+g[xx,prof-zz];
            line(2*trunc(xvelho),trunc(zvelho),2*trunc(xnovo),trunc(znovo));
 {           writeln(lst,'MA',5*2*trunc(xvelho),',',5*trunc(zvelho),';');
            writeln(lst,'PD;DA',5*2*trunc(xnovo),',',5*trunc(znovo),';');}
            xvelho := xnovo; zvelho := znovo;
         end;
{         writeln(lst,'PU');}
      end;
   readln;
   closegraph;
 {  writeln(lst,'PU;PU;RE;');}
end.
