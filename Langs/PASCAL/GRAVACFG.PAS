program gravacfg;
{$M 8192,0,0}
uses crt,dos;
var abcd : pointer;
    f:word;
procedure grava;
interrupt;
  var
  FromF, ToF: file;
  NumRead, NumWritten: Word;
  buf: array[1..2048] of Char;
begin
  Assign(FromF,'lhx.cfg');
  Reset(FromF, 1);
  Assign(ToF,'mouser.cfg');
  Rewrite(ToF, 1);
  repeat
    BlockRead(FromF,buf,
              SizeOf(buf),NumRead);
    BlockWrite(ToF,buf,NumRead,NumWritten);
  until (NumRead = 0) or
        (NumWritten <> NumRead);
  Close(FromF);
  Close(ToF);
  for f:=220 to 440 do begin sound(f); delay(50); end;
  for f:=440 downto 220 do begin sound(f); delay(50); end;
  nosound;
end;
{-------------------}
begin
  getintvec($1b,abcd);
  setintvec($1b,SaveInt1B);
  getintvec($5,abcd);
  setintvec($5,@grava);
  keep(768);
end.