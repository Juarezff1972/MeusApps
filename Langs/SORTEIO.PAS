unit sorteio;
interface
uses crt,dos;
var _dest,_orig: word;
	t : array[1..4096] of byte;
	fundo                      : byte; { Plano de fundo }
	fundomenu,frentemenu       : byte; { Menu principal }
	menutitulo                 : byte; { Titulo do menu }
	fundomenusel,frentemenusel : byte; { Menu principal - selecionado }
	fundobarra,frentebarra     : byte; { Barra superior e inferior }
	SombraAtrib                : word; { Cor da sombra }

	regs                  : registers;
	segm                  : pointer;     tela                  : array[1..3] of word;

Procedure MovePagina;
Procedure Retangulo(x1,y1,x2,y2:byte);
Procedure Sombra(x1,y1,x2,y2:byte;sombraatrib:word);
Procedure Cursor(o:boolean;cursoroff:word);
Procedure Cores(cor_fundo,cor_texto:byte);
Function  Caracter(x,y : byte) : char;
Procedure EscreveBarra(x,y,tam : byte; texto : string; fundo,frente : byte);
Procedure Relogio(x,y:integer);
Procedure Sobre;
{-------------------------------------------------------------------------}
implementation
{-------------------------------------------------------------------------}
Procedure MovePagina;
Begin
   InLine($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>_Dest/            { mov dx,_Dest }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>_Orig/            { mov dx,_Orig }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }
End;
{---------------------------------------------------------------------------}
procedure retangulo;
var x,y : integer;
begin
   window(x1,y1,x2,y2);
   gotoxy(1,1);             writeln('É');
   gotoxy(x2-x1,y2-y1);     writeln('¼');
   gotoxy(x2-x1,1);         writeln('»');
   gotoxy(1,y2-y1);         writeln('È');
   for y:=2 to y2-(y1+1) do
   begin
      gotoxy(1,y);         writeln('º');
		gotoxy(x2-x1,y);     writeln('º');
	end;
	for x:=2 to x2-(x1+1) do
	begin
		gotoxy(x,1);         writeln('Í');
		gotoxy(x,y2-y1);     writeln('Í');
   end;
   window(x1+1,y1+1,x2-2,y2-2);
   clrscr;
   window(1,1,80,25);
end;
{---------------------------------------------------------------------------}
procedure sombra;
var x,y : integer;
    z,t : byte;
begin
   for x:=x1 to x2-3 do
   begin
		gotoxy(x,y2);
      regs.ah:=8;
      intr($10,regs);
      z:=regs.ah;
      t:=regs.al;
      if (z=sombraatrib)or(z=0) then z:=0 else z:=sombraatrib;
      if t=219 then z:=0;
      regs.cx:=1;
      regs.bx:=z;
      regs.ah:=9;
      intr($10,regs);
   end;
   for x:=x1-2 to x1-1 do
      for y:=y1+1 to y2 do
      begin
         gotoxy(x,y);
         regs.ah:=8;
         intr($10,regs);
         z:=regs.ah;
         t:=regs.al;
         if (z=sombraatrib)or(z=0) then z:=0 else z:=sombraatrib;
         if t=219 then z:=0;
         regs.cx:=1;
         regs.bx:=z;
         regs.ah:=9;
         intr($10,regs);
      end;
end;
{---------------------------------------------------------------------------}
Procedure Cursor;
begin
   if o=true then
   begin
      regs.ax:=$168;
      regs.cx:=cursoroff; { Desativa cursor }
      intr($10,regs);
   end;
   if o=false then
   begin
      regs.ax:=$168;
      regs.cx:=$607;      { Ativa cursor }
      intr($10,regs);
   end;
end;
{---------------------------------------------------------------------------}
procedure cores;
begin
  textattr:=cor_fundo*16+cor_texto;
end;
{--------------------------------------------------------------------------}
Function Caracter;
begin
	gotoxy(x,y);
	regs.ah:=8;
	regs.bh:=0;
	intr($10,regs);
	caracter:=chr(regs.al);
end;
{-------------------------------------------------------------------------}
Procedure Relogio;
var h,m,s,c : word;
	 texto   : string[5];
	 aux     : string[2];
	 v,u,w   : byte;
begin
	texto:='';
	gettime(h,m,s,c);
	str(h,aux);	if h<10 then texto:='0'+aux else texto:=aux;
	if (s div 2)=(s/2) then texto:=texto+':' else texto:=texto+' ';
	str(m,aux);	if m<10 then texto:=texto+'0'+aux else texto:=texto+aux;
	for s:=1 to 5 do
	begin
		v:=73+lo(s);
		u:=ord(texto[s]);
		w:=7*16+0;
		asm
			mov ah,2;mov bh,0;mov dh,0;mov dl,v;int $10
			mov ah,9;mov al,u;xor bh,bh;mov bl,w;mov cx,1;int $10
		end;
	end;
	gotoxy(x,y);
end;
{-------------------------------------------------------------------------}
Procedure EscreveBarra;
var i : byte;
begin
	gotoxy(x,y);
	regs.ah:=$9;
	regs.al:=32;
	regs.cx:=tam;
	regs.bh:=0;
	regs.bl:=fundo*16+frente;
	intr($10,regs);
	cores(fundo,frente);
	gotoxy(x+((tam div 2)-(length(texto)div 2)),y); write(texto);
end;
{-------------------------------------------------------------------------}
Procedure Sobre;
var c 	 : char;
	 i 	 : byte;
	 go    : boolean;
begin
	go:=false;
	cores(fundomenu,frentemenu);
	Sombra(23,6,59,19,SombraAtrib);
	Retangulo(23,6,59,19);
	gotoxy(28,8); write('Controle de Jurisprudˆncia');
	gotoxy(36,10); write('VersÆo 1');
	gotoxy(33,12);write('Copyright ¸ 1993');
	gotoxy(30,14);write('Juarez Fioravanti Filho');
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(38,16);
	write('  Ok  ');
	cores(FundoMenu,Black);
	for i:=37 to 42 do begin gotoxy(i,17);write(chr(223));end;
	gotoxy(37,16);write(chr(220));
	repeat until keypressed;
	cores(FundoMenuSel,FrenteMenuSel);
	gotoxy(37,16);
	write('  Ok  ');
	cores(FundoMenu,FrenteMenu);
	for i:=37 to 42 do begin gotoxy(i,17); write(chr(32)); end;
	gotoxy(43,16); write(' ');
end;
{----------------------------------------------------------------}
begin
end.
