unit champ;
interface
uses crt,dos,drivers;
type
    medi = record
               nome : string;
               end1 : string;
               end2 : string;
               cpf  : string;
               tel  : string;
               espec: string;
               cart : string;
               crm  : string;
               conve: string;
               entr : real;
               saida: real;
               intv : real;
               sala : string;
             end;
    pac = record
                 med : byte;
                 nome: string;
                 ende: string;
                 tel : string;
                 ult : string
               end;
var _dest,_orig: word;
   fundo,fundomenu,frentemenu,fundobarra,frentebarra,fundoopcao,frenteopcao: byte;
   regs                  : registers;   rato                  : tevent;
   dia,mes,ano,semana    : word;        posdia,posmes         : byte;
   posano                : word;        hora,minuto,sec,decim : word;
   paciente              : pac;         medico                : array [1..12] of medi;
   horario               : real;        numeromedico          : byte;
   numerohorario         : byte;        NumeroPaciente        : word;
   cadastrado,continuar  : boolean;     remover               : byte;
   segm                  : pointer;     tela                  : array[1..3] of word;
   alteracao             : boolean;     MarcarHora            : array [1..100] of string[6];
   horaamarcar           : string[6];
Procedure MovePagina;
Procedure Retangulo(x1,y1,x2,y2:byte);
Procedure Sombra(x1,y1,x2,y2:byte;sombraatrib:word);
Procedure Cursor(o:boolean;cursoroff:word);
Procedure Cores(cor_fundo,cor_texto:byte);
Function  DecimalHora(var x : real)           : string;
Function  DiaDaSemana(Dia, Mes, Ano: Integer) : Integer;
Function  NomeDoMes(mes : byte)               : string;
Function  CriaDiretorio                       : string;
Procedure MostraData;
Procedure BarraDeCores(var Frente,Fundo : byte);
Procedure Calendario(var resp : boolean);
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
{---------------------------------------------------------------------------}
Function DecimalHora;
var hr,mn : string;
begin
   str(trunc(x):2,hr);
   str(trunc(frac(x)*60),mn);
   if trunc(frac(x)*60) < 10 then mn:='0'+mn;
   DecimalHora:=hr+':'+mn;
end;
{--------------------------------------------------------------------------}
Function DiaDaSemana;
var
  century, yr, dw,me,an: Integer;
begin
  me:=mes;
  an:=ano;
  if Mes < 3 then
  begin
    Inc(Mes, 10);
    Dec(Ano);
  end
  else
     Dec(Mes, 2);
  century := Ano div 100;
  yr := Ano mod 100;
  dw := (((26 * Mes - 2) div 10) + Dia + yr + (yr div 4) +
    (century div 4) - (2 * century)) mod 7;
  if dw < 0 then DiaDaSemana := dw + 7
  else DiaDaSemana := dw;

   if (dia>31) then if (me=1)or(me=3)or(me=5)or(me=7)or(me=8)or(me=10)or(me=12) then diadasemana:=7;
   if (dia>30) then if (me=4)or(me=6)or(me=9)or(me=11) then diadasemana:=7;
   if (dia>29) and (me=2) then if (an mod 4)=0  then diadasemana:=7;
   if (dia>28) and (me=2) then if (an mod 4)>0  then diadasemana:=7;
end;
{--------------------------------------------------------------------------}
Function NomeDoMes;
begin
   case mes of
   1 : NomeDoMes:=' Janeiro';
   2 : NomeDoMes:=' Fevereiro';
   3 : NomeDoMes:=' Mar‡o';
   4 : NomeDoMes:=' Abril';
   5 : NomeDoMes:=' Maio';
   6 : NomeDoMes:=' Junho';
   7 : NomeDoMes:=' Julho';
   8 : NomeDoMes:=' Agosto';
   9 : NomeDoMes:=' Setembro';
   10: NomeDoMes:=' Outubro';
   11: NomeDoMes:=' Novembro';
   12: NomeDoMes:=' Dezembro';
   end;
end;
{--------------------------------------------------------------------------}
Function CriaDiretorio;
var dia,mes,ano,semana    : word;
    nome : string;
    s    : searchrec;
    dt   : datetime;
    hora : longint;
    f    : file;
