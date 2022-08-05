program esfera_pas;

uses graph,crt;

var
    e,c,f,nf              : integer;
    p,k,l,m,n             : real;
    graphdriver,graphmode : integer;

begin
      graphdriver := detect;
      graphmode := 0;
      write('Numero de arestas: ');
      readln(nf);
      initgraph(graphdriver,graphmode,'');
      cleardevice;
      m := 1;
      p := 0;
      while (p < 6.284) do
      begin
         c := 127;
         e := 80;
         f := 40;
         k := (cos(p));
         l := (sin(p));
         moveto(2*c,5);
         lineto(2*trunc(k*f+c),trunc(l*5+15));
         lineto(2*trunc(m*f+c),trunc(n*5+15));
         lineto(2*trunc(m*e+c),trunc(n*10+40));
         lineto(2*trunc(k*e+c),trunc(l*10+40));
         lineto(2*trunc(k*95+c),trunc(l*10+e));
         lineto(2*trunc(m*95+c),trunc(n*10+e));
         lineto(2*trunc(m*e+c),trunc(n*10+120));
         lineto(2*trunc(k*e+c),trunc(l*10+120));
         lineto(2*trunc(k*f+c),trunc(l*5+150));
         lineto(2*trunc(m*f+c),trunc(n*5+150));
         lineto(2*c,160);
         p := p + (2*3.1415 / nf);
         m := k;
         n := l;
      end;
      repeat until keypressed;
      closegraph;
end.