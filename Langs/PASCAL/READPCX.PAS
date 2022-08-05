uses dos,graph,crt;
var gd,gm               : integer;
	 nome                : file of byte;
	 i,ii                : byte;
	 xmin,xmax,ymin,ymax : word;
	 xsize,ysize         : word;
	 hdpi,vdpi           : word;
	 cores               : byte;
	 r,g,b               : byte;
	 nplanes             : byte;
	 bytesperline        : word;
	 totalbytes          : word;
	 count,data          : byte;
	 x,y,z               : word;
begin
	writeln;
	assign(nome,'c:\simearth\snap1.pcx');
	reset(nome);
	read(nome,i);	writeln('Fabricante: ',i);
	read(nome,i);	writeln('Vers„o: ',i);
	read(nome,i);	writeln('Codifica‡„o: ',i);
	read(nome,i);	writeln('Bits por Pixel: ',i);
	read(nome,i);  xmin:=i;	read(nome,i);  xmin:=i*256+xmin;
	read(nome,i);  ymin:=i;	read(nome,i);  ymin:=i*256+ymin;
	read(nome,i);  xmax:=i;	read(nome,i);  xmax:=i*256+xmax;
	read(nome,i);  ymax:=i;	read(nome,i);  ymax:=i*256+ymax;
	writeln('Coordenadas: ',xmin,'x',ymin,' - ',xmax,'x',ymax);
	xsize:=xmax-xmin+1;ysize:=ymax-ymin+1;
	writeln('Tamanho: ',xsize,'x',ysize);
	read(nome,i);  hdpi:=i; read(nome,i);  hdpi:=i*256+hdpi;
	read(nome,i);  vdpi:=i; read(nome,i);  vdpi:=i*256+vdpi;
	writeln('Resolu‡„o horizontal: ',hdpi);
	writeln('Resolu‡„o vertical: ',vdpi);
	writeln;
	writeln('Aperte algo para continuar...');
	repeat until keypressed;
	gd:=VGA;
	gm:=VGAHI;
	initgraph(gd,gm,'');
	for cores:=1 to 16 do
	begin
		setfillstyle(solidfill,cores-1);
		bar((cores-1)*16,0,cores*16,400);
	end;
	for i:=0 to 5 do
	begin
		read(nome,r);read(nome,g);read(nome,b);setrgbpalette(i,r,g,b);
	end;
   read(nome,r);read(nome,g);read(nome,b);setrgbpalette(20,r,g,b);
	read(nome,r);read(nome,g);read(nome,b);setrgbpalette(7,r,g,b);
	for i:=56 to 63 do
	begin
		read(nome,r);read(nome,g);read(nome,b);setrgbpalette(i,r,g,b);
	end;
	read(nome,i);
	read(nome,nplanes);
	read(nome,i);bytesperline:=i;read(nome,i);bytesperline:=256*i+bytesperline;
	totalbytes:=nplanes*bytesperline;
	for i:=1 to 60 do read(nome,i);
	x:=xmin;y:=ymin;
	repeat
	for z:=1 to totalbytes do
	begin
		read(nome,count);
		if $c0=($c0 and count) then
		begin
			count:=$3f and count;
			read(nome,data);
		end
		else
		begin
			data:=count;
			count:=1;
		end;
		for ii:=1 to count do
		begin
			putpixel(x,y,data);
			inc(x); if x>xmax then begin x:=xmin;inc(y);end;
		end;
	end;
	until (not eof(nome));
	close(nome);
	readln;
	readln;
   closegraph;
end.