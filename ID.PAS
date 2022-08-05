Program ComputerID;
uses DOS,CRT,Graph;
var data        : array[1..2] of ^char;
    modelo      : ^byte;     c   : word;
    submodelo   : ^byte;     bit : array[0..15] of byte;
    revisao     : ^byte;     equ : word;
    caracterist : ^byte;     crs : byte;
    equipamento : ^word;     mem : ^word;
    nome        : string;    tam : byte;
    dia,mes,ano : string;    gd  : integer;
    regs        : registers; gm  : integer;
    largura     : ^word;     dn  : string;
    nlinhas     : ^byte;     ff  : searchrec;
    altura      : ^word;     fi  : text;
    pagevideo   : ^byte;     li  : byte;
    linha       : string;    b   : byte;
    MaxByteDisk : ^word;
    FBCSProtg   : ^word;
    NumDrives   : ^byte;
    LASTDRIVE   : ^byte;
    BUFFERSx    : ^word;
    BUFFERSy    : ^word;
    MemExt      : ^word;
    JoinedDrive : ^byte;
    FormatoDeData       : word;
    SimboloDeMoeda      : string[5];
    SeparadorMilhares   : string[2];
    SeparadorDecimal    : string[2];
    SeparadorData       : string[2];
    SeparadorHora       : string[2];
    Local               : byte;
    NumeroDecimaisMoeda : byte;
    FormatoDeHora       : byte;
    SeparadorDeLista    : string[2];
    Atual               : string[34];
    Novo                : string[34];
    i,y                 : integer;
    aux                 : string;
    pais                : array[1..22] of string[40];
    Telefone            : array[1..22] of word;
    auxiliar,auxiliar2  : string;
    IOAddr              : word;
{--------------------------------------------------------------------------}
Function SpeedCheck : word;
var timer : ^longint;
    conta : longint;
    tempo : longint;
    aux   : word;
    x,y   : byte;
    cb    : boolean;
label Z;
begin
   getcbreak(cb);
   setcbreak(false);
   x:=wherex;
   y:=wherey;
   timer:=ptr($40,$6c);
   conta:=timer^;
   tempo:=80+conta;
   aux:=0;
   repeat
      gotoxy(x,y);write(aux);
      inc(aux);
   until timer^>=tempo;
   SpeedCheck:=aux;
   gotoxy(x,y);
   setcbreak(cb);
end;
{--------------------------------------------------------------------------}
Function InitSb : boolean;
var saida : word;
label A,B,C;
begin
   asm
      mov al,1
      mov dx,IOaddr
      add dx,6
      out dx,al
      in al,dx
      in al,dx
      in al,dx
      in al,dx
      mov al,0
      out dx,al
      add dx,4
      mov cx,100
    A:in al,dx
      cmp al,$aa
      je B
      loop A
      mov ax,1
      jmp C
    B:mov ax,0
    C:mov saida,ax
   end;
   if saida=0 then InitSb:=true;
   if saida<>0 then InitSb:=false;
end;
{--------------------------------------------------------------------------}
Procedure Dec_Hexadecimal(var hexa : string; decimal : string);
var nd,d1,d2,n4,n3 : word;
    c3,c4          : char;
begin
   val(decimal,nd,nd);
   d1:=trunc(nd/16);
   d2:=trunc(d1/16);
   n4:=nd-d1*16;
   n3:=d1-d2*16;
   if (n4>=10) and (n4<=15) then n4:=n4+55;
   if (n3>=10) and (n3<=15) then n3:=n3+55;
   if (n4>= 0) and (n4< 10) then n4:=n4+48;
   if (n3>= 0) and (n3< 10) then n3:=n3+48;
   c3:=chr(n3);
   c4:=chr(n4);
   hexa:=c3+c4;
end;
{-----------------------------------------------------------------------}
Function CMOSInfo(valor:byte) : byte;
var aux : byte;
label a,b,c;
begin
   asm
      mov al,valor
      cli
      jmp c
   c: out $70,al
      jmp a
   a: in al,$71
      jmp b
   b: sti
      mov aux,al
   end;
   CMOSInfo:=aux;
