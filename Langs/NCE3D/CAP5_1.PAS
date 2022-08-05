{-------------------------------------------------------------}
{                                                             }
{      Pinta uma serie de poligonos lidos de um               }
{      arquivo de dados.                                      }
{                                                             }
{      Autor: Mauricio                                        }
{                                                             }
{      Em Novembro de 1992                                    }
{                                                             }
{-------------------------------------------------------------}
program PintaPoligs;
uses graph, nce3d;
var
    nvert: integer;
    vert: array [1..24] of PONTO;
    arq: text;

{-------------------------------------------------------------}
{                  Leitura dos poligonos                      }
{-------------------------------------------------------------}

procedure LePoligono;
var
    i: integer;
begin
    readln (arq, nvert);
    for i := 1 to nvert do
        with vert[i] do
            readln (arq, x, y, z, cor);
end;

{-------------------------------------------------------------}

begin
    InicVideo3d (SVGA256);
    Janela3d ( 40,                     { distancia olho-tela         }
               600,                    { distancia ao plano de fundo }
               20.0, 15.0,             { tamanho real da janela      }
               0, 0,                   { posicao da janela           }
               getmaxx+1, getmaxy+1);  { numero de pontos da janela  }

    InicZbuffer3d ( getmaxy+1, 255, 'C:' );
    Observador3d ( 80, 80, -200, 0, 0, 0 );

    assign (arq, 'poligs.dat' );
    reset (arq);
    InicTransf3d;
    Translada3d ( -30,-30,0 );
    while not eof(arq) do
        begin
            LePoligono;
            Translada3d ( 1.5, 0, 20 );
            PintaPolig3d (nvert, vert);
        end;
    close (arq);

    readln;
    Termina3d;
end.





