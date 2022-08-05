{-------------------------------------------------------------}
{                                                             }
{   Modelagem de uma estacao espacial                         }
{                                                             }
{   Autor: Mauricio                                           }
{                                                             }
{   Em Novembro de 1992                                       }
{                                                             }
{-------------------------------------------------------------}

{$M 30000,0,655360}
program Modelagem;

uses graph, nce3d, crt;
var i, x, y: integer;

begin
    InicVideo3d (SVGA256);
    Janela3d ( 40,                      { distancia olho-tela         }
               400,                     { distancia ao plano de fundo }
               20.0, 15.0,              { tamanho real da janela      }
               0, 0,                    { posicao da janela           }
               getmaxx+1, getmaxy+1 );  { numero de pontos da janela  }

    InicZbuffer3d ( getmaxy+1, 0, 'c:');

    { desenha um fundo sintetico }
    for i := 1 to 100 do
        putpixel (random (getmaxx), random(getmaxy), 230);

    { Planeta }
    Observador3d ( 0, 60, -150, 15, -15, 0 );
    SelecLuz3d (-200,-200,-200);
    SelecLuzAmbiente3d (0.0);
    SelecRGBPolig3d (0,0.5,0.8);
    InicTransf3d;
    Translada3d (50,-50,0);
    esfera3d (6,48);

    { Estacao Espacial }
    SelecCorPolig3d (DOURADO);
    SelecLuzAmbiente3d (0.2);
    InicTransf3d;
    escala3d (0.6,0.6,0.6);
    esfera3d (3,5);
    toro3d (48,18,25,2);
    cilindro3d (9,1,25);
    roday3d (pi/2);
    cilindro3d (9,1,25);
    roday3d (pi/2);
    cilindro3d (9,1,25);
    roday3d (pi/2);
    cilindro3d (9,1,25);

    readln;
    Termina3d;
end.