end;
{--------------------------------------------------------------------------}
Procedure Information;
begin
   pais[1] :='Estados Unidos';         Telefone[1] :=001;
   pais[2] :='Canad  Francˆs';         Telefone[2] :=002;
   pais[3] :='Am‚rica Latina';         Telefone[3] :=003;
   pais[4] :='Terras Baixas';          Telefone[4] :=031;
   pais[5] :='B‚lgica';                Telefone[5] :=032;
   pais[6] :='Fran‡a';                 Telefone[6] :=033;
   pais[7] :='Espanha';                Telefone[7] :=034;
   pais[8] :='Hungria';                Telefone[8] :=036;
   pais[9] :='Iugosl via';             Telefone[9] :=038;
   pais[10]:='It lia';                 Telefone[10]:=039;
   pais[11]:='Su¡‡a';                  Telefone[11]:=041;
   pais[12]:='Tchecoslov quia';        Telefone[12]:=042;
   pais[13]:='Reino Unido';            Telefone[13]:=044;
   pais[14]:='Dinamarca';              Telefone[14]:=045;
   pais[15]:='Su‚cia';                 Telefone[15]:=046;
   pais[16]:='Noruega';                Telefone[16]:=047;
   pais[17]:='Pol“nia';                Telefone[17]:=048;
   pais[18]:='Alemanha';               Telefone[18]:=049;
   pais[19]:='Brasil';                 Telefone[19]:=055;
   pais[20]:='Inglˆs Internacional';   Telefone[20]:=061;
   pais[21]:='Portugal';               Telefone[21]:=351;
   pais[22]:='Finlƒndia';              Telefone[22]:=358;
   atual:='';
   for i:=1 to 34 do atual:=atual+' ';
   for i:=1 to 34 do atual[i]:=#0;
   regs.ah:=$38;
   regs.ds:=seg(Atual);
   regs.bx:=0;
   regs.dx:=ofs(atual)+1;
   regs.al:=0;
   intr($21,regs);
   for i:=1 to 22 do if telefone[i]=regs.bx then
   begin
      writeln('Pa¡s: ',pais[i]);
      writeln('C¢digo Telef“nico: ',telefone[i]);
   end;
   {-----------------------------------------------------}
   SeparadorData:='';
   for i:=1 to 2 do if atual[11+i]<>#0 then SeparadorData:=SeparadorData+Atual[11+i];
   write('Formato da data: ');
   FormatoDeData:=ord(atual[2])*256+ord(atual[1]);
   case (FormatoDeData) of
      0 :  writeln('MM',SeparadorData,'DD',SeparadorData,'AA');
      1 :  writeln('DD',SeparadorData,'MM',SeparadorData,'AA');
      2 :  writeln('AA',SeparadorData,'MM',SeparadorData,'DD');
   end;
   {-----------------------------------------------------}
   SeparadorMilhares:='';
   SeparadorDecimal:='';
   for i:=1 to 2 do if atual[7+i]<>#0 then SeparadorMilhares:=SeparadorMilhares+Atual[7+i];
   for i:=1 to 2 do if atual[9+i]<>#0 then SeparadorDecimal:=SeparadorDecimal+Atual[9+i];
   writeln('Formato de n£mero: 10',SeparadorMilhares,'587',SeparadorDecimal,'243');
   {-----------------------------------------------------}
   SimboloDeMoeda:='';
   aux:='';
   for i:=1 to ord(atual[17]) do aux:=aux+'2';
   for i:=1 to 5 do if atual[2+i]<>#0 then SimboloDeMoeda:=SimboloDeMoeda+atual[2+i];
   write('Formato de moeda: ');
   case ord(Atual[16]) of
      0 :  writeln(SimboloDeMoeda,'500',SeparadorDecimal,aux);
      1 :  writeln('500',SeparadorDecimal,aux,SimboloDeMoeda);
      2 :  writeln(SimboloDeMoeda,' 500',SeparadorDecimal,aux);
      3 :  writeln('500',SeparadorDecimal,aux,' ',SimboloDeMoeda);
      4 :  writeln('500',SimboloDeMoeda,aux);
   end;
   {-----------------------------------------------------}
   SeparadorHora:='';
   for i:=1 to 2 do if atual[13+i]<>#0 then SeparadorHora:=SeparadorHora+Atual[13+i];
   write('Formato de hora : ');
   case ord(atual[18]) of
      0 : writeln('5',SeparadorHora,'35',SeparadorHora,'23',SeparadorDecimal,'57p');
      1 : writeln('17',SeparadorHora,'35',SeparadorHora,'23',SeparadorDecimal,'57');
   end;
   {-----------------------------------------------------}
   write('Formato de lista: ');
   SeparadorDeLista:='';
   for i:=1 to 2 do if atual[22+i]<>#0 then SeparadorDeLista:=SeparadorDeLista+atual[22+i];
   writeln('a',SeparadorDeLista,'b',SeparadorDeLista,'c',SeparadorDeLista,'d');
