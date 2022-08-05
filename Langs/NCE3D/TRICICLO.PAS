{-------------------------------------------------------------}
{                                                             }
{   Animacao de um Triciclo                                   }
{                                                             }
{   Mauricio Bomfim e Antonio Borges                          }
{   Modelagem: Werner Adler                                   }
{                                                             }
{   Maio de 1993                                              }
{                                                             }
{-------------------------------------------------------------}

program Tciclo;
uses crt, graph, nce3d;

const
    NVERT   = 65;

var
    ang, jan: integer;
    rad: real;
    x, y, z: array [ 1..NVERT ] of real;
    x1,y1,x2,y2: integer;

    xdesl1, ydesl1, zdesl1, xdesl2, ydesl2, zdesl2: real;
    xquad1, yquad1, zquad1, xquad2, yquad2, zquad2: real;
    xjoel1, yjoel1, zjoel1, xjoel2, yjoel2, zjoel2: real;
    xtorn1, ytorn1, ztorn1, xtorn2, ytorn2, ztorn2: real;
    xtorn3, ytorn3, ztorn3, xtorn4, ytorn4, ztorn4: real;

{-------------------------------------------------------------}

procedure InicPontos;

begin

{----------------  triciclo -------------}

{ corpo da bicicleta }
    x[ 1 ] := 2;     y[ 1 ] := 1;   z[ 1 ] := 4;
    x[ 2 ] := 4;     y[ 2 ] := 3;   z[ 2 ] := 4;
    x[ 3 ] := 14.5;  y[ 3 ] := 3;   z[ 3 ] := 4;
    x[ 4 ] := 14;    y[ 4 ] := 9;   z[ 4 ] := 4;

{ eixo de tras }
    x[ 5 ] := 2;  y[ 5 ] := 1;   z[ 5 ] := 0;
    x[ 6 ] := 2;  y[ 6 ] := 1;   z[ 6 ] := 8;

{ guidon }
    x[ 7 ] := 14;   y[ 7 ] := 9;     z[ 7 ] := 3;
    x[ 8 ] := 14;   y[ 8 ] := 9;     z[ 8 ] := 5;
    x[ 11 ] := 14;  y[ 11 ] := 10;   z[ 11 ] := 1;
    x[ 12 ] := 14;  y[ 12 ] := 10;   z[ 12 ] := 2;
    x[ 64 ] := 14;  y[ 64 ] := 10;   z[ 64 ] := 6;
    x[ 65 ] := 14;  y[ 65 ] := 10;   z[ 65 ] := 7;

{ apoio do selim }
    x[ 9 ] := 9;    y[ 9 ] := 3;     z[ 9 ] := 4;
    x[ 10 ] := 8;   y[ 10 ] := 7;    z[ 10 ] := 4;

{ rodas de tras }
    x[ 13 ] := 1;   y[ 13 ] := 0.5;  z[ 13 ] := 0;
    x[ 14 ] := 3;   y[ 14 ] := 0.5;  z[ 14 ] := 0;
    x[ 15 ] := 2;   y[ 15 ] := 2;    z[ 15 ] := 0;

    x[ 16 ] := 1;   y[ 16 ] := 0.5;  z[ 16 ] := 8;
    x[ 17 ] := 3;   y[ 17 ] := 0.5;  z[ 17 ] := 8;
    x[ 18 ] := 2;   y[ 18 ] := 2;    z[ 18 ] := 8;

    x[ 58 ] := 1;   y[ 58 ] := 1.5;  z[ 58 ] := 0;
    x[ 59 ] := 3;   y[ 59 ] := 1.5;  z[ 59 ] := 0;
    x[ 60 ] := 2;   y[ 60 ] := 0;    z[ 60 ] := 0;

    x[ 61 ] := 1;   y[ 61 ] := 1.5;  z[ 61 ] := 8;
    x[ 62 ] := 3;   y[ 62 ] := 1.5;  z[ 62 ] := 8;
    x[ 63 ] := 2;   y[ 63 ] := 0;    z[ 63 ] := 8;

{ roda da frente }
    x[ 19 ] := 11;  y[ 19 ] := 1.5;  z[ 19 ] := 4;
    x[ 20 ] := 17;  y[ 20 ] := 1.5;  z[ 20 ] := 4;
    x[ 21 ] := 14;  y[ 21 ] := 6;    z[ 21 ] := 4;

    x[ 55 ] := 11;  y[ 55 ] := 4.5;  z[ 55 ] := 4;
    x[ 56 ] := 17;  y[ 56 ] := 4.5;  z[ 56 ] := 4;
    x[ 57 ] := 14;  y[ 57 ] := 0;    z[ 57 ] := 4;

