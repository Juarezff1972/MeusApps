Program DiskInfo3;
uses DOS,CRT;
var regs                            : registers;
    x                               : ^word;
    Drive                           : byte;
    Drive2                          : byte;
    Unidade                         : byte;
    BytesPorSetor                   : word;
    SetorPorCluster                 : byte;
    BytesPorCluster                 : word;
    NumeroBootSector                : word;
    NumeroFAT                       : word;
    EntradasRaiz                    : word;
    PrimeiroSetorOcupado            : word;
    NumeroDeClusters                : word;
    NumeroDeSetoresDaFAT            : word;
    NumeroDoPrimeiroSetorDoRoot     : word;
    SegmentoDoHeader                : word;
    OffsetDoHeader                  : word;
    DescritorDeMedia                : byte;
    DiscoAcessado                   : byte;
    SegmentoProximaTabelaParametros : word;
    OffsetProximaTabelaParametros   : word;
    BytesLivres,BytesTotal          : longint;
    DosVersao,nivel,DirTotal        : word;
    DiretorioAtivo                  : string;
    NumeroDeDiretorios              : array[1..10] of word;
    NumeroDeArquivos                : word;
    NomeDoDisco                     : string[12];
    atual,DriveNameParm             : string;
    DriveName                       : char;
    tam                             : integer;
{---------------------------------------------------------------------------}
Procedure LerArvore;
var path  : string;
    lista : searchrec;
begin
  getdir(drive,path);
  nivel:=nivel+1;
  if length(path)>3 then path:=path+'\';
  FindFirst(path+'*.*',anyfile,lista);
  if (((lista.attr=volumeid+archive)or(lista.attr=volumeid)) and (doserror=0) and (NomeDoDisco='Sem nome'))
     then NomeDoDisco:=lista.name;
  while(((lista.attr<>directory)or((lista.attr=volumeid+archive)or(lista.attr=volumeid)))and(doserror=0)) do
  begin
     if (((lista.attr=volumeid+archive)or(lista.attr=volumeid)) and (doserror=0) and (NomeDoDisco='Sem nome'))
        then NomeDoDisco:=lista.name;
     FindNext(lista);
     if (((lista.attr=volumeid+archive)or(lista.attr=volumeid)) and (doserror=0) and (NomeDoDisco='Sem nome'))
        then NomeDoDisco:=lista.name;
     NumeroDeArquivos:=NumeroDeArquivos+1;
  end;
  while (DosError = 0) and (lista.attr = directory) do
  begin
    if (lista.name <> '.') and (lista.name <> '..') then
    begin
       chdir(lista.name);
       getdir(drive,path);
       NumeroDeDiretorios[nivel]:=NumeroDeDiretorios[nivel]+1;
       LerArvore;
       nivel:=nivel-1;
       chdir('..');
    end;
    FindNext(lista);
    if (((lista.attr=volumeid+archive)or(lista.attr=volumeid)) and (doserror=0) and (NomeDoDisco='Sem nome'))
       then NomeDoDisco:=lista.name;
    while((lista.attr<>directory)and(doserror=0)) do
    begin
       if (((lista.attr=volumeid+archive)or(lista.attr=volumeid)) and (doserror=0) and (NomeDoDisco='Sem nome'))
          then NomeDoDisco:=lista.name;
       FindNext(lista);
       if (((lista.attr=volumeid+archive)or(lista.attr=volumeid)) and (doserror=0) and (NomeDoDisco='Sem nome'))
          then NomeDoDisco:=lista.name;
       NumeroDeArquivos:=NumeroDeArquivos+1;
    end;
  end;