begin
   str(posano,nome);
   nome:=copy(nome,3,2);
   nome:=copy(nomedomes(posmes),2,8)+'.'+nome;
   s.name:='';
   findfirst(nome,anyfile-readonly,s);
   UnpackTime(s.time,dt);
   if (s.name='') then
   begin
      mkdir(nome);
      dt.day:=07;  dt.month:=09; dt.year:=1992;
      dt.hour:=11; dt.min:=34;   dt.sec:=15;
      packtime(dt,hora);
      assign(f,nome);
      setfattr(f,hidden);
   end;
   criadiretorio:=nome;
end;
{--------------------------------------------------------------------------}
Procedure MostraData;
begin
    getdate(ano,mes,dia,semana);
    gettime(hora,minuto,sec,decim);
    cores(fundobarra,frentebarra);
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
{--------------------------------------------------------------------------}
Procedure BarraDeCores;
var i : integer;
    c : char;
begin
  hidemouse;
  Cores(Black,White);
  retangulo(5,15,63,20);
  sombra(5,15,63,20,7);
  gotoxy(6,15); write('[ð]');
  Retangulo(45,11,77,14);
  Sombra(45,11,77,14,7);
  Cores(fundo,frente);
  GotoXY(47,12);
  Write('Use ',Chr(24),Chr(25),Chr(26),Chr(27),' para alterar a cor');
  For i:=0 to 15 do
  Begin
     GotoXY(9+(3*i),17);
     Regs.AH:=9;
     Regs.AL:=219;
     Regs.BH:=0;
     Regs.BL:=i;
     Regs.CX:=3;
     Intr($10,Regs);
  End;
  c:=#2;
  Repeat
     mostradata;
     if c<>#1 then
     begin
        hidemouse;
        Cores(0,15);
        GotoXY(10+(3*fundo),18);
        Write(Chr(30));
        GotoXY(10+(3*frente),16);
        Write(Chr(31));
        showmouse;
        c:=#1;
     end;
     showmouse;
     gotoxy(1,1);
     getmouseevent(rato);
     if ((rato.where.x>=8)and(rato.where.x<=55))
     and(rato.where.y=15)and(rato.buttons=mbleftbutton)then
     begin
        hidemouse;
        Cores(0,15);
        GotoXY(10+(3*frente),16);
        Write(Chr(32));
        frente:=trunc((rato.where.x+2-10)/3);
        If frente = 16 Then frente:=0;
        If frente > 16 Then frente:=15;
        c:=#2;
        showmouse;
     end;
     if ((rato.where.x>=5)and(rato.where.x<=7)
     and(rato.where.y=14))and(rato.buttons=mbleftbutton)then c:=#13;
     if ((rato.where.x>=22)and(rato.where.x<=24)and(rato.where.y=4))and
         (rato.buttons=mbleftbutton)then c:=#13;
     if ((rato.where.x>=8)and(rato.where.x<=55)
     and(rato.where.y=17))and(rato.buttons=mbleftbutton)then
     begin
        hidemouse;
        Cores(0,15);
        GotoXY(10+(3*fundo),18);
        Write(Chr(32));
        fundo:=trunc((rato.where.x+2-10)/3);
        If fundo  = 16 Then fundo:=0;
        If fundo  > 16 Then fundo:=15;
        c:=#2;
        showmouse;
     end;
     if keypressed then
     c:=ReadKey;
     if c<>#1 then
     begin
        hidemouse;
        Cores(0,15);
        GotoXY(10+(3*fundo),18);
        Write(Chr(32));
        GotoXY(10+(3*frente),16);
        Write(Chr(32));
        showmouse;
     end;
     If c=#80 Then begin inc(frente); c:=#2;  end;
     If c=#72 Then begin dec(frente); c:=#2;  end;
     If c=#77 Then begin inc(fundo); c:=#2;  end;
     If c=#75 Then begin dec(fundo); c:=#2;  end;
     If frente = 16 Then frente:=0;
     If frente > 16 Then frente:=15;
     If fundo  = 16  Then fundo:=0;
     If fundo  > 16  Then fundo:=15;
     if c<>#1 then
     begin
        hidemouse;
        Retangulo(45,11,77,14);
        Cores(fundo,frente);
        GotoXY(47,12);
        Write('Use ',Chr(24),Chr(25),Chr(26),Chr(27),' para alterar a cor');
        showmouse;
     end;
  Until ((c=#13)or(c=#27));
  c:=#2;
  hidemouse;
end;
{--------------------------------------------------------------------------}
Procedure Calendario;
   var a,x,y : byte;
    I,J,K:integer;
    posx,posy,ultimodia:byte;
    b: char;
{--------------------------------------------------------------}
Procedure MudaMes(direcao : boolean);
begin
   if (posdia=31) then if (posmes=3)or(posmes=5)or(posmes=8)or(posmes=10) then posdia:=30;
   if (posdia=30) then if (posmes=4)or(posmes=6)or(posmes=9)or(posmes=11) then posdia:=31;
   if ((posdia=30)or(posdia=31)) and (posmes=1) then if (ano mod 4)=0  then posdia:=29;
   if ((posdia=29)or(posdia=30)or(posdia=31)) and (posmes=1) then if (ano mod 4)>0  then posdia:=28;
   if ((posdia=29)or(posdia=28)) and (posmes=2) then posdia:=31;
   if direcao then inc(posmes);
   if not direcao then dec(posmes);
   if posmes>12 then
   begin
      posmes:=1;
      inc(posano);
   end;
   if posmes<1 then
   begin
      posmes:=12;
      dec(posano);
   end;
   if posano>9999 then posano:=1;
   if posano<1 then posano:=9999;
   hidemouse;
   gotoxy(11,6);
   regs.ah:=9;
   regs.al:=219;
   regs.bl:=fundobarra;
   regs.bh:=0;
   regs.cx:=58;
   intr($10,regs);
   cores(fundobarra,frentebarra);
   gotoxy(12,6); write(chr(17));
   gotoxy(67,6); write(chr(16));
   gotoxy(40-length('Consultas para o mˆs de'+NomedoMes(posmes)+'/    ') div 2,6);
   write('Consultas para o mˆs de'+NomedoMes(posmes)+'/',posano);
   showmouse;
   i:=diadasemana(1,posmes,posano);
   x:=16;
   y:=10;
   a:=1;
   cores(fundomenu,frentemenu);
   window(12,10,68,21); clrscr; window(1,1,80,25);
   repeat
     cores(fundomenu,frentemenu);
     if diadasemana(a,posmes,posano)=0 then cores(fundomenu,lightred);
     if (a=posdia) then
     begin
        cores(black,lightgreen);
        posx:=x+8;
        if posdia=1 then posx:=16+(diadasemana(1,posmes,posano)*8);
        posy:=y;
        if posx>65 then
        begin
         posx:=16;
         inc(posy,2);
        end;
     end;
     x:=16+(i*8);
     if (x>65) then
     begin
        x:=16;
        i:=0;
        inc(y,2);
     end;
     hidemouse;
     gotoxy(x,y); write(a);
     showmouse;
     inc(i);
     inc(a);
   until diadasemana(a,posmes,posano)=7;
   ultimodia:=a-1;
end;
{--------------------------------------------------------------}
begin
   resp:=false;
   ultimodia:=0;
   hidemouse;
   cores(fundomenu,frentemenu);
   retangulo(10,5,70,23);
   sombra(10,5,70,23,7);
   gotoxy(11,6);
   regs.ah:=9;
   regs.al:=219;
   regs.bl:=fundobarra;
   regs.bh:=0;
   regs.cx:=58;
   intr($10,regs);
   gotoxy(11,5); write('[',chr(240),']');
   cores(fundobarra,frentebarra);
   gotoxy(12,6); write(chr(17));
   gotoxy(67,6); write(chr(16));
   gotoxy(40-length('Consultas para o mˆs de'+NomedoMes(mes)+'/    ') div 2,6);
   write('Consultas para o mˆs de'+NomedoMes(mes)+'/',ano);
   for i:=0 to 6 do
   begin
      cores(fundomenu,frentemenu);
      gotoxy(13+(i*8),8);
      case i of
       0 : write('Domingo');
       1 : write(' Segunda');
       2 : write('  Ter‡a ');
       3 : write(' Quarta');
       4 : write(' Quinta');
       5 : write(' Sexta ');
       6 : write('S bado');
    end;
    cores(fundomenu,lightred);
    gotoxy(13,8); write('Domingo');
   end;
   i:=diadasemana(1,mes,ano);
   x:=16;
   y:=10;
   a:=1;
   showmouse;
   repeat
      mostradata;
      cores(fundomenu,frentemenu);
      if diadasemana(a,mes,ano)=0 then cores(fundomenu,lightred);
      if (a=dia) then
      begin
        cores(black,lightgreen);
        posx:=x+8;
        posy:=y;
        if posx>65 then
        begin
           posx:=16;
           inc(posy,2);
        end;
      end;
      x:=16+(i*8);     
      if (x>65) then
      begin
         x:=16;
         i:=0;
         inc(y,2);
      end;
      hidemouse;
      gotoxy(x,y); write(a);
      showmouse;
      inc(i);
      inc(a);
   until diadasemana(a,mes,ano)=7;
   ultimodia:=a-1;
   posmes:=mes;
   posdia:=dia;
   posano:=ano;
   posx:=16+(diadasemana(posdia,posmes,posano)*8);
   repeat
   mostradata;
   showmouse;
   b:=#1;
   if keypressed then b:=readkey;
   getmouseevent(rato);
   if ((rato.where.x>=10)and(rato.where.x<=12)and
      (rato.where.y=4)and(rato.buttons=mbleftbutton)) then b:=#27;
   if ((rato.where.x>=10)and(rato.where.x<=64))and
      ((rato.where.y>=9 )and(rato.where.y<=19))and
      (rato.buttons=mbleftbutton) then
   begin
      cores(fundomenu,frentemenu);
      if diadasemana(posdia,posmes,posano)=0 then cores(fundomenu,lightred);
      gotoxy(posx,posy);
      write(posdia);
      i:=((rato.where.x-12) div 8);
      j:=((rato.where.y-9) div 2);
      k:=(-diadasemana(1,posmes,posano)+i+j*7)+1;
      if (k>0) and (k<=ultimodia) then
      begin
         posdia:=k;
         posx:=16+(diadasemana(posdia,posmes,posano)*8);
         posy:=(11+j*2)-1;
      end;
   end;
   if b=#72 then
   begin
     hidemouse;
     gotoxy(posx,posy);
     cores(fundomenu,frentemenu);
     if diadasemana(posdia,posmes,posano)=0 then cores(fundomenu,lightred);
     write(posdia);
     dec(posdia,7);
     dec(posy,2);
     if (posdia<1)or(posdia>ultimodia) then
     begin
       inc(posdia,7);
       inc(posy,2);
     end;
   end;
   if b=#80 then
   begin
     hidemouse;
     gotoxy(posx,posy);
     cores(fundomenu,frentemenu);
     if diadasemana(posdia,posmes,posano)=0 then cores(fundomenu,lightred);
     write(posdia);
     inc(posdia,7);
     inc(posy,2);
     if posdia>ultimodia then
     begin
       dec(posdia,7);
       dec(posy,2);
     end;
   end;
   if b=#75 then
   begin
     hidemouse;
     gotoxy(posx,posy);
     cores(fundomenu,frentemenu);
     if diadasemana(posdia,posmes,posano)=0 then cores(fundomenu,lightred);
     write(posdia);
     dec(posdia,1);
     dec(posx,8);
     if ((posx<16)or(posdia<1))or((posx<16)and(posdia<1)) then
     begin
       inc(posdia,1);
       inc(posx,8);
     end;
   end;
   if b=#77 then
   begin
     hidemouse;
     gotoxy(posx,posy);
     cores(fundomenu,frentemenu);
     if diadasemana(posdia,posmes,posano)=0 then cores(fundomenu,lightred);
     write(posdia);
     inc(posdia,1);
     inc(posx,8);
     if ((posx>64)and(ultimodia<posdia))or((posx>64)or(ultimodia<posdia)) then
     begin
       dec(posdia,1);
       dec(posx,8);
     end;
   end;
   if ((rato.where.x=66)and(rato.where.y=5)and(rato.buttons=mbleftbutton))
      or(b=#73) then mudames(true);
   if ((rato.where.x=11)and(rato.where.y=5)and(rato.buttons=mbleftbutton))
      or(b=#81) then mudames(false);
   if (rato.double)and(rato.buttons=mbleftbutton) then b:=#13;
   gotoxy(posx,posy);
   cores(black,lightgreen);
   if (b>#1) or (rato.buttons>0) then
   begin
      hidemouse;
      write(posdia);
   end;
   showmouse;
   until (b=#27) or (b=#13);
   resp:=false;
   if b=#13 then resp:=true;
end;
{--------------------------------------------------------------------------}
begin
end.