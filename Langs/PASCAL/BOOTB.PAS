Program Boot_From_B;
uses dos;
var regs : registers;
	 arq  : file of byte;
	 p    : pointer;
	 i,s,o: word;
	 x    : byte;
	 y    : ^byte;
begin
	getmem(p,512);
	s:=seg(p^);
	o:=ofs(p^);
	assign(arq,'BOOTB.SYS');
	reset(arq);
	for i:=1 to 512 do
	begin
		read(arq,x);
      y:=ptr(s,o);
		y^:=x;
		inc(o);
	end;
	close(arq);
	regs.ax:=$301;
	regs.bx:=0;
	regs.cx:=1;
	regs.dx:=0;
	regs.es:=s;
	intr($13,regs);
end.