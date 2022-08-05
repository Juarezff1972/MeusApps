{--------------------------------------------------------}
{                                                        }
{  Demonstrativo de superficies de Bezier,               }
{  B-Spline e Hermitiana.                                }
{                                                        }
{  Autores: Mauricio                                     }
{                                                        }
{  Em junho de 1992                                      }
{                                                        }
{--------------------------------------------------------}

program SBezier;
uses crt, graph, ncemenu, nce3d;

const
    WIREFRAME = TRUE;
var
    pc    : array [0..30] of COORDPTO;
    nptu,
    nptv  : integer;      { numero de pontos de controle }

    ptos  : array [0..800] of PONTO;
    nu, nv: integer;      { numero de ptos gerados }
    du, dv: real;

{--------------------------------------------------------}
{            Leitura dos pontos de controle              }
{--------------------------------------------------------}

procedure LePontos;
var
    i, j: integer;
    arq: text;
begin
    assign (arq, 'pcontr.dat' );
    reset (arq);

    readln (arq, nptu, nptv);
    readln (arq, du, dv);

    for i := 0 to nptu-1 do
        for j := 0 to nptv-1 do
           with pc[i*nptv+j] do
               readln (arq, x,y,z);

    close (arq);
end;

{--------------------------------------------------------}
{              Exibe os pontos de controle               }
{--------------------------------------------------------}

procedure MostraPontos;
var
    i, j: integer;
    v: VERTICES;

    procedure AtribuiVert ( n,i,j: integer );
       begin
               v[n].x:= pc[i*nptv+j].x;
               v[n].y:= pc[i*nptv+j].y;
               v[n].z:= pc[i*nptv+j].z;
       end;

begin
    SelecModoPintura3d ( PINTAARAMADO );

    selecRGBTraco3d (1,1,1);
    Reta3d ( -100,   0,   0, 100,  0,  0 );
    Reta3d (    0,-100,   0,   0,100,  0 );
    Reta3d (    0,   0,-100,   0,  0,100 );

    SelecRGBTraco3d (1,1,0);
    for i := 0 to nptu-2 do
        for j := 0 to nptv-2 do
           begin
              Atribuivert ( 1,i,  j );
              Atribuivert ( 2,i+1,j );
              Atribuivert ( 3,i+1,j+1 );
              Atribuivert ( 4,i,  j+1 );
              PintaPolig3d ( 4,v );
           end;
    selecRGBTraco3d (1,1,1);

    readln;
    SelecModoPintura3d ( PINTASOLIDO );
end;

{--------------------------------------------------------}

procedure CalcSuperficie;
    var
        opcao : integer;
    begin
        opcao:= menubarra ( 30,30, '1 - Superficie de Bezier/'  +
                                   '2 - Superficie B-Spline/'   +
                                   '3 - Superficie Hermitiana/' +
                                   '4 - Fim do Programa' );

        OutTextXY ( 10, 10, 'Calculando a superficie.');
        OutTextXY ( 10, 30, 'Aguarde um momento...' );
        case opcao of
          1: CalcSupBezier3d  ( pc, nptu,nptv, du, dv,
                                ptos, nu,nv );
          2: CalcSupBSpline3d ( pc, nptu,nptv, du, dv,
                                ptos, nu,nv );
          3: CalcSupHermit3d  ( pc, nptu,nptv, du, dv,
                                ptos, nu,nv );
          4: begin
               Termina3d;
               Halt;
             end;
        end;
        OutTextXY ( 10, 50, 'Ok.' );
        delay (500);
    end;

{--------------------------------------------------------}

begin
    LePontos;
    InicVideo3d (SVGA256);
    Janela3d ( 20,
               350,
               20.0, 15.0,
               0, 0,
               getmaxx+1, getmaxy+1 );
    Observador3d ( -75, -100, -100, 0,0,-50 );

    while TRUE do
        begin
            MostraPontos;
            CalcSuperficie;

            { Desenha a superficie }
            if WIREFRAME then
                begin
                   SelecModoPintura3d (PINTAARAMADO);
                   SelecRGBTraco3d (0.2,0.2,1.0);
                end
            else
                InicZBuffer3d ( getmaxy+1, 0, 'C:' );
            DesenhaSuperf3d (ptos,nu,nv);
            readln;
            cleardevice;
        end;
end.
