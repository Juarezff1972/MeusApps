Program Relogio;
{$M 16384,0,16384}
Uses DOS,CRT;

var hora,minuto,segundo,centesimo : word;
    x,y : byte;
    a   : char;

begin
   repeat
   a:=#1;
   if keypressed then a:=readkey;
   if a=#0 then
   begin
      a:=readkey;
      if a=#19 then exit;
   end;
   gettime(hora,minuto,segundo,centesimo);
   x:=wherex;
   y:=wherey;
   gotoxy(68,1);
   writeln(hora:2,':',minuto:2,':',segundo:2,'.',centesimo:2);
   gotoxy(x,y);
   until a=#255;
   keep(0);
end.
