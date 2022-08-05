Program Nothing;

Uses CRT,DOS,Math;

var x,y,z : real;
	 gd,gm : integer;
	 regs  : registers;
	 modo  : byte;
	 cores : word;
	 resx  : word;
	 resy  : word;
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
	z:=15;
	repeat
		x:=20;
		repeat
			y:=-10*COS(2*SQRT((X-5)*(X-5)+(Z-5)*(Z-5)))/2+50;
		 {	y:=-10*sin(2*SQRT((X-5)*(X-5)+(Z-5)*(Z-5)))/2+50;}
		 {	y:=-10*tan(2*SQRT((X-5)*(X-5)+(Z-5)*(Z-5)))/2+50;}
		 {	y:=-10*ln(2*SQRT((X-5)*(X-5)+(Z-5)*(Z-5)))/2+50; }
		 {	y:=-10*COS(4/SQRT((X-5)*(X-5)+(Z-5)*(Z-5)))/2+50;}
		 {	y:=-10*sin(4/SQRT((X-5)*(X-5)+(Z-5)*(Z-5)))/2+50;}
			putpixel(trunc((resx/640)*(2*(X*20+Z*3+20))),
						trunc((resy/200)*(Y+Z*10)),
						trunc(y) );
					 {	trunc( (2*y/((x+10)+(z+6)))+8 )); }
			x:=x-0.1;
      until x<-10;
      z:=z-0.1;
   until z<-6;
	readln;
	regs.ah:=0;
	regs.al:=3;
	intr($10,regs);
end.