Program Print;

Uses DOS,CRT,Printer;

Type Crc = array[1..255] of char;

var Cs      : Crc;
    Gs      : Char;
    Fim,Cai : boolean;
    Hora,Minuto,Segundo,Decimo,Ano,Mes,Dia,Semana : Word;
    kl      : boolean;
{---------------------------------------------------------------------}
Procedure Menu;
Var As : String;
    Xt : integer;
begin
   As:='*** Printer! ***';
   Xt:=((80-length(As)) div 2);
   ClrScr;
   GotoXY(Xt,1);
   Writeln(As);
   Writeln;
   Writeln('Escolha op‡„o:');
   Writeln;
   Writeln('   1  - Ler arquivo texto');
   Writeln('   2  - Imprimir arquivo texto');
   Writeln('   3  - Mudar configura‡„o');
   Writeln('   4  - Sair');
   Writeln;
end;
{-----------------------------------------------------------------------}
Procedure Ler;
var File1,File2 : text;
    Arq         : string[12];
    Temporaria  : Char;
begin
   Writeln;
   Write('Qual o nome do arquivo? ');
   Readln(Arq);
   Assign(File1,Arq);
   Assign(File2,'A010.111');
   Reset(File1);
   Rewrite(File2);
   Repeat
      Read(File1,Temporaria);
      Write(Temporaria);
      Write(File2,Temporaria);
   Until EOF(File1);
   Close(File1);
   Close(File2);
   Cai:=True;
end;
{-----------------------------------------------------------------------}
Procedure Imprimir;
Var PrintFile : Text;
    As        : Char;
    Ps        : Char;
    A         : Integer;
Begin
   Assign(PrintFile,'A010.111');
   Reset(PrintFile);
   Writeln(lst,chr(27),'R0');
   Writeln(lst,'');
   Repeat
      Read(PrintFile,As);
      A:=Ord(As);
      Ps:=Cs[A];
      if A=13 then Writeln(lst,Ps) else Write(lst,Ps);
   Until EOF(PrintFile);
   Close(PrintFile);
end;
{-----------------------------------------------------------------------}
Procedure GravarCFG;
var config : text;
    i      : integer;
begin
   assign(config,'prin.cfg');
   rewrite(config);
   for i:=32 to 255 do
      write(config,cs[i]);
   close(config);
end;
{-----------------------------------------------------------------------}
Procedure LerCFG;
var config : text;
    i      : integer;
begin
   assign(config,'prin.cfg');
   reset(config);
   for i:=32 to 255 do
      read(config,cs[i]);
   close(config);
end;
{-----------------------------------------------------------------------}
Procedure Escolha;
var i,jff : integer;
label start;
begin
start:
   writeln('Escolha caracter:');
   for i:=32 to 255 do
   begin
      gs:=cs[i];
      write(i:3,'-',gs,' ',chr(i),'/');
   end;
   writeln;
   write('Qual? (0 para nenhum) ');
   readln(jff);
   if (jff<32) or (jff>255) then exit;
   writeln('Caracter:',chr(jff));
   Writeln('Para impressora:',cs[jff]);
   write('Para impressora:');
   readln(cs[jff]);
   goto start;
end;
{-----------------------------------------------------------------------}
Procedure Final;
begin
   kl:=true;
end;
{-----------------------------------------------------------------------}
Procedure Mudar;
var dss : integer;
label inicio;
begin
inicio:
   ClrScr;
   if kl=true then
   begin
      kl:=false;
      exit;
   end;
   writeln('Mudar configura‡„o');
   writeln;
   writeln(' 1 - Ler configura‡„o do disco');
   writeln(' 2 - Gravar configura‡„o em disco');
   writeln(' 3 - Alterar configura‡„o');
   writeln(' 4 - Voltar ao menu principal');
   writeln;
   write('Qual ?');
   readln(dss);
   case dss of
      1 : LerCFG;
      2 : GravarCFG;
      3 : Escolha;
      4 : Final;
   end;
   goto inicio;
end;
{-----------------------------------------------------------------------}
Procedure Tempo;
begin
   GetTime(Hora,Minuto,Segundo,Decimo);
   GetDate(Ano,Mes,Dia,Semana);
   GotoXY(1,12);
   Writeln('Hora: ',Hora,':',Minuto,':',Segundo);
   GotoXY(1,13);
   Writeln('Data: ',Dia,'/',Mes,'/',Ano);
end;
{-----------------------------------------------------------------------}
begin
   kl:=false;
   Repeat
      Menu;
      Fim:=False;
      Cai:=False;
      Repeat
         Tempo;
         GotoXY(1,10);
         Writeln('Escolha: ');
         if keypressed then Gs:=readkey;
         case Ord(Gs) of
            49 : Ler;
            50 : Imprimir;
            51 : Mudar;
            52 : Fim:=true;
         end;
         if Fim then Cai:=true;
      until Cai;
   until Fim;
end.



