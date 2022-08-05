Program Jurisprudencia;
uses crt,dos,cpjtools,drivers;

type Descontos = record
			nome  : array[1..30] of string[40];
			valor : array[1..30] of real;
		end;
type Indices = record
			nome  : array[1..30] of string[10];
			valor : array[1..30] of real;
		end;
type Serv = record
			decr : array[1..255] of string[10];
			indc : array[1..255] of string[10];
			valr : array[1..255] of real;
			advs : array[1..255] of string[3];
			desc : array[1..255] of real;
			invr : array[1..255] of real;
		end;
type Acts = record
			Sigla : array[1..255] of string[5];
			QReu  : array[1..255] of string[10];
			QAut  : array[1..255] of string[10];
			Desc  : array[1..255] of string[20];
		end;

var ClasseDesconto : Descontos;
	 IndiceMon      : Indices;
	 Servicos       : Serv;
	 Acoes          : Acts;
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
Procedure DescontoPorClasse;
var i,y : byte;   	 z   : byte;
	 s	  : searchrec;  arq : text;
	 c   : char;       aux : string[7];
	 aux2: string;     erro: integer;
	 event : tevent;
begin
	hidemouse;
	for i:=1 to 30 do
	begin
		str(i,aux); if i<10 then aux:='0'+aux;
		ClasseDesconto.nome[i]:='Classe '+aux;
		ClasseDesconto.valor[i]:=i-1;
	end;
	findfirst('DESCONTO.CLA',anyfile,s);
	if doserror=0 then
	begin
		i:=1;
		assign(arq,'DESCONTO.CLA');
		reset(arq);
		repeat
		  readln(arq,ClasseDesconto.nome[i]);
		  readln(arq,ClasseDesconto.valor[i]);
		  inc(i);
		until (i=31);
		close(arq);
		dec(i);
	end;
	cores(fundomenu,frentemenu);
	retangulo(15,7,67,21);
	sombra(15,7,67,21,8);
	gotoxy(15,7); write('ÃÂÁ');
	gotoxy(64,10); write('À',chr(30)); gotoxy(64,19); write('Ê',chr(31));
   cores(fundomenusel,frentemenu);
	for i:=1 to 8 do begin gotoxy(64,10+i); write('±°'); end;
	gotoxy(64,11); write('Åþ');
	cores(fundomenu,frentemenu);
	gotoxy(16,8);
	write('              Classe                  Desconto');
	for i:=1 to 10 do
	begin
		str(ClasseDesconto.valor[i]:3:2,aux);
		gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i],' ':40-length(ClasseDesconto.nome[i]));
		gotoxy(56,9+i);write(' ',aux,' ':6-length(aux));
	end;
	i:=1;
	y:=0;
	cores(fundomenusel,frentemenusel);
   str(ClasseDesconto.valor[i]:3:2,aux);
	gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i],' ':40-length(ClasseDesconto.nome[i]));
	gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
	cores(fundomenu,frentemenu);
	c:=#2;
	showmouse;
	repeat
		if keypressed then c:=readkey;
		relogio;
		if c=#0 then c:=readkey;
		getmouseevent(event);
		if ((caracter(event.where.x+1,event.where.y+1)=#30)
			or(caracter(event.where.x+1,event.where.y+1)='À'))
			and(event.buttons=mbleftbutton) then c:=#72;
		if ((caracter(event.where.x+1,event.where.y+1)=#31)
			or(caracter(event.where.x+1,event.where.y+1)='Ê'))
			and(event.buttons=mbleftbutton) then c:=#80;
		if ((event.where.x+1)>=15)and((event.where.x+1)<=17)
			and((event.where.y+1)=7)and(event.buttons=mbleftbutton)then c:=#27;
		if ((event.where.x+1)>=16)and((event.where.x+1)<=63)
			and((event.where.y+1)>=10)and((event.where.y+1)<=19)
			and(event.buttons=mbleftbutton) then
		begin
			i:=(event.where.y+1)-9;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			if event.double then c:=#13;
		end;
		if c=#80 then
		begin
			inc(i);
			if i=11 then
			begin
				dec(i);
				inc(y);
				if (i+y)>30 then begin i:=10; y:=20; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			c:=#2;
		end;
		if c=#72 then
		begin
			dec(i);
			if i=0 then
			begin
				inc(i);
				dec(y);
				if (i+y)=256 then begin i:=1; y:=0; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
         cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			c:=#2;
		end;
		cores(fundomenu,frentemenu);
      if c=#13 then
		begin
			hidemouse;
			retangulo(17,7+i,57,11+i);
			gotoxy(17,7+i); write('ÃÂÁ');
			sombra(17,7+i,57,11+i,Sombraatrib);
			gotoxy(19,8+i); write('Entre com o NOME da classe:');
			cores(FundoMenu,FrenteMenuSel);gotoxy(31,8+i);write('NOME');
			cores(FundoMenu,FrenteMenu);
			showmouse;
			Trata_char(18,9+i,aux2,38);
			str((i+y),aux); if (i+y)<10 then aux:='0'+aux;
			if aux2=' ' then aux2:='Classe '+aux;
			if aux2='' then aux2:=ClasseDesconto.nome[i+y];
			ClasseDesconto.nome[i+y]:=aux2;
			hidemouse;
			retangulo(17,7+i,57,11+i);
			gotoxy(17,7+i); write('ÃÂÁ');
			gotoxy(19,8+i); write('Entre com o VALOR da classe:');
			cores(FundoMenu,FrenteMenuSel);gotoxy(31,8+i);write('VALOR');
			cores(FundoMenu,FrenteMenu);
			showmouse;
			Trata_char(18,9+i,aux2,7);
			if aux2<>'' then val(aux2,ClasseDesconto.valor[i+y],erro);
			if aux2=' ' then ClasseDesconto.valor[i+y]:=i+y-1;
			if ClasseDesconto.valor[i+y]>999 then ClasseDesconto.valor[i+y]:=999;
			z:=1;
			assign(arq,'DESCONTO.CLA');
			rewrite(arq);
			repeat
			  writeln(arq,ClasseDesconto.nome[z]);
			  writeln(arq,ClasseDesconto.valor[z]);
			  inc(z);
			until (z=31);
			close(arq);
			hidemouse;
			cores(fundomenu,frentemenu);
			retangulo(15,7,67,21);
			gotoxy(15,7); write('ÃÂÁ');
			gotoxy(64,10); write('À',chr(30)); gotoxy(64,19); write('Ê',chr(31));
         cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			gotoxy(16,8);
			write('              Classe                  Desconto');
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			c:=#2;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure TiposDeServico;
var i,y : byte;   	 z,x : byte;
	 s	  : searchrec;  arq : text;
	 c   : char;       aux : string[12];
	 aux2: string;     erro: integer;
	 event : tevent;   num : byte;
	 maux : byte;
{---------------------------------}
Procedure Editar(numero : byte);
var i,y,z : byte;
	 c     : char;
begin
	hidemouse;
	retangulo(17,7+numero,57,11+numero);
	gotoxy(17,7+numero); write('ÃÂÁ');
	sombra(17,7+numero,57,11+numero,Sombraatrib);
	gotoxy(19,8+numero); write('Entre com a Descri‡Æo:');
	cores(FundoMenu,FrenteMenu);
	showmouse;
	Trata_char(18,9+numero,aux2,11);
	if aux2=' ' then aux2:='Servico '+chr(numero+64);
	if aux2='' then aux2:=Servicos.decr[numero];
	Servicos.decr[numero]:=aux2;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	c:=#2;
	for i:=1 to 30 do
	begin
		str(i,aux); if i<10 then aux:='0'+aux;
		IndiceMon.nome[i]:='Öndice '+aux;
		IndiceMon.valor[i]:=1;
	end;
	findfirst('INDICE.MON',anyfile,s);
	if doserror=0 then
	begin
		i:=1;
		assign(arq,'INDICE.MON');
		reset(arq);
		repeat
		  readln(arq,IndiceMon.nome[i]);
		  readln(arq,IndiceMon.valor[i]);
		  inc(i);
		until (i=31);
		close(arq);
		dec(i);
	end;
	cores(fundomenu,frentemenu);
	retangulo(15,7,67,21);
	sombra(15,7,67,21,8);
	gotoxy(15,7); write('ÃÂÁ');
	gotoxy(64,10); write('À',chr(30)); gotoxy(64,19); write('Ê',chr(31));
   cores(fundomenusel,frentemenu);
	for i:=1 to 8 do begin gotoxy(64,10+i); write('±°'); end;
	gotoxy(64,11); write('Åþ');
	cores(fundomenu,frentemenu);
	gotoxy(16,8);
	write('              Öndice                  Valor');
	for i:=1 to 10 do
	begin
		str(IndiceMon.valor[i]:6:4,aux);
		gotoxy(16,9+i);write(' ',IndiceMon.nome[i],' ':33-length(IndiceMon.nome[i]));
		gotoxy(50,9+i);write(' ',aux,' ':11-length(aux));
	end;
	i:=1;
	y:=0;
	cores(fundomenusel,frentemenusel);
	str(IndiceMon.valor[i]:6:4,aux);
	gotoxy(16,9+i);write(' ',IndiceMon.nome[i],' ':33-length(IndiceMon.nome[i]));
	gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
	cores(fundomenu,frentemenu);
	c:=#2;
	showmouse;
	repeat
		if keypressed then c:=readkey;
		relogio;
		if c=#0 then c:=readkey;
		getmouseevent(event);
		if ((caracter(event.where.x+1,event.where.y+1)=#30)
			or(caracter(event.where.x+1,event.where.y+1)='À'))
			and(event.buttons=mbleftbutton) then c:=#72;
		if ((caracter(event.where.x+1,event.where.y+1)=#31)
			or(caracter(event.where.x+1,event.where.y+1)='Ê'))
			and(event.buttons=mbleftbutton) then c:=#80;
		if ((event.where.x+1)>=15)and((event.where.x+1)<=17)
			and((event.where.y+1)=7)and(event.buttons=mbleftbutton)then c:=#27;
		if ((event.where.x+1)>=16)and((event.where.x+1)<=63)
			and((event.where.y+1)>=10)and((event.where.y+1)<=19)
			and(event.buttons=mbleftbutton) then
		begin
			i:=(event.where.y+1)-9;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:6:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:6:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			if event.double then c:=#13;
		end;
		if c=#80 then
		begin
			inc(i);
			if i=11 then
			begin
				dec(i);
				inc(y);
				if (i+y)>30 then begin i:=10; y:=20; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:6:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:6:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			c:=#2;
		end;
		if c=#72 then
		begin
			dec(i);
			if i=0 then
			begin
				inc(i);
				dec(y);
				if (i+y)=256 then begin i:=1; y:=0; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:6:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
         cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:6:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			c:=#2;
		end;
		cores(fundomenu,frentemenu);
		if c=#13 then
		begin
			Servicos.indc[numero]:=IndiceMon.Nome[i+y];
			Servicos.invr[numero]:=IndiceMon.Valor[i+y];
		end;
	until (c=#27)or(c=#13);
	c:=#2;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	retangulo(17,7+numero,57,11+numero);
	sombra(17,7+numero,57,11+numero,sombraatrib);
	gotoxy(17,7+numero); write('ÃÂÁ');
	gotoxy(19,8+numero); write('Entre com o Valor:');
	cores(FundoMenu,FrenteMenu);
	showmouse;
	Trata_char(18,9+numero,aux2,10);
	if aux2<>'' then val(aux2,Servicos.valr[numero],erro);
	if aux2=' ' then Servicos.valr[numero]:=1;
	if Servicos.valr[numero]>999999.99 then Servicos.valr[numero]:=999999.99;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	retangulo(17,7+numero,57,11+numero);
	sombra(17,7+numero,57,11+numero,sombraatrib);
	gotoxy(17,7+numero); write('ÃÂÁ');
	gotoxy(19,8+numero); write('H  participa‡Æo do advogado?');
	cores(FundoMenu,FrenteMenu);
	gotoxy(19,9+numero); write('Sim/NÆo');
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(19,9+numero); write('Sim');
	showmouse;
	c:=#2;
	Servicos.advs[numero]:='Sim';
	repeat
		if keypressed then c:=readkey;
		relogio;
		getmouseevent(event);
		if (event.where.x+1>=17)and(event.where.x+1<=19)
			and(event.where.y+1=7+numero)and(event.buttons=mbleftbutton)
				then c:=#13;
		if (event.where.x+1>=19)and(event.where.x<=21)
		and(event.where.y+1=9+numero)and(event.buttons=mbleftbutton)
					then c:='S';
		if (event.where.x+1>=23)and(event.where.x<=25)
		and(event.where.y+1=9+numero)and(event.buttons=mbleftbutton)
					then c:='N';
		if event.double then c:=#13;
		if (c=#75)or(upcase(c)='S') then
		begin
			cores(FundoMenu,FrenteMenu);
			gotoxy(19,9+numero); write('Sim/NÆo');
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(19,9+numero); write('Sim');
			Servicos.advs[numero]:='Sim';
			c:=#2;
		end;
		if (c=#77)or(upcase(c)='N') then
		begin
			cores(FundoMenu,FrenteMenu);
			gotoxy(19,9+numero); write('Sim/NÆo');
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(23,9+numero); write('NÆo');
			Servicos.advs[numero]:='NÆo';
			c:=#2;
		end;
	until (c=#13);
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	c:=#2;
	for i:=1 to 30 do
	begin
		str(i,aux); if i<10 then aux:='0'+aux;
		ClasseDesconto.nome[i]:='Classe '+aux;
		ClasseDesconto.valor[i]:=i-1;
	end;
	findfirst('DESCONTO.CLA',anyfile,s);
	if doserror=0 then
	begin
		i:=1;
		assign(arq,'DESCONTO.CLA');
		reset(arq);
		repeat
		  readln(arq,ClasseDesconto.nome[i]);
		  readln(arq,ClasseDesconto.valor[i]);
		  inc(i);
		until (i=31);
		close(arq);
		dec(i);
	end;
	cores(fundomenu,frentemenu);
	retangulo(15,7,67,21);
	sombra(15,7,67,21,8);
	gotoxy(15,7); write('ÃÂÁ');
	gotoxy(64,10); write('À',chr(30)); gotoxy(64,19); write('Ê',chr(31));
   cores(fundomenusel,frentemenu);
	for i:=1 to 8 do begin gotoxy(64,10+i); write('±°'); end;
	gotoxy(64,11); write('Åþ');
	cores(fundomenu,frentemenu);
	gotoxy(16,8);
	write('              Classe                  Desconto');
	for i:=1 to 10 do
	begin
		str(ClasseDesconto.valor[i]:3:2,aux);
		gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i],' ':40-length(ClasseDesconto.nome[i]));
		gotoxy(56,9+i);write(' ',aux,' ':6-length(aux));
	end;
	i:=1;
	y:=0;
	cores(fundomenusel,frentemenusel);
   str(ClasseDesconto.valor[i]:3:2,aux);
	gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i],' ':40-length(ClasseDesconto.nome[i]));
	gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
	cores(fundomenu,frentemenu);
	c:=#2;
	showmouse;
	repeat
		if keypressed then c:=readkey;
		relogio;
		if c=#0 then c:=readkey;
		getmouseevent(event);
		if ((caracter(event.where.x+1,event.where.y+1)=#30)
			or(caracter(event.where.x+1,event.where.y+1)='À'))
			and(event.buttons=mbleftbutton) then c:=#72;
		if ((caracter(event.where.x+1,event.where.y+1)=#31)
			or(caracter(event.where.x+1,event.where.y+1)='Ê'))
			and(event.buttons=mbleftbutton) then c:=#80;
		if ((event.where.x+1)>=15)and((event.where.x+1)<=17)
			and((event.where.y+1)=7)and(event.buttons=mbleftbutton)then c:=#27;
		if ((event.where.x+1)>=16)and((event.where.x+1)<=63)
			and((event.where.y+1)>=10)and((event.where.y+1)<=19)
			and(event.buttons=mbleftbutton) then
		begin
			i:=(event.where.y+1)-9;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			if event.double then c:=#13;
		end;
		if c=#80 then
		begin
			inc(i);
			if i=11 then
			begin
				dec(i);
				inc(y);
				if (i+y)>30 then begin i:=10; y:=20; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			c:=#2;
		end;
		if c=#72 then
		begin
			dec(i);
			if i=0 then
			begin
				inc(i);
				dec(y);
				if (i+y)=256 then begin i:=1; y:=0; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(ClasseDesconto.valor[z+y]:3:2,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',ClasseDesconto.nome[z+y],' ':40-length(ClasseDesconto.nome[z+y]));
				gotoxy(56,9+z);write(' ',aux,' ':7-length(aux));
			end;
         cores(fundomenusel,frentemenusel);
			str(ClasseDesconto.valor[i+y]:3:2,aux);
			gotoxy(16,9+i);write(' ',ClasseDesconto.nome[i+y],' ':40-length(ClasseDesconto.nome[i+y]));
			gotoxy(56,9+i);write(' ',aux,' ':7-length(aux));
			showmouse;
			c:=#2;
		end;
		cores(fundomenu,frentemenu);
		if c=#13 then Servicos.desc[numero]:=ClasseDesconto.valor[i+y];
	until (c=#27)or(c=#13);
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;showmouse;
end;
{---------------------------------}
begin
	hidemouse;
	for num:=1 to 10 do
	begin
		Servicos.decr[num]:='Servi‡o '+chr(num+64);
		Servicos.indc[num]:='Cr$';
		Servicos.valr[num]:=1;
		Servicos.advs[num]:='Sim';
		Servicos.desc[num]:=0;
		Servicos.invr[num]:=1;
	end;
	findfirst('TIPO.SRV',anyfile,s);
	if doserror=0 then
	begin
		num:=1;
		assign(arq,'TIPO.SRV');
		reset(arq);
		repeat
		  readln(arq,Servicos.decr[num]);
		  readln(arq,Servicos.indc[num]);
		  readln(arq,Servicos.valr[num]);
		  readln(arq,Servicos.advs[num]);
		  readln(arq,Servicos.desc[num]);
		  readln(arq,Servicos.invr[num]);
		  inc(num);
		until (num=255)or(eof(arq));
		close(arq);
		dec(num);
	end;
	cores(fundomenu,frentemenu);
	retangulo(12,7,80,21);
	sombra(12,7,80,21,8);
	gotoxy(12,7); write('ÃÂÁ');
	gotoxy(66,10); write('À',chr(30)); gotoxy(66,19); write('Ê',chr(31));
	cores(fundomenusel,frentemenu);
	for y:=1 to 8 do begin gotoxy(66,10+y); write('±°'); end;
	gotoxy(66,11); write('Åþ');
	cores(fundomenu,frentemenu);
	gotoxy(14,8); write(' Descri‡Æo    Öndice       Valor   Adv.   Desconto');
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(70,12);write(' Incluir ');cores(FundoMenu,Black);
	for y:=69 to 77 do begin gotoxy(y,13);write(chr(223));end;
	gotoxy(69,12);write(chr(220));cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(70,14);write(' Remover ');cores(FundoMenu,Black);
	for y:=69 to 77 do begin gotoxy(y,15);write(chr(223));end;
	gotoxy(69,14);write(chr(220));cores(fundomenusel,frentemenusel);
	gotoxy(70,16);write(' Alterar ');cores(FundoMenu,Black);
	for y:=69 to 77 do begin gotoxy(y,17);write(chr(223));end;
	gotoxy(69,16);write(chr(220));cores(fundomenu,frentemenu);
	z:=num;if num>10 then z:=10;
	for y:=1 to z do
	begin
		gotoxy(14,9+y);write(Servicos.decr[y]);
		gotoxy(26+((10-length(Servicos.indc[y]))div 2),9+y);write(Servicos.indc[y]);
		gotoxy(37,9+y);write(Servicos.valr[y]:6:3);
		gotoxy(49,9+y);write(Servicos.advs[y]);
		gotoxy(58,9+y);write(Servicos.desc[y]:3:2);
	end;
	cores(fundomenusel,frentemenusel);
	gotoxy(13,10);write(' ':53);
	gotoxy(14,10);write(Servicos.decr[1]);
	gotoxy(26+((10-length(Servicos.indc[1]))div 2),10);write(Servicos.indc[1]);
	gotoxy(37,10);write(Servicos.valr[1]:6:3);
	gotoxy(49,10);write(Servicos.advs[1]);
	gotoxy(58,10);write(Servicos.desc[1]:3:2);
	cores(fundomenu,frentemenu);
	showmouse;
	i:=1;
	y:=0;
	repeat
		if keypressed then c:=readkey;
		relogio;
		if c=#0 then c:=readkey;
		getmouseevent(event);
		if (event.where.x+1>=70)and(event.where.x+1<=78) then
		begin
			if (event.where.y+1=12)and(event.buttons=mbleftbutton) then c:='I';
			if (event.where.y+1=14)and(event.buttons=mbleftbutton) then c:='R';
			if (event.where.y+1=16)and(event.buttons=mbleftbutton) then c:='A';
		end;
		if ((caracter(event.where.x+1,event.where.y+1)=#30)
			or(caracter(event.where.x+1,event.where.y+1)='À'))
			and(event.buttons=mbleftbutton) then c:=#72;
		if ((caracter(event.where.x+1,event.where.y+1)=#31)
			or(caracter(event.where.x+1,event.where.y+1)='Ê'))
			and(event.buttons=mbleftbutton) then c:=#80;
		if ((event.where.x+1)>=12)and((event.where.x+1)<=14)
			and((event.where.y+1)=7)and(event.buttons=mbleftbutton)then c:=#27;
		maux:=19;if num<11 then maux:=num+9;
		if ((event.where.x+1)>=13)and((event.where.x+1)<=65)
			and((event.where.y+1)>=10)and((event.where.y+1)<=maux)
			and(event.buttons=mbleftbutton) then
		begin
			i:=(event.where.y+1)-9;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				str(Servicos.valr[z+y]:6:3,aux);
				str(Servicos.desc[z+y]:3:2,aux2);
				cores(fundomenu,frentemenu);
				gotoxy(13,9+z);write(' ':53);
				gotoxy(14,9+z);write(Servicos.decr[z+y]);
				gotoxy(26+((10-length(Servicos.indc[z+y]))div 2),9+z);write(Servicos.indc[z+y]);
				gotoxy(37,9+z);write(aux);
				gotoxy(49,9+z);write(Servicos.advs[z+y]);
				gotoxy(58,9+z);write(aux2);
			end;
			cores(fundomenusel,frentemenusel);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			showmouse;
			if event.double then c:=#13;
		end;
      if (upcase(c)='A')or(c=#13) then
		begin
			HideMouse;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(69,16);
			write(' Alterar ');
			cores(FundoMenu,FrenteMenu);
			for x:=69 to 77 do begin gotoxy(x,17); write(chr(32)); end;
			gotoxy(78,16); write(' ');
			showmouse;
			c:=#13;
		end;
		if c=#80 then
		begin
			inc(i);
			x:=num;if num>10 then x:=10;
			if i=x+1 then
			begin
				dec(i);
				inc(y);
				if (i+y)>num then begin i:=x; y:=num-x; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				str(Servicos.valr[z+y]:6:3,aux);
				str(Servicos.desc[z+y]:3:2,aux2);
				cores(fundomenu,frentemenu);
				gotoxy(13,9+z);write(' ':53);
				gotoxy(14,9+z);write(Servicos.decr[z+y]);
				gotoxy(26+((10-length(Servicos.indc[z+y]))div 2),9+z);write(Servicos.indc[z+y]);
				gotoxy(37,9+z);write(aux);
				gotoxy(49,9+z);write(Servicos.advs[z+y]);
				gotoxy(58,9+z);write(aux2);
			end;
			cores(fundomenusel,frentemenusel);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			showmouse;
			c:=#2;
		end;
		if c=#72 then
		begin
			dec(i);
			if i=0 then
			begin
				inc(i);
				dec(y);
				if (i+y)=256 then begin i:=1; y:=0; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				str(Servicos.valr[z+y]:6:3,aux);
				str(Servicos.desc[z+y]:3:2,aux2);
				cores(fundomenu,frentemenu);
				gotoxy(13,9+z);write(' ':53);
				gotoxy(14,9+z);write(Servicos.decr[z+y]);
				gotoxy(26+((10-length(Servicos.indc[z+y]))div 2),9+z);write(Servicos.indc[z+y]);
				gotoxy(37,9+z);write(aux);
				gotoxy(49,9+z);write(Servicos.advs[z+y]);
				gotoxy(58,9+z);write(aux2);
			end;
			cores(fundomenusel,frentemenusel);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			showmouse;
			c:=#2;
		end;
		cores(fundomenu,frentemenu);
      if c=#13 then
		begin
			delay(50);
			hidemouse;
			cores(fundomenusel,frentemenusel);
			gotoxy(70,16);write(' Alterar ');cores(FundoMenu,Black);
			for z:=69 to 77 do begin gotoxy(z,17);write(chr(223));end;
			gotoxy(69,16);write(chr(220));cores(fundomenu,frentemenu);showmouse;
			hidemouse;_orig:=$b800;_dest:=tela[3];movepagina;showmouse;
			Editar(i+y);
			hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;showmouse;
			c:=#2;
			assign(arq,'TIPO.SRV');
			rewrite(arq);
			for x:=1 to num do
			begin
			  writeln(arq,Servicos.decr[x]);writeln(arq,Servicos.indc[x]);
			  writeln(arq,Servicos.valr[x]);writeln(arq,Servicos.advs[x]);
			  writeln(arq,Servicos.desc[x]);writeln(arq,Servicos.invr[x]);
			end;
			close(arq);
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			cores(fundomenusel,frentemenusel);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			showmouse;
		end;
		if upcase(c)='I' then
		begin
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(69,12);
			write(' Incluir ');
			cores(FundoMenu,FrenteMenu);
			for z:=69 to 77 do begin gotoxy(z,13); write(chr(32)); end;
			gotoxy(78,12); write(' ');
			delay(50);
			inc(num);
			cores(fundomenusel,frentemenusel);
			gotoxy(70,12);write(' Incluir ');cores(FundoMenu,Black);
			for x:=69 to 77 do begin gotoxy(x,13);write(chr(223));end;
			gotoxy(69,12);write(chr(220));cores(fundomenu,frentemenu);
			hidemouse;_orig:=$b800;_dest:=tela[3];movepagina;showmouse;
			Editar(num);
			hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;showmouse;
			c:=#2;
			assign(arq,'TIPO.SRV');
			rewrite(arq);
			for x:=1 to num do
			begin
			  writeln(arq,Servicos.decr[x]);writeln(arq,Servicos.indc[x]);
			  writeln(arq,Servicos.valr[x]);writeln(arq,Servicos.advs[x]);
			  writeln(arq,Servicos.desc[x]);writeln(arq,Servicos.invr[x]);
			end;
			close(arq);
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				str(Servicos.valr[z+y]:6:3,aux);
				str(Servicos.desc[z+y]:3:2,aux2);
				cores(fundomenu,frentemenu);
				gotoxy(13,9+z);write(' ':53);
				gotoxy(14,9+z);write(Servicos.decr[z+y]);
				gotoxy(26+((10-length(Servicos.indc[z+y]))div 2),9+z);write(Servicos.indc[z+y]);
				gotoxy(37,9+z);write(aux);
				gotoxy(49,9+z);write(Servicos.advs[z+y]);
				gotoxy(58,9+z);write(aux2);
			end;
			cores(fundomenusel,frentemenusel);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			showmouse;
		end;
		if (upcase(c)='R')or(c=#83) then
		begin
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(69,14);
			write(' Remover ');
			cores(FundoMenu,FrenteMenu);
			for z:=69 to 77 do begin gotoxy(z,15); write(chr(32)); end;
			gotoxy(78,14); write(' ');
			delay(50);
         cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(70,14);write(' Remover ');cores(FundoMenu,Black);
			for z:=69 to 77 do begin gotoxy(z,15);write(chr(223));end;
			gotoxy(69,14);write(chr(220));cores(fundomenu,frentemenu);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			if num > 1 then
			begin
				for x:=(i+y) to num-1 do
				begin
					Servicos.decr[x]:=Servicos.decr[x+1];
					Servicos.indc[x]:=Servicos.indc[x+1];
					Servicos.valr[x]:=Servicos.valr[x+1];
					Servicos.advs[x]:=Servicos.advs[x+1];
					Servicos.desc[x]:=Servicos.desc[x+1];
					Servicos.invr[x]:=Servicos.invr[x+1];
				end;
				dec(num);
				z:=num;if num>10 then z:=10;
				for x:=1 to z do
				begin
					str(Servicos.valr[x]:6:3,aux);
					str(Servicos.desc[x]:3:2,aux2);
					gotoxy(13,9+x);write(' ':53);
					gotoxy(14,9+x);write(Servicos.decr[x]);
					gotoxy(26+((10-length(Servicos.indc[x]))div 2),9+x);write(Servicos.indc[x]);
					gotoxy(37,9+x);write(aux);
					gotoxy(49,9+x);write(Servicos.advs[x]);
					gotoxy(58,9+x);write(aux2);
				end;
				if num<10 then
				for x:=1 to 11-(num+1) do
				begin
					gotoxy(13,20-x);write(' ':53);
				end;
				i:=1;y:=0;
				assign(arq,'TIPO.SRV');
				rewrite(arq);
				for x:=1 to num do
				begin
				  writeln(arq,Servicos.decr[x]);writeln(arq,Servicos.indc[x]);
				  writeln(arq,Servicos.valr[x]);writeln(arq,Servicos.advs[x]);
				  writeln(arq,Servicos.desc[x]);writeln(arq,Servicos.invr[x]);
				end;
				close(arq);
			end;
			c:=#2;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenusel,frentemenusel);
			str(Servicos.valr[i+y]:6:3,aux);
			str(Servicos.desc[i+y]:3:2,aux2);
			gotoxy(13,9+i);write(' ':53);
			gotoxy(14,9+i);write(Servicos.decr[i+y]);
			gotoxy(26+((10-length(Servicos.indc[i+y]))div 2),9+i);write(Servicos.indc[i+y]);
			gotoxy(37,9+i);write(aux);
			gotoxy(49,9+i);write(Servicos.advs[i+y]);
			gotoxy(58,9+i);write(aux2);
			showmouse;
		end;
	until c=#27;
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure Acao;
var i,y : byte;   	 z,x : byte;
	 s	  : searchrec;  arq : text;
	 c   : char;       aux : string[12];
	 aux2: string;     erro: integer;
	 event : tevent;   num : byte;
	 maux  : byte;
{---------------------------------}
Procedure Editar(numero : byte);
var i,y,z : byte;
	 c     : char;
begin
	hidemouse;
	retangulo(17,7+numero,57,11+numero);
	gotoxy(17,7+numero); write('ÃÂÁ');
	sombra(17,7+numero,57,11+numero,Sombraatrib);
	gotoxy(19,8+numero); write('Entre com a Descri‡Æo:');
	cores(FundoMenu,FrenteMenu);
	showmouse;
	Trata_char(18,9+numero,aux2,31);
	if aux2=' ' then aux2:='A‡Æo '+chr(numero+64);
	if aux2='' then aux2:=Acoes.desc[numero];
	Acoes.desc[numero]:=aux2;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	c:=#2;
	retangulo(17,7+numero,57,11+numero);
	gotoxy(17,7+numero); write('ÃÂÁ');
	sombra(17,7+numero,57,11+numero,Sombraatrib);
	gotoxy(19,8+numero); write('Entre com a Sigla:');
	cores(FundoMenu,FrenteMenu);
	showmouse;
	Trata_char(18,9+numero,aux2,6);
	if aux2=' ' then aux2:='A'+chr(numero+64);
	if aux2='' then aux2:=Acoes.sigla[numero];
	Acoes.sigla[numero]:=aux2;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	c:=#2;
	retangulo(17,7+numero,57,11+numero);
	gotoxy(17,7+numero); write('ÃÂÁ');
	sombra(17,7+numero,57,11+numero,Sombraatrib);
	gotoxy(19,8+numero); write('Entre com a Qualifica‡Æo do R‚u:');
	cores(FundoMenu,FrenteMenu);
	showmouse;
	Trata_char(18,9+numero,aux2,31);
	if aux2=' ' then aux2:='A'+chr(numero+64);
	if aux2='' then aux2:=Acoes.qreu[numero];
	Acoes.qreu[numero]:=aux2;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	c:=#2;
	retangulo(17,7+numero,57,11+numero);
	gotoxy(17,7+numero); write('ÃÂÁ');
	sombra(17,7+numero,57,11+numero,Sombraatrib);
	gotoxy(19,8+numero); write('Entre com a Qualifica‡Æo do Autor:');
	cores(FundoMenu,FrenteMenu);
	showmouse;
	Trata_char(18,9+numero,aux2,31);
	if aux2=' ' then aux2:='A'+chr(numero+64);
	if aux2='' then aux2:=Acoes.qaut[numero];
	Acoes.qaut[numero]:=aux2;
	hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;
	c:=#2;
	showmouse;
end;
{---------------------------------}
begin
	hidemouse;
	for num:=1 to 10 do
	begin
		Acoes.desc[num]:='A‡Æo '+chr(num+64);
		Acoes.sigla[num]:='A'+chr(num+64);
		Acoes.QReu[num]:='R‚u';
		Acoes.QAut[num]:='Autor';
	end;
	findfirst('ACAO.ACT',anyfile,s);
	if doserror=0 then
	begin
		num:=1;
		assign(arq,'ACAO.ACT');
		reset(arq);
		repeat
		  readln(arq,Acoes.desc[num]);
		  readln(arq,Acoes.sigla[num]);
		  readln(arq,Acoes.QReu[num]);
		  readln(arq,Acoes.QAut[num]);
		  inc(num);
		until (num=255)or(eof(arq));
		close(arq);
		dec(num);
	end;
	cores(fundomenu,frentemenu);
	retangulo(2,7,80,21);
	sombra(2,7,80,21,8);
	gotoxy(2,7); write('ÃÂÁ');
	gotoxy(66,10); write('À',chr(30)); gotoxy(66,19); write('Ê',chr(31));
	cores(fundomenusel,frentemenu);
	for y:=1 to 8 do begin gotoxy(66,10+y); write('±°'); end;
	gotoxy(66,11); write('Åþ');
	cores(fundomenu,frentemenu);
	gotoxy(4,8); write('      Descri‡Æo         Sigla   Qualif. R‚u  Quailf. Autor');
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(70,12);write(' Incluir ');cores(FundoMenu,Black);
	for y:=69 to 77 do begin gotoxy(y,13);write(chr(223));end;
	gotoxy(69,12);write(chr(220));cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(70,14);write(' Remover ');cores(FundoMenu,Black);
	for y:=69 to 77 do begin gotoxy(y,15);write(chr(223));end;
	gotoxy(69,14);write(chr(220));cores(fundomenusel,frentemenusel);
	gotoxy(70,16);write(' Alterar ');cores(FundoMenu,Black);
	for y:=69 to 77 do begin gotoxy(y,17);write(chr(223));end;
	gotoxy(69,16);write(chr(220));cores(fundomenu,frentemenu);
	z:=num;if num>10 then z:=10;
	for y:=1 to z do
	begin
		gotoxy(4,9+y);write(Acoes.desc[y]);
		gotoxy(25+((10-length(Acoes.sigla[y]))div 2),9+y);write(Acoes.sigla[y]);
		gotoxy(37+((10-length(Acoes.Qreu[y]))div 2),9+y);write(Acoes.QReu[y]);
		gotoxy(51+((10-length(Acoes.Qaut[y]))div 2),9+y);write(Acoes.QAut[y]);
	end;
	cores(fundomenusel,frentemenusel);
	gotoxy(3,10);write(' ':63);
	gotoxy(4,10);write(Acoes.Desc[1]);
	gotoxy(25+((10-length(Acoes.sigla[1]))div 2),10);write(Acoes.sigla[1]);
	gotoxy(37+((10-length(Acoes.Qreu[1]))div 2),10);write(Acoes.QReu[1]);
	gotoxy(51+((10-length(Acoes.Qaut[1]))div 2),10);write(Acoes.QAut[1]);
	cores(fundomenu,frentemenu);
	showmouse;
	i:=1;
	y:=0;
	repeat
		if keypressed then c:=readkey;
		relogio;
		if c=#0 then c:=readkey;
		getmouseevent(event);
		if (event.where.x+1>=70)and(event.where.x+1<=78) then
		begin
			if (event.where.y+1=12)and(event.buttons=mbleftbutton) then c:='I';
			if (event.where.y+1=14)and(event.buttons=mbleftbutton) then c:='R';
			if (event.where.y+1=16)and(event.buttons=mbleftbutton) then c:='A';
		end;
		if ((caracter(event.where.x+1,event.where.y+1)=#30)
			or(caracter(event.where.x+1,event.where.y+1)='À'))
			and(event.buttons=mbleftbutton) then c:=#72;
		if ((caracter(event.where.x+1,event.where.y+1)=#31)
			or(caracter(event.where.x+1,event.where.y+1)='Ê'))
			and(event.buttons=mbleftbutton) then c:=#80;
		if ((event.where.x+1)>=2)and((event.where.x+1)<=4)
			and((event.where.y+1)=7)and(event.buttons=mbleftbutton)then c:=#27;
		maux:=19;if num<11 then maux:=num+9;
		if ((event.where.x+1)>=13)and((event.where.x+1)<=65)
			and((event.where.y+1)>=10)and((event.where.y+1)<=maux)
			and(event.buttons=mbleftbutton) then
		begin
			i:=(event.where.y+1)-9;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				aux:=Acoes.QReu[z+y];
				cores(fundomenu,frentemenu);
				gotoxy(3,9+z);write(' ':63);
				gotoxy(4,9+z);write(Acoes.Desc[z+y]);
				gotoxy(25+((10-length(Acoes.sigla[z+y]))div 2),9+z);write(Acoes.sigla[z+y]);
				gotoxy(37+((10-length(Acoes.Qreu[z+y]))div 2),9+z);write(aux);
				gotoxy(51+((10-length(Acoes.Qaut[z+y]))div 2),9+z);write(Acoes.QAut[z+y]);
			end;
			cores(fundomenusel,frentemenusel);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			showmouse;
			if event.double then c:=#13;
		end;
      if (upcase(c)='A')or(c=#13) then
		begin
			HideMouse;
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(69,16);
			write(' Alterar ');
			cores(FundoMenu,FrenteMenu);
			for x:=69 to 77 do begin gotoxy(x,17); write(chr(32)); end;
			gotoxy(78,16); write(' ');
			showmouse;
			c:=#13;
		end;
		if c=#80 then
		begin
			inc(i);
			x:=num;if num>10 then x:=10;
			if i=x+1 then
			begin
				dec(i);
				inc(y);
				if (i+y)>num then begin i:=x; y:=num-x; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				aux:=Acoes.QReu[z+y];
				cores(fundomenu,frentemenu);
				gotoxy(3,9+z);write(' ':63);
				gotoxy(4,9+z);write(Acoes.Desc[z+y]);
				gotoxy(25+((10-length(Acoes.sigla[z+y]))div 2),9+z);write(Acoes.sigla[z+y]);
				gotoxy(37+((10-length(Acoes.Qreu[z+y]))div 2),9+z);write(aux);
				gotoxy(51+((10-length(Acoes.Qaut[z+y]))div 2),9+z);write(Acoes.QAut[z+y]);
			end;
			cores(fundomenusel,frentemenusel);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			showmouse;
			c:=#2;
		end;
		if c=#72 then
		begin
			dec(i);
			if i=0 then
			begin
				inc(i);
				dec(y);
				if (i+y)=256 then begin i:=1; y:=0; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				aux:=Acoes.QReu[z+y];
				cores(fundomenu,frentemenu);
				gotoxy(3,9+z);write(' ':63);
				gotoxy(4,9+z);write(Acoes.Desc[z+y]);
				gotoxy(25+((10-length(Acoes.sigla[z+y]))div 2),9+z);write(Acoes.sigla[z+y]);
				gotoxy(37+((10-length(Acoes.Qreu[z+y]))div 2),9+z);write(aux);
				gotoxy(51+((10-length(Acoes.Qaut[z+y]))div 2),9+z);write(Acoes.QAut[z+y]);
			end;
			cores(fundomenusel,frentemenusel);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			showmouse;
			c:=#2;
		end;
		cores(fundomenu,frentemenu);
      if c=#13 then
		begin
			delay(50);
			hidemouse;
			cores(fundomenusel,frentemenusel);
			gotoxy(70,16);write(' Alterar ');cores(FundoMenu,Black);
			for z:=69 to 77 do begin gotoxy(z,17);write(chr(223));end;
			gotoxy(69,16);write(chr(220));cores(fundomenu,frentemenu);showmouse;
			hidemouse;_orig:=$b800;_dest:=tela[3];movepagina;showmouse;
			Editar(i+y);
			hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;showmouse;
			c:=#2;
			assign(arq,'ACAO.ACT');
			rewrite(arq);
			for x:=1 to num do
			begin
			  writeln(arq,Acoes.Desc[x]);writeln(arq,Acoes.sigla[x]);
			  writeln(arq,Acoes.QReu[x]);writeln(arq,Acoes.QAut[x]);
			end;
			close(arq);
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			cores(fundomenusel,frentemenusel);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			showmouse;
		end;
		if upcase(c)='I' then
		begin
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(69,12);
			write(' Incluir ');
			cores(FundoMenu,FrenteMenu);
			for z:=69 to 77 do begin gotoxy(z,13); write(chr(32)); end;
			gotoxy(78,12); write(' ');
			delay(50);
			inc(num);
			cores(fundomenusel,frentemenusel);
			gotoxy(70,12);write(' Incluir ');cores(FundoMenu,Black);
			for x:=69 to 77 do begin gotoxy(x,13);write(chr(223));end;
			gotoxy(69,12);write(chr(220));cores(fundomenu,frentemenu);
			hidemouse;_orig:=$b800;_dest:=tela[3];movepagina;showmouse;
			Editar(num);
			hidemouse;_dest:=$b800;_orig:=tela[3];movepagina;showmouse;
			c:=#2;
			assign(arq,'ACAO.ACT');
			rewrite(arq);
			for x:=1 to num do
			begin
			  writeln(arq,Acoes.Desc[x]);writeln(arq,Acoes.sigla[x]);
			  writeln(arq,Acoes.QReu[x]);writeln(arq,Acoes.QAut[x]);
			end;
			close(arq);
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			x:=num;if num>10 then x:=10;
			for z:=1 to x do
			begin
				aux:=Acoes.QReu[z+y];
				cores(fundomenu,frentemenu);
				gotoxy(3,9+z);write(' ':63);
				gotoxy(4,9+z);write(Acoes.Desc[z+y]);
				gotoxy(25+((10-length(Acoes.sigla[z+y]))div 2),9+z);write(Acoes.sigla[z+y]);
				gotoxy(37+((10-length(Acoes.Qreu[z+y]))div 2),9+z);write(aux);
				gotoxy(51+((10-length(Acoes.Qaut[z+y]))div 2),9+z);write(Acoes.QAut[z+y]);
			end;
			cores(fundomenusel,frentemenusel);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			showmouse;
		end;
		if (upcase(c)='R')or(c=#83) then
		begin
			cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(69,14);
			write(' Remover ');
			cores(FundoMenu,FrenteMenu);
			for z:=69 to 77 do begin gotoxy(z,15); write(chr(32)); end;
			gotoxy(78,14); write(' ');
			delay(50);
         cores(FundoMenuSel,FrenteMenuSel);
			gotoxy(70,14);write(' Remover ');cores(FundoMenu,Black);
			for z:=69 to 77 do begin gotoxy(z,15);write(chr(223));end;
			gotoxy(69,14);write(chr(220));cores(fundomenu,frentemenu);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			if num > 1 then
			begin
				for x:=(i+y) to num-1 do
				begin
					Acoes.Desc[x]:=Acoes.Desc[x+1];Acoes.sigla[x]:=Acoes.sigla[x+1];
					Acoes.QReu[x]:=Acoes.QReu[x+1];Acoes.QAut[x]:=Acoes.QAut[x+1];
				end;
				dec(num);
				z:=num;if num>10 then z:=10;
				for x:=1 to z do
				begin
					aux:=Acoes.QReu[x];
					gotoxy(3,9+x);write(' ':63);
					gotoxy(4,9+x);write(Acoes.Desc[x]);
					gotoxy(25+((10-length(Acoes.sigla[x]))div 2),9+x);write(Acoes.sigla[x]);
					gotoxy(37+((10-length(Acoes.Qreu[x]))div 2),9+x);write(aux);
					gotoxy(51+((10-length(Acoes.Qaut[x]))div 2),9+x);write(Acoes.QAut[x]);
				end;
				if num<10 then
				for x:=1 to 11-(num+1) do
				begin
					gotoxy(3,20-x);write(' ':63);
				end;
				i:=1;y:=0;
				assign(arq,'ACAO.ACT');
				rewrite(arq);
				for x:=1 to num do
				begin
				  writeln(arq,Acoes.Desc[x]);writeln(arq,Acoes.sigla[x]);
				  writeln(arq,Acoes.QReu[x]);writeln(arq,Acoes.QAut[x]);
				end;
				close(arq);
			end;
			c:=#2;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(66,10+z); write('±°'); end;
			gotoxy(66,11+trunc(((y+i)/num)*7)); write('Åþ');
			cores(fundomenusel,frentemenusel);
			aux:=Acoes.QReu[i+y];
			gotoxy(3,9+i);write(' ':63);
			gotoxy(4,9+i);write(Acoes.Desc[i+y]);
			gotoxy(25+((10-length(Acoes.sigla[i+y]))div 2),9+i);write(Acoes.sigla[i+y]);
			gotoxy(37+((10-length(Acoes.Qreu[i+y]))div 2),9+i);write(aux);
			gotoxy(51+((10-length(Acoes.Qaut[i+y]))div 2),9+i);write(Acoes.QAut[i+y]);
			showmouse;
		end;
	until c=#27;
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure IndiceMonetario;
var i,y : byte;   	 z   : byte;
	 s	  : searchrec;  arq : text;
	 c   : char;       aux : string[12];
	 aux2: string;     erro: integer;
	 event : tevent;
begin
	hidemouse;
	for i:=1 to 30 do
	begin
		str(i,aux); if i<10 then aux:='0'+aux;
		IndiceMon.nome[i]:='Öndice '+aux;
		IndiceMon.valor[i]:=1;
	end;
	findfirst('INDICE.MON',anyfile,s);
	if doserror=0 then
	begin
		i:=1;
		assign(arq,'INDICE.MON');
		reset(arq);
		repeat
		  readln(arq,IndiceMon.nome[i]);
		  readln(arq,IndiceMon.valor[i]);
		  inc(i);
		until (i=31);
		close(arq);
		dec(i);
	end;
	cores(fundomenu,frentemenu);
	retangulo(15,7,67,21);
	sombra(15,7,67,21,8);
	gotoxy(15,7); write('ÃÂÁ');
	gotoxy(64,10); write('À',chr(30)); gotoxy(64,19); write('Ê',chr(31));
   cores(fundomenusel,frentemenu);
	for i:=1 to 8 do begin gotoxy(64,10+i); write('±°'); end;
	gotoxy(64,11); write('Åþ');
	cores(fundomenu,frentemenu);
	gotoxy(16,8);
	write('              Öndice                  Valor');
	for i:=1 to 10 do
	begin
		str(IndiceMon.valor[i]:7:4,aux);
		gotoxy(16,9+i);write(' ',IndiceMon.nome[i],' ':33-length(IndiceMon.nome[i]));
		gotoxy(50,9+i);write(' ',aux,' ':11-length(aux));
	end;
	i:=1;
	y:=0;
	cores(fundomenusel,frentemenusel);
	str(IndiceMon.valor[i]:7:4,aux);
	gotoxy(16,9+i);write(' ',IndiceMon.nome[i],' ':33-length(IndiceMon.nome[i]));
	gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
	cores(fundomenu,frentemenu);
	c:=#2;
	showmouse;
	repeat
		if keypressed then c:=readkey;
		relogio;
		if c=#0 then c:=readkey;
		getmouseevent(event);
		if ((caracter(event.where.x+1,event.where.y+1)=#30)
			or(caracter(event.where.x+1,event.where.y+1)='À'))
			and(event.buttons=mbleftbutton) then c:=#72;
		if ((caracter(event.where.x+1,event.where.y+1)=#31)
			or(caracter(event.where.x+1,event.where.y+1)='Ê'))
			and(event.buttons=mbleftbutton) then c:=#80;
		if ((event.where.x+1)>=15)and((event.where.x+1)<=17)
			and((event.where.y+1)=7)and(event.buttons=mbleftbutton)then c:=#27;
		if ((event.where.x+1)>=16)and((event.where.x+1)<=63)
			and((event.where.y+1)>=10)and((event.where.y+1)<=19)
			and(event.buttons=mbleftbutton) then
		begin
			i:=(event.where.y+1)-9;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:7:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:7:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			if event.double then c:=#13;
		end;
		if c=#80 then
		begin
			inc(i);
			if i=11 then
			begin
				dec(i);
				inc(y);
				if (i+y)>30 then begin i:=10; y:=20; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:7:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:7:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			c:=#2;
		end;
		if c=#72 then
		begin
			dec(i);
			if i=0 then
			begin
				inc(i);
				dec(y);
				if (i+y)=256 then begin i:=1; y:=0; end;
			end;
			hidemouse;
			cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:7:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
         cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:7:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			c:=#2;
		end;
		cores(fundomenu,frentemenu);
      if c=#13 then
		begin
			hidemouse;
			retangulo(17,7+i,57,11+i);
			gotoxy(17,7+i); write('ÃÂÁ');
			sombra(17,7+i,57,11+i,Sombraatrib);
			gotoxy(19,8+i); write('Entre com o NOME do ¡ndice:');
			cores(FundoMenu,FrenteMenuSel);gotoxy(31,8+i);write('NOME');
			cores(FundoMenu,FrenteMenu);
			showmouse;
			Trata_char(18,9+i,aux2,38);
			str((i+y),aux); if (i+y)<10 then aux:='0'+aux;
			if aux2=' ' then aux2:='Öndice '+aux;
			if aux2='' then aux2:=IndiceMon.nome[i+y];
			IndiceMon.nome[i+y]:=aux2;
			hidemouse;
			retangulo(17,7+i,57,11+i);
			gotoxy(17,7+i); write('ÃÂÁ');
			gotoxy(19,8+i); write('Entre com o VALOR do ¡ndice:');
			cores(FundoMenu,FrenteMenuSel);gotoxy(31,8+i);write('VALOR');
			cores(FundoMenu,FrenteMenu);
			showmouse;
			Trata_char(18,9+i,aux2,12);
			if aux2<>'' then val(aux2,IndiceMon.valor[i+y],erro);
			if aux2=' ' then IndiceMon.valor[i+y]:=1;
			if IndiceMon.valor[i+y]>9999999.9999 then IndiceMon.valor[i+y]:=9999999.9999;
			z:=1;
			assign(arq,'INDICE.MON');
			rewrite(arq);
			repeat
			  writeln(arq,IndiceMon.nome[z]);
			  writeln(arq,IndiceMon.valor[z]);
			  inc(z);
			until (z=31);
			close(arq);
			hidemouse;
			cores(fundomenu,frentemenu);
			retangulo(15,7,67,21);
			gotoxy(15,7); write('ÃÂÁ');
			gotoxy(64,10); write('À',chr(30)); gotoxy(64,19); write('Ê',chr(31));
         cores(fundomenusel,frentemenu);
			for z:=1 to 8 do begin gotoxy(64,10+z); write('±°'); end;
			gotoxy(64,11+trunc(((y+i)/30)*7)); write('Åþ');
			cores(fundomenu,frentemenu);
			gotoxy(16,8);
			write('              Öndice                  Valor ');
			for z:=1 to 10 do
			begin
				str(IndiceMon.valor[z+y]:7:4,aux);
				cores(fundomenu,frentemenu);
				gotoxy(16,9+z);write(' ',IndiceMon.nome[z+y],' ':33-length(IndiceMon.nome[z+y]));
				gotoxy(50,9+z);write(' ',aux,' ':12-length(aux));
			end;
			cores(fundomenusel,frentemenusel);
			str(IndiceMon.valor[i+y]:7:4,aux);
			gotoxy(16,9+i);write(' ',IndiceMon.nome[i+y],' ':33-length(IndiceMon.nome[i+y]));
			gotoxy(50,9+i);write(' ',aux,' ':12-length(aux));
			showmouse;
			c:=#2;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure Tabelas(texto:string;var saida1,saida2:byte);
var esc : array[1..11] of string[22];
	 i,y : byte;
	 c   : char;
    itens : array[1..26] of string[80];
	 esco,ii: byte;
	 xC,yC : array[1..26] of byte;
	 event : tevent;
{---------------------------------}
Procedure Desenhar;
var y : byte;
begin
   cores(FundoMenu,FrenteMenu);
	retangulo(4,2,27,14);
	sombra(4,2,27,14,8);
	cores(FundoMenu,FrenteMenu);
	for y:=1 to 10 do
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
	esc[1]:= ' Desconto por classe ';
	esc[2]:= ' Tipos de servi‡o    ';
	esc[3]:= ' A‡äes               ';
	esc[4]:= ' Tribunais/Comarcas  ';
	esc[5]:= ' àrgÆos julgadores   ';
	esc[6]:= ' Eventos             ';
	esc[7]:= ' Advogados           ';
	esc[8]:= ' Mat‚rias            ';
	esc[9]:= ' Öndices monet rios  ';
	esc[10]:=' Controle            ';
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
		if ((event.where.x>3)and(event.where.x<25))and
			((event.where.y>1)and(event.where.y<12))and
			(event.buttons=mbleftbutton) then
		begin
			hidemouse;
			cores(FundoMenu,FrenteMenu);
			for y:=1 to 10 do
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
			for y:=1 to 10 do
			begin
				gotoxy(5,2+y);
				write(esc[y]);
			end;
			dec(i);
			if i=0 then i:=10;
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
			for y:=1 to 10 do
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
			_orig:=$b800;_dest:=tela[2];movepagina;
			case i of
				1 : DescontoPorClasse;
				2 : TiposDeServico;
				3 : Acao;
				9 : IndiceMonetario;
			end;
			c:=#2;
			_dest:=$b800;_orig:=tela[2];movepagina;
			Desenhar;showmouse;
		end;
	until (c=#27);
	hidemouse;
end;
{----------------------------------------------------------------}
Procedure MenuPrincipal;
var esc : array[1..10] of string[14];
	 i,y : byte;
	 c   : char;
	 z   : byte;
	 menu: string[80];
begin
	while keypressed do c:=readkey;
	c:=#2;
	esc[1]:=' Tabelas      ';
	esc[2]:=' Clientes     ';
	esc[3]:=' Processos    ';
	esc[4]:=' Relat¢rios   ';
	esc[5]:=' Aplicativos  ';
	esc[6]:=' Financeiro   ';  {ÄÄÄÄÄ¿                }
	esc[7]:=' Comunica‡Æo  ';  {ÄÄÄÄÄÅÄÄÄ- EstÆo no   }
	esc[8]:=' Configura‡Æo ';  {ÄÄÄÄÄÅÄÄÄ- ¡tem Op‡äes}
	esc[9]:=' Arquivos     ';  {ÄÄÄÄÄÙ                }
	esc[10]:=' Sair         ';
	hidemouse;
	cores(fundo,0);
	y:=1;
	clrscr;
   menu:=' ð '+#255+' Tabelas '+#255+' Clientes '+#255+' Processos '+#255
	  +' Relat¢rios '+#255+' Aplicativos '+#255+' Op‡äes '+#255+' Sair '+#255;
	EscreveBarra(1,25,'Controle de Jurisprudˆncia',fundobarra,frentebarra);
	EscreveBarra(1,1, '¸ Juarez Fioravanti Filho ',fundobarra,frentebarra);
	cores(fundomenu,frentemenu);
	Sombra(23,6,59,17,SombraAtrib);
	Retangulo(23,6,59,17);
	cores(fundomenu,lightred);
	gotoxy(28,8); write('Controle de Jurisprudˆncia');
	cores(fundomenu,frentemenu);
	gotoxy(36,10); write('VersÆo 1');
	gotoxy(33,12);write('Copyright ¸ 1993');
	cores(fundomenu,yellow);
	gotoxy(30,14);write('Juarez Fioravanti Filho');
	Espera(3000);cores(fundo,0);window(21,6,59,19);clrscr;window(1,1,79,25);
	repeat
		showmouse;
		z:=Menu_de_Barra(1,1,menu,fundobarra,frentebarra,fundomenusel,frentemenusel,y);
		y:=z;
		hidemouse;_orig:=$b800;_dest:=tela[1];movepagina;showmouse;
		case z of
			1 : Sobre;
			2 : Tabelas(menu,y,z);
			8 : Sair(menu,y,z);
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
	writeln('Carregando Controle de Jurisprudˆncia...');
	LerTabelaASCII;
	cursor(true,6400);
	initevents;

	fundo:=darkgray;        SombraAtrib:=darkgray;

	fundomenu:=blue;	   	frentemenu:=white;   	menutitulo:=lightred;
	Fundomenusel:=red;		frentemenusel:=yellow;

	fundobarra:=lightgray;  frentebarra:=black;

	getmem(segm,16384);	tela[1]:=seg(segm^);
	getmem(segm,16384);	tela[2]:=seg(segm^);
	getmem(segm,16384);	tela[3]:=seg(segm^);
	MenuPrincipal;
	freemem(ptr(tela[1],0),16384);
	freemem(ptr(tela[2],0),16384);
	freemem(ptr(tela[3],0),16384);
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
