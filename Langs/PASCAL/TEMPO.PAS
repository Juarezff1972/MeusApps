Program Tempo;

Uses DOS,CRT;

var x : string;
    y : word;

begin
   x:=paramstr(1);
   val(x,y,y);
   if ((y<1) or (y>65535)) then halt(1);
   delay(y);
end.