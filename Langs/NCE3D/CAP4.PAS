{-------------------------------------------------------------}
{                                                             }
{    Quarto exemplo da NCE3D                                  }
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
               x := R * cos (ang);
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
    InicVideo3d ( CGAHI );

    {   Primeira janela - vista frontal   }
    observador3d ( 0,0,-100, 0,0,0 );
    janela3d ( 40,                 { distancia olho-tela }
               100,                { plano de fundo }
               20.0, 15.0,         { tamanho real da janela }
               0, 0,               { posicao da janela }
               160, 100 );         { numero de pontos da janela }
    rectangle ( 0, 0, 160, 100 );
    desenha_mola;

    {   Segunda janela - vista superior   }
    observador3d ( 0,-100,0, 0,0,0 );
    janela3d ( 40,                 { distancia olho-tela }
               100,                { plano de fundo }
               20.0, 15.0,         { tamanho real da janela }
               0, 100,             { posicao da janela }
               160, 100 );         { numero de pontos da janela }
    rectangle ( 0, 100, 160, 199 );
    desenha_mola;

    {   Terceira janela - vista inclinada  }
    observador3d ( -100,-100,-100, 0,0,0 );
    janela3d ( 40,                 { distancia olho-tela }
               100,                { plano de fundo }
               20.0, 15.0,         { tamanho real da janela }
               160, 0,             { posicao da janela }
               480,200     );      { numero de pontos da janela }
    rectangle ( 160, 0, 639, 199 );
    desenha_mola;

    readln;
    Termina3d;
end.
