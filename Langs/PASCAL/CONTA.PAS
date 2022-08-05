program contacorrente;
uses dos,crt,util,escrbar;
type cadastro = record
                   nome     : string;
                   endereco : string;
                   cep      : string;
                   cidade   : string;
                   estado   : string;
                   cpf      : string;
                   rg       : string;
                 end;
var
   dados                 : cadastro;
   f                     : file of cadastro;
   ano,mes,dia,semana    : word;
   hora,minuto,sec,decim : word;
   orig,dest             : word;
{---------------------------------------------------------------------------}
Procedure MostraData;
begin
    getdate(ano,mes,dia,semana);
    gettime(hora,minuto,sec,decim);
    cores(lightgray,black);
    gotoxy(2,1); write(hora:2,':');
    gotoxy(5,1); if minuto<10 then write('0',minuto,':') else write(minuto,':');
    gotoxy(8,1); if sec<10 then write('0',sec) else write(sec);
    gotoxy(75,1); write(ano);
    gotoxy(72,1); if mes<10 then write('0',mes,'/') else write(mes,'/');
    gotoxy(69,1); write(dia:2,'/');
    gotoxy(2,25);
    case semana of
       0 : write('Domingo');
       1 : write('Segunda');
       2 : write('Ter‡a  ');
       3 : write('Quarta ');
       4 : write('Quinta ');
       5 : write('Sexta  ');
       6 : write('S bado ');
    end;
end;
{---------------------------------------------------------------------------}
Procedure Apresentacao;
begin
  gotoxy(1,1); preenche(219,2000,0,8);
  gotoxy(1,1); preenche(219,80,1,7);
  gotoxy(1,25);preenche(219,80,1,7);
  cores(blue,white);
  retangulo(25,6,55,18);
  sombra(25,6,55,18,0);
  cores(lightgray,black);
  gotoxy(26,7); write(' ':12,'MENU',' ':12);
  cursor(true,6400);
end;
{---------------------------------------------------------------------------}
Procedure MovePagina;
begin
   inline($b9/$80/$3e/               { mov cx,3E80  }
          $33/$ff/                   { xor di,di    }
          $8b/$16/>dest/             { mov dx,_DEST }
          $8e/$c2/                   { mov es,dx    }
          $33/$f6/                   { xor si,si    }
          $8b/$16/>orig/             { mov dx,_ORIG }
          $1e/                       { push ds      }
          $8e/$da/                   { mov ds,dx    }
          $d1/$e9/                   { shr cx,1     }
          $f3/$a5/                   { rep movsw    }
          $1f);                      { pop ds       }

          { Pagina 0 : $b800 }  { Pagina 8 : $a400 }
          { Pagina 1 : $8800 }  { Pagina 9 : $a800 }
          { Pagina 2 : $8c00 }  { Pagina 10: $ac00 }
          { Pagina 3 : $9000 }  { Pagina 11: $b000 }
          { Pagina 4 : $9400 }  { Pagina 12: $b400 }
          { Pagina 5 : $9800 }
          { Pagina 6 : $9c00 }
          { Pagina 7 : $a000 }
end;
{---------------------------------------------------------------------------}
Procedure trata_char(posx,posy:integer;letra:char;tam:byte;
                     var s:string;var resp:boolean);
