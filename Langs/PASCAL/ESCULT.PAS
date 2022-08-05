uses dos,crt;
{$M 8192,0,0}
var lixo : pointer;
{------------------------------------------------------}
Procedure PutPixel(x,y,cor : word);
var regs : registers;
begin
	regs.ah:=$c;
	regs.al:=Cor;
	regs.bh:=0;
	regs.dx:=y;
	regs.cx:=x;
	intr($10,regs);
end;
{------------------------------------------------------}
Function GetPixel(x,y : word) : word;
var regs : registers;
begin
	regs.ah:=$d;
	regs.bh:=0;
	regs.dx:=y;
	regs.cx:=x;
	intr($10,regs);
	GetPixel:=regs.al;
end;
{------------------------------------------------------}
Procedure Escultura;
interrupt;
var x,y,cororig,corfinal : word;
	 c                    : char;
	 regs                 : registers;
	 GetMaxColor,GetMaxX,GetMaxY : word;
begin
	regs.ah:=$f;
	intr($10,regs);
	case regs.al of
	  $4 : begin GetMaxColor:=3;  GetMaxX:=320; GetMaxY:=200; end;
	  $5 : begin GetMaxColor:=3;  GetMaxX:=320; GetMaxY:=200; end;
	  $6 : begin GetMaxColor:=1;  GetMaxX:=640; GetMaxY:=200; end;
	  $d : begin GetMaxColor:=15; GetMaxX:=320; GetMaxY:=200; end;
	  $e : begin GetMaxColor:=15; GetMaxX:=640; GetMaxY:=200; end;
	  $f : begin GetMaxColor:=1;  GetMaxX:=640; GetMaxY:=350; end;
	  $10: begin GetMaxColor:=15; GetMaxX:=640; GetMaxY:=350; end;
	  $11: begin GetMaxColor:=1;  GetMaxX:=640; GetMaxY:=480; end;
	  $12: begin GetMaxColor:=15; GetMaxX:=640; GetMaxY:=480; end;
	  $13: begin GetMaxColor:=15; GetMaxX:=320; GetMaxY:=200; end;
	  $54: begin GetMaxColor:=15; GetMaxX:=800; GetMaxY:=600; end;
	  $61: begin GetMaxColor:=15; GetMaxX:=640; GetMaxY:=400; end;
	  $62: begin GetMaxColor:=15; GetMaxX:=640; GetMaxY:=480; end;
	  $63: begin GetMaxColor:=15; GetMaxX:=800; GetMaxY:=600; end;
	  $64: begin GetMaxColor:=15; GetMaxX:=1024;GetMaxY:=768; end;
	end;
	CorFinal:=getmaxcolor div 2;
	while keypressed do c:=readkey;
	for x:=0 to getmaxx do
	for y:=0 to getmaxy do
	begin
		CorOrig:=GetPixel(x,y);
		PutPixel(x,y,CorFinal);
		if CorOrig<CorFinal then PutPixel(x,y,black);
		if CorOrig>CorFinal then putpixel(x,y,getmaxcolor);
		If CorOrig=CorFinal then
		begin
			if getpixel(x+1,y)=CorOrig then putpixel(x,y,getmaxcolor div 2);
			if getpixel(x+1,y)<CorOrig then putpixel(x,y,getmaxcolor);
			if getpixel(x+1,y)>CorOrig then putpixel(x,y,black);
		end;
		CorFinal:=CorOrig;
		if keypressed then
		begin
			while keypressed do c:=readkey;
			exit;
		end;
	end;
end;
{------------------------------------------------------}
begin
	getintvec($5,lixo);
	setintvec($5,@Escultura);
	keep(768);
end.