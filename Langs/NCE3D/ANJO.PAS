{--------------------------------------------------------}
{                                                        }
{  Anjo sintetico                                        }
{                                                        }
{  Autoras: Larissa Araujo e Isabele Lemos Carestiato    }
{                                                        }
{  Curso de Computacao Grafica nas Artes                 }
{                                                        }
{  Escola de Belas Artes da UFRJ                         }
{                                                        }
{  Prof. Mauricio Bomfim                                 }
{                                                        }
{  Em maio de 1993                                       }
{                                                        }
{--------------------------------------------------------}

{$m 30000,0,655360}

program anjo;
uses
   graph,nce3d;

begin
   inicvideo3d(SVGA256);
   janela3d(40,400,20.0,15.0,0,0,getmaxx+1,getmaxy+1);
   iniczbuffer3d(getmaxy+1,2,'c:');

   selecluz3d(-40,-40,-100);
   selecluzambiente3d(0.4);
   observador3d(40,-5,-80,0,0,0);

   { Corpo }
   seleccorpolig3d(VERMELHO);
   rodax3d(-pi/2);
   translada3d(0,-10,0);
   cone3d(18,5,15);

   { Cabeca }
   inictransf3d;
   seleccorpolig3d(ROSA);
   translada3d(0,7,-2);
   esfera3d(3,2);

   { Bracos }
   inictransf3d;
   seleccorpolig3d(BRANCO);
   translada3d(0,0,0);
   rodax3d(-pi/4);
   toro3d(36,6,4,0.7);

   { Aureola }
   inictransf3d;
   seleccorpolig3d(DOURADO);
   translada3d(-1,10,-2);
   rodax3d(pi/8);
   toro3d(36,6,2,0.15);

   { Bolas }
   inictransf3d;
   seleccorpolig3d(BRANCO);
   translada3d(0,-10,-5);
   esfera3d(3,0.8);
   translada3d(-2,-1,-2);
   esfera3d(3,1.5);
   translada3d(0,3,1);
   esfera3d(3,2);
   translada3d(10,-2,-1);
   esfera3d(3,3.5);
   translada3d(2,-3,-2);
   esfera3d(3,1.5);
   translada3d(-7,1,3);
   esfera3d(3,2.6);
   translada3d(-12,1,3);
   esfera3d(3,3);

   readln;
   termina3d;
end.