{ pedal }
    x[ 22] := 14;  y[ 22] := 3;     z[ 22] := 3;
    x[ 23] := 14;  y[ 23] := 3;     z[ 23] := 5;
    x[ 24] := 14;  y[ 24] := 1.5;   z[ 24] := 2;
    x[ 25] := 14;  y[ 25] := 1.5;   z[ 25] := 3;
    x[ 26] := 14;  y[ 26] := 4.5;   z[ 26] := 5;
    x[ 27] := 14;  y[ 27] := 4.5;   z[ 27] := 6;

{----------------  boneco -------------}

{ quadril }
    x[ 28] := 8;  y[ 28] := 7; z[ 28] := 2;
    x[ 29] := 8;  y[ 29] := 7; z[ 29] := 6;

{ corpo e pescoco }
    x[ 30] := 8;  y[ 30] := 15; z[ 30] := 4;

{ ombros }
    x[ 31] := 8;  y[ 31] := 13; z[ 31] := 1.5;
    x[ 32] := 8;  y[ 32] := 13; z[ 32] := 6.5;

{ bracos }
    x[ 33] := 11;  y[ 33] := 11;   z[ 33] := 1.5;
    x[ 34] := 14;  y[ 34] := 10;   z[ 34] := 1.5;
    x[ 35] := 11;  y[ 35] := 11;   z[ 35] := 6.5;
    x[ 36] := 14;  y[ 36] := 10;   z[ 36] := 6.5;

{ cabeca }
    x[ 37] := 7; y[ 37] := 15; z[ 37] := 3;
    x[ 38] := 9; y[ 38] := 15; z[ 38] := 3;
    x[ 39] := 7; y[ 39] := 15; z[ 39] := 5;
    x[ 40] := 9; y[ 40] := 15; z[ 40] := 5;

    x[ 41] := 7; y[ 41] := 17; z[ 41] := 3;
    x[ 42] := 9; y[ 42] := 17; z[ 42] := 3;
    x[ 43] := 7; y[ 43] := 17; z[ 43] := 5;
    x[ 44] := 9; y[ 44] := 17; z[ 44] := 5;

{ chapeu }
    x[ 45] := 8;   y[ 45] := 18;   z[ 45] := 3;
    x[ 46] := 8;   y[ 46] := 18;   z[ 46] := 5;
    x[ 47] := 10.5; y[ 47] := 16.5; z[ 47] := 3;
    x[ 48] := 10.5; y[ 48] := 16.5; z[ 48] := 5;

{ perna }
    x[ 49] := 13;  y[ 49] := 3; z[ 49] := 2.5;
    x[ 50] := 15;  y[ 50] := 3; z[ 50] := 2.5;

    x[ 51] := 13; y[ 51] := 5; z[ 51] := 5.5;
    x[ 52] := 15; y[ 52] := 5; z[ 52] := 5.5;

    x[ 53] := 12; y[ 53] := 6;  z[ 53] := 2.5;
    x[ 54] := 12; y[ 54] := 8;  z[ 54] := 5.5;
end;

{-------------------------------------------------------------}

procedure PreparaJanela (j: integer);
var
    xo, yo, zo: real;
begin
    case j of
        1: begin
               x1 := getmaxx div 4;
               y1 := 0;
               x2 := getmaxx;
               y2 := getmaxy;
               Observador3d (65,60,-65, 0,0,0);
               Janela3d (40, 200, 20.0, 15.0, x1, y1, x2-x1, y2-y1);
           end;
        2: begin
               x1 := 0;
               y1 := 0;
               x2 := getmaxx div 4 - 1;
               y2 := getmaxy div 3;
               Observador3d (0,0,-170, 0,0,0);
               Janela3d (40, 200, 20.0, 15.0, x1, y1, x2-x1, y2-y1);
           end;
        3: begin
               x1 := 0;
               y1 := getmaxy div 3 + 1;
               x2 := getmaxx div 4 - 1;
               y2 := 2 * getmaxy div 3;
               Observador3d (0,170,0, 0,0,0);
               Janela3d (40, 200, 20.0, 15.0, x1, y1, x2-x1, y2-y1);
           end;
        4: begin
               x1 := 0;
               y1 := 2 * getmaxy div 3 + 1;
               x2 := getmaxx div 4 - 1;
               y2 := getmaxy;

               xo := 70 * cos (rad+pi/2);
               yo := 10;
               zo := -70 * sin (rad+pi/2);
               Observador3d (xo, yo, zo, 0,0,0);
               Janela3d (40, 200, 20.0, 15.0, x1, y1, x2-x1, y2-y1);
           end;
    end;
