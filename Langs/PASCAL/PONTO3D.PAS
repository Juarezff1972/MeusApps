uses crt,graph;
var op:char;
    k,col,lin,maxy,maxx:integer;

procedure ponto(x,y,z:integer);
var col,lin,desvio:real;
    cor : integer;
begin
  desvio:=x*0.707106;
  col:=(maxx div 2) - desvio + y;
  lin:=(maxy div 2) + desvio - z;
  if ((x mod 5)=0) or ((y mod 5)=0) then cor:=5
   else cor:=3;
  putpixel(trunc(col),trunc(lin),cor)
end;

begin
  k:=detect;
  initgraph(k,k,'');
  setcolor(15);
  maxx:=getmaxx;
  maxy:=getmaxy;
  line(maxx div 2,maxy div 2,maxx,maxy div 2);
  line(maxx div 2,maxy div 2,maxx div 2,0);
  line(maxx div 2,maxy div 2,(maxx div 2)-200,(maxy div 2)+200);
  for lin:=-100 to 100 do
   for col:=-100 to 100 do
     ponto(col,lin,round(sqrt(col*col+lin*lin)));

  op:=readkey

end.