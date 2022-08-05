{-------------------------------------------------------------}
{                                                             }
{    Roda um triangulo em relacao aos eixos X, Y e Z          }
{                                                             }
{    Autor:  Jose' Antonio Borges                             }
{            Mauricio Nunes Bomfim                            }
{                                                             }
{    Em julho/92                                              }
{                                                             }
{-------------------------------------------------------------}

program roda_triangulo;
uses nce3d, graph, crt;
var
   c: char;

procedure poeTriangulo;
   begin
     move3d  (0,0,0);
     risca3d (0,15,0);
     risca3d (15,0,0);
     risca3d (0,0,0);
   end;

begin
    InicVideo3d ( VGADefault );
    Observador3d ( 0,0,-100, 0,0,0 );
    setwritemode (XORPUT);
    poeTriangulo;

    c:= readkey;
    while (upcase(c)='X') or (upcase(c)='Y') or (upcase(c)='Z') do
       begin
           poeTriangulo;
           case upcase(c) of
	      'X' : rodax3d (20 * pi / 180);
	      'Y' : roday3d (20 * pi / 180);
	      'Z' : rodaz3d (20 * pi / 180);
           end;
           poeTriangulo;
           c:= readkey;
       end;

    setwritemode (NORMALPUT);
    Termina3d;
end.

