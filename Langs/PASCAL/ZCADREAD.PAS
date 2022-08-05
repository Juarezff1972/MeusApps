uses dos,graph;
var arq : file of byte;
	 x,y : word;
	 d,m : integer;
	 pt  : byte;

begin
	d:=VGA;
	m:=VGAhi;
	x:=0;
	y:=0;
	initgraph(d,m,'');
	assign(arq,'d:\teste.prp');
	reset(arq);
   for m:=1 to 12 do read(arq,pt);
	repeat
		read(arq,pt);
		if pt=$27 then begin read(arq,pt); read(arq,pt); x:=0; inc(y); end;
		putpixel(x,y,pt);
		inc(x);
		if x>639 then begin x:=0; inc(y); end;
	until eof(arq);
	close(arq);
end.
