uses dos,crt;
var regs : registers;
	 modo : char;
	 i,x,y: byte;
	 x1,y1:word;
	 Curs : array[1..4,1..9] of byte;
	 a,b  : word;
	 p    : pointer;
	 r    : registers;

	 LeftButton,RightButton,MidButton : boolean;
	 MouseX,MouseY                    : word;

label fim;
{------------------------------------------------------------------------}
Procedure DefinirCursor;
var i,x,y : byte;
begin
	for x:=1 to 4 do for y:=1 to 9 do Curs[x,y]:=0;

	for i:=1 to 7 do Curs[1,i]:=15;
	for i:=7 to 9 do Curs[4,i]:=15;
	for i:=5 to 7 do Curs[3,i]:=15;
	for i:=3 to 5 do Curs[2,i]:=12;
	Curs[2,2]:=15;Curs[2,6]:=15;Curs[3,3]:=15;Curs[3,4]:=12;Curs[4,4]:=15;

end;
{------------------------------------------------------------------------}
procedure MouseCursor;
interrupt;
var r   : registers;
	 a,b : byte;
begin
	r.ax:=11;
	intr($33,r);
	if (r.cx<>0)or(r.dx<>0) then
	begin
		for a:=0 to 3 do for b:=0 to 8 do
		begin
			regs.ah:=13;
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			intr($10,regs);
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			regs.ah:=12;
			regs.al:=regs.al xor Curs[a+1,b+1];
			intr($10,regs);
		end;
		regs.ax:=3;
		intr($33,regs);
		x1:=regs.cx;
		y1:=regs.dx;
		for a:=0 to 3 do for b:=0 to 8 do
		begin
			regs.ah:=13;
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			intr($10,regs);
			regs.cx:=x1+a;
			regs.dx:=y1+b;
			regs.ah:=12;
			regs.al:=regs.al xor Curs[a+1,b+1];
			intr($10,regs);
		end;
	end;
	r.ax:=3;
	intr($33,r);
	MouseX:=r.cx;
	MouseY:=r.dx;
	Leftbutton:=false;MidButton:=false;Rightbutton:=false;
	if r.bx >=4 then begin MidButton:=true; r.bx:=r.bx-4; end;
	if r.bx >=2 then begin RightButton:=true; r.bx:=r.bx-2; end;
	if r.bx >=1 then begin LeftButton:=true; r.bx:=r.bx-1; end;
end;
{------------------------------------------------------------------------}
begin
	Leftbutton:=false;MidButton:=false;Rightbutton:=false;
	writeln;
	writeln('Escolha sistema de video: ');
	writeln;
	writeln('1 - 8514/A          2 - VESA         3 - ATI VGA Wonder (512K)');
	writeln('4 - Chips & Tech    5 - Orchid       6 - STB');
	writeln('7 - GENOA           8 - SIGMA        9 - Paradise');
	writeln('A - AST             B - Compaq VGA   C - Trident');
	writeln('D - Video-7 (512K)');
	writeln;
	readln(modo);	modo:=upcase(modo);
	regs.bx:=0; regs.cx:=0; regs.dx:=0;

	if modo='1' then begin regs.ax:=3;     regs.dx:=1;      end;
	if modo='2' then begin regs.ax:=$4f02; regs.bx:=$101;   end;
	if modo='D' then begin regs.ax:=$6f05; regs.bx:=$67;    end;

	if modo='3' then  regs.ax:=$62;
	if modo='4' then  regs.ax:=$79;
	if modo='C' then regs.ax:=$5d;

	if (modo='5')or(modo='6')or(modo='7')or(modo='8') then regs.ax:=$2e;
	if (modo='9')or(modo='A')or(modo='B')             then regs.ax:=$5f;

	intr($10,regs);

	x:=1;
	y:=1;
	for i:=0 to 255 do
	begin
		gotoxy(x,y);
		inc(x);if x>79 then begin x:=1; inc(y); end;
		regs.bh:=DarkGray;
		regs.bl:=i;
		regs.cx:=1;
		regs.al:=i;
		regs.ah:=9;
		intr($10,regs);
	end;
	x1:=0;y1:=0;
	DefinirCursor;
	regs.ax:=3;
	intr($33,regs);
	x1:=regs.cx;
	y1:=regs.dx;
	for a:=0 to 3 do for b:=0 to 8 do
	begin
		regs.ah:=13;
		regs.cx:=x1+a;
		regs.dx:=y1+b;
		intr($10,regs);
		regs.cx:=x1+a;
		regs.dx:=y1+b;
		regs.ah:=12;
		regs.al:=regs.al xor Curs[a+1,b+1];
		intr($10,regs);
	end;
	getintvec($1c,p);
	setintvec($1c,@MouseCursor);
	repeat
		if LeftButton then
		asm
			mov ah,12
			mov al,15
			mov cx,MouseX
			mov dx,MouseY
			int $10
		end;
	until keypressed;
	setintvec($1c,p);
	regs.ah:=0;
	regs.al:=3;
	intr($10,regs);
end.

