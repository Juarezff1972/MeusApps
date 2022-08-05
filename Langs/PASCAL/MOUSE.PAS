Program MouseTest;
Uses DOS,CRT;
var regs : registers;
    a    : byte;
    b,c,d: string;
begin
   clrscr;
   regs.ax:=0;
   regs.bx:=0;
   intr($33,regs);
   if regs.ax=$ffff then writeln('Mouse Instalado...');
   if regs.ax=0 then
   begin
      writeln('Instale o mouse primeiro...');
      halt(0);
   end;
   writeln('Existem ',regs.bx,' botoes no mouse...');
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
      if a>=1 then
      begin
         d:='Botao Esquerdo...';
         a:=a-1;
      end;
      gotoxy(10,4);
      write(b,c,d,'                    ');
      b:='';
      c:='';
      d:='';
      gotoxy(10,5);
      write('Coordenada X=',regs.cx,' - Coordenada Y=',regs.dx,'     ');
   until regs.bx=7;
end.