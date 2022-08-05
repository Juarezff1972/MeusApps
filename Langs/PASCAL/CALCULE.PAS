program Calcule;

uses crt;

type a = array[1..127] of string[2];
     g = array[1..127] of string[10];

var b : a;
    c : g;
    d : integer;
    e : string[127];
    f : integer;
 code : integer;
    v : real;

begin
   f:=paramcount;
   e:= #00;
   for d:=1 to f do e:=e+paramstr(d)+' ';
   f:=length(e);
   for d:=1 to f do b[d] := #00;
   for d:=1 to f do b[d] := copy(e,d,1);
   for d:=1 to f do write(b[d]);
   writeln;
   val(e,v,code);
   writeln('Total: ',code);
end.