end;

{-------------------------------------------------------------}
{                       desenha o chao                        }
{-------------------------------------------------------------}

procedure desenhaChao;
var i, x: integer;
begin
    for i:= 0 to 10 do
        begin
            x:= -25 + i * 5;
            Reta3d (x,0,-25,x,0,25);
        end;

    for i:= 0 to 10 do
        begin
            x:= -25 + i * 5;
            Reta3d (-25,0,x,25,0,x);
        end;
end;

{-------------------------------------------------------------}
{              desenha os elementos do figura                 }
{-------------------------------------------------------------}

procedure DesenhaCorpo;
var i : integer;
begin
{ corpo da bicicleta }
    Reta3d (x[ 1 ], y[ 1 ], z[ 1 ],   x[ 2 ], y[ 2 ], z[ 2 ]);
    Reta3d (x[ 2 ], y[ 2 ], z[ 2 ],   x[ 3 ], y[ 3 ], z[ 3 ]);
    Reta3d (x[ 3 ], y[ 3 ], z[ 3 ],   x[ 4 ], y[ 4 ], z[ 4 ]);
    Reta3d (x[ 5 ], y[ 5 ], z[ 5 ],   x[ 6 ], y[ 6 ], z[ 6 ]);
    Reta3d (x[ 9 ], y[ 9 ], z[ 9 ],   x[ 10], y[ 10], z[ 10]);
    Reta3d (x[ 11], y[ 11], z[ 11],   x[ 12], y[ 12], z[ 12]);
    Reta3d (x[ 7 ], y[ 7 ], z[ 7 ],   x[ 12], y[ 12], z[ 12]);
    Reta3d (x[ 7 ], y[ 7 ], z[ 7 ],   x[ 8 ], y[ 8 ], z[ 8 ]);
    Reta3d (x[ 64], y[ 64], z[ 64],   x[ 8 ], y[ 8 ], z[ 8 ]);
    Reta3d (x[ 64], y[ 64], z[ 64],   x[ 65], y[ 65], z[ 65]);

{ boneco }
    Reta3d (x[ 28], y[ 28], z[ 28],   x[ 29], y[ 29], z[ 29]);
    Reta3d (x[ 10], y[ 10], z[ 10],   x[ 30], y[ 30], z[ 30]);
    Reta3d (x[ 31], y[ 31], z[ 31],   x[ 32], y[ 32], z[ 32]);
    Reta3d (x[ 31], y[ 31], z[ 31],   x[ 33], y[ 33], z[ 33]);
    Reta3d (x[ 33], y[ 33], z[ 33],   x[ 34], y[ 34], z[ 34]);
    Reta3d (x[ 32], y[ 32], z[ 32],   x[ 35], y[ 35], z[ 35]);
    Reta3d (x[ 35], y[ 35], z[ 35],   x[ 36], y[ 36], z[ 36]);

{ cabeca }
    Reta3d (x[ 37], y[ 37], z[ 37],   x[ 38], y[ 38], z[ 38]);
    Reta3d (x[ 38], y[ 38], z[ 38],   x[ 39], y[ 39], z[ 39]);
    Reta3d (x[ 39], y[ 39], z[ 39],   x[ 40], y[ 40], z[ 40]);
    Reta3d (x[ 40], y[ 40], z[ 40],   x[ 37], y[ 37], z[ 37]);
    Reta3d (x[ 41], y[ 41], z[ 41],   x[ 42], y[ 42], z[ 42]);
    Reta3d (x[ 42], y[ 42], z[ 42],   x[ 43], y[ 43], z[ 43]);
    Reta3d (x[ 43], y[ 43], z[ 43],   x[ 44], y[ 44], z[ 44]);
    Reta3d (x[ 44], y[ 44], z[ 44],   x[ 41], y[ 41], z[ 41]);
    Reta3d (x[ 37], y[ 37], z[ 37],   x[ 41], y[ 41], z[ 41]);
    Reta3d (x[ 38], y[ 38], z[ 38],   x[ 42], y[ 42], z[ 42]);
    Reta3d (x[ 39], y[ 39], z[ 39],   x[ 43], y[ 43], z[ 43]);
    Reta3d (x[ 40], y[ 40], z[ 40],   x[ 44], y[ 44], z[ 44]);

