uses dos,crt;
var regs      : registers;
	 i,c,y,x   : byte;
	 largura   : ^byte;
	 nlinhas   : ^byte;
	 altura 	  : ^word;
	 pagevideo : ^byte;
	 auxiliar  : ^byte;
	 auxiliar2 : ^word;
	 showmode  : string[4];
begin
	directvideo:=false;
	writeln;
	writeln('Entre com o modo de video:');
	readln(i);
	begin
		regs.ah:=0;
		regs.al:=i;
		intr($10,regs);
		gotoxy(1,1);
		largura:=ptr($40,$4a);   		nlinhas:=ptr($40,$84);
		showmode:='Î';
		for x:=0 to largura^-1 do
		for y:=0 to nlinhas^ do
		begin
			regs.ah:=$13;
			regs.al:=0;
			regs.bl:=15;
			regs.bh:=0;
			regs.cx:=1;
			regs.dl:=x;
			regs.dh:=y;
			regs.es:=seg(showmode);
			regs.bp:=ofs(showmode[1]);
			intr($10,regs);
		end;
		gotoxy(1,1);
      regs.dx:=0;
		for c:=0 to 255 do
		begin
			str(i:3,showmode);
			showmode:='°'+showmode+chr(0);
			regs.ah:=$13;
			regs.al:=1;
			regs.bl:=c;
			regs.bh:=0;
			regs.cx:=4;
			inc(regs.dl,4);
			if regs.dl>largura^ then
			begin
				regs.dl:=0;
				inc(regs.dh);
			end;
			regs.es:=seg(showmode);
			regs.bp:=ofs(showmode[1]);
			intr($10,regs);
		end;
		writeln;
		textattr:=15;
		auxiliar:=ptr($40,$49);		writeln('Bios -> Modo: ',auxiliar^,'   ');
		auxiliar2:=ptr($40,$4a); 	writeln('Bios -> Caracteres: ',auxiliar2^,'   ');
		auxiliar2:=ptr($40,$4c); 	writeln('Bios -> Tamanho: ',auxiliar2^,'   ');
		regs.ah:=$11;
		regs.al:=$30;
		regs.bh:=$7;
		intr($10,regs);
		writeln('VGA -> Altura dos caracteres: ',regs.cx,'   ');
		writeln('VGA -> Linha do fundo: ',regs.dl,'   ');
		altura:=ptr($40,$85);    		pagevideo:=ptr($40,$62);
		writeln('Caracteres ',largura^,' x ',nlinhas^+1,' ativado.   ');
		writeln('Existem ',altura^,' linhas de varredura   ');
		writeln('P gina ativa: ',pagevideo^,'   ');
		writeln;
		readln;
	end;
{	regs.ah:=$11;
	regs.al:=$10;
	regs.bh:=altura^;
	regs.bl:=0;
	regs.dx:=0;
	regs.cx:=0;
	intr($10,regs);
   regs.ax:=$1200;
   regs.bx:=$0020;
   regs.cx:=0;
   regs.dx:=0;
	intr($10,regs);
	textattr:=3;}
	clrscr;
end.
