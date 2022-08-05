{$M 8192,0,0}
uses dos,crt;
var p : pointer;
Procedure GO;
interrupt;
var x,y  : byte;
    regs : registers;
begin
   x:=wherex;
   y:=wherey;
   gotoxy(1,1);
   regs.ah:=$f;
   intr($10,regs);
   write('Modo: ',regs.al,'     ');
   gotoxy(x,y);
end;
begin
   directvideo:=false;
   getintvec($1c,p);
   setintvec($1c,@GO);
   keep(768);
end.