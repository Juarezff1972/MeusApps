Program NothingAgain;

Uses CRT,DOS;

var

 A             : integer;
 B,C,D,E,F,G   : real;    { Constants defined }
 n1,n2         : real;
 H,BB          : integer; { Loop Variables }
 AA {,BB}      : integer; { Dependent on H }
 CC,D1,DD      : real;    { Dependent on H and BB }
 X,Y           : real;
 {X,}X1,{Y,}Y1 : integer; { Plotting variables }
 {  Y,Y1       : integer;}{ Dependent on DD and H }
 gd,gm         : integer;
 regs  : registers;
 modo  : byte;
 cores : word;
 resx  : word;
 resy  : word;
{-------------------------------------------------------------------}
Procedure GotoXY(x,y : byte);
begin
  regs.ah:=2;
  regs.bh:=0;
  regs.dh:=y;
  regs.dl:=x;
  intr($10,regs);
end;
{----------------------------------------------------------------}
Procedure PutPixel(x,y : word;cor : word);
begin
	if x<1 then exit;
	if y<1 then exit;
	if x>resx then exit;
	if y>resy then exit;
	regs.ah:=$c;
	regs.al:=lo(cor);
	regs.bh:=0;
	regs.cx:=x;
	regs.dx:=y;
	intr($10,regs);
end;
{----------------------------------------------------------------}
Procedure Line(x1,y1,x2,y2,cor : word);
var x,y,i,t1,t2,d,a,b : word;
	 dx,dy             : integer;
begin
	a:=abs(x2-x1);
	b:=abs(y2-y1);
	if (x2-x1) > 0 then dx:=1 else dx:=-1;
	if (y2-y1) > 0 then dy:=1 else dy:=-1;
	x:=x1;
	y:=y1;
	t1:=2*a;
	t2:=2*b;
	d:=b-a;
	for i:=0 to (a+b) do
	begin
		putpixel(x,y,cor);
		if d>0 then
		begin
			y:=y+dy;
			d:=d-t1;
		end else
		begin
			x:=x+dx;
			d:=d+t2;
		end;
	end;
end;
{----------------------------------------------------------------}
Procedure Instalagraficos;
begin
	Modo:=$62;
	Case modo of
		$04 : begin cores:=4;     resx:=319;  resy:=199; end;
		$05 : begin cores:=4;     resx:=319;  resy:=199; end;
		$06 : begin cores:=2;     resx:=639;  resy:=199; end;
		$0d : begin cores:=16;    resx:=319;  resy:=199; end;
		$0e : begin cores:=16;    resx:=639;  resy:=199; end;
		$0f : begin cores:=2;     resx:=639;  resy:=349; end;
		$10 : begin cores:=16;    resx:=639;  resy:=349; end;
		$11 : begin cores:=2;     resx:=639;  resy:=479; end;
		$12 : begin cores:=16;    resx:=639;  resy:=479; end;
		$13 : begin cores:=256;   resx:=319;  resy:=199; end;
		$54 : begin cores:=16;    resx:=799;  resy:=599; end;
		$62 : begin cores:=256;   resx:=639;  resy:=479; end;
		$63 : begin cores:=256;   resx:=799;  resy:=599; end;
		$64 : begin cores:=256;   resx:=1023; resy:=767; end;
	end;
	regs.ah:=0;
	regs.al:=Modo;
	intr($10,regs);
end;
{-------------------------------------------------------------------}
Procedure Fundos;
begin
   x:=bb+(h/b)+e;
	y:=dd-(h/b)+f;
	x1:=trunc(0.85*x);
	y1:=trunc(0.9 *(g-y));
	if ((y1<0) or (y1>190)) then exit;
	putpixel((resx div 639)*2*x1,(resy div 199)*y1,trunc(cc*20)+1);
	line((resx div 639)*2*x1,(resy div 199)*y1+1,
		  (resx div 639)*2*x1,(resy div 199)*y1+trunc(n1) ,0);
end;
{-------------------------------------------------------------------------}
Procedure Criar;
begin
	h:=-a;
	repeat
		aa:=trunc(0.5+sqrt(a*a-h*h));
		for bb:=-aa to aa do
		begin
			cc:=sqrt(bb*bb+h*h)*d;
{*************************************************************************}
{          d1:=5*exp(-cc)*sin(2*3.14159266*cc);}
{			  d1:=cos(2*cc)+cos(5*cc);}
{			  d1:=cos(2*cc)+sin(5*cc);}
{          d1:=sin(2*cc)+cos(5*cc);}
{          d1:=sin(2*cc)+sin(5*cc);}
{          d1:=sin(2*cc)+sin(2*cc);}
			 d1:=cos(2*cc)-cos(5*cc);
{*************************************************************************}
         dd:=d1*c;
         Fundos;
      end;
      h:=h+trunc(b);
   until (h=a);
end;
{-------------------------------------------------------------------------}
begin
   clrscr;
	writeln;
	writeln;
	writeln('Para expandir ou comprimir o desenho verticalmente, entre um numero');
	write  ('entre -40 e 40 (20 e'' tipico).'); readln(n1);
	writeln;
	writeln;
	writeln('Para mover o desenho para cima ou para baixo na tela, entre um numero');
	write  ('entre -50 e 150 (90 e'' tipico).'); readln(n2);
	a:=144;
	b:=2.25;
	c:=n1;
	d:=0.0327;
	e:=160;
	f:=n2;
	g:=199;
	instalagraficos;
	c:=n1;
	Criar;
	readln;
	regs.ah:=0;
	regs.al:=3;
	intr($10,regs);
end.


