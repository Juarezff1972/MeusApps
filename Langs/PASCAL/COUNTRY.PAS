{$M 8192,0,0}
uses DOS,CRT;
var FormatoDeData       : word;
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
    regs                : registers;
    i,y                 : integer;
    aux                 : string;
    pais                : array[1..22] of string[40];
    Telefone            : array[1..22] of word;

{--------------------------------------------------------------------------}
Procedure Information;
begin
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
      writeln;
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
      4 :  writeln(' 4 - 500',SimboloDeMoeda,aux);
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
procedure choose;
begin
   for i:=1 to 22 do writeln(i:3,' - ',pais[i]);
   writeln;
   write('Escolha o pais: ');
   readln(i);
   if (i<1) and (i>22) then i:=19;
   regs.flags:=0;
   regs.ah:=$38;
   regs.bx:=telefone[i];
   regs.dx:=$ffff;
   regs.al:=$ff;
   intr($21,regs);
   information;
   halt(0);
end;
{--------------------------------------------------------------------------}
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
   y:=0;
   asm
      mov ax,$1400
      int $2f
      mov y,ax
   end;
   y:=lo(y);
   if y=0 then
   begin
      information;
      writeln;
      write('NLSFUNC n„o instalado. Deseja instal -lo? (S/N) ');
      readln(aux);
      if upcase(aux[1])='S' then
      begin
         swapvectors;
         exec(GetEnv('COMSPEC'),'/C NLSFUNC');
         swapvectors;
         if doserror<>0 then
         begin
            writeln('ERRO AO EXECUTAR NLSFUNC.EXE');
            writeln;
            case doserror of
             2   : writeln('NLSFUNC n„o encontrado.');
             3   : writeln('NLSFUNC n„o encontrado.');
             5   : writeln('Acesso Negado.');
             6   : writeln('Handle Inv lido.');
             8   : writeln('Falta Mem¢ria.');
            10   : writeln('Ambiente Inv lido.');
            11   : writeln('Formato Inv lido.');
            end;
            exit;
         end;
      end;
      if upcase(aux[1])='N' then halt(1);
      choose;
      exit;
   end;
   if y=1 then
   begin
      information;
      writeln;
      writeln('NLSFUNC n„o instalado. Existe algum problema e n„o pode ser instalado.');
      writeln;
      exit;
   end;
   if paramcount>0 then
   for i:=1 to 22 do
   begin
      str(telefone[i],aux);
      if aux=paramstr(1) then
      begin
         regs.flags:=0;
         regs.ah:=$38;
         regs.bx:=telefone[i];
         regs.dx:=$ffff;
         regs.al:=$ff;
         intr($21,regs);
         information;
         exit;
      end;
   end;
   if paramcount=0 then
   begin
      writeln;
      writeln('Configura‡„o atual: ');
      information;
      writeln;
      write('Aperte <ENTER> para prosseguir.');
      readln;
      writeln;
      choose;
   end;
   regs.flags:=0;
   regs.ah:=$38;
   regs.bx:=telefone[i];
   regs.dx:=$ffff;
   regs.al:=$ff;
   intr($21,regs);
   information;
end.
  
