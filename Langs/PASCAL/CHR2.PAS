Program Teste;
uses dos,crt;
var regs  : registers;
	 carac : array[1..16] of string[9];
	 asci  : file of byte;
{----------------------------------------------------------------}
Procedure CopyChar(code:byte);
var aux,i,v : byte;
	 u       : integer;
	 arquivo : text;
	 nome    : string[12];
begin
  str(code,nome);
  nome:=nome+'.pas';
  assign(arquivo,nome);
  rewrite(arquivo);
  writeln(arquivo,'Procedure Desenha'+nome+';');
  writeln(arquivo,'var i : byte;');
  writeln(arquivo,'Begin');
  aux:=0;
	for i:=1 to 16 do
	begin
		val(copy(carac[i],1,1),v,u);      aux:=2*2*2*2*2*2*2*v;
		val(copy(carac[i],2,1),v,u);      aux:=2*2*2*2*2*2*v+aux;
		val(copy(carac[i],3,1),v,u);      aux:=2*2*2*2*2*v+aux;
		val(copy(carac[i],4,1),v,u);      aux:=2*2*2*2*v+aux;
		val(copy(carac[i],5,1),v,u);      aux:=2*2*2*v+aux;
		val(copy(carac[i],6,1),v,u);      aux:=2*2*v+aux;
		val(copy(carac[i],7,1),v,u);      aux:=2*v+aux;
		val(copy(carac[i],8,1),v,u);      aux:=v+aux;
		writeln(arquivo,'   t[',code*16+(i-1),']:=',aux,';');
		write(code:3,' -- ',carac[i],' -- ',aux:3,' -- ',chr(code),' -- ',i);
		writeln;
	end;
	writeln(arquivo,'   regs.ah:=$11;regs.al:=$0;regs.cx:=256;regs.dx:=0;regs.bh:=16;');
	writeln(arquivo,'   regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);');
	writeln(arquivo,'end;');
	writeln(arquivo,'{------------------------------}');
	writeln;
	close(arquivo);
end;
{----------------------------------------------------------------}
Procedure Draw;
var i : byte;
begin
	carac[ 1]:='11111111';
	carac[ 2]:='10000001';
	carac[ 3]:='10000001';
	carac[ 4]:='10000001';
	carac[ 5]:='10011001';
	carac[ 6]:='10011001';
	carac[ 7]:='10011001';
	carac[ 8]:='10011001';
	carac[ 9]:='11111111';
	carac[10]:='11111111';
	carac[11]:='10111101';
	carac[12]:='10111101';
	carac[13]:='10011001';
	carac[14]:='10011001';
	carac[15]:='10000001';
	carac[16]:='11111111';
	copychar(207);
end;
{----------------------------------------------------------------}
begin
	draw;
end.