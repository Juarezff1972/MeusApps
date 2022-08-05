Program Teste;
uses DOS,CRT;
var salto : array[1..4] of pointer;
    go    : procedure;
procedure Main;
begin
   writeln('Main');
end;
procedure First;
begin
   writeln('First');
end;
procedure Second;
begin
   writeln('Second');
end;
begin
   salto[1]:=@main;
   salto[2]:=@First;
   salto[3]:=@Second;
   main;
   @go:=salto[2];
   go;
   @go:=salto[3];
   go;
   @go:=salto[1];
   go;
end.

