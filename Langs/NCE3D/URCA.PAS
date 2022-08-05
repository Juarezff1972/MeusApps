{--------------------------------------------------------}
{                                                        }
{  Geracao de uma animacao a partir de uma               }
{  modelagem do Pao de Acucar feita com                  }
{  superficies e curvas de Bezier                        }
{                                                        }
{  Autor: Mauricio Bomfim                                }
{  Modelagem: Joao Paulo Bento                           }
{                                                        }
{  Em agosto de 1992                                     }
{                                                        }
{--------------------------------------------------------}
program PaodeAcucar;
uses crt, dos, graph, nce3d, utnce3d;

const
    NSUP   = 5;
    NCRV   = 7;
    dt     = 0.5;
    COR_DO_MAR  = 10;
    COR_DO_CEU  = 23;
    EM_DISCO = FALSE;
    NOMEARQ = 'URCA';

type
    CURVA = record
              np: integer;
              p: array[0..10] of COORDPTO;
            end;

    SUPBEZI = record
                 nu, nv: integer;
                 ptos: array [0..120] of PONTO;
              end;

var
    nptu, nptv: integer;             { Ptos de controle das superficies. }
    pc: array [0..30] of COORDPTO;

    sup: array [1..NSUP] of SUPBEZI; { Superficies de Bezier calculadas. }
    du, dv: real;

    crv: array [1..NCRV] of CURVA;   { Ptos de controle das curvas.      }

    caminho: CURVA;                  { Ptos de controle da trajetoria.   }


{--------------------------------------------------------}
{    Leitura dos pontos de controle de uma superficie    }
{--------------------------------------------------------}

procedure LePontosCtrSup ( nomearq:string );
    var
      i, j: integer;
      arq: text;
    begin
        assign (arq, nomearq );
        reset (arq);
        readln (arq, nptu, nptv);
        for i := 0 to nptu-1 do
            for j := 0 to nptv-1 do
                with pc[i*nptv+j] do
                    readln (arq, x,y,z);
        close (arq);
    end;


{--------------------------------------------------------}
{    Leitura dos pontos de controle de uma curva         }
{--------------------------------------------------------}

procedure LePontosCtrCrv ( nomearq:string; var c:CURVA );
    var
      i: integer;
      arq: text;
    begin
        assign (arq, nomearq );
        reset (arq);
        with c do
           begin
              readln (arq, np);

              for i := 0 to np-1 do
                   readln (arq, p[i].x, p[i].y, p[i].z);
           end;
        close (arq);
    end;


{--------------------------------------------------------}
{           Calculo das superficies de Bezier            }
{--------------------------------------------------------}

procedure CalculaSuperficies;
  var
     du, dv: real;
  begin
    OutTextXY ( 10, 30, 'Calculando as superficies.');
    OutTextXY ( 10, 50, 'Aguarde um momento...' );
    du:= 0.1; dv:= 0.1;

    LePontosCtrSup ( 'sup1.ctr' );
    CalcSupBezier3d ( pc, nptu,nptv, du, dv,
                      sup[1].ptos, sup[1].nu, sup[1].nv );

    LePontosCtrSup ( 'sup2.ctr' );
    CalcSupBezier3d ( pc, nptu,nptv, du, dv,
                      sup[2].ptos, sup[2].nu, sup[2].nv );

    LePontosCtrSup ( 'sup3.ctr' );
    CalcSupBezier3d ( pc, nptu,nptv, du, dv,
                      sup[3].ptos, sup[3].nu, sup[3].nv );

    LePontosCtrSup ( 'sup4.ctr' );
    CalcSupBezier3d ( pc, nptu,nptv, du, dv,
                      sup[4].ptos, sup[4].nu, sup[4].nv );

    LePontosCtrSup ( 'sup5.ctr' );
    CalcSupBezier3d ( pc, nptu,nptv, du, dv,
                      sup[5].ptos, sup[5].nu, sup[5].nv );

    OutTextXY ( 10, 70, 'Ok.' );
    delay (500);
    cleardevice;
  end;


{--------------------------------------------------------}
{        Le os pontos de controle de todas as curvas     }
{--------------------------------------------------------}

procedure CalculaCurvas;
   var
     i: integer;
     nome: string;
   begin
      OutTextXY ( 10, 30, 'Calculando as curvas.');
      OutTextXY ( 10, 50, 'Aguarde um momento...' );
      LePontosctrCrv ( 'caminho.ctr', caminho );

      for i:= 1 to NCRV do
         begin
           str ( i:1, nome );
           nome:= 'cur' + nome + '.ctr';
           LePontosctrCrv ( nome, crv[i] );
         end;

      OutTextXY ( 10, 70, 'Ok.' );
      delay (500);
      cleardevice;
   end;

{--------------------------------------------------------}
{        Desenha todas as superficies da cena            }
{--------------------------------------------------------}
procedure DesenhaSuperficies;
   var
      i: integer;
   begin
      for i:= 1 to NSUP do
         with sup[i] do
             DesenhaSuperf3d (ptos,nu,nv);
   end;


{--------------------------------------------------------}
{        Desenha todas as curvas da cena                 }
{--------------------------------------------------------}

procedure DesenhaCurvas;
   var
     xt, yt,
     i, j: integer;
     saida: boolean;
begin
    {------ Colore o ceu ------}
    setfillstyle (SolidFill,COR_DO_CEU);
    bar (0,0,getmaxx,120);

    {------ Desenha o contorno  ------}
    SetColor (255);
    for i:= 1 to NCRV do
       with crv[i] do
           CurvaBezier3d ( np, p );
end;

{--------------------------------------------------------}

var
    t, xb, yb, zb: real;
    ntela: integer;
    extensao: string;

begin
    InicVideo3d (SVGA256);
    Janela3d (20, 350, 20.0, 15.0, 0, 0, getmaxx+1, getmaxy+1);
    SelecLuz3d ( 100,100,-100 );
    SelecLuzAmbiente3d (0.2);
    SelecCorPolig3d (MARROM);
    CalculaSuperficies;
    CalculaCurvas;

    t := 0.0;
    ntela:= 0;
    while t <= 1.001 do
        begin
            PtoBezier3d ( t, caminho.np, caminho.p, xb, yb, zb );
            Observador3d ( xb, yb, zb, 0, 0, 0 );
            InicZBuffer3d ( getmaxy+1, COR_DO_MAR, 'C:' );
            DesenhaCurvas;
            DesenhaSuperficies;
            if EM_DISCO then
                begin
                    str (ntela, extensao);
                    GravaPCX (NOMEARQ + extensao,0,0,getmaxx,getmaxy);
                    ntela:= ntela + 1;
                end;
            t := t + dt;
        end;

    readln;
    Termina3d;
end.
