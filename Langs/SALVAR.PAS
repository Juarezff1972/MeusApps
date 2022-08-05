uses dos,crt,graph;
var gd,gm : integer;
	 nome  : file of byte;
	 x,y   : word;
	 c1,c2 : byte;
{------------------------------------------------------}
Procedure DesenhaAlgo;
var i : byte;
begin
	randomize;
	for i:=1 to 100 do
	begin
	  {setfillstyle(solidfill,random(getmaxcolor));}
		setcolor(random(getmaxcolor));
		circle(random(getmaxx),random(getmaxy),random(100));
	  {bar(random(getmaxx),random(getmaxy),random(getmaxx),random(getmaxy));}
	end;
end;
{------------------------------------------------------}
Procedure GravaTela;
var x1,y1,i : word;
	 a,b,c,d : byte;
begin
	x:=0;
	y:=0;
	assign(nome,'TELA.PIC');
	rewrite(nome);
	x1:=getmaxx;	a:=hi(x1);b:=lo(x1);   { canto inferior }
	y1:=getmaxy;	c:=hi(y1);d:=lo(y1);
	write(nome,a);write(nome,b);
	write(nome,c);write(nome,d);
	a:=0;b:=0;                            { canto superior }
	write(nome,a,a);
	write(nome,b,b);
	repeat
		a:=getpixel(x,y)*16+getpixel(x+1,y);
		write(nome,a);
		putpixel(x-1,1,15);
		inc(y);
		if y=getmaxy then
		begin
			y:=0;
			inc(x,2);
		end;
	until (x>x1);
	close(nome);
end;
{------------------------------------------------------}
Procedure LeTela;
var x1,x2,y1,y2,x,y : word;
	 valr,ponto      : byte;
	 i               : word;
	 size            : byte;
begin
	assign(nome,'TELA.PIC');
	reset(nome);
	read(nome,valr);x2:=valr;read(nome,valr);x2:=x2*256+valr;
	read(nome,valr);y2:=valr;read(nome,valr);y2:=y2*256+valr;
	read(nome,valr);x1:=valr;read(nome,valr);x1:=x1*256+valr;
	read(nome,valr);y1:=valr;read(nome,valr);y1:=y1*256+valr;
	y:=y1;
	x:=x1;
	size:=1;
	cleardevice;
	repeat
		read(nome,ponto);
		c1:=trunc(ponto/16);
		c2:=ponto-(c1*16);
		if c1<>0 then putpixel(trunc(x / size),trunc(y / size),c1);
		if c2<>0 then putpixel(trunc((x+1) / size),trunc(y / size),c2);
		inc(y);
		if y>=y2 then
		begin
			y:=y1;
			inc(x,2);
		end;
	until (x>x2);
	close(nome);
end;
{------------------------------------------------------}
begin
	gd:=detect;
	initgraph(gd,gm,'');
	DesenhaAlgo;
	readln;
	GravaTela;
	cleardevice;
	readln;
	LeTela;
	readln;
	closegraph;
end.