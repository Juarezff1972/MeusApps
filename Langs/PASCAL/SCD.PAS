Program SuperChangeDirectory;

uses Dos,Crt;

var drive,i,x,z              : byte;
    BytesLivres,BytesTotal   : longint;
    DosVersao,nivel,DirTotal : word;
    DiretorioAtivo           : string;
    NumeroDeDiretorios       : array[1..10] of word;
    NumeroDeArquivos,direct  : word;
    NomeDoDisco              : string[12];
    atual,DriveNameParm      : string;
    DriveName                : char;
    diretorios               : array[1..255] of string[80];
    aux                      : string;
    c                        : char;
    regs                     : registers;

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
       diretorios[direct]:=path;
       inc(direct);
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
{-------------------------------------------------------------}
begin
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   for nivel:=0 to 10 do NumeroDeDiretorios[nivel]:=0;
   for i:=1 to 255 do diretorios[i]:='';
   direct:=2;
   getdir(0,atual);
   nivel:=0;
   DirTotal:=0;
   NomeDoDisco:='Sem nome';
   NumeroDeArquivos:=0;
   drive:=255;
   aux:=copy(atual,1,1);
   DriveName:=UpCase(aux[1]);
   drive:=ord(DriveName)-64;
   BytesLivres:=DiskFree(drive);
   if ((drive>0) and (drive<9) and (BytesLivres<0)) then
   begin
      writeln;
      writeln('Drive ',DriveName,': n„o est  pronto...');
      writeln;
      halt(3);
   end;
   textcolor(15);
   textbackground(0);
   BytesTotal:=DiskSize(drive);
   DosVersao:=DosVersion;    {word}
   GetDir(drive,DiretorioAtivo);
   clrscr;
   gotoxy(1,1);
   writeln('Drive: ',DriveName,':    Dos Vers„o ',lo(DosVersao),'.',hi(DosVersao),'        Super Change Directory');
   writeln;
   writeln('Diret¢rio Ativo: ',DiretorioAtivo);
   writeln(BytesLivres,' bytes livres. ',BytesTotal,' bytes no total. ');
   writeln;
   chdir(DriveName+':\');
   LerArvore;
   diretorios[1]:=DriveName+':\';
   for i:=1 to 248 do
   begin
      aux:='';
      repeat
         aux:=aux+' ';
      until length(aux)=(75-length(diretorios[i]));
      diretorios[i]:=diretorios[i]+aux;
   end;
   writeln(NumeroDeArquivos,' arquivos no disco.');
   writeln('ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
   gotoxy(1,24);
   writeln('ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ');
   x:=1;
   z:=1;
   repeat
      for z:=x to x+15 do
      begin
         gotoxy(1,8+(z-x));
         if z<248 then writeln(diretorios[z]);
      end;
      textcolor(15);
      textbackground(11);
      gotoxy(1,8);
      if x<247 then writeln(diretorios[x]);
      c:=readkey;
      textcolor(15);
      textbackground(0);
      if c=#0 then
      begin
         c:=readkey;
         case ord(c) of
           72 : x:=x-1;
           80 : x:=x+1;
           73 : x:=x-15;
           81 : x:=x+15;
           71 : x:=1;
           79 : x:=direct-1;
         end;
         c:=#2;
      end;
      if x<1 then x:=1;
      if x>(direct-1) then x:=direct-1;
   until (ord(c)=13);
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
   if x>1 then chdir(diretorios[x]);
   if x=1 then chdir(copy(diretorios[x],1,3));
   gotoxy(1,25);
   textattr:=7;
end.

