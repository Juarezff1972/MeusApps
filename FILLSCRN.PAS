uses dos,crt;
Procedure DirectWrite(char,atributo,pagina:byte);
var regs  : registers;
    xx,yy : byte;
begin
   xx:=wherex;
   yy:=wherey;
   regs.al:=char;
   regs.bh:=pagina;
   regs.bl:=atributo;
   regs.cx:=$2000;
   regs.ah:=9;
   gotoxy(1,1);
   intr($10,regs);
   gotoxy(xx,yy);
end;
begin
   clrscr;
   DirectWrite(176,7,0);
   readln;
   DirectWrite(177,7,0);
   readln;
   DirectWrite(178,7,0);
   readln;
end.