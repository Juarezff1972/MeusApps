  driver:=vga;
  mode:=vgahi;
  initgraph(driver,mode,'');
repeat
  repeat
    getmouseevent(evento);
    if evento.buttons=mbleftbutton then
    begin
      x:=0; y:=0; z:=0; t:=0;
      regs.ax:=3;
      intr($33,regs);
      x:=regs.cx;
      y:=regs.dx;
      repeat
        setcolor(7);
        hidemouse;
        rectangle(x,y,z,t);
        showmouse;
        regs.ax:=3;
        intr($33,regs);
        z:=regs.cx;
        t:=regs.dx;
        setcolor(0);
        hidemouse;
        rectangle(x,y,z,t);
        showmouse;
        getmouseevent(evento);
      until evento.buttons=mbrightbutton;
    end;
  until evento.double;
  hidemouse;
  if (z-x<0) then begin aux:=z; z:=x; x:=aux; end;
  if (t-y<0) then begin aux:=t; t:=y; y:=aux; end;
  setlinestyle(solidln,1,normwidth);
  botao(x,y,z-x,t-y,blue);
  showmouse;
  
until keypressed;



