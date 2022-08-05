uses dos;
var regs    : registers;
	 x       : ^byte;
	 nlinhas : ^byte;
	 i       : word;
	 asci    : file of byte;
	 aux     : string[4];
	 y       : byte;
begin
	regs.ah:=$11;
	regs.al:=$30;
	regs.bh:=6;
	intr($10,regs);
	nlinhas:=ptr($40,$85);
	str(nlinhas^,aux);
	writeln('Salvando...');
	assign(asci,'FONT'+aux+'.CHR');
	rewrite(asci);
	for i:=0 to (255*nlinhas^)+(nlinhas^-1) do
	begin
		x:=ptr(regs.es,regs.bp+i);
		write(asci,x^);
		y:=x^;
		if y>=128 then begin y:=y-128; write('O'); end  else write('.');
		if y>=64 then  begin y:=y-64;write('O'); end  else write('.');
		if y>=32 then  begin y:=y-32;write('O'); end  else write('.');
		if y>=16 then  begin y:=y-16;write('O'); end  else write('.');
		if y>=8 then   begin y:=y-8; write('O'); end  else write('.');
		if y>=4 then   begin y:=y-4; write('O'); end  else write('.');
		if y>=2 then   begin y:=y-2; write('O'); end  else write('.');
		if y>=1 then   begin y:=y-1; write('O'); end  else write('.');
		writeln;
	end;
	close(asci);
	writeln;
end.
