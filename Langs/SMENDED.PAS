Program SMENUDEDICATED;
{$M 8192,0,33000}
uses dos,crt,zzz111,drivers;
{----------------------------------------------------------------}
Function Menu_de_Barra(x,y:byte;texto:string;FunNorm,FreNorm,FunSel,FreSel,posi:byte):byte;
var itens : array[1..26] of string[80];
	 esc,ii: byte;
	 i,itn : byte;
	 xC,yC : array[1..26] of byte;
	 c     : char;
	 event : tevent;
begin
	if length(texto)>79 then texto:=copy(texto,1,79)+#255;
	esc:=1;
	yC[esc]:=y;
	xC[esc]:=x;
	repeat
		itens[esc]:=copy(texto,1,pos(#255,texto)-1);
		delete(texto,1,pos(#255,texto));
		inc(esc);
		yC[esc]:=y;
		xC[esc]:=xC[esc-1]+length(itens[esc-1]);
	until length(texto)=0;
	dec(esc);
	gotoxy(x,y);
	hidemouse;
	textcolor(FreNorm);	textbackground(FunNorm);
	for i:=1 to esc do write(itens[i]);
	if posi>esc then posi:=1;
	textcolor(FreSel);	textbackground(FunSel);
	gotoxy(xC[posi],yC[posi]);	write(itens[posi]);
	showmouse;
	c:=#2;
	itn:=posi;
	repeat
		getmouseevent(event);
		for ii:=1 to esc do
		if (((event.where.x>=(xC[ii]-0))and(event.where.x<(xC[ii+1]-0)))
		and (event.where.y=(yC[ii]-1)))and(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			textcolor(FreNorm);textbackground(FunNorm);
			gotoxy(x,y);for i:=1 to esc do write(itens[i]);
			textcolor(FreSel);textbackground(FunSel);
			gotoxy(xC[ii],yC[ii]);write(itens[ii]);
			showmouse;
			itn:=ii;if event.double then c:=#13;
		end;
		relogio;
		if keypressed then c:=readkey;
		if c=#0 then c:=readkey;
		if c=#77 then
		begin
			hidemouse;
			textcolor(FreNorm);	textbackground(FunNorm);
			gotoxy(xC[itn],yC[itn]); write(itens[itn]);
			inc(itn); if itn > esc then itn:=1;
			textcolor(FreSel);	textbackground(FunSel);
			gotoxy(xC[itn],yC[itn]); write(itens[itn]);
			c:=#2;
			showmouse;
		end;
		if c=#75 then
		begin
			hidemouse;
			textcolor(FreNorm);	textbackground(FunNorm);
			gotoxy(xC[itn],yC[itn]); write(itens[itn]);
			dec(itn); if itn=0 then itn:=esc;
			textcolor(FreSel);	textbackground(FunSel);
			gotoxy(xC[itn],yC[itn]); write(itens[itn]);
			c:=#2;
			showmouse;
		end;
	until (c=#13)or(c=#27);
	if c=#27 then Menu_de_Barra:=0;
	if c=#13 then Menu_de_Barra:=itn;
end;
{----------------------------------------------------------------}
Procedure Tabelas(texto:string;var saida1,saida2:byte);
var esc : array[1..3] of string[10];
	 i,y : byte;
	 c   : char;
    itens : array[1..5] of string[80];
	 esco,ii: byte;
	 xC,yC : array[1..5] of byte;
	 event : tevent;
{---------------------------------}
Procedure Desenhar;
var y : byte;
begin
   cores(FundoMenu,FrenteMenu);
	retangulo(4,2,16,7);
	sombra(4,2,16,7,8);
	cores(FundoMenu,FrenteMenu);
	for y:=1 to 3 do
	begin
		gotoxy(5,2+y);
		write(esc[y]);
	end;
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(5,2+i);
	write(esc[i]);
end;
{---------------------------------}
begin
	if length(texto)>79 then texto:=copy(texto,1,79)+#255;
	esco:=1;
	yC[esco]:=1;
	xC[esco]:=1;
	repeat
		itens[esco]:=copy(texto,1,pos(#255,texto)-1);
		delete(texto,1,pos(#255,texto));
		inc(esco);
		yC[esco]:=1;
		xC[esco]:=xC[esco-1]+length(itens[esco-1]);
	until length(texto)=0;
	dec(esco);
	while keypressed do c:=readkey;
	c:=#2;      { 184 = ¸ }
	esc[1]:= ' Diagonal ';
	esc[2]:= ' Inferior ';
	esc[3]:= ' Superior ';
	i:=1;
	Desenhar;
	showmouse;
	repeat
      getmouseevent(event);
		for ii:=1 to esco do
		if (((event.where.x>=(xC[ii]))and(event.where.x<(xC[ii+1])))
		and (event.where.y=(yC[ii]-1)))and(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			textcolor(FrenteBarra);textbackground(FundoBarra);
			gotoxy(1,1);for i:=1 to esco do write(itens[i]);
			textcolor(FrenteMenuSel);textbackground(FundoMenuSel);
			gotoxy(xC[ii],yC[ii]);write(itens[ii]);
			showmouse;
			saida1:=ii;saida2:=ii;
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
			end;
		end;
		if ((event.where.x>3)and(event.where.x<14))and
			((event.where.y>1)and(event.where.y<5))and
			(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 3 do
			begin
				gotoxy(5,2+y);
				write(esc[y]);
			end;
			i:=event.where.y-1;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(5,2+i);
			write(esc[i]);
			showmouse;
			if event.double then
			asm
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
		end;
		relogio;
		if keypressed then c:=readkey;
		if c=#75 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,75; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
			c:=#2;
		end;
		if c=#77 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,77; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
         c:=#2;
		end;
		if c=#72 then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 3 do
			begin
				gotoxy(5,2+y);
				write(esc[y]);
			end;
			dec(i);
			if i=0 then i:=3;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(5,2+i);
			write(esc[i]);
			showmouse;
			c:=#2;
		end;
		if c=#80 then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 3 do
			begin
				gotoxy(5,2+y);
				write(esc[y]);
			end;
			inc(i);
			if i=11 then i:=1;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(5,2+i);
			write(esc[i]);
			showmouse;
			c:=#2;
		end;
		if (c=#13) then
		begin
			hidemouse;_orig:=$b800;_dest:=tela[2];movepagina;
         cores(Black,LightCyan);
         clrscr;
			case i of
				1 : begin
                   swapvectors;
                   exec('DIAGONAL.EXE','');
                   swapvectors;
                end;
				2 : begin
                   swapvectors;
                   exec('MATINF.EXE','');
                   swapvectors;
                end;
				3 : begin
                   swapvectors;
                   exec('MATSUP.EXE','');
                   swapvectors;
                end;
			end;
         while keypressed do c:=readkey;
			c:=#2;
			_dest:=$b800;_orig:=tela[2];movepagina;
			Desenhar;showmouse;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure Listas(texto:string;var saida1,saida2:byte);
var esc : array[1..2] of string[10];
	 i,y : byte;
	 c   : char;
    itens : array[1..5] of string[80];
	 esco,ii: byte;
	 xC,yC : array[1..5] of byte;
	 event : tevent;
{---------------------------------}
Procedure Desenhar;
var y : byte;
begin
   cores(FundoMenu,FrenteMenu);
	retangulo(14,2,25,5);
	sombra(14,2,25,5,8);
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(15,3);
	write(esc[1]);
end;
{---------------------------------}
begin
	if length(texto)>79 then texto:=copy(texto,1,79)+#255;
	esco:=1;
	yC[esco]:=1;
	xC[esco]:=1;
	repeat
		itens[esco]:=copy(texto,1,pos(#255,texto)-1);
		delete(texto,1,pos(#255,texto));
		inc(esco);
		yC[esco]:=1;
		xC[esco]:=xC[esco-1]+length(itens[esco-1]);
	until length(texto)=0;
	dec(esco);
	while keypressed do c:=readkey;
	c:=#2;      { 184 = ¸ }
	esc[1]:= ' Esparsa ';
	i:=1;
	Desenhar;
	showmouse;
	repeat
      getmouseevent(event);
		for ii:=1 to esco do
		if (((event.where.x>=(xC[ii]))and(event.where.x<(xC[ii+1])))
		and (event.where.y=(yC[ii]-1)))and(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			textcolor(FrenteBarra);textbackground(FundoBarra);
			gotoxy(1,1);for i:=1 to esco do write(itens[i]);
			textcolor(FrenteMenuSel);textbackground(FundoMenuSel);
			gotoxy(xC[ii],yC[ii]);write(itens[ii]);
			showmouse;
			saida1:=ii;saida2:=ii;
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
			end;
		end;
		if ((event.where.x>13)and(event.where.x<24))and
			((event.where.y>1)and(event.where.y<3))and
			(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(15,3);
			write(esc[1]);
			showmouse;
			if event.double then
			asm
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
		end;
		relogio;
		if keypressed then c:=readkey;
		if c=#75 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,75; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
			c:=#2;
		end;
		if c=#77 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,77; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
         c:=#2;
		end;
		if (c=#13) then
		begin
			hidemouse;_orig:=$b800;_dest:=tela[2];movepagina;
         cores(Black,LightCyan);
         clrscr;
         swapvectors;
         exec('LEMATESP.EXE','');
         swapvectors;
         while keypressed do c:=readkey;
			c:=#2;
			_dest:=$b800;_orig:=tela[2];movepagina;
			Desenhar;showmouse;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure Bolhas(texto:string;var saida1,saida2:byte);
var esc : array[1..2] of string[10];
	 i,y : byte;
	 c   : char;
    itens : array[1..5] of string[80];
	 esco,ii: byte;
	 xC,yC : array[1..5] of byte;
	 event : tevent;
{---------------------------------}
Procedure Desenhar;
var y : byte;
begin
   cores(FundoMenu,FrenteMenu);
	retangulo(30,2,41,5);
	sombra(30,2,41,5,8);
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(31,3);
	write(esc[1]);
end;
{---------------------------------}
begin
	if length(texto)>79 then texto:=copy(texto,1,79)+#255;
	esco:=1;
	yC[esco]:=1;
	xC[esco]:=1;
	repeat
		itens[esco]:=copy(texto,1,pos(#255,texto)-1);
		delete(texto,1,pos(#255,texto));
		inc(esco);
		yC[esco]:=1;
		xC[esco]:=xC[esco-1]+length(itens[esco-1]);
	until length(texto)=0;
	dec(esco);
	while keypressed do c:=readkey;
	c:=#2;      { 184 = ¸ }
	esc[1]:= ' Strings ';
	i:=1;
	Desenhar;
	showmouse;
	repeat
      getmouseevent(event);
		for ii:=1 to esco do
		if (((event.where.x>=(xC[ii]))and(event.where.x<(xC[ii+1])))
		and (event.where.y=(yC[ii]-1)))and(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			textcolor(FrenteBarra);textbackground(FundoBarra);
			gotoxy(1,1);for i:=1 to esco do write(itens[i]);
			textcolor(FrenteMenuSel);textbackground(FundoMenuSel);
			gotoxy(xC[ii],yC[ii]);write(itens[ii]);
			showmouse;
			saida1:=ii;saida2:=ii;
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
			end;
		end;
		if ((event.where.x>29)and(event.where.x<40))and
			((event.where.y>1)and(event.where.y<3))and
			(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(31,3);
			write(esc[1]);
			showmouse;
			if event.double then
			asm
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
		end;
		relogio;
		if keypressed then c:=readkey;
		if c=#75 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,75; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
			c:=#2;
		end;
		if c=#77 then
		begin
			asm
				mov ah,5; mov ch,0; mov cl,27; int $16
				mov ah,5; mov ch,77; mov cl,0; int $16
				mov ah,5; mov ch,0; mov cl,13; int $16
			end;
         c:=#2;
		end;
		if (c=#13) then
		begin
			hidemouse;_orig:=$b800;_dest:=tela[2];movepagina;
         cores(Black,LightCyan);
         clrscr;
         swapvectors;
         exec('LEMATESP.EXE','');
         swapvectors;
         while keypressed do c:=readkey;
			c:=#2;
			_dest:=$b800;_orig:=tela[2];movepagina;
			Desenhar;showmouse;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure MenuPrincipal;
var i,y : byte;
	 c   : char;
	 z   : byte;
	 menu: string[80];
begin
	while keypressed do c:=readkey;
	c:=#2;
	hidemouse;
	cores(fundo,0);
	y:=1;
	clrscr;
   menu:=' ğ '+#255+' Matrizes '+#255+' Matriz Esparsa '+#255+' Caracteres '+#255+' Sair '+#255;
	EscreveBarra(1,25,'Super Menu Dedicado',fundobarra,frentebarra);
	EscreveBarra(1,1, ' VersÆo 1 ',fundobarra,frentebarra);
	cores(fundomenu,frentemenu);
	Sombra(23,6,59,17,SombraAtrib);
	Retangulo(23,6,59,17);
	cores(fundomenu,lightred);
	gotoxy(28,8); write('Super Menu - VersÆo Dedicada');
	cores(fundomenu,frentemenu);
	gotoxy(33,10);write('Copyright ¸ 1994');
	cores(fundomenu,yellow);
	gotoxy(30,12);write('Juarez Fioravanti Filho');
   gotoxy(32,14);write('Aderson Cleber Pifer');
	Espera(4000);cores(fundo,0);window(21,6,59,19);clrscr;window(1,1,79,25);
	repeat
		showmouse;
		z:=Menu_de_Barra(1,1,menu,fundobarra,frentebarra,fundomenusel,frentemenusel,y);
		y:=z;
		hidemouse;_orig:=$b800;_dest:=tela[1];movepagina;showmouse;
		case z of
			1 : Sobre;
			2 : Tabelas(menu,y,z);
         3 : Listas(menu,y,z);
         4 : Bolhas(menu,y,z);
			5 : Sair(menu,y,z);
		end;
		hidemouse;_dest:=$b800;_orig:=tela[1];movepagina;showmouse;
	until z=0;
	hidemouse;
end;
{----------------------------------------------------------------}
begin
	regs.ah:=$10;
	regs.al:=$3;
	regs.bl:=0;
	intr($10,regs);
	writeln;
	writeln('Carregando Super Menu Dedicado...');
	LerTabelaASCII;
	cursor(true,6400);
	initevents;
	fundo:=darkgray;        SombraAtrib:=darkgray;
	fundomenu:=blue;	   	frentemenu:=white;   	menutitulo:=lightred;
	Fundomenusel:=red;		frentemenusel:=yellow;
	fundobarra:=lightgray;  frentebarra:=black;
	getmem(segm,16384);	tela[1]:=seg(segm^);
	getmem(segm,16384);	tela[2]:=seg(segm^);
	MenuPrincipal;
	freemem(ptr(tela[1],0),16384);
	freemem(ptr(tela[2],0),16384);
	doneevents;
	cursor(false,6400);
	regs.ah:=$10;
	regs.al:=$3;
	regs.bl:=1;
	intr($10,regs);
	textattr:=3;
	clrscr;
	regs.ah:=0;
	regs.al:=3;
	intr($10,regs);
end.
