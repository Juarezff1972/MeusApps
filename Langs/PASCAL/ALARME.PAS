Program ALARME;
{$M 8192,0,0}
uses DOS,CRT;
var _orig,_dest : word;
    compromisso : string[29];
    mm,hh,hora,a: string;
    m,h         : integer;
    regs        : registers;
    abcd        : pointer;
Procedure Disparar;
interrupt;
begin
   _orig:=$b800;
   _dest:=$8800;
   inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/
          $f6/$8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
   gotoxy(5,1);
   writeln('**************************************');
   writeln('*   Est  na hora do compromisso!!!   *');
   writeln('*                                    *');
   writeln('*   ',compromisso,'   *');
   writeln('**************************************');
   repeat until keypressed;
   _orig:=$8800;
   _dest:=$b800;
   inline($b9/$80/$3e/$33/$ff/$8b/$16/>_dest/$8e/$c2/$33/
          $f6/$8b/$16/>_orig/$1e/$8e/$da/$d1/$e9/$f3/$a5/$1f);
end;
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   writeln;
   write('Quer ativar o alarme?');
   readln(a);
   if upcase(a[1])<>'S' then halt(1);
   writeln;
   write('Entre com a hora: (HH:MM)');
   readln(hora);
   write('Entre com a mensagem: (at‚ 29 caracteres)');
   readln(compromisso);
   if length(compromisso)<29 then
   repeat
      compromisso:=compromisso+' ';
   until length(compromisso)=29;
   getintvec($4a,abcd);
   setintvec($4a,@Disparar);
   hh:=copy(hora,1,pos(':',hora));
   mm:=copy(hora,pos(':',hora)+1,2);
   hh:='$'+hh;
   mm:='$'+mm;
   val(hh,h,h);
   val(mm,m,m);
   regs.ah:=7;
   intr($1a,regs);
   regs.ah:=6;
   regs.ch:=h;
   regs.cl:=m;
   regs.dh:=0;
   intr($1a,regs);
   writeln;
   writeln('Alarme pronto para ',hora,' horas.');
   writeln;
   keep(768);
end.
