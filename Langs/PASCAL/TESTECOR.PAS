uses dos;
var regs  : registers;
    x1,y1 : word;
    c     : byte;
begin
   regs.ah:=0;
   regs.al:=$62;
   intr($10,regs);
   x1:=0;
   y1:=0;
   c:=0;
   repeat
      regs.ah:=$c;
      regs.bh:=0;
      regs.dx:=y1;
      regs.cx:=x1;
      regs.al:=c;
      intr($10,regs);
      x1:=x1+1;
      if x1>640 then x1:=0;
      y1:=y1+1;
      if y1>480 then y1:=0;
      c:=c+1;
      if c>256 then c:=0;
   until (x1=640)and(y1=480);

end.