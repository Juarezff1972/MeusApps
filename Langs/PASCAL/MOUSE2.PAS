Program MouseTest;
Uses DOS,CRT,Printer,Graph;
var regs : registers;
    a,dn : byte;
    b,c,d: string;
    xx,yy: longint;
    gd,gm: integer;
begin
   clrscr;
   gd:=CGA;
   gm:=CGAHi;
   initgraph(gd,gm,'');
   cleardevice;
   dn:=0;
   regs.ax:=$168;
   regs.cx:=$1900;
   intr($10,regs);
   regs.ax:=0;
   regs.bx:=0;
   intr($33,regs);
   if regs.ax=$ffff then outtext('Mouse Instalado...');
   if regs.ax=0 then
   begin
      restorecrtmode;
      writeln('Instale o mouse primeiro...');
      halt(0);
   end;
   regs.ax:=1;
   intr($33,regs);
   b:='';
   c:='';
   d:='';
   a:=0;
   repeat
      regs.ax:=3;
      intr($33,regs);
      a:=regs.bx;
      if a>=4 then
      begin
         b:='Botao Central...';
         a:=a-4;
      end;
      if a>=2 then
      begin
         c:='Botao Direito...';
         a:=a-2;
      end;
      dn:=0;
      if a>=1 then
      begin
         d:='Botao Esquerdo...';
         a:=a-1;
         dn:=1;
      end;
      gotoxy(10,4);
      b:='';
      c:='';
      d:='';
      gotoxy(10,5);
      xx:=regs.cx * 25;
      yy:=regs.dx * 18;
      xx:=xx*10;
      yy:=yy*10;
      xx:=xx*10;
      yy:=yy*10;
      xx:=2500-trunc(xx / 640);
      yy:=trunc(yy / 200);
      if dn=1 then
      begin
         writeln(lst,'DA',xx,',',yy,';');
         lineto(regs.cx,regs.dx);
      end else
      begin
         writeln(lst,'MA',xx,',',yy,';');
         moveto(regs.cx,regs.dx);
      end;
   until regs.bx=7;
   regs.ax:=$168;
   regs.cx:=$607;
   intr($10,regs);
   restorecrtmode;
   writeln(lst,'PU;');
end.