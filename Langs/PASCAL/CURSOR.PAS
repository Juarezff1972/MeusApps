uses dos,crt;
var Xs,Ys : string;
    x,y   : integer;
    regs  : registers;
Begin
   If ParamCount <> 2 then
   begin
      writeln;
      writeln('Uso: CURSOR x y');
      writeln;
      writeln('Onde x ‚ a linha superior do cursor (0 a 255) e');
      writeln('     y ‚ a linha inferior do cursor (0 a 255).');
      writeln;
      writeln('Exemplo: CURSOR 6 7 (‚ o tamanho do cursor normal)');
      writeln;
      halt(1);
   end;
   Xs:=ParamStr(1);
   Ys:=ParamStr(2);
   val(Xs,x,x);
   val(Ys,y,y);
   regs.ax:=$168;
   regs.cx:=x*256+y;
   intr($10,regs);
end.