end;
{---------------------------------------------------------------------------}
begin
   for nivel:=0 to 10 do NumeroDeDiretorios[nivel]:=0;
   getdir(0,atual);
   nivel:=0;
   DirTotal:=0;
   NomeDoDisco:='Sem nome';
   NumeroDeArquivos:=0;
   if ParamCount=0 then
   begin
      writeln;
      writeln('Escreva: DISKINFO X: onde X e'' o drive a ser lido.');
      writeln;
      halt(2);
   end;
   drive:=255;
   DriveNameParm:=ParamStr(1);
   DriveName:=UpCase(DriveNameParm[1]);
   drive:=ord(DriveName)-64;
   if drive=255 then
   begin
      writeln;
      writeln('Drive ',DriveName,': n„o existe...');
      writeln;
      halt(1);
   end;
   BytesLivres:=DiskFree(drive);
   if ((drive>0) and (drive<9) and (BytesLivres<0)) then
   begin
      writeln;
      writeln('Drive ',DriveName,': n„o est  pronto...');
      writeln;
      halt(3);
   end;
   regs.ah:=$32;
   regs.dl:=drive;
   intr($21,regs);
   x:=ptr(regs.ds,regs.bx);
   Drive2:=x^;
   x:=ptr(regs.ds,regs.bx+1);
   Unidade:=x^;
   x:=ptr(regs.ds,regs.bx+2);
   BytesPorSetor:=x^;
   x:=ptr(regs.ds,regs.bx+4);
   SetorPorCluster:=x^-1;
   x:=ptr(regs.ds,regs.bx+5);
   BytesPorCluster:=2*lo(x^)*BytesPorSetor;
   x:=ptr(regs.ds,regs.bx+6);
   NumeroBootSector:=x^;
   x:=ptr(regs.ds,regs.bx+8);
   NumeroFAT:=x^;
   x:=ptr(regs.ds,regs.bx+9);
   EntradasRaiz:=x^;
   x:=ptr(regs.ds,regs.bx+$b);
   PrimeiroSetorOcupado:=x^;
   x:=ptr(regs.ds,regs.bx+$d);
   NumeroDeClusters:=x^-1;
   x:=ptr(regs.ds,regs.bx+$f);
   NumeroDeSetoresDaFAT:=x^;
   x:=ptr(regs.ds,regs.bx+$11);
   NumeroDoPrimeiroSetorDoRoot:=x^;
   x:=ptr(regs.ds,regs.bx+$13);
   SegmentoDoHeader:=x^;
   x:=ptr(regs.ds,regs.bx+$15);
   OffsetDoHeader:=x^;
   x:=ptr(regs.ds,regs.bx+$17);
   DescritorDeMedia:=x^;
   x:=ptr(regs.ds,regs.bx+$18);
   DiscoAcessado:=x^;
   x:=ptr(regs.ds,regs.bx+$19);
   OffsetProximaTabelaParametros:=x^;
   x:=ptr(regs.ds,regs.bx+$21);
   SegmentoProximaTabelaParametros:=x^;
   BytesTotal:=DiskSize(drive);
   DosVersao:=DosVersion;    {word}
   GetDir(drive,DiretorioAtivo);
   writeln('Drive: ',DriveName,':');
   writeln('Dos Vers„o ',lo(DosVersao),'.',hi(DosVersao));
   writeln;
   writeln('Diret¢rio Ativo: ',DiretorioAtivo);
   writeln('Existem ',BytesLivres,' bytes livres. (',BytesLivres/1024:1:2,' Kb ou ',(BytesLivres/1024)/1024:1:2,' Mb)');
   writeln('Existem ',BytesTotal,' bytes no total. (',BytesTotal/1024:1:2,' Kb ou ',(BytesTotal/1024)/1024:1:2,' Mb)');
   writeln;
   chdir(DriveName+':\');
   LerArvore;
   writeln('Existem ',NumeroDeArquivos,' arquivos no disco.');
   for nivel:=1 to 10 do
   begin
      if NumeroDeDiretorios[nivel]>0 then
      begin
         DirTotal:=DirTotal+NumeroDeDiretorios[nivel];
         writeln('Existem ',NumeroDeDiretorios[nivel],' subdiret¢rios de nivel ',nivel);
      end;
   end;
   writeln('Existem no total ',DirTotal,' subdiret¢rios.');
   writeln;
   writeln('Volume: ',copy(NomeDoDisco,1,8),copy(NomeDoDisco,10,3));
   writeln;
   writeln('Aperte algo para continuar...');
   repeat until keypressed;
   writeln;
   chdir(atual);
   writeln('Drive Numero: ',Drive2);
   write('Unidade Numero: ',Unidade);
   if (unidade=0) and (drive2 > 1) then writeln(' - Unidade l¢gica') else writeln;
   writeln('Bytes por setor: ',BytesPorSetor);
   writeln('Setores por granulo: ',SetorPorCluster);
   writeln('Bytes por Granulo:',BytesPorCluster);
   writeln('Numero de setores de Boot: ',NumeroBootSector);
   writeln('Numero de c¢pias da FAT: ',lo(NumeroFAT));
   writeln('Numero de entradas no diret¢rio raiz: ',EntradasRaiz);
   writeln('Primeiro Setor Ocupado: ',PrimeiroSetorOcupado);
   writeln('Numero de Clusters: ',NumeroDeClusters);
   writeln('Numero de setores em cada FAT: ',NumeroDeSetoresDaFAT);
   writeln('Numero do Primeiro setor do diret¢rio raiz: ',NumeroDoPrimeiroSetorDoRoot);
   writeln('Endere‡o do cabe‡alho do drive: ',SegmentoDoHeader,':',OffsetDoHeader);
   writeln('Descritor de Media: ',DescritorDeMedia);
   writeln('Indicador de acesso: ',DiscoAcessado);
   writeln('Endere‡o da pr¢xima tabela de parametros: ',SegmentoProximaTabelaParametros,':',OffsetProximaTabelaParametros);
   regs.ah:=8;
   regs.dl:=0;
   intr($13,regs);
   if (regs.bl=01) then writeln('Drive A: 360Kb - 5¬"');
   if (regs.bl=02) then writeln('Drive A: 1.2Mb - 5¬"');
   if (regs.bl=03) then writeln('Drive A: 720Kb - 3«"');
   if (regs.bl=04) then writeln('Drive A: 1.4Mb - 3«"');
   regs.ah:=8;
   regs.dl:=1;
   intr($13,regs);
   if (regs.bl=01) then writeln('Drive B: 360Kb - 5¬"');
   if (regs.bl=02) then writeln('Drive B: 1.2Mb - 5¬"');
   if (regs.bl=03) then writeln('Drive B: 720Kb - 3«"');
   if (regs.bl=04) then writeln('Drive B: 1.4Mb - 3«"');
   for tam:=3 to 26 do
   begin
      regs.ah:=$36;  regs.dl:=tam; intr($21,regs);
      if regs.ax<>$ffff then writeln('Disco fixo ',chr(64+tam),': - ',(disksize(tam) / 1024) / 1024:1:2,'Mb');
   end;
end.

