uses crt;

Procedure Plot(x,y:word);
begin
   gotoxy(x,y);
   write('*');
end;

Procedure FracLine(x1,y1,x2,y2 : word);
var mx,my : word;
begin
   mx:=(x1+x2) div 2;
   my:=(y1+y2) div 2;
   if ((mx=x1)and(my=y1))or((x1=x2)and(y1=y2)) then exit;
   plot(mx,my);
   delay(500);
   Fracline(x1,y1,mx,my);
   Fracline(mx,my,x2,y2);
   textcolor(lightred);
   plot(x1,y1);plot(x2,y2);
   textcolor(lightcyan);
end;

begin
   clrscr;
   textcolor(yellow);
   plot(1,1);
   plot(70,20);
   textcolor(lightcyan);
   Fracline(1,1,70,20);
end.