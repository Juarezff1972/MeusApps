Program Int10;
uses dos;
var regs : registers;
begin
	writeln;
	write('Entre com AX: '); readln(regs.ax);
	write('Entre com BX: '); readln(regs.bx);
	write('Entre com CX: '); readln(regs.cx);
	write('Entre com DX: '); readln(regs.dx);
	intr($10,regs);
	writeln;
	writeln('AX: ',regs.AX);
	writeln('BX: ',regs.BX);
	writeln('CX: ',regs.CX);
	writeln('DX: ',regs.DX);
	writeln('ES: ',regs.ES);
	writeln('DS: ',regs.DS);
	writeln('SI: ',regs.SI);
	writeln('BP: ',regs.BP);
	writeln('Flags: ',regs.flags);
	writeln;
end.