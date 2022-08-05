uses dos;
var data : ^string;
    modelo : ^byte;
    i    : byte;
begin
   data:=ptr($f000,$fff4);
   modelo:=ptr($f000,$fffe);
   for i:=1 to 8 do write(data^[i]);
   writeln('Modelo: ',modelo^);
end.