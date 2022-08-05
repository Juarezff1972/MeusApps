uses crt;
var go : array[1..3] of procedure;
    i  : byte;
    ch : char;

Procedure One;
begin
   writeln('Um');
end;
Procedure Two;
begin
   writeln('Dois');
end;
Procedure Three;
begin
   writeln('Tres');
end;

begin
   @go[1]:=@One;
   @go[2]:=@Two;
   @go[3]:=@Three;
   i:=0;
   repeat
      ch:=readkey;
      if ch='1' then i:=1;
      if ch='2' then i:=2;
      if ch='3' then i:=3;
      go[i];
   until ch='Q';
end.
