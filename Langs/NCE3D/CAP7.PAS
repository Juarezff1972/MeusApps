{$m 30000,0,655360}
{--------------------------------------------------------}
{                                                        }
{  Exemplo de utilizacao das rotinas de pintura          }
{  de poligonos em degrade                               }
{                                                        }
{  Autor: Silvia Rodrigues e Carla Haschelevici          }
{         Curso de Computacao Grafica nas Artes II       }
{         Escola de Belas Artes                          }
{         Prof. Mauricio Bomfim                          }
{                                                        }
{  Em junho de 1993                                      }
{                                                        }
{--------------------------------------------------------}
program sorvete;
uses graph, nce3d;
begin
     inicvideo3d (svga256);
     janela3d (40,400,20.0,15.0,0,0,getmaxx+1,getmaxy+1);
     iniczbuffer3d (getmaxy+1,14,'c:');
     selecluz3d (120,120,-120);
     observador3d (0,60,-250,0,0,0);
     selecluzambiente3d (0.5);

     {casquinha}
     seleccorpolig3d (dourado);
     rodax3d (pi/2);
     cone3d (18,8,40);

     {primeira bola de sorvete}
     seleccorpolig3d (salmao);
     translada3d (0,9,0);
     esfera3d (3,12);

     {segunda bola de sorvete}
     seleccorpolig3d (laranja);
     translada3d (0,15,0);
     esfera3d (3,10);

     {cereja}
     seleccorpolig3d (vermelho);
     translada3d (0,11,0);
     esfera3d (2,2);
     inictransf3d;

     {papel}
     seleccorpolig3d (branco);
     rodax3d (pi/2);
     translada3d (0,-16,0);
     cone3d (20,5,25);

     {biscoito}
     inictransf3d;
     seleccorpolig3d (marrom);
     rodax3d (-pi/2);
     rodaz3d (pi/4);
     translada3d (-7,10,0);
     cilindro3d (8,1,15);

     readln;
     termina3d;
end.
