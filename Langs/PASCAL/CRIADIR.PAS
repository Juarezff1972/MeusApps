{$M 64000,0,0}
{$I-}
uses dos,crt;
var aux : string[12];
    aux2 : string;
{-----------------------------------------------------------------}
procedure CriaDir;
var i : word;
begin
   for i:=1 to 9 do
   begin
      if keypressed then halt(1);
      str(i,aux);
      getdir(0,aux2);
      writeln(aux2);
      mkdir(aux);
      if ioresult=0 then
      begin
         chdir(aux);
         criadir;
         chdir('..');
      end;
   end;
end;
{-----------------------------------------------------------------}
begin
   writeln;
   CriaDir;
end.