uses dos,graph,crt;

const LeftButton=1;
const	RightButton=2;
const	MidButton=4;

type mask=array[1..32] of word;

var regs    : registers;
	 MouseIn : boolean;
	 curs    : mask;
	 NumButtons : word;
{---------------------------------------------------------}
Procedure InitEvents;
begin
	regs.ax:=0;
	intr($33,regs);
	MouseIn:=True;
	if regs.ax=0 then MouseIn:=False;
	if MouseIn then NumButtons:=regs.bx;
end;
{---------------------------------------------------------}
Procedure ShowMouse;
begin
	if MouseIn then
	begin
		regs.ax:=1;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure HideMouse;
begin
	if MouseIn then
	begin
		regs.ax:=2;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure GetMousePos(var x,y,bot:word);
begin
	if MouseIn then
	begin
		regs.ax:=3;
		intr($33,regs);
		bot:=regs.bx;
		x:=regs.cx;
		y:=regs.dx;
	end;
end;
{---------------------------------------------------------}
Procedure SetMousePos(x,y:word);
begin
	if MouseIn then
	begin
		regs.ax:=4;
		regs.cx:=x;
		regs.dx:=y;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure GetButtonPress(bot:word;var status,numb,x,y : word);
begin
	if MouseIn then
	begin
		regs.ax:=5;
		regs.bx:=bot;
		intr($33,regs);
		status:=regs.ax;
		numb:=regs.bx;
		x:=regs.cx;
		y:=regs.dx;
	end;
end;
{---------------------------------------------------------}
Procedure GetButtonRelease(bot:word;var status,numb,x,y : word);
begin
	if MouseIn then
	begin
		regs.ax:=6;
		regs.bx:=bot;
		intr($33,regs);
		status:=regs.ax;
		numb:=regs.bx;
		x:=regs.cx;
		y:=regs.dx;
	end;
end;
{---------------------------------------------------------}
Procedure DefineX(x1,x2 : word);
var aux:word;
begin
	if MouseIn then
	begin
		if x1>x2 then
		begin
			aux:=x1;
			x1:=x2;
			x2:=aux;
		end;
		regs.ax:=7;
		regs.cx:=x1;
		regs.dx:=x2;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure DefineY(x1,x2 : word);
var aux:word;
begin
	if MouseIn then
	begin
		if x1>x2 then
		begin
			aux:=x1;
			x1:=x2;
			x2:=aux;
		end;
		regs.ax:=8;
		regs.cx:=x1;
		regs.dx:=x2;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure DefineGraphCursor(x,y: integer);
begin
	if MouseIn then
	begin
		regs.ax:=9;
		regs.bx:=x;
		regs.cx:=y;
		regs.dx:=ofs(curs);
		regs.es:=seg(curs);
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure DefineTextCursor(kind,scrmask,crsmask : word);
begin
	if MouseIn then
	begin
		regs.ax:=10;
		regs.bx:=kind;
		regs.cx:=scrmask;
		regs.dx:=crsmask;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure ReadMotion(var x,y : integer);
begin
	if MouseIn then
	begin
		regs.ax:=11;
		intr($33,regs);
		x:=regs.cx;
		y:=regs.dx;
	end;
end;
{---------------------------------------------------------}
Procedure DefineSensitivity(x,y : word);
begin
	if MouseIn then
	begin
		regs.ax:=15;
		regs.cx:=x;
		regs.dx:=y;
		intr($33,regs);
	end;
end;
{---------------------------------------------------------}
Procedure DoneEvents;
begin
	HideMouse;
end;
{---------------------------------------------------------}
Procedure SetCursorMap;
var i,j : byte;
var bits : array[1..32] of string[16];
{--------------------------------------------}
Function Pot(x,y : real):real;
begin
   if y>0 then pot:=exp(y*ln(x)) else pot:=1;
end;
{--------------------------------------------}
begin
	bits[ 1]:='1...............';
	bits[ 2]:='1111............';
	bits[ 3]:='1111111.........';
	bits[ 4]:='111111111.......';
	bits[ 5]:='11111111111.....';
	bits[ 6]:='11111111111111..';
	bits[ 7]:='1111111111111111';
	bits[ 8]:='111111111111....';
	bits[ 9]:='111111111.......';
	bits[10]:='1111..1111......';
	bits[11]:='11....1111......';
	bits[12]:='........1111....';
	bits[13]:='........1111....';
	bits[14]:='..........1111..';
	bits[15]:='..........1111..';
	bits[16]:='............1111';

{	bits[ 1]:='1111111111111111';
	bits[ 2]:='1..............1';
	bits[ 3]:='1..............1';
	bits[ 4]:='1..............1';
	bits[ 5]:='1111........1111';
	bits[ 6]:='1111111..1111111';
	bits[ 7]:='1.111111111111.1';
	bits[ 8]:='1...11111111...1';
	bits[ 9]:='1.....1111.....1';
	bits[10]:='1...11....11...1';
	bits[11]:='1.11........11.1';
	bits[12]:='11.....11.....11';
	bits[13]:='1....111111....1';
	bits[14]:='1..1111111111..1';
	bits[15]:='1111111111111111';
	bits[16]:='1111111111111111';}

	for i:=1 to 16 do
	begin
		curs[i]:=0;
		for j:=16 downto 1 do
		begin
			if copy(bits[i,j],1,1)='1' then curs[i]:=curs[i]+trunc(pot(2,16-j));
		end;
		curs[i+16]:=curs[i];
		curs[i]:=$ffff-curs[i];
	end;
end;
{---------------------------------------------------------}
Procedure Start;
var gd,gm : integer;
	 i     : word;
	 k     : real;
	 x,y,bot : word;
begin
	gd:=detect;
	initgraph(gd,gm,'');
	for i:=0 to 15 do
	begin
		k:=getmaxx/16;
		setfillstyle(solidfill,i);
		bar(trunc(i*k),getmaxy div 8,trunc(k*i+k),getmaxY-1);
	end;
	InitEvents;
{	SetCursorMap;             { Opcional }
 {	DefineGraphCursor(0,0);   { Opcional }
	ShowMouse;
	repeat
	directvideo:=false;
	gotoxy(1,1);
	getmousepos(x,y,bot);
	writeln('X = ',x,'   ');
	writeln('Y = ',y,'   ');
	writeln('Bot = ',bot,'   ');
	until keypressed;
	DoneEvents;
	closegraph;
end;
{---------------------------------------------------------}
begin
	Start;
end.