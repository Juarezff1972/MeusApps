{$M 8192,0,0}
uses dos,crt;
var x : string;
    r : registers;

begin
   x:='dir /w'+chr(13)+chr(10)+chr(0);
   r.ds:=seg(x);
   r.si:=ofs(x)+1;
   intr($2e,r);
end.