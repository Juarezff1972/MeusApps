uses dos;
var regs : registers;
	 esc  : byte;
{-----------------------------------------------------------------}
Procedure Fonte_8x8;
begin
	regs.ah:=$11;
	regs.al:=$12;
	intr($10,regs);
	regs.ah:=2;
	regs.bh:=0;
	regs.dh:=43;
	regs.dl:=1;
	intr($10,regs);
	writeln;
end;
{-----------------------------------------------------------------}
Procedure Fonte_8x14;
begin
	regs.ah:=$11;
	regs.al:=$11;
	intr($10,regs);
	regs.ah:=2;
	regs.bh:=0;
	regs.dh:=29;
	regs.dl:=1;
	intr($10,regs);
	writeln;
end;
{-----------------------------------------------------------------}
Procedure Fonte_8x16;
begin
	regs.ah:=$11;
	regs.al:=$14;
	intr($10,regs);
	regs.ah:=2;
	regs.bh:=0;
	regs.dh:=23;
	regs.dl:=1;
	intr($10,regs);
	writeln;
end;
{-----------------------------------------------------------------}
begin
	writeln;
	write('Qual tamanho? (8/14/16)');
	readln(esc);
	case esc of
		8  : Fonte_8x8;
		14 : Fonte_8X14;
		16 : Fonte_8x16;
	end;

end.