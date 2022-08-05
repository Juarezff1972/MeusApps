{$M 8192,0,0}
uses DOS,CRT;
var i     : word;
    saida : string;
    abcd  : pointer;
    regs  : registers;
Procedure MacroAtivada;
interrupt;
begin
  if not(keypressed) then
  begin
     inc(i);
     if i>length(saida) then
     begin
        setintvec($28,abcd);
        dec(i);
     end;
     regs.ah:=5;
     regs.ch:=0;
     regs.cl:=ord(saida[i]);
     intr($16,regs);
  end;
end;
begin
   i:=1;
   saida:=' mem /program'+chr(13)+chr(13);
   writeln(saida);
   readln;
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($28,abcd);
   setintvec($28,@MacroAtivada);
   keep(768);
end.