begin
  if (letra<>#13) and (letra<>#8) and (letra<>#27) then
  begin
    letra:=upcase(letra);
    if length(s)<tam then s:=s+letra;
    gotoxy(posx,posy); write(s);
  end;
  if (letra=#8) then
  begin
    delete(s,length(s),1);
    gotoxy(posx,posy); write(' ':tam);
    gotoxy(posx,posy); write(s);
  end;
  if letra=#27 then
  begin
    s:='';
    resp:=true;
  end;
  if letra=#13 then resp:=true;
end;
{---------------------------------------------------------------------------}
Procedure Dados_Cadastrais;
var b          : char;
    pos        : byte;
    resp,cond  : boolean;
{----------------------}
Procedure le_arquivo;
var
    proc : searchrec;
begin
   FindFirst('c:\sinoda\cc\info.cc',anyfile,proc);
   if doserror=18 then halt(1);
   chdir('c:\sinoda\cc');
   assign(f,'info.cc');
   reset(f);
   read(f,dados);
   close(f);
end;
{----------------------}
Procedure Entrada(pos1,pos2,t:byte;var s:string;var c:boolean);
begin
  b:=#1;
  resp:=false;
  repeat
    mostradata;
    cores(blue,yellow);
    b:=#1;
    if keypressed then
    begin
      b:=readkey;
      if b=#27 then c:=true;
      if b=#00 then begin b:=readkey; b:=#1; end;
      if b<>#1 then trata_char(pos1,pos2,b,t,s,resp);
    end;
    if resp then b:=#13;
    if b<>#13 then
    begin
      gotoxy(length(s)+pos1,pos2);
      cores(blue,yellow+blink);
      write(chr(05));
    end;
    if b=#13 then
    begin
      gotoxy(length(s)+pos1,pos2);
      cores(blue,blue);
      write(chr(05));
    end;
  until resp;
end;
{----------------------}
Procedure Gravar_Arquivo;
var
    proc : searchrec;
begin
   FindFirst('c:\sinoda\cc\info.cc',anyfile,proc);
   if doserror=18 then halt(1);
   chdir('c:\sinoda\cc');
   assign(f,'info.cc');
   rewrite(f);
   write(f,dados);
   close(f);
end;
{----------------------}
begin
  orig:=$b800;
  dest:=$8800;
  movepagina;
  cores(blue,white);
  retangulo(15,6,65,18);
  sombra(15,6,65,18,8);
  cores(lightgray,black);
  gotoxy(16,7); write(' ':16,'DADOS CADASTRAIS',' ':16);
  cores(blue,white);
  le_arquivo;
  gotoxy(16,9); write(' NOME: '    ); cores(blue,yellow); write(dados.nome); cores(blue,white);
  gotoxy(16,10);write(' ENDERE€O: '); cores(blue,yellow); write(dados.endereco); cores(blue,white);
  gotoxy(16,11);write(' CEP: '     ); cores(blue,yellow); write(dados.cep); cores(blue,white);
  gotoxy(16,12);write(' CIDADE: '  ); cores(blue,yellow); write(dados.cidade); cores(blue,white);
  gotoxy(16,13);write(' ESTADO: '  ); cores(blue,yellow); write(dados.estado); cores(blue,white);
  gotoxy(16,14);write(' CPF: '     ); cores(blue,yellow); write(dados.cpf); cores(blue,white);
  gotoxy(16,15);write(' R.G.: '    ); cores(blue,yellow); write(dados.rg); cores(blue,white);
  cond:=false;
  entrada(23,9,40,dados.nome,cond);
  if cond=false then entrada(27,10,36,dados.endereco,cond);
  if cond=false then entrada(22,11,41,dados.cep,cond);
  if cond=false then entrada(25,12,38,dados.cidade,cond);
  if cond=false then entrada(25,13,38,dados.estado,cond);
  if cond=false then entrada(22,14,41,dados.cpf,cond);
  if cond=false then entrada(23,15,40,dados.rg,cond);
  if cond<>true then
  begin
    cores(blue,white);
    retangulo(30,20,50,23);
    sombra(30,20,50,23,0);
    writexy(31,21,' GRAVAR?  (S/N)');
    cores(blue,lightred);
    writexy(44,21,'N');
    repeat
      b:=readkey;
      b:=upcase(b);
      if b=#78 then b:=#27;
      if b=#83 then
      begin
        cores(blue,white);
        writexy(44,21,'N');
        cores(blue,lightred);
        writexy(42,21,'S');
        gravar_arquivo;
        b:=#27;
      end;
    until (b=#27);
  end;
  orig:=$8800;
  dest:=$b800;
  movepagina;
end;
{---------------------------------------------------------------------------}
Procedure Botao(bt:byte;resp:boolean);
var s   : string;
    x,y : byte;
begin
  case bt of
    0: exit;
    1: begin x:=57;  y:=6;  s:=' Inclus„o '; end;
    2: begin x:=57;  y:=8;  s:=' Consulta '; end;
    3: begin x:=57;  y:=10; s:=' Exclus„o '; end;
    4: begin x:=57;  y:=12; s:=' Altera‡„o'; end;
    5: begin x:=57;  y:=16; s:=' Finalizar'; end;
    6: begin x:=57;  y:=14; s:=' ImPress„o'; end;
  end;
  if resp then
  begin
    gotoxy(x,y);
    cores(green,white);
    write(s);
    cores(blue,black);
    writexy(x+10,y,'Ü');
    writexy(x+1,y+1,'ßßßßßßßßßß');
    cores(brown,white);
    case bt of
      0: exit;
      1: writexy(58,6,'I');
      2: writexy(58,8,'C');
      3: writexy(58,10,'E');
      4: writexy(58,12,'A');
      5: writexy(58,16,'F');
      6: writexy(60,14,'P');
    end;
  end;
  if resp=false then
  begin
    gotoxy(x,y);
    cores(red,white);
    write(s);
    cores(blue,black);
    writexy(x+10,y,'Ü');
    writexy(x+1,y+1,'ßßßßßßßßßß');
    cores(lightgray,black);
    case bt of
      0: exit;
      1: writexy(58,6,'I');
      2: writexy(58,8,'C');
      3: writexy(58,10,'E');
      4: writexy(58,12,'A');
      5: writexy(58,16,'F');
      6: writexy(60,14,'P');
    end;
  end;
end;
{---------------------------------------------------------------------------}
Procedure Plano;
var b    : char;
    bt   : byte;
    resp : boolean;
    i    : byte;
begin
  orig:=$b800;
  dest:=$8800;
  movepagina;
  cores(blue,white);
  retangulo(10,3,70,22);
  sombra(10,3,70,22,8);
  writexy(11,5,'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄ¿');
  writexy(11,6,'³             DESCRI€O              ³  N§  ³');
  writexy(11,7,'ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄ´');
  for i:=8 to 20 do writexy(11,i,'³');
  for i:=8 to 20 do writexy(55,i,'³');
  for i:=8 to 20 do writexy(48,i,'³');
  writexy(11,20,'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÙ');
  cores(lightgray,black);
  gotoxy(11,4); write(' ':22,'PLANO DE CONTAS',' ':21);
  botao(1,true);
  botao(2,true);
  botao(3,true);
  botao(4,true);
  botao(5,true);
  botao(6,true);
  cores(blue,white);
  writexy(12,10 ,'Conta n§ :');
  writexy(12,12,'Descri‡ao:');
  writexy(12,14,'D/C :');
  resp:=false;
  bt:=0;
{  repeat
    mostradata;
    if keypressed then b:=readkey;
    b:=upcase(b);
    if b=#73 then begin botao(bt,true); botao(1,false); bt:=1; plano_incluir;  b:=#1 end;
    if b=#67 then begin botao(bt,true); botao(2,false); bt:=2; plano_consultar;b:=#1 end;
    if b=#69 then begin botao(bt,true); botao(3,false); bt:=3; plano_excluir;  b:=#1 end;
    if b=#65 then begin botao(bt,true); botao(4,false); bt:=4; plano_alterar;  b:=#1 end;
  until (b=#27)or resp=true;
}
  orig:=$8800;
  dest:=$b800;
  movepagina;
end;
{---------------------------------------------------------------------------}
Procedure Opcao;
var esc : array[1..8] of string;
    i   : integer;
    b   : char;
begin
  cores(blue,white);
  esc[1]:=' Lan‡amentos              ';
  esc[2]:=' Hist¢ricos               ';
  esc[3]:=' Plano de Contas          ';
  esc[4]:=' Relat¢rios               ';
  esc[5]:=' Limpeza                  ';
  esc[6]:=' Op‡”es                   ';
  esc[7]:=' Dados Cadastrais         ';
  esc[8]:=' Finalizar                ';
  for i:=1 to 8 do writexy(27,i+8,esc[i]);
  cores(green,white);
  writexy(27,9,esc[1]);
  i:=1;
  b:=#00;
  repeat
    mostradata;
    b:=#00;
    if keypressed then b:=readkey;
    if b=#72 then
    begin
      cores(blue,white);
      writexy(27,i+8,esc[i]);
      dec(i);
      if i<1 then i:=8;
      cores(green,white);
      writexy(27,i+8,esc[i]);
      b:=#00;
    end;
    if b=#80 then
    begin
      cores(blue,white);
      writexy(27,i+8,esc[i]);
      inc(i);
      if i>8 then i:=1;
      cores(green,white);
      writexy(27,i+8,esc[i]);
      b:=#00;
    end;
    {  if (b=#13)and(i=1) then Lancamentos;
       if (b=#13)and(i=2) then Historicos;}

       if (b=#13)and(i=3) then Plano;

{      if (b=#13)and(i=4) then Relatorios;
       if (b=#13)and(i=5) then Limpeza;
       if (b=#13)and(i=6) then Opcoes;}

       if (b=#13)and(i=7) then Dados_Cadastrais;
       if (b=#13)and(i=8) then b:=#27;
  until (b=#27);

end;
{---------------------------------------------------------------------------}
begin
  cont:=1;
  assign(asci,'C:\AUXILIAR\char860');
  reset(asci);
  regs.ah:=$A;regs.al:=$4;regs.bl:=0;intr($10,regs);
  regs.ah:=$A;regs.al:=$30;regs.bh:=6;intr($10,regs);
  for cont:=1 to (255*16)+15 do read(asci,t[cont]);
  close(asci);
  Desenha187;Desenha176;Desenha177;Desenha179;Desenha186;Desenha188;
  Desenha192;Desenha196;Desenha197;Desenha202;Desenha205;Desenha254;
  Desenha30;Desenha31;Desenha193;Desenha194;Desenha195;Desenha201;
  Desenha200;
  regs.ah:=$11;regs.al:=$0;regs.bh:=16;regs.cx:=256;regs.dx:=0;
  regs.es:=seg(t);regs.bp:=ofs(t)-1;intr($10,regs);
  dados.nome:='';
  dados.endereco:='';
  dados.cep:='';
  dados.estado:='';
  dados.cpf:='';
  dados.rg:='';
  clrscr;
  apresentacao;
  opcao;
  regs.ah:=$11;regs.al:=$4;regs.bl:=0;intr($10,regs);
  regs.ah:=$11;regs.al:=$30;regs.bh:=6;intr($10,regs);
end.