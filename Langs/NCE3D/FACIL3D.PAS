{-------------------------------------------------------------}
{                                                             }
{    FACIL3D - Unit para mascarar o uso de vetores e          }
{              records na NCE3D.                              }
{                                                             }
{              Desenvolvida especialmente para alunos de      }
{              Computacao Grafica nas Artes II                }
{              da Escola de Belas Artes                       }
{                                                             }
{                                                             }
{    Versao 0.1 ( Junho de 1993 )                             }
{                                                             }
{                                                             }
{    Desenvolvido pelo:                                       }
{        Grupo de Computacao Grafica da DAU                   }
{        NCE/UFRJ                                             }
{        Caixa Postal 2324 - Rio de Janeiro - RJ              }
{        CEP 20001-970                                        }
{        Tel. (021) 598-3117                                  }
{        Fax. (021) 270-8554                                  }
{        E-mail: mauricio@nce.ufrj.br                         }
{                                                             }
{                                                             }
{    Autor: Mauricio Nunes da Costa Bomfim                    }
{                                                             }
{                                                             }
{-------------------------------------------------------------}

unit facil3d;

interface
uses nce3d;

procedure InicPtoCtrl (num: integer; xc,yc,zc: real);
procedure PtoBezier   (t: real; np: integer; var xb,yb,zb: real);
procedure CurvaBezier (np:integer);
procedure PtoBSpline (t: real; i: integer; var xb,yb,zb: real);
procedure CurvaBSpline (np: integer);
procedure PtoHermit (t: real; i,np: integer; var xb,yb,zb: real);
procedure CurvaHermit (np: integer);
procedure CalcSupBezier  (narq1: string; du,dv: real; narq2: string);
procedure CalcSupBSpline (narq1: string; du,dv: real; narq2: string);
procedure CalcSupHermit  (narq1: string; du,dv: real; narq2: string);
procedure DesenhaSuperf  (narq: string);

{-------------------------------------------------------------}

implementation

var
    p: array [0..10] of COORDPTO;

    pc: array [0..100] of COORDPTO;
    nptu,nptv: integer;
    sup: array [0..2000] of PONTO;
    iu, iv: integer;

{-------------------------------------------------------------}
{                            Curvas                           }
{-------------------------------------------------------------}

procedure InicPtoCtrl (num: integer; xc,yc,zc: real);
begin
    with p[num-1] do
        begin
            x:= xc;
            y:= yc;
            z:= zc;
        end;
end;

{-------------------------------------------------------------}

procedure PtoBezier (t: real; np: integer; var xb,yb,zb: real);
begin
    PtoBezier3d (t, np, p, xb,yb,zb);
end;

{-------------------------------------------------------------}

procedure CurvaBezier (np:integer);
begin
    CurvaBezier3d (np, p);
end;

{-------------------------------------------------------------}

procedure PtoBSpline (t: real; i: integer; var xb,yb,zb: real);
begin
    PtoBSpline3d     (t, i, p, xb,yb,zb);
end;

{-------------------------------------------------------------}

procedure CurvaBSpline (np: integer);
begin
    CurvaBSpline3d (np,p);
end;

{-------------------------------------------------------------}

procedure PtoHermit (t: real; i,np: integer; var xb,yb,zb: real);
begin
    PtoHermit3d (t,i,np,p, xb,yb,zb);
end;

{-------------------------------------------------------------}

procedure CurvaHermit (np: integer);
begin
    CurvaHermit3d (np,p);
end;

{-------------------------------------------------------------}
{                       Superficies                           }
{-------------------------------------------------------------}


procedure LeArqControle (narq:string);
var
    arq: text;
    i,j: integer;
begin
    assign (arq,narq+'.ctr');
    reset (arq);
    readln (arq,nptu,nptv);
    for i:= 0 to nptu-1 do
        for j:= 0 to nptv-1 do
            with pc[i*nptv+j] do
                readln (arq, x,y,z);
    close (arq);
end;

{-------------------------------------------------------------}

procedure LeArqInterpolado (narq:string);
var
    arq: text;
    i,j: integer;
begin
    assign (arq,narq+'.sup');
    reset (arq);
    readln (arq,nptu,nptv);
    for i:= 0 to nptu-1 do
        for j:= 0 to nptv-1 do
            with sup[i*nptv+j] do
                readln (arq, x,y,z, cor);
    close (arq);
end;

{-------------------------------------------------------------}

procedure GravaArqInterpolado (narq:string);
var
    arq: text;
    i,j: integer;
begin
    assign (arq,narq+'.sup');
    rewrite (arq);
    writeln (arq, iu,' ',iv);
    for i:= 0 to iu-1 do
        for j:= 0 to iv-1 do
            with sup[i*iv+j] do
                writeln (arq, x:8:2,y:8:2,z:8:2, cor:8:2);
    close (arq);
end;

{-------------------------------------------------------------}

procedure CalcSupBezier  (narq1: string; du,dv: real; narq2: string);
begin
    LeArqControle (narq1);
    CalcSupBezier3d  (pc, nptu,nptv, du,dv, sup,iu,iv);
    GravaArqInterpolado (narq2);
end;

{-------------------------------------------------------------}

procedure CalcSupBSpline (narq1: string; du,dv: real; narq2: string);
begin
    LeArqControle (narq1);
    CalcSupBSpline3d (pc, nptu,nptv, du,dv, sup,iu,iv);
    GravaArqInterpolado (narq2);
end;

{-------------------------------------------------------------}

procedure CalcSupHermit  (narq1: string; du,dv: real; narq2: string);
begin
    LeArqControle (narq1);
    CalcSupHermit3d (pc, nptu,nptv, du,dv, sup,iu,iv);
    GravaArqInterpolado (narq2);
end;

{-------------------------------------------------------------}

procedure DesenhaSuperf  (narq: string);
begin
    LeArqInterpolado (narq);
    DesenhaSuperf3d  (sup, nptu,nptv);
end;

end.