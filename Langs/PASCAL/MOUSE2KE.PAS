Program Mouse2Key;
{$M 8192,0,0}
uses DOS;
var regs : registers;
    abcd : pointer;
    efgh : byte;
procedure Ativar;
interrupt;
begin
   regs.ax:=11;
   regs.cx:=0;
   regs.dx:=0;
   inc(efgh);
   if efgh=2 then
   begin
      intr($33,regs);
      efgh:=0;
   end;
   if (regs.cx>0)and(regs.cx<=32768) then
   begin
      regs.ah:=5;
      regs.ch:=$4d;
      regs.cl:=0;
      intr($16,regs);
      regs.dx:=0;
   end;
   if (regs.cx>32768)and(regs.cx<=65535) then
   begin
      regs.ah:=5;
      regs.ch:=$4b;
      regs.cl:=0;
      intr($16,regs);
      regs.dx:=0;
   end;
   if (regs.dx>0)and(regs.dx<=32768) then
   begin
      regs.ah:=5;
      regs.ch:=$50;
      regs.cl:=0;
      intr($16,regs);
      regs.cx:=0;
   end;
   if (regs.dx>32768)and(regs.dx<=65535) then
   begin
      regs.ah:=5;
      regs.ch:=$48;
      regs.cl:=0;
      intr($16,regs);
      regs.cx:=0;
   end;
end;
begin
   efgh:=0;
   regs.ax:=0;
   intr($33,regs);
   if regs.ax=0 then
   begin
      writeln;
      writeln('N„o existe mouse instalado...');
      writeln;
      halt(0);
   end;
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($1c,abcd);
   setintvec($1c,@ativar);
   writeln;
   writeln('Mouse ---> Teclado Ativado...');
   writeln;
   keep(768);
end.