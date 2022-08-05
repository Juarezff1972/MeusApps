{-------------------------------------------------------------}
{                                                             }
{    Visao de uma mola                                        }
{    a partir de tres posicoes do observador.                 }
{                                                             }
{    Autor:  Jose' Antonio Borges                             }
{            Mauricio Nunes Bomfim                            }
{                                                             }
{    Em julho/92                                              }
{                                                             }
{-------------------------------------------------------------}

program mola;
uses nce3d, graph, crt;

procedure desenha_mola;
   const
       R = 5;
   var
       t, x, y, z, ang: real;
   begin
       t := 0;
       while t < 720 do
           begin
               ang := (t * 5) * pi / 180.0;
               x := R * cos(ang);
               y := t/50.0 - 7;
               z := R * sin(ang);
               if t = 0 then
                    move3d (x, y, z)
               else
                    risca3d (x, y, z);
               t := t + 1;
           end;
   end;

{-------------------------------------------------------------}

begin
    InicVideo3d (CGAC0);

    {   Vista frontal   }
    observador3d ( 0,0,-100, 0,0,0 );
    rectangle (0,0,getmaxx,getmaxy);
    desenha_mola;
    readln;

    {   Vista superior   }
    cleardevice;
    observador3d ( 0,100,0, 0,0,0 );
    rectangle (0,0,getmaxx,getmaxy);
    desenha_mola;
    readln;

    {   Vista inclinada  }
    cleardevice;
    observador3d ( -80,-80,-80, 0,0,0 );
    rectangle (0,0,getmaxx,getmaxy);
    desenha_mola;
    readln;

    Termina3d;
end.
