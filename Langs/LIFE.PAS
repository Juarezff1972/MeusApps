Program LifeGen;
uses dos,crt;
var cell   : ^byte;
	i,j,c,b : byte;
	a  	  : array[1..25,1..80] of byte;
	cycle   : longint;
	regs    : registers;
label lp,ex;
begin
	randomize;
	regs.ax:=$168;
	regs.cx:=6400;
	intr($10,regs);
	clrscr;
	cycle:=1;
	for i:=3 to 21 do
		for j:=3 to 76 do
			if (random(800)=5) then
			begin
				cell:=ptr($b800,i*160+j*2);
				cell^:=219;
			end else
			begin
				cell:=ptr($b800,i*160+j*2);
				cell^:=255;
			end;
	cell:=ptr($b800,i*160+j*2+1);
	cell^:=10;
lp:delay(00);
	for i:=1 to 25 do
		for j:=1 to 80 do
		begin
			cell:=ptr($b800,i*160+j*2);
			b:=cell^;
			if b=219 then a[i,j]:=219 else a[i,j]:=0;
		end;
	for i:=2 to 24 do
		for j:=2 to 79 do
		begin
			cell:=ptr($b800,i*160+j*2);
			c:=a[i,j-1];
			c:=c+a[i-1,j]+a[i+1,j];
			c:=c+a[i,j+1];
			if (c=219)or(c=219*3) then cell^:=219 else cell^:=255;
			cell:=ptr($b800,i*160+j*2+1);
			cell^:=10;
		end;
	inc(cycle);
	if keypressed then goto ex;
	goto lp;
ex:writeln;
	writeln('Foram-se ',Cycle,' ciclos.');
	writeln;
   regs.ax:=$168;
	regs.cx:=$607;
	intr($10,regs);
end.