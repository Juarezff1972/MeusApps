program editor;
uses graph,crt,dos,drivers;

var driver,mode : integer;
    i           : integer;
    evento      : tevent;
    x,y,z,t     : integer;
    aux,as : integer;
    regs: registers;
    corx : word;
    c : string;
{---------------------------------------------------------------------------}
procedure botao(x,y,z,t,cor:word);
begin
  setcolor(0);
  rectangle(x,y,x+z,y+t);
  setcolor(15);
     line(x+1,y+1,x+1,y+(t-1));
     line(x+2,y+1,x+2,y+(t-2));
     line(x+1,y+1,x+(z-1),y+1);
     line(x+1,y+2,x+(z-2),y+2);
  setcolor(8);
     line(x+(z-1),y+(t-1),x+1,y+(t-1));
     line(x+(z-1),y+(t-2),x+2,y+(t-2));
     line(x+(z-1),y+(t-1),x+(z-1),y+1);
     line(x+(z-2),y+(t-1),x+(z-2),y+2);
  setfillstyle(solidfill,cor);
  bar(x+3,y+3,x+(z-3),y+(t-3));
end;
{---------------------------------------------------------------------------}
Procedure Botao2(x,y,z,t,cor:word);
begin
  setcolor(0);
  rectangle(x,y,x+z,y+t);
  line(x-1,y+1,x-1,y+t+1);
  line(x-1,y+t+1,x+z-1,y+t+1);
  setfillstyle(solidfill,cor);
  bar(x+1,y+1,x+z-1,y+t-1);
  settextstyle(as,horizdir,10);
  outtextxy(x+t+3,y+2,'Texto');
end;
{---------------------------------------------------------------------------}
begin
  initevents;
  driver:=vga;
  mode:=vgahi;
  initgraph(driver,mode,'');
  setfillstyle(solidfill,7);
  bar(1,1,getmaxx,getmaxy);
  botao(10,60,50,50,7);
  i:=7;
  setcolor(0);
  repeat
    line(270,103+i,578,103+i);
    inc(i,7);
  until i>330;
  i:=7;
  repeat
    line(263+i,110,263+i,432);
    inc(i,7);
  until i>320;
  showmouse;
  repeat
    getmouseevent(evento);
    if evento.buttons=mbleftbutton then
    begin
      regs.ax:=3;
      intr($33,regs);
      if (regs.cx>270)and(regs.cx<578)and(regs.dx>110)and(regs.dx<432) then
      begin
        setfillstyle(solidfill,magenta);
        hidemouse;
        floodfill(regs.cx,regs.dx,0);
        corx:=regs.cx div 7;
        putpixel(corx-25,(regs.dx div 7)+46,magenta);
        showmouse;
      end;
    end;




    if evento.buttons=mbrightbutton then
    begin
      regs.ax:=3;
      intr($33,regs);
      if (regs.cx>200)and(regs.cx<560)and(regs.dx>50)and(regs.dx<430) then
      begin
        setfillstyle(solidfill,7);
        hidemouse;
        floodfill(regs.cx,regs.dx,0);
        showmouse;
      end;
    end;





  until keypressed;



{  repeat begin botao(320-i,240-i,2*i,2*i,i); dec(i,3); end; until i<1;}

end.