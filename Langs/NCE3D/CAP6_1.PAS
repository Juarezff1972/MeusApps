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

var i:integer;

begin
    InicVideo3d (SVGA256);
    InicZbuffer3d ( getmaxy+1, CalcCorAproximada3d(0,0,0.5), 'c:');
    Observador3d ( 0, 60, -120, 0, 0, 0 );

    { Estrelas de fundo }
    for i:= 1 to 200 do
        PutPixel ( random(getmaxx), random(getmaxy), 255 );

    { Estacao Espacial }
    InicTransf3d;
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

