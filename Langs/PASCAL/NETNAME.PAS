uses dos,crt;
var regs : registers;
{------------------------------------------------------------}
function GetNetNameNumber : byte;
begin
	regs.ax:=$b807;
	intr($2f,regs);
	GetNetNameNumber:=regs.ch;
end;
{------------------------------------------------------------}
Procedure SetMachineName(var nome : string;numero : byte);
var x : string;
begin
	x:=nome+chr(0);
	regs.ax:=$5e01;
	regs.cx:=1;
	regs.cl:=numero;
	regs.ds:=seg(x);
	regs.dx:=ofs(x)+1;
	intr($21,regs);
end;
{------------------------------------------------------------}
begin
	writeln;
	writeln(GetNetNameNumber);
	writeln;
end.