{ chapeu }
    Reta3d (x[ 41], y[ 41], z[ 41],   x[ 45], y[ 45], z[ 45]);
    Reta3d (x[ 42], y[ 42], z[ 42],   x[ 45], y[ 45], z[ 45]);
    Reta3d (x[ 43], y[ 43], z[ 43],   x[ 46], y[ 46], z[ 46]);
    Reta3d (x[ 44], y[ 44], z[ 44],   x[ 46], y[ 46], z[ 46]);
    Reta3d (x[ 45], y[ 45], z[ 45],   x[ 46], y[ 46], z[ 46]);
    Reta3d (x[ 41], y[ 41], z[ 41],   x[ 47], y[ 47], z[ 47]);
    Reta3d (x[ 44], y[ 44], z[ 44],   x[ 48], y[ 48], z[ 48]);
    Reta3d (x[ 47], y[ 47], z[ 47],   x[ 48], y[ 48], z[ 48]);
end;

{-------------------------------------------------------------}

procedure DesenhaRodasTras;
begin
{ roda da direita }
    Reta3d (x[ 13], y[ 13], z[ 13],   x[ 58], y[ 58], z[ 58]);
    Reta3d (x[ 58], y[ 58], z[ 58],   x[ 15], y[ 15], z[ 15]);
    Reta3d (x[ 15], y[ 15], z[ 15],   x[ 59], y[ 59], z[ 59]);
    Reta3d (x[ 59], y[ 59], z[ 59],   x[ 14], y[ 14], z[ 14]);
    Reta3d (x[ 14], y[ 14], z[ 14],   x[ 60], y[ 60], z[ 60]);
    Reta3d (x[ 60], y[ 60], z[ 60],   x[ 13], y[ 13], z[ 13]);

{ roda da esquerda }
    Reta3d (x[ 16], y[ 16], z[ 16],   x[ 61], y[ 61], z[ 61]);
    Reta3d (x[ 61], y[ 61], z[ 61],   x[ 18], y[ 18], z[ 18]);
    Reta3d (x[ 18], y[ 18], z[ 18],   x[ 62], y[ 62], z[ 62]);
    Reta3d (x[ 62], y[ 62], z[ 62],   x[ 17], y[ 17], z[ 17]);
    Reta3d (x[ 17], y[ 17], z[ 17],   x[ 63], y[ 63], z[ 63]);
    Reta3d (x[ 63], y[ 63], z[ 63],   x[ 16], y[ 16], z[ 16]);
end;

{-------------------------------------------------------------}

procedure DesenhaRodaFrente;
begin
{ roda }
    Reta3d (x[ 19], y[ 19], z[ 19],   x[ 55], y[ 55], z[ 55]);
    Reta3d (x[ 55], y[ 55], z[ 55],   x[ 21], y[ 21], z[ 21]);
    Reta3d (x[ 21], y[ 21], z[ 21],   x[ 56], y[ 56], z[ 56]);
    Reta3d (x[ 56], y[ 56], z[ 56],   x[ 20], y[ 20], z[ 20]);
    Reta3d (x[ 20], y[ 20], z[ 20],   x[ 57], y[ 57], z[ 57]);
    Reta3d (x[ 57], y[ 57], z[ 57],   x[ 19], y[ 19], z[ 19]);

{ pedal }
    Reta3d (x[ 24], y[ 24], z[ 24],   x[ 25], y[ 25], z[ 25]);
    Reta3d (x[ 25], y[ 25], z[ 25],   x[ 22], y[ 22], z[ 22]);
    Reta3d (x[ 22], y[ 22], z[ 22],   x[ 23], y[ 23], z[ 23]);
    Reta3d (x[ 23], y[ 23], z[ 23],   x[ 26], y[ 26], z[ 26]);
    Reta3d (x[ 26], y[ 26], z[ 26],   x[ 27], y[ 27], z[ 27]);
end;

{-------------------------------------------------------------}
 
procedure DesenhaPernas;
begin
{ traca quadril ao joelho }
      Reta3d (xquad1,yquad1,zquad1,  xjoel1,yjoel1,zjoel1);
{ traca joelho ao tornozelo }
      Reta3d (xjoel1,yjoel1,zjoel1,  xtorn1,ytorn1,ztorn1);
{ traca tornozelo a ponta do pe }
      Reta3d (xtorn1,ytorn1,ztorn1,  xtorn2,ytorn2,ztorn2);

{ traca quadril ao joelho }
      Reta3d (xquad2,yquad2,zquad2,  xjoel2,yjoel2,zjoel2);
{ traca joelho ao tornozelo }
      Reta3d (xjoel2,yjoel2,zjoel2,  xtorn3,ytorn3,ztorn3);
{ traca tornozelo a ponta do pe }
      Reta3d (xtorn3,ytorn3,ztorn3,  xtorn4,ytorn4,ztorn4);
