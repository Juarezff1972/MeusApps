uses dos,crt;
type chtb = array[1..4096] of byte;
var regs : registers;
    i    : word;
    t    : chtb;
    asci : file of byte;
begin
   if length(paramstr(1))<>3 then halt(1);
   assign(asci,'CHAR'+paramstr(1));
   reset(asci);
   regs.ah:=$11;
   regs.al:=4;
   regs.bl:=0;
   intr($10,regs);
   regs.ah:=$11;
   regs.al:=$30;
   regs.bh:=6;
   intr($10,regs);
   for i:=0 to 255*16+15 do read(asci,t[i]);
   close(asci);
   regs.ah:=$11;
   regs.al:=$0;
   regs.cx:=256;
   regs.dx:=0;
   regs.bh:=16;
   regs.es:=seg(t);
   regs.bp:=ofs(t)-1;
   intr($10,regs);
end.
