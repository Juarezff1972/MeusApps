uses dos,crt;
var regs : registers;
	 i    : word;
	 k    : real;
	 Modo,cores,resx,resy : word;
{----------------------------------------------------------------}
Procedure GotoXY(x,y : byte);
begin
  regs.ah:=2;
  regs.bh:=0;
  regs.dh:=y;
  regs.dl:=x;
  intr($10,regs);
end;
{----------------------------------------------------------------}
Procedure PutPixel(x,y : word;cor :byte);
begin
	regs.ah:=$c;
	regs.al:=cor;
	regs.bh:=0;
	regs.cx:=x;
	regs.dx:=y;
	intr($10,regs);
end;
{----------------------------------------------------------------}
Procedure Line(x1,y1,x2,y2 : word;cor : byte);
var x,y,i,t1,t2,d,a,b : word;
	 ddx,ddy           : word;
label st1,st2,st3;
begin
	a:=abs(x2-x1);
	b:=abs(y2-y1);
	if (x2-x1) > 0 then ddx:=1 else ddx:=$ffff;
	if (y2-y1) > 0 then ddy:=1 else ddy:=$ffff;
	x:=x1;
	y:=y1;
	t1:=2*a;
	t2:=2*b;
	d:=b-a;
	asm
		mov di,a
		add di,b
		mov si,d
		mov ah,$c
		mov al,cor
		mov bh,0
		mov cx,x
		mov dx,y
 st1: push di
		push si
		int $10
		pop si
		pop di
		cmp si,0
		jng st2
		add dx,ddy
		sub si,t1
		jmp st3
 st2: add cx,ddx
		sub si,t2
 st3: dec di
		cmp di,0
		jne st1
	end;
end;
{----------------------------------------------------------------}
Procedure Box(x1,y1,x2,y2 : word;cor : byte);
var max,mix,may,miy : word;
label st1,st2,st3;
begin
	if x1>x2 then begin maX:=x1;miX:=x2;end else begin maX:=x2;miX:=x1;end;
	if y1>y2 then begin may:=y1;miy:=y2;end else begin may:=y2;miy:=y1;end;
	asm
		mov ah,$c
		mov al,cor
      mov bh,0
		mov cx,mix
		sub cx,1
st2:  inc cx
		cmp cx,max
		jg st3
		mov dx,miy
st1:  int $10
		inc dx
		cmp dx,may
		je st2
		jmp st1
st3:  nop
	end;
end;
{----------------------------------------------------------------}
begin
	Modo:=$64;
	Case modo of
		$04 : begin cores:=4;     resx:=320;  resy:=200; end;
		$05 : begin cores:=4;     resx:=320;  resy:=200; end;
		$06 : begin cores:=2;     resx:=640;  resy:=200; end;
		$0d : begin cores:=16;    resx:=320;  resy:=200; end;
		$0e : begin cores:=16;    resx:=640;  resy:=200; end;
		$0f : begin cores:=2;     resx:=640;  resy:=350; end;
		$10 : begin cores:=16;    resx:=640;  resy:=350; end;
		$11 : begin cores:=2;     resx:=640;  resy:=480; end;
		$12 : begin cores:=16;    resx:=640;  resy:=480; end;
		$13 : begin cores:=256;   resx:=320;  resy:=200; end;
		$54 : begin cores:=16;    resx:=800;  resy:=600; end;
		$62 : begin cores:=256;   resx:=640;  resy:=480; end;
		$63 : begin cores:=256;   resx:=800;  resy:=600; end;
		$64 : begin cores:=256;   resx:=1024; resy:=768; end;
	end;
	directvideo:=false;
	regs.ah:=0;
	regs.al:=Modo;
	intr($10,regs);
	textcolor(15);
   textbackground(0);
	gotoxy(1,1);
	writeln('Modo: ',modo,' -- Cores:',cores,' -- Resolu‡„o X:',resx,' -- Resolu‡„o Y:',resy);
	for i:=0 to Cores-1 do
	begin
		k:=ResX/Cores;
      gotoxy(1,2);
		writeln('X: ',trunc(i*k),' -- Y: ',resy,' -- Cor:',i);
		box(trunc(i*k),resy div 8,trunc(k*i+k),ResY-1,i);
	end;
	readln;
	regs.ah:=0;
	regs.al:=3;
	intr($10,regs);
end.
