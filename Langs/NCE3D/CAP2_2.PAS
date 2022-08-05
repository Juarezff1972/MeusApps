{-----------------------------------------------------------}
{                                                           }
{     Modelagem das letras N.C.E.                           }
{                                                           }
{     Autor: Mauricio Bomfim                                }
{     Em: agosto de 1992                                    }
{                                                           }
{-----------------------------------------------------------}
program NCE;
uses
    crt, graph, nce3d;
const
    NLETRAS  = 3;
    NQUADROS = 16;
    { coordenadas do ponto inicial do movimento do observador }
    x0 = 100;
    y0 = 100;
    z0 =-300;
    { coordenadas do ponto final do movimento do observador }
    x1 = 100;
    y1 = 100;
    z1 = 300;

type
    POLIGONO = record
                    np: integer;
                    letra: array [ 1..24 ] of COORDPTO;
               end;
var
    carac: array [1..NLETRAS] of POLIGONO;

{---------------------------------------------------------------}

procedure InicLetras;
begin
    with carac[1] do
        begin
            np:= 10;
            letra[1].x:= -20;   letra[1].y:= 0;   letra[1].z:= 0;
            letra[2].x:= -17;   letra[2].y:= 0;   letra[2].z:= 0;
            letra[3].x:= -17;   letra[3].y:= 9;   letra[3].z:= 0;
            letra[4].x:= -12.5; letra[4].y:= 0;   letra[4].z:= 0;
            letra[5].x:= -9.5;  letra[5].y:= 0;   letra[5].z:= 0;
            letra[6].x:= -9.5;  letra[6].y:= 15;  letra[6].z:= 0;
            letra[7].x:= -12.5; letra[7].y:= 15;  letra[7].z:= 0;
            letra[8].x:= -12.5; letra[8].y:= 6;   letra[8].z:= 0;
            letra[9].x:= -17;   letra[9].y:= 15;  letra[9].z:= 0;
            letra[10].x:= -20;  letra[10].y:= 15; letra[10].z:= 0;
        end;

    with carac[2] do
        begin
            np:= 8;
            letra[1].x:= 0;    letra[1].y:= 0;  letra[1].z:= 0;
            letra[2].x:= 10.5; letra[2].y:= 0;  letra[2].z:= 0;
            letra[3].x:= 10.5; letra[3].y:= 3;  letra[3].z:= 0;
            letra[4].x:= 3;    letra[4].y:= 3;  letra[4].z:= 0;
            letra[5].x:= 3;    letra[5].y:= 12; letra[5].z:= 0;
            letra[6].x:= 10.5; letra[6].y:= 12; letra[6].z:= 0;
            letra[7].x:= 10.5; letra[7].y:= 15; letra[7].z:= 0;
            letra[8].x:= 0;    letra[8].y:= 15; letra[8].z:= 0;
        end;

    with carac[3] do
        begin
            np:= 12;
            letra[1].x:= 20;    letra[1].y:= 0;   letra[1].z:= 0;
            letra[2].x:= 30.5;  letra[2].y:= 0;   letra[2].z:= 0;
            letra[3].x:= 30.5;  letra[3].y:= 3;   letra[3].z:= 0;
            letra[4].x:= 23;    letra[4].y:= 3;   letra[4].z:= 0;
            letra[5].x:= 23;    letra[5].y:= 6;   letra[5].z:= 0;
            letra[6].x:= 27;    letra[6].y:= 6;   letra[6].z:= 0;
            letra[7].x:= 27;    letra[7].y:= 9;   letra[7].z:= 0;
            letra[8].x:= 23;    letra[8].y:= 9;   letra[8].z:= 0;
            letra[9].x:= 23;    letra[9].y:= 12;  letra[9].z:= 0;
            letra[10].x:= 30.5; letra[10].y:= 12; letra[10].z:= 0;
            letra[11].x:= 30.5; letra[11].y:= 15; letra[11].z:= 0;
            letra[12].x:= 20;   letra[12].y:= 15; letra[12].z:= 0;
        end;
end;

{---------------------------------------------------------------}

procedure Desenha ( simb:POLIGONO );
   const
      PROF = 5;
   var
      i : integer;
   begin
      with simb do
        begin
           { Face da frente }
           Move3d ( letra[1].x, letra[1].y, letra[1].z );
           for i:= 2 to np do
                Risca3d ( letra[i].x, letra[i].y, letra[i].z );
           Risca3d ( letra[1].x, letra[1].y, letra[1].z );

           { Face de tras }
           Move3d ( letra[1].x, letra[1].y, letra[1].z-PROF );
           for i:= 2 to np do
                Risca3d ( letra[i].x, letra[i].y, letra[i].z-PROF );
           Risca3d ( letra[1].x, letra[1].y, letra[1].z-PROF );

           { Arestas laterais }
           for i:= 1 to np do
              Reta3d ( letra[i].x, letra[i].y, letra[i].z,
                       letra[i].x, letra[i].y, letra[i].z-PROF );
        end;
   end;

{---------------------------------------------------------------}

var i,j: integer;
    t,x,y,z: real;

begin
   InicVideo3d ( CGAHI );
   InicLetras;

   AbreArqAnimacao3d ( 'vinheta' );
   for i:=0 to NQUADROS do
      begin
         t:= i/NQUADROS;
         x:= (1-t)*x0 + t*x1;
         y:= (1-t)*y0 + t*y1;
         z:= (1-t)*z0 + t*z1;
         Observador3d (x,y,z, 0,0,0);
         ClearDevice;
         for j:= 1 to NLETRAS do
             Desenha(carac[j]);
         FechaCenaAnimacao3d;
      end;
   FechaArqAnimacao3d;
   Termina3d;
end.