uses dos,crt;
var x : ^byte;
    y : byte;
    i : word;
begin
   i:=0;
   repeat
      x:=ptr($c000,($2920)+i);
      y:=x^;
      write(i:4,' - ');
      if y>=128 then begin y:=y-128; write('O'); end  else write('.');
      if y>=64 then  begin y:=y-64;write('O'); end  else write('.');
      if y>=32 then  begin y:=y-32;write('O'); end  else write('.');
      if y>=16 then  begin y:=y-16;write('O'); end  else write('.');
      if y>=8 then   begin y:=y-8; write('O'); end  else write('.');
      if y>=4 then   begin y:=y-4; write('O'); end  else write('.');
      if y>=2 then   begin y:=y-2; write('O'); end  else write('.');
      if y>=1 then   begin y:=y-1; write('O'); end  else write('.');
      writeln(' - ',x^);
      delay(20);
      inc(i);
   until i=65535;
end.
