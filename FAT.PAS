Program Fatorial;

uses crt;

var n,i   : longint;
    m,f,g : double;
    c     : char;
    fim   : boolean;

Function Pot(x,y:real):real;
begin
   if y>0 then pot:=exp(y*ln(x)) else pot:=1;
end;

begin
   fim:=false;
   repeat
      clrscr;
      m:=1;
      gotoxy(1,12);
      writeln('0! = 1');
      gotoxy(1,1);
      write('Entre com o numero: ');
      readln(g);
      if g > 268609166 then g:=268609166;
      n:=trunc(g);
      if n > 268609166 then n:=268609166;
      if n < 0 then n:=-n;
      gotoxy(1,12);
      writeln(n,'! =                                 ');
      if n < 34 then for i:=1 to n do m:=m*i;
      if n >= 34 then
      begin
         f:=((n*ln(n))+(ln(2*pi*n))/2-n)/(ln(10));
         m:=pot(10,(f-trunc(f)));
         m:=trunc((m + 0.005)*100) / 100;
      end;
      gotoxy(1,12);
      if n < 34 then writeln(n,'! = ',m:1:0);
      if n >= 34 then writeln(n,'! = ',m:1:2,' E+',trunc(f));
      repeat
         gotoxy(1,24);
         write('Quer continuar? (S/N) ');
         readln(c);
         c:=upcase(c);
         if c='S' then fim:=false;
         if c='N' then fim:=true;
      until (c='S') or (c='N');
   until fim;
end.