{-------------------------------------------------------------}
{                                                             }
{    Caminha atraves de quatro piramides                      }
{                                                             }
{    Autor:  Jose' Antonio Borges                             }
{            Mauricio Nunes Bomfim                            }
{                                                             }
{    Em Abril/92                                              }
{                                                             }
{-------------------------------------------------------------}

program caminhador;

uses nce3d, graph, crt;

const
    MAIS  = '+';
    MENOS = '-';
    CIMA  = #72;
    BAIXO = #80;
    ESQ   = #75;
    DIR   = #77;
    ESC   = #27;

var
    c : char;
    modo, pag : integer;

{-------------------------------------------------------------}

procedure mostraTorres;

   procedure fazPiramide (x1,y1,z1, x2,y2,z2: integer);
      var
          xm, zm: integer;

      begin
          { Desenha base }
          reta3D ( x1, y1, z1, x2, y1, z1 );
          reta3D ( x2, y1, z1, x2, y1, z2 );
          reta3D ( x2, y1, z2, x1, y1, z2 );
          reta3D ( x1, y1, z2, x1, y1, z1 );

          { Desenha arestas }
          xm:= (x1+x2) div 2;
          zm:= (z1+z2) div 2;
          reta3D ( x1, y1, z1, xm, y2, zm );
          reta3D ( x1, y1, z2, xm, y2, zm );
          reta3D ( x2, y1, z2, xm, y2, zm );
          reta3D ( x2, y1, z1, xm, y2, zm );

      end;

begin
    { piramides em -30 e +30, em x e z }
    Setcolor (blue);
    fazPiramide ( +30, 0, +30, +50, 50, +50 );
    Setcolor (yellow);
    fazPiramide ( +30, 0, -30, +50, 50, -50 );
    Setcolor (magenta);
    fazPiramide ( -30, 0, +30, -50, 50, +50 );
    Setcolor (cyan);
    fazPiramide ( -30, 0, -30, -50, 50, -50 );

    setcolor (white);
    rectangle (0,0, getmaxx, getmaxy);
end;

{-----------------------------------------------------------}

begin
    pag := 1;
    modo := EGAHI;   { ou outro qualquer }

    InicVideo3d ( modo );
    Janela3d    ( 40,300,20,15, 0,0,getmaxx+1,getmaxy+1);

    translada3d ( 0, -15, 200 );
    mostraTorres;

    directvideo := false;
    gotoxy (2,24);
    writeln ('Tecle: x, y, z, +, - ou setas');

    repeat
        c := readkey;

        if modo = EGAHI then  setactivepage (pag);

        ClearDevice;

        if c = #00 then  c := readkey;
        case upcase(c) of
            MAIS:  translada3d ( 0, 0,-20);
            MENOS: translada3d ( 0, 0, 20);
            DIR:   translada3d ( 5, 0,  0);
            ESQ:   translada3d (-5, 0,  0);
            CIMA:  translada3d ( 0, 5,  0);
            BAIXO: translada3d ( 0,-5,  0);
            'X':  begin
                    SelecModoTransf3d (HIERARQUICO);
                    rodaX3d (10 * pi / 180);
                    SelecModoTransf3d (NORMAL);
                  end;
            'Y':  begin
                    SelecModoTransf3d (HIERARQUICO);
                    rodaY3d (10 * pi / 180);
                    SelecModoTransf3d (NORMAL);
                  end;
            'Z':  begin
                    SelecModoTransf3d (HIERARQUICO);
                    rodaZ3d (10 * pi / 180);
                    SelecModoTransf3d (NORMAL);
                  end;
        end;

        mostraTorres;

        if modo = EGAHI then  setvisualpage (pag);
        pag := (pag + 1) and 1;

   until c = ESC;

   Termina3d;

end.
