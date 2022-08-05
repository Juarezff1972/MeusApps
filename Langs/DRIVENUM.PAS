uses dos;
var s: string;
    drive : char;
    num   : byte;
begin
   getdir(0,s);
   drive:=s[1];
   halt(ord(drive)-65);
end.