end;
{--------------------------------------------------------------------------}

begin
   directvideo:=false;
   clrscr;
   writeln('Informa‡„o da BIOS');
	regs.ah:=$52;
	intr($21,regs);
	MaxByteDisk:=ptr(regs.es,regs.bx+$10);
	FBCSProtg:=ptr(regs.es,regs.bx+$1e);
	NumDrives:=ptr(regs.es,regs.bx+$20);
	LASTDRIVE:=ptr(regs.es,regs.bx+$21);
	BUFFERSx:=ptr(regs.es,regs.bx+$3f);
	BUFFERSy:=ptr(regs.es,regs.bx+$41);
	MemExt:=ptr(regs.es,regs.bx+$45);
	JoinedDrive:=ptr(regs.es,regs.bx+$34);
	regs.ah:=$c0;
	intr($15,regs);
	submodelo:=ptr(regs.es,regs.bx+3);
	revisao:=ptr(regs.es,regs.bx+4);
	modelo:=ptr($f000,$fffe);
   mem:=ptr($40,$13);
   largura:=ptr($40,$4a);
   nlinhas:=ptr($40,$84);
   altura:=ptr($40,$85);
   pagevideo:=ptr($40,$62);
	caracterist:=ptr(regs.es,regs.bx+5);
	crs:=caracterist^;
	equipamento:=ptr($40,$10);
	equ:=equipamento^;
	writeln;
	for b:=0 to 15 do bit[b]:=0;
	b:=16;
	c:=32768;
	repeat
		if equ>=c then
		begin
			bit[b-1]:=1;
			equ:=equ-c;
		end;
		dec(b);
		c:=c div 2;
	until b=0;
	if crs>=128 then
  	begin
		crs:=crs-128;
		writeln('A BIOS do disco fixo usa Canal 3 do DMA');
	end;
	if crs>=64 then
	begin
		crs:=crs-64;
		writeln('Nivel 2 de interrup‡„o de cascata (IRQ2)');
	end;
	if crs>=32 then
	begin
		crs:=crs-32;
		writeln('O Relogio de tempo real est  presente');
   end;
   if crs>=16 then
   begin
      crs:=crs-16;
      writeln('Intercepta‡„o do teclado na BIOS implementada');
   end;
   if crs>=8 then
   begin
      crs:=crs-8;
      writeln('Espera de evento externo suportada');
   end;
   if crs>=4 then
   begin
      crs:=crs-4;
      writeln('Area de dados estendida da BIOS alocada');
   end;
   if crs>=2 then
   begin
      crs:=crs-2;
      writeln('Barramento de Microcanal presente');
   end;
   data[1]:=ptr($f000,$fff5); data[2]:=ptr($f000,$fff6);
	mes:=data[1]^+data[2]^;
   val(mes,i,i);
   case i of
      1 : mes:='Janeiro';
      2 : mes:='Fevereiro';
      3 : mes:='Mar‡o';
      4 : mes:='Abril';
      5 : mes:='Maio';
      6 : mes:='Junho';
      7 : mes:='Julho';
      8 : mes:='Agosto';
      9 : mes:='Setembro';
      10: mes:='Outubro';
      11: mes:='Novembro';
      12: mes:='Dezembro';
   end;
   data[1]:=ptr($f000,$fff8); data[2]:=ptr($f000,$fff9);
	dia:=data[1]^+data[2]^;
   data[1]:=ptr($f000,$fffb); data[2]:=ptr($f000,$fffc);
	ano:=data[1]^+data[2]^;
	if modelo^=$ff then nome:='e'' um IBM PC';
	if modelo^=$fe then nome:='e'' um IBM PC/XT ou Portable PC';
   if (modelo^=$fd) then nome:='e'' um IBM PC jr';
	if (modelo^=$fc) then nome:='e'' um IBM PC AT';
	if ((modelo^=$fc)and(submodelo^=1)and(revisao^=0)) then nome:='e'' um IBM PC AT';
	if ((modelo^=$fc)and(submodelo^=0)and(revisao^=1)) then nome:='e'' um IBM PC AT';
	if ((modelo^=$fc)and(submodelo^=2)and(revisao^=0)) then nome:='e'' um IBM PC XT286';
	if ((modelo^=$fc)and(submodelo^=4)and(revisao^=0)) then nome:='e'' um IBM PS/2 Modelo 50';
	if ((modelo^=$fc)and(submodelo^=5)and(revisao^=0)) then nome:='e'' um IBM PS/2 Modelo 60';
	if ((modelo^=$fc)and(submodelo^=$b)and(revisao^=0)) then nome:='e'' um IBM PS/1';
	if (modelo^=$fb) then nome:='e'' um IBM PC/XT';
	if (modelo^=$fa) then nome:='e'' um IBM PS/2 Modelo 30';
	if (modelo^=$f9) then nome:='e'' um IBM PC Conversivel';
	if (modelo^=$f8) then nome:='e'' um IBM PS/2 Modelo 80';
	if (modelo^=$fe) then nome:='e'' um Compaq DeskPro';
	if (modelo^=$2d) then nome:='e'' um Compaq Portable';
	if (modelo^=$9a) then nome:='e'' um Compaq Portable Plus';
	writeln('Este computador ',nome,'.');
   writeln('A data da BIOS e'' ',dia,' de ',mes,' de ',ano);
   writeln('Mem¢ria utiliz vel: ',mem^,'Kb');
   writeln('Mem¢ria Extendida:  ',MemExt^,'Kb');
   writeln('Tamanho da tela: ',largura^,' caracteres X ',nlinhas^+1,' linhas');
   if altura^<>0 then writeln('Altura dos caracteres: ',altura^,' linhas de varredura');
   writeln('P gina de video atual: ',pagevideo^);
   detectgraph(gd,gm);
   dn:='Adaptador Monocrom tico';
   if gd=CGA then      dn:='Adaptador Gr fico Colorido (CGA)';
   if gd=MCGA then     dn:='Arranjo Gr fico Multicolorido (MCGA)';
   if gd=EGA then      dn:='Adaptador Gr fico Melhorado (EGA)';
   if gd=EGA64 then    dn:='Adaptador Gr fico Melhorado (EGA 64Kb)';
   if gd=VGA then      dn:='Arranjo Gr fico de Video (VGA)';
   if gd=HERCmono then dn:='Hercules';
   if gd=ATT400 then   dn:='AT&T 400';
   if gd=PC3270 then   dn:='IBM PC 3270';
   if gd=EGAmono then  dn:='Adaptador Gr fico Melhorado (EGA Monocrom tico)';
   if gd=IBM8514 then  dn:='IBM PC 8514 (Super VGA - Arranjo Gr fico de Video)';
   writeln('Tipo de video: ',dn);
	if bit[1]=1 then writeln('Coprocessador Matem tico presente');
	write('Modo de video inicial:');
	if (bit[5]=1)and(bit[4]=0) then writeln(' 80 colunas / colorido');
	if (bit[5]=1)and(bit[4]=1) then writeln(' monocromatico');
	if (bit[5]=0)and(bit[4]=1) then writeln(' 40 colunas');
	if bit[1]=1 then writeln('Existem ',2*bit[7]+bit[6]+1,' drive''s de disquete');
   writeln('Existe(m) ',2*2*bit[11]+2*bit[10]+bit[9],' saida(s) serial(is)');
   {if bit[12]=1 then writeln('Adaptador de jogo instalado');}
   writeln('Existe(m) ',2*bit[15]+bit[14],' saida(s) paralela(s)');
   writeln;
   writeln('Aperte ENTER para continuar...');
   repeat until readkey=#13;
   clrscr;
   information;
   tam:=2;
   repeat
      inc(tam);
      regs.ah:=$36;  regs.dl:=tam; intr($21,regs);
      if regs.ax<>$ffff then findfirst(chr(64+tam)+':\CONFIG.SYS',anyfile,ff);
   until doserror=0;
   writeln('O disco de BOOT est  no drive ',chr(64+tam),':');
   if modelo^=$ff then writeln('Existem ',16*(2*bit[3]+bit[2]),'Kb na placa m„e');
   writeln('Existem ',MaxByteDisk^,' bytes por setor no m ximo em cada disco');
   writeln('Existem ',FBCSProtg^,' Blocos de Controle de Arquivo (FBC) protegidos');
   writeln('Existem ',NumDrives^,' drives de disco');
   writeln('N£mero de drives unidos (JOIN):',JoinedDrive^);
   writeln('LASTDRIVE = ',LASTDRIVE^,' (',chr(LASTDRIVE^+64),')');
   write('BUFFERS = ',BUFFERSx^);if BUFFERSy^<>0 then writeln(',',BUFFERSy^) else writeln;
       if ((modelo^=$fc)or(modelo^=$fa)) then
   begin
      regs.ah:=8;
      regs.dl:=0;
      intr($13,regs);
      if (regs.bl=01)or((modelo^=$ff)or(modelo^=$fb)) then writeln('Drive A: 360Kb - 5¬"');
      if (regs.bl=02)and((modelo^<>$ff)and(modelo^<>$fb)) then writeln('Drive A: 1.2Mb - 5¬"');
      if (regs.bl=03)or((modelo^=$ff)or(modelo^=$fb)) then writeln('Drive A: 720Kb - 3«"');
      if (regs.bl=04)and((modelo^<>$ff)and(modelo^<>$fb)) then writeln('Drive A: 1.4Mb - 3«"');
      regs.ah:=8;
      regs.dl:=1;
      intr($13,regs);
      if (regs.bl=01)or((modelo^=$ff)or(modelo^=$fb)) then writeln('Drive B: 360Kb - 5¬"');
      if (regs.bl=02)and((modelo^<>$ff)and(modelo^<>$fb)) then writeln('Drive B: 1.2Mb - 5¬"');
      if (regs.bl=03)or((modelo^=$ff)or(modelo^=$fb)) then writeln('Drive B: 720Kb - 3«"');
      if (regs.bl=04)and((modelo^<>$ff)and(modelo^<>$fb)) then writeln('Drive B: 1.4Mb - 3«"');
      for tam:=3 to 26 do
      begin
         regs.ah:=$36;  regs.dl:=tam; intr($21,regs);
         if regs.ax<>$ffff then writeln('Drive ',chr(64+tam),': - ',(disksize(tam) / 1024) / 1024:1:2,'Mb');
      end;
      regs.ah:=3;
      intr($10,regs);
      writeln('Tamanho do cursor: de ',regs.ch,' a ',regs.cl);
      regs.ah:=$12;
      intr($16,regs);
      if regs.al>=128 then
      begin
         regs.al:=regs.al-128;
         writeln('Inser‡„o ativado');
      end;
      if regs.al>=64 then
      begin
         regs.al:=regs.al-64;
         writeln('Caps Lock ativado');
      end;
      if regs.al>=32 then
      begin
         regs.al:=regs.al-32;
         writeln('Num Lock ativado');
      end;
      if regs.al>=16 then
      begin
         regs.al:=regs.al-16;
         writeln('Scroll Lock ativado');
      end;
      regs.ah:=9;
      intr($1a,regs);
      if regs.dl=1 then writeln('Alarme setado.');
      if regs.dl=0 then writeln('Alarme n„o setado.');
      writeln;
      writeln('Aperte ENTER para continuar...');
      repeat until readkey=#13;
      clrscr;
   end;
   tam:=2;
   repeat
      inc(tam);
      regs.ah:=$36;  regs.dl:=tam; intr($21,regs);
      if regs.ax<>$ffff then findfirst(chr(64+tam)+':\CONFIG.SYS',anyfile,ff);
   until doserror=0;
   writeln('Listagem do ',chr(64+tam),':\CONFIG.SYS');
   writeln;
   assign(fi,chr(64+tam)+':\CONFIG.SYS');
   reset(fi);
   li:=0;
   repeat
      inc(li);
      readln(fi,linha);
      writeln(linha);
      if (li mod 20)=0 then
      begin
         writeln('Aperte ENTER para continuar...');
         repeat until readkey=#13;
      end;
   until eof(fi);
   writeln;
   writeln('Aperte ENTER para continuar...');
   repeat until readkey=#13;
   tam:=2;
   repeat
      inc(tam);
      regs.ah:=$36;  regs.dl:=tam; intr($21,regs);
      if regs.ax<>$ffff then findfirst(chr(64+tam)+':\AUTOEXEC.BAT',anyfile,ff);
   until doserror=0;
   clrscr;
   writeln('Listagem do ',chr(64+tam),':\AUTOEXEC.BAT');
   writeln;
   assign(fi,chr(64+tam)+':\AUTOEXEC.BAT');
   reset(fi);
   li:=0;
   repeat
      inc(li);
      readln(fi,linha);
      writeln(linha);
      if (li mod 20)=0 then
      begin
         writeln;
         writeln('Aperte ENTER para continuar...');
         repeat until readkey=#13;
      end;
   until eof(fi);
   writeln;
   writeln('Aperte ENTER para continuar...');
   repeat until readkey=#13;
   clrscr;
   writeln('Informa‡„o da CMOS');
   writeln;
   write('Hora atual: ');
   tam:=CMOSInfo(4);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   write(':');
   tam:=CMOSInfo(2);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   write(':');
   tam:=CMOSInfo(0);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   writeln;
   write('Alarme: ');
   tam:=CMOSInfo(5);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   write(':');
   tam:=CMOSInfo(3);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   write(':');
   tam:=CMOSInfo(1);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   writeln;
   write('Data atual: ');
   tam:=CMOSInfo(7);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   write(' de ');
   tam:=CMOSInfo(8);
   if tam=01 then auxiliar2:='Janeiro';
   if tam=02 then auxiliar2:='Fevereiro';
   if tam=03 then auxiliar2:='Mar‡o';
   if tam=04 then auxiliar2:='Abril';
   if tam=05 then auxiliar2:='Maio';
   if tam=06 then auxiliar2:='Junho';
   if tam=07 then auxiliar2:='Julho';
   if tam=08 then auxiliar2:='Agosto';
   if tam=09 then auxiliar2:='Setembro';
   if tam=10 then auxiliar2:='Outubro';
   if tam=11 then auxiliar2:='Novembro';
   if tam=12 then auxiliar2:='Dezembro';
   write(auxiliar2);
   write(' de ');
   tam:=CMOSInfo(50);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   tam:=CMOSInfo(9);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   write(auxiliar2);
   writeln;
   writeln('Mem¢ria base: ',CMOSInfo($16)*256+CMOSInfo($15),'Kb');
   writeln('Mem¢ria expandida: ',CMOSInfo($18)*256+CMOSInfo($17),'Kb');
   writeln('Expans„o de mem¢ria atual: ',CMOSInfo($31)*256+CMOSInfo($30),'Kb');
   writeln('Tipo do disco fixo prim rio: ',CMOSInfo($19));
   writeln('Tipo do disco fixo secund rio: ',CMOSInfo($1A));
   write('Flag de Informa‡„o: ');
   tam:=CMOSInfo($33);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   writeln(tam,' (',auxiliar2,'h)');
   tam:=CMOSInfo($d);
   if tam>127 then writeln('Bateria do rel¢gio funcionando.');
   write('Registrador A: ');
   tam:=CMOSInfo($A);
   str(tam,auxiliar);
   Dec_Hexadecimal(auxiliar2,auxiliar);
   writeln(tam,' (',auxiliar2,'h)');
   tam:=CMOSInfo($b);
   if tam>=128 then tam:=tam-128;
   if tam>=64 then
   begin
      tam:=tam-64;
      writeln('Interrup‡„o peri¢dica ativa.');
   end;
   if tam>=32 then
   begin
      tam:=tam-32;
      writeln('Interrup‡„o de alarme ativa.');
   end;
   if tam>=16 then
   begin
      tam:=tam-16;
      writeln('Interrup‡„o de t‚rmino de atualiza‡„o ativa.');
   end;
   if tam>=8 then
   begin
      tam:=tam-8;
      writeln('Onda quadrada do registrador A ativa.');
   end;
   if tam>=4 then
   begin
      tam:=tam-4;
      writeln('Calend rio em formato bin rio.');
   end;
   if tam>=2 then
   begin
      tam:=tam-2;
      writeln('Rel¢gio em formato 24 horas.');
   end;
   if tam>=1 then writeln('Hor rio de ver„o ativo.');
   writeln;
   writeln('Aperte ENTER para continuar...');
   repeat until readkey=#13;
   clrscr;
   writeln('Informa‡”es miscelƒneas');
   writeln;
   for b:=1 to 2*2*bit[11]+2*bit[10]+bit[9] do
   begin
      modelo:=ptr($40,2*b-1);
      str(modelo^,auxiliar);
      Dec_hexadecimal(auxiliar2,auxiliar);
      write('Endere‡o da COM',b,': ',auxiliar2);
      modelo:=ptr($40,2*b-2);
      str(modelo^,auxiliar);
      Dec_hexadecimal(auxiliar2,auxiliar);
      writeln(auxiliar2,'h');
   end;
   for b:=1 to 2*bit[15]+bit[14] do
   begin
      modelo:=ptr($40,2*b-1+8);
      str(modelo^,auxiliar);
      Dec_hexadecimal(auxiliar2,auxiliar);
      write('Endere‡o da LPT',b,': ',auxiliar2);
      modelo:=ptr($40,2*b-2+8);
      str(modelo^,auxiliar);
      Dec_hexadecimal(auxiliar2,auxiliar);
      writeln(auxiliar2,'h');
   end;
   IOAddr:=$220;if InitSb then writeln('Endere‡o da Sound Blaster: 0220h');
   IOAddr:=$240;if InitSb then writeln('Endere‡o da Sound Blaster: 0240h');
   regs.ah:=$33;
   regs.al:=$6;
   intr($21,regs);
   writeln('DOS Vers„o ',regs.bl,'.',regs.bh);
   if regs.dh=$8 then writeln('DOS em ROM');
   if regs.dh=$10 then writeln('DOS na HMA (Mem¢ria alta)');
   regs.ah:=$30;
   intr($21,regs);
   write('N§ de s‚rie OEM: ',regs.bh,'       ');
   writeln('N§ de s‚rie: ',regs.cx+(65536*regs.bl));
   write('Verifica‡„o de CTRL+C ');
   regs.ax:=$3300;
   intr($21,regs);
   if regs.dl=1 then write('');
   if regs.dl=0 then write('des');
   writeln('ativada.');
   write('Estrat‚gia de aloca‡„o de mem¢ria: ');
   regs.ax:=$5800;
   intr($21,regs);
   if (regs.flags mod 2)=0 then
   case regs.ax of
      $00 : writeln('Primeiro espa‡o livre na baixa');
      $01 : writeln('Melhor espa‡o livre na baixa');
      $02 : writeln('Ultimo espa‡o livre na baixa');
      $40 : writeln('Primeiro espa‡o livre na alta somente');
      $41 : writeln('Melhor espa‡o livre na alta somente');
      $42 : writeln('Ultimo espa‡o livre na alta somente');
      $80 : writeln('Primeiro espa‡o livre na alta');
      $81 : writeln('Melhor espa‡o livre na alta');
      $82 : writeln('Ultimo espa‡o livre na alta');
   end else writeln('Nenhuma');
   regs.ax:=$5802;
   intr($21,regs);
   if regs.al=1 then writeln('Mem¢ria alta unida');
   write('Velocidade: ');writeln((SpeedCheck+SpeedCheck+SpeedCheck) div 3);
   writeln;
end.