end;

{-------------------------------------------------------------}
{                 Transforma e desenha o corpo                }
{-------------------------------------------------------------}

procedure Corpo;
begin
    SalvaTransf3d;

    aplicatransfobjeto3d (x[28],y[28],z[28], xquad1,yquad1,zquad1);
    aplicatransfobjeto3d (x[29],y[29],z[29], xquad2,yquad2,zquad2);
    aplicatransfobjeto3d (x[53],y[53],z[53], xjoel1,yjoel1,zjoel1);
    aplicatransfobjeto3d (x[54],y[54],z[54], xjoel2,yjoel2,zjoel2);
    DesenhaCorpo;
    RecuperaTransf3d;
end;

{-------------------------------------------------------------}
{             Transforma e desenha rodas de tras              }
{-------------------------------------------------------------}

procedure RodasTras;
begin
    SalvaTransf3d;

    Translada3d (2,1,0);     { recoloca no lugar original }
    RodaZ3d (-rad);          { roda em z }
    Translada3d (-2,-1,0);   { trago o eixo de tras para a origem }

    DesenhaRodasTras;
    RecuperaTransf3d;
end;

{-------------------------------------------------------------}
{             Transforma e desenha roda da frente             }
{-------------------------------------------------------------}

procedure RodaFrente;
begin
    SalvaTransf3d;

    Translada3d (14,3,0);      { recoloco a roda no lugar original }
    RodaZ3d (-rad);            { rodo em z }
    Translada3d (-14,-3,0);    { trago o centro da roda da frente para
                                  a origem }

{ Salva pontos transformados dos tornozelos }
    aplicatransfobjeto3d (x[24],y[24],2.5,xtorn1,ytorn1,ztorn1);
                               { z = 2.5 e o ponto medio do pedal direito }
    aplicatransfobjeto3d (x[27],y[27],5.5,xtorn3,ytorn3,ztorn3);
                               { z = 5.5 e o ponto medio do pedal esquerdo }

{ Salva pontos transformados das extremidades do pedal para calcular
      subida e descida dos joelhos }
    aplicatransfObjeto3d (x[24],y[24],z[24], xdesl1,ydesl1,zdesl1);
    aplicatransfObjeto3d (x[27],y[27],z[27], xdesl2,ydesl2,zdesl2);

{ Salva pontos transformados da ponta do pe }
    aplicatransfobjeto3d (x[24],y[24],2.5,xtorn2,ytorn2,ztorn2);
    aplicatransfobjeto3d (x[27],y[27],5.5,xtorn4,ytorn4,ztorn4);

    DesenhaRodaFrente;
    RecuperaTransf3d;
end;
{-------------------------------------------------------------}
{                Transforma e desenha pernas                  }
{-------------------------------------------------------------}

procedure Pernas;
var
      dy1, dy2: real;
begin
    SalvaTransf3d;

{ calcula posicao dos joelhos }
    dy1 := y[24] - ydesl1;
    yjoel1 := yjoel1 - dy1;
    dy2 := y[27] - ydesl2;
    yjoel2 := yjoel2 - dy2;

    InicTransf3d;
    DesenhaPernas;
    RecuperaTransf3d;
end;

{-------------------------------------------------------------}
{                Transforma e desenha triciclo                }
{-------------------------------------------------------------}

procedure Triciclo;
begin
    SalvaTransf3d;

    RodaY3d (rad);
    Translada3d (-7,0,7);

    Corpo;
    RodasTras;
    RodaFrente;
    Pernas;
    RecuperaTransf3d;
end;

{-------------------------------------------------------------}
{                       programa principal                    }
{-------------------------------------------------------------}

label termina;

begin
    inicVideo3d (EGAHi);
    SelecModoTransf3d (HIERARQUICO);
    InicPontos;

    while true do
         begin
             for ang := 0 to 35 do
                 begin
                     InicTransf3d;
                     rad := 10 * ang * pi / 180.0;
                     setactivepage (ang and 1);
                     cleardevice;
                     if keypressed then goto termina;

                     for jan := 1 to 4 do
                         begin
                             preparaJanela (jan);

                             setcolor (brown);
                             desenhaChao;

                             setcolor (white);
                             Triciclo;

                             rectangle (x1,y1,x2,y2);
                         end;
                     setvisualpage (ang and 1);
                 end;
         end;
termina:
    Termina3d;
end.