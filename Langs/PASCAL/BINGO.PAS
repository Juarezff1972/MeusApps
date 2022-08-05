program bingo;
uses dos,crt,sorteio,printer;
type
  list = array[1..25] of byte;
var
  regs		  : registers;
  data		  : list;
  a			  : byte;
  numcartelas : longint;
  premios     : array[1..10] of string[23];
  preco       : string[20];
  i           : longint;
  tecla,p  : char;
{-------------------------------------------------------------------------}
procedure entrada;
var b    	: char;
	 numcart : string[6];
	 erro    : integer;
begin
  numcart:='';
  cores(1,15);
  clrscr;
  escrevebarra(1,1,80,'Gerador de Cartelas de Bingo',7,0);
  escrevebarra(1,25,80,'Holder Sistemas Ltda.',7,0);
  cores(1,15);
  retangulo(8,4,74,7);
  sombra(8,4,74,7,7);
  gotoxy(10,5); write('Entre com o n£mero de cartelas a serem processadas: ');
  cursor(true,$1900);
  repeat
	 relogio(1,70);
	 gotoxy(62+length(numcart),5);
	 cores(1,15+blink); write('_ ');
	 cores(1,15);
	 if keypressed then b:=readkey else b:=#1;
	 if (ord(b)>47)and(ord(b)<58) then numcart:=numcart+b;
	 if (ord(b)=8) then
	 begin
		delete(numcart,length(numcart),1);
		gotoxy(62,5); write(numcart);
		cores(1,15+blink); write('_ ');
		cores(1,15);
	 end;
	 if (ord(b)=13)and(numcart<>'') then b:=#00;
	 if (ord(b)=27) then begin cores(0,15); clrscr; cursor(false,$0607); halt(1); end;
	 if (ord(b)>47)and(ord(b)<58) then
	 begin
		gotoxy(61+length(numcart),5);
		write(b);
		cores(1,15+blink); write('_');
		cores(1,15);
	 end;
  until b=#00;
  gotoxy(62+length(numcart),5); write(' ');
  val(numcart,numcartelas,erro);
  retangulo(8,9,74,23);
  sombra(8,9,74,23,7);
  escrevebarra(9,10,64,'Cadastramento dos Prˆmios',7,0);
  cores(1,15);
  for a:=1 to 10 do premios[a]:='';
  for a:=1 to 10 do
  begin
	 gotoxy(10,11+a);
	 if a<10 then write(' ');
	 write(a,'§ Prˆmio: ');
	 repeat
		relogio(1,70);
		gotoxy(22+length(premios[a]),11+a);
		cores(1,15+blink); write('_ ');
		cores(1,15);
		if keypressed then b:=readkey else b:=#1;
		if b=#0 then
		begin
		  b:=readkey;
		  if (b<>#72)and(b<>#80) then b:=#0;
		  if b=#80 then b:=#13;
		  if b=#72 then
		  begin
			 gotoxy(22+length(premios[a]),11+a);
			 write(' ');
			 dec(a);
			 if a < 1 then a:=1;
			 b:=#0;
		  end;
		end;
		if b=#39 then
		begin
		  b:=readkey;
		  if b='C' then b:='€';
		  if b='c' then b:='‡';
		  if b='a' then b:=' ';
		  if b='e' then b:='‚';
		  if b='i' then b:='¡';
		  if b='o' then b:='¢';
		  if b='u' then b:='£';
		  if b='E' then b:='';
		end;
		if b=#94 then
		begin
		  b:=readkey;
		  if b='e' then b:='ˆ';
		  if b='o' then b:='“';
		end;
		if b=#126 then
		begin
		  b:=readkey;
		  if b='a' then b:='„';
		  if b='A' then b:='';
		end;
		if (ord(b)>31)and(ord(b)<255) then premios[a]:=premios[a]+b;
		if (ord(b)=8) then
		begin
		  delete(premios[a],length(premios[a]),1);
		  gotoxy(22,11+a); write(premios[a]);
		  cores(1,15+blink); write('_ ');
		  cores(1,15);
		end;
		if (ord(b)=13)and(premios[a]<>'') then b:=#2;
		if (ord(b)=27) then begin cores(0,15); clrscr; cursor(false,$0607); halt(1); end;
		if (ord(b)>31)and(ord(b)<255) then
		begin
		  gotoxy(21+length(premios[a]),11+a);
		  write(b);
		  cores(1,15+blink); write('_');
		  cores(1,15);
		end;
	 until b=#2;
	 gotoxy(22+length(premios[a]),11+a);
	 write(' ');
  end;
  preco:='';
  retangulo(8,4,74,7);
  sombra(8,4,74,7,7);
  gotoxy(10,5); write('Entre com o pre‡o das cartelas: ');
  cursor(true,$1900);
  repeat
	 relogio(1,70);
	 gotoxy(42+length(preco),5);
	 cores(1,15+blink); write('_ ');
	 cores(1,15);
	 if keypressed then b:=readkey else b:=#1;
	 if (((ord(b)>47)and(ord(b)<58))or(ord(b)=46)or(ord(b)=44)) then
		 preco:=preco+b;
	 if (ord(b)=8) then
	 begin
		delete(preco,length(preco),1);
		gotoxy(42,5); write(preco);
		cores(1,15+blink); write('_ ');
		cores(1,15);
	 end;
	 if (ord(b)=13)and(preco<>'') then b:=#00;
	 if (ord(b)=27) then begin cores(0,15); clrscr; halt(1); end;
	 if (((ord(b)>47)and(ord(b)<58))or(ord(b)=46)or(ord(b)=44)) then
	 begin
		gotoxy(41+length(preco),5);
		write(b);
		cores(1,15+blink); write('_');
		cores(1,15);
	 end;
  until b=#00;
  gotoxy(42+length(preco),5); write(' ');
end;
{-------------------------------------------------------------------------}
procedure quicksort(var a: list; Lo,Hi: integer);
{-------------------------------------------------}
procedure sort(l,r: integer);
var
  i,j,x,y: integer;
begin
  i:=l; j:=r; x:=a[(l+r) DIV 2];
  repeat
    while a[i]<x do i:=i+1;
    while x<a[j] do j:=j-1;
    if i<=j then
    begin
      y:=a[i]; a[i]:=a[j]; a[j]:=y;
      i:=i+1; j:=j-1;
    end;
  until i>j;
  if l<j then sort(l,j);
  if i<r then sort(i,r);
end;
{-------------------------------------------------}
begin {quicksort};
  sort(Lo,Hi);
end;
{-------------------------------------------------------------------------}
procedure numsorteio;
var aux,i  : byte;
	 vf     : boolean;
	 cont   : byte;
begin
  randomize;
  cont:=0;
  vf:=true;
  repeat
	 aux:=random(75)+1;
	 for i:=1 to cont do
	 begin
		if aux=data[i] then vf:=false else vf:=true;
		if vf=false then i:=cont;
    end;
	 if vf=true then
	 begin
		inc(cont);
		data[cont]:=aux;
	 end;
  until (cont>=25);
end;
{-------------------------------------------------------------------------}
procedure imprimir;
var numero : string;
	 aux    : string[1];
	 erro   : byte;
begin
  {$I-}
  write(lst,chr(27),'@',chr(27),'t',chr(1));
  erro:=ioresult;
  if erro<>0 then
  begin
	 cores(red,white);
	 retangulo(25,12,55,16);
	 gotoxy(27,13); write('Falha na Impressao');
	 gotoxy(27,14); write('Pressione qualquer tecla');
     repeat
		 write(lst,chr(27),'@',chr(27),'t',chr(1));
		 erro:=ioresult;
		 if keypressed then p:=readkey;
		 if p=#27 then exit;
	  until (erro=0)and(keypressed);
  end;
  {$i+}
  cores(blue,white);
  retangulo(25,12,55,16);
  gotoxy(27,13); write('Aguarde ...');
  gotoxy(27,14); write('Imprimindo cartela n§ ',i);
  str(i,numero);  for a:=1 to 6-length(numero) do numero:='0'+numero;
  preco:='CR$ '+preco;
  writeln(lst,numero,preco:71);
  writeln(lst,'ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»');
  writeln(lst,'º         Movimento Familiar Filhos de Nossa Senhora de Guadalupe           º');
  writeln(lst,'º                     Capela Nossa Senhora da Penha                         º');
  writeln(lst,'º                      Vila Guaracy - Colombo - Pr.                         º');
  writeln(lst,'ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼');
  writeln(lst,'ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»');
  for a:=1 to 5 do
  begin
	 if a<5 then aux:=' ' else aux:='';
	 writeln(lst,'º ',a,'§ Prˆmio: ',Premios[a],' ':(24-length(Premios[a])),
				'  ',aux,a+5,'§ Prˆmio: ',Premios[a+5],' ':(24-length(Premios[a+5])),' º');
  end;
  writeln(lst,'ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼');
  writeln(lst,'');
  writeln(lst,'');
  writeln(lst,'');
  writeln(lst,'   1§ Prˆmio           2§ Prˆmio            3§ Prˆmio           4§ Prˆmio');
  writeln(lst,'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿');
  for a:=1 to 5 do
  writeln(lst,'³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³    ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³     ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³    ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³');
  writeln(lst,'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ');
  writeln(lst,'');
  writeln(lst,'');
  writeln(lst,'');
  writeln(lst,'   5§ Prˆmio           6§ Prˆmio            7§ Prˆmio           8§ Prˆmio');
  writeln(lst,'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿');
  for a:=1 to 5 do
  writeln(lst,'³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³    ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³     ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³    ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³');
  writeln(lst,'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ');
  writeln(lst,'');
  writeln(lst,'');
  writeln(lst,'');
  writeln(lst,'   9§ Prˆmio          10§ Prˆmio           Conferˆncia');
  writeln(lst,'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿');
  for a:=1 to 5 do
  writeln(lst,'³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³    ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³     ',
			 '³',data[a]:2,' ',data[a+5]:2,' ',data[a+10]:2,' ',data[a+15]:2,' ',data[a+20]:2,'³');
  writeln(lst,'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ');
  writeln(lst,chr(12));
end;
{-------------------------------------------------------------------------}
begin
  p:=#00;
  for a:=1 to 25 do data[a]:=0;
  entrada;
  retangulo(25,12,55,16);
  sombra(25,12,55,16,7);
  for i:=1 to numcartelas do
  begin
	 numsorteio;
	 quicksort(data,1,25);
	 imprimir;
  end;
  if p=#27 then begin gotoxy(27,13); write('Impressao Cancelada!'); end
  else begin gotoxy(27,13); write('Impressao Completa!'); end;
  gotoxy(27,14); write('Aperte qualquer tecla');
  while keypressed do tecla:=readkey;
  repeat
	 tecla:=readkey;
  until tecla<>#00;
  cores(0,15);
  clrscr;
  cursor(false,$0607)
end.