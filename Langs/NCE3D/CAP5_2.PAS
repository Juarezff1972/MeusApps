{--------------------------------------------------------}
{                                                        }
{  Exemplo de utilizacao das rotinas de pintura          }
{  de poligonos em degrade                               }
{                                                        }
{  Autor: Silvia Rezende                                 }
{         Curso de Computacao Grafica nas Artes II       }
{         Escola de Belas Artes                          }
{         Prof. Mauricio Bomfim                          }
{                                                        }
{  Em junho de 1993                                      }
{                                                        }
{--------------------------------------------------------}
program surpresa;
uses crt,graph,nce3d,utnce3d;

procedure Linha (x1,y1,x2,y2,Cor : integer);
begin
   InicVertDegrade (1,x1,y1,cor);
   InicVertDegrade (2,x1,y1,cor);
   InicVertDegrade (3,x2,y2,cor);
   InicVertDegrade (4,x2,y2,cor);
   PintaPoligDegrade (4);
end;

{--------------------------------------------------------}

procedure Esfera (Xesf,Yesf,Resf,Xdeg,Ydeg,Rdeg,CorInicio,CorFim : integer);
var ang     : real;
    x,y,i : integer;
begin
   ang:= 0;
   for i := 1 to 18 do
      begin
         x := Xesf + trunc (Resf * cos (ang * pi / 180));
         y := Yesf + trunc (Resf * sin (ang * pi / 180));
         InicVertDegrade (i,x,y,CorInicio);
         ang := ang + 20;
      end;
   degradecircular (18,Xdeg,Ydeg,Rdeg,CorInicio,CorFim);
end;

{--------------------------------------------------------}

procedure Elipse (Xc,Yc,Rx,Ry,dx,dy,CorInicio,CorFim : integer);
var ang   : real;
    x,y,i : integer;
begin
   ang:= 0;
   for i := 1 to 9 do
      begin
         x := Xc + trunc (Rx * cos (ang * pi / 180));
         y := Yc + trunc (Ry * sin (ang * pi / 180));
         InicVertDegrade (i,x,y,CorInicio);
         ang := ang + 40;
      end;
   degradelinear (9,dx,dy,CorInicio,CorFim);
end;

{--------------------------------------------------------}

procedure Poligono (x1,y1,x2,y2,x3,y3,x4,y4,dx,dy,CorInicio,CorFim : integer);
begin
   InicVertDegrade (1,x1,y1,CorInicio);
   InicVertDegrade (2,x2,y2,CorInicio);
   InicVertDegrade (3,x3,y3,CorInicio);
   InicVertDegrade (4,x4,y4,CorInicio);
   degradelinear (4,dx,dy,CorInicio,CorFim);
end;

{--------------------------------------------------------}

var
   i : integer;

begin
   inicvideo3d (svga256);
   {fundo}
   poligono (0,0,639,0,639,479,0,479,0,-1,rosa,azul);
   {caixa}
   Poligono (240,440,360,440,360,320,240,320,-2,-2,vermelho,preto);
   Poligono (240,320,280,280,400,280,360,320,1,-4,preto,preto);
   Poligono (360,440,400,400,400,280,360,320,1,-4,vermelho,preto);
   Poligono (280,280,400,280,430,220,310,220,-2,-2,vermelho,preto);
   {mola}
   Poligono (270,320,350,320,330,310,290,310,0,1,dourado,marrom);
   Poligono (290,310,330,310,350,300,270,300,0,-1,dourado,marrom);
   Poligono (270,300,350,300,330,290,290,290,0,1,dourado,marrom);
   Poligono (290,290,330,290,350,280,270,280,0,-1,dourado,marrom);
   Poligono (270,280,350,280,330,270,290,270,0,1,dourado,marrom);
   Poligono (290,270,330,270,346,256,273,266,0,-1,dourado,marrom);
   Poligono (273,266,346,256,328,248,288,259,0,1,dourado,marrom);
   Poligono (288,259,328,248,342,234,272,261,0,-1,dourado,marrom);
   Poligono (272,261,342,234,320,230,284,250,0,1,dourado,marrom);
   Poligono (284,250,320,230,326,212,268,254,0,-1,dourado,marrom);
   Poligono (268,258,326,212,304,212,276,242,0,1,dourado,marrom);
   Poligono (276,242,304,212,304,190,262,250,0,-1,dourado,marrom);
   Poligono (262,250,304,190,280,194,264,235,0,1,dourado,marrom);
   Poligono (264,235,280,194,269,175,254,248,0,-1,dourado,marrom);
   Poligono (254,248,269,175,250,190,250,230,0,1,dourado,marrom);
   {mascara}
   Esfera (210,210,52,200,220,80,branco,amarelo);
   Poligono (200,210,210,200,220,210,210,220,0,0,ciano,ciano);
   Poligono (170,210,180,200,190,210,180,220,0,0,ciano,ciano);
   Linha (180,197,180,223,preto);
   Linha (210,197,210,223,preto);
   Esfera (185,225,10,180,230,13,rosa,vermelho);
   {chapeu}
   Elipse (210,170,60,25,-5,-1,violeta,azul);
   Poligono (210,170,208,170,128,120,131,120,-1,-1,verde,musgo);
   for i := 170  downto 130  do
       Elipse (210,i,45,18,-5,-1,vermelho,violeta);
   { flor }
   Elipse (130,120,7,7,-4,-1,amarelo,laranja);
   Elipse (140,120,7,5,-4,-1,rosa,salmao);
   Elipse (130,110,5,7,-4,-1,rosa,salmao);
   Elipse (120,120,7,5,-4,-1,rosa,salmao);
   Elipse (130,130,5,7,-4,-1,rosa,salmao);
   readln;
   Termina3d;
end.