{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include SUPERF3D.PAS                     }
{            Modulo Calculo de Superficies                    }
{                                                             }
{                                                             }
{    Versao 2.7 ( Agosto de 1993 )                            }
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
{    Autores:  Jose' Antonio Borges                           }
{              Mauricio Bomfim                                }
{                                                             }
{-------------------------------------------------------------}

var
    TamJ: integer;  { Numero de colunas da matriz de pontos calculados }


{-------------------------------------------------------------}
{          Gera um indice de acesso ao vetor Sup              }
{-------------------------------------------------------------}

function ind ( i,j:integer ) : integer;
   begin
      ind:= i * TamJ + j;
   end;

{-------------------------------------------------------------}
{              Armazena um ponto no vetor Sup                 }
{-------------------------------------------------------------}

procedure SaiPonto ( var sup; x,y,z,cor: real; iu,iv:integer );
    begin
       MALHA(sup)[ind(iu,iv)].x:= x;
       MALHA(sup)[ind(iu,iv)].y:= y;
       MALHA(sup)[ind(iu,iv)].z:= z;
       MALHA(sup)[ind(iu,iv)].cor:= cor;
    end;

{-------------------------------------------------------------}
{                calcula funcao de bernstein                  }
{        bernstein := comb (n,i) * t**i * (1-t)**(n-i)        }
{-------------------------------------------------------------}

function bernstein (i, n: integer; t: real): real;
var
    v: integer;
    comb, fat,
    tt, pot, pot1: real;

begin
    comb := 1;
    for v := n downto i+1 do
        comb := comb * v;

    fat := 1;
    for v := 2 to n-i do
        fat := fat * v;

    comb := comb / fat;

    pot := 1.0;
    for v := 1 to i do
        pot := pot * t;

    tt := 1-t;
    pot1 := 1.0;
    for v := 1 to n-i do
        pot1 := pot1 * tt;

    bernstein := comb * pot * pot1;
end;


{-------------------------------------------------------------}
{          Calcula um ponto de uma curva de Bezier            }
{-------------------------------------------------------------}

procedure PtoBezier3d ( t:real; np:integer; var p;
                        var xb,yb,zb:real );

    var
       j : integer;
       bt: real;

    begin
        xb:= 0; yb:= 0; zb:= 0;
        for j:= 0 to np-1 do
           begin
             bt:= bernstein (j,np-1,t);
             xb:= xb + PCONTR(p)[j].x * bt;
             yb:= yb + PCONTR(p)[j].y * bt;
             zb:= zb + PCONTR(p)[j].z * bt;
           end;
    end;


{-------------------------------------------------------------}
{                 Desenha uma curva de Bezier                 }
{-------------------------------------------------------------}

procedure CurvaBezier3d ( np:integer; var p );
   var
      t, xb,yb,zb: real;

   begin
      t:= 0.0;
      while t <= 1.001 do
         begin
            PtoBezier3d ( t, np, p, xb, yb, zb );
            if t = 0.0 then
               Move3d ( xb,yb,zb )
            else
               Risca3d ( xb,yb,zb );
            t:= t + 0.05;
         end;
   end;

{-------------------------------------------------------------}
{          Calcula um ponto de uma curva B-Spline             }
{-------------------------------------------------------------}

procedure PtoBSpline3d ( t:real; i:integer; var p;
                         var xb,yb,zb:real );
   var
      bm0, bm1, bm2, bm3: real;

   begin
      bm0:= t*t*t / 6;
      bm1:= ( 1 + 3*t + 3*t*t - 3*t*t*t ) / 6;
      bm2:= ( 4 - 6*t*t + 3*t*t*t ) / 6;
      bm3:= ( 1 - 3*t + 3*t*t - t*t*t ) / 6;
      xb:=  PCONTR(p)[i-3].x * bm3 +
            PCONTR(p)[i-2].x * bm2 +
            PCONTR(p)[i-1].x * bm1 +
            PCONTR(p)[i-0].x * bm0;
      yb:=  PCONTR(p)[i-3].y * bm3 +
            PCONTR(p)[i-2].y * bm2 +
            PCONTR(p)[i-1].y * bm1 +
            PCONTR(p)[i-0].y * bm0;
      zb:=  PCONTR(p)[i-3].z * bm3 +
            PCONTR(p)[i-2].z * bm2 +
            PCONTR(p)[i-1].z * bm1 +
            PCONTR(p)[i-0].z * bm0;
   end;

{-------------------------------------------------------------}
{                Desenha uma curva B-Spline                   }
{-------------------------------------------------------------}

procedure CurvaBSpline3d ( np:integer; var p );

   const
        dt = 0.05;
   var
        i: integer;
        t, xb,yb,zb: real;
        primeiro: boolean;

   begin
       primeiro:= true;
       for i:= 3 to np-1 do
           begin
               t:= 0.0;
               while t<=1.001 do
                   begin
                       PtoBSpline3d ( t,i,p,xb,yb,zb );
                       if primeiro then
                           begin
                              primeiro:= false;
                              Move3d (xb,yb,zb);
                           end
                       else
                           Risca3d (xb,yb,zb);

                       t:= t + dt;
                   end;
           end;
   end;


{-------------------------------------------------------------}
{                     Curva Hermitiana                        }
{-------------------------------------------------------------}

procedure CalcDerivadas ( i,np: integer; var p; var d1, d2: COORDPTO );
   begin
     if i=0 then
        begin
           d1.x:= PCONTR(p)[1].x - PCONTR(p)[0].x;
           d1.y:= PCONTR(p)[1].y - PCONTR(p)[0].y;
           d1.z:= PCONTR(p)[1].z - PCONTR(p)[0].z;
           d2.x:= (PCONTR(p)[i+2].x - PCONTR(p)[i].x)/2;
           d2.y:= (PCONTR(p)[i+2].y - PCONTR(p)[i].y)/2;
           d2.z:= (PCONTR(p)[i+2].z - PCONTR(p)[i].z)/2;
        end
     else
        if i=np-2 then
           begin
              d1.x:= (PCONTR(p)[i+1].x - PCONTR(p)[i-1].x)/2;
              d1.y:= (PCONTR(p)[i+1].y - PCONTR(p)[i-1].y)/2;
              d1.z:= (PCONTR(p)[i+1].z - PCONTR(p)[i-1].z)/2;
              d2.x:= PCONTR(p)[np-1].x - PCONTR(p)[np-2].x;
              d2.y:= PCONTR(p)[np-1].y - PCONTR(p)[np-2].y;
              d2.z:= PCONTR(p)[np-1].z - PCONTR(p)[np-2].z;
           end
        else
           begin
              d1.x:= (PCONTR(p)[i+1].x - PCONTR(p)[i-1].x)/2;
              d1.y:= (PCONTR(p)[i+1].y - PCONTR(p)[i-1].y)/2;
              d1.z:= (PCONTR(p)[i+1].z - PCONTR(p)[i-1].z)/2;
              d2.x:= (PCONTR(p)[i+2].x - PCONTR(p)[i].x)/2;
              d2.y:= (PCONTR(p)[i+2].y - PCONTR(p)[i].y)/2;
              d2.z:= (PCONTR(p)[i+2].z - PCONTR(p)[i].z)/2;
           end;
   end;

{-------------------------------------------------------------}

procedure Coeficientes ( i:integer; var p; d1,d2:COORDPTO;
                         var a,b,c,d: COORDPTO );
   begin
     { Coeficientes da Funcao X(t). }
     a.x := PCONTR(p)[i].x;
     b.x := d1.x;
     c.x := 3*(PCONTR(p)[i+1].x-PCONTR(p)[i].x) - 2*d1.x - d2.x;
     d.x := 2*(PCONTR(p)[i].x-PCONTR(p)[i+1].x) + d1.x + d2.x;

     { Coeficientes da Funcao Y(t). }
     a.y := PCONTR(p)[i].y;
     b.y := d1.y;
     c.y := 3*(PCONTR(p)[i+1].y-PCONTR(p)[i].y) - 2*d1.y - d2.y;
     d.y := 2*(PCONTR(p)[i].y-PCONTR(p)[i+1].y) + d1.y + d2.y;

     { Coeficientes da Funcao Z(t). }
     a.z := PCONTR(p)[i].z;
     b.z := d1.z;
     c.z := 3*(PCONTR(p)[i+1].z-PCONTR(p)[i].z) - 2*d1.z - d2.z;
     d.z := 2*(PCONTR(p)[i].z-PCONTR(p)[i+1].z) + d1.z + d2.z;
   end;

{-------------------------------------------------------------}

function poliX ( t: real; a,b,c,d:COORDPTO ) : real;
   begin
     poliX := a.x + t*(b.x + t*(c.x + t*d.x) );
   end;

function poliY ( t: real; a,b,c,d:COORDPTO ) : real;
   begin
     poliY :=  a.y + t*(b.y + t*(c.y + t*d.y) ) ;
   end;

function poliZ ( t: real; a,b,c,d:COORDPTO ) : real;
   begin
     poliZ :=  a.z + t*(b.z + t*(c.z + t*d.z) ) ;
   end;

{-------------------------------------------------------------}
{         Calcula um ponto de uma curva Hermitiana            }
{-------------------------------------------------------------}

procedure PtoHermit3d ( t:real; i,np:integer; var p;
                        var xb,yb,zb:real );

var
    d1,d2,
    a,b,c,d: COORDPTO;

begin
    CalcDerivadas ( i,np,p,d1,d2 );
    Coeficientes ( i,p,d1,d2,a,b,c,d );
    xb:= poliX ( t,a,b,c,d );
    yb:= poliY ( t,a,b,c,d );
    zb:= poliZ ( t,a,b,c,d );
end;


{-------------------------------------------------------------}
{               Desenha uma curva Hermitiana                  }
{-------------------------------------------------------------}

procedure CurvaHermit3d ( np:integer; var p );
   const
        dt = 0.05;
   var
     d1, d2,
     a,b,c,d: COORDPTO;
     i : integer;
     t,
     xt,yt,zt : real;

{-------------------------------------------------------------}

  begin
     for i:= 0 to np-2 do
         begin
             CalcDerivadas ( i,np,p,d1,d2 );
             Coeficientes ( i,p,d1,d2,a,b,c,d );
             t:= 0.0;
             while t <= 1.01 do
                 begin
                     xt:= poliX ( t,a,b,c,d );
                     yt:= poliY ( t,a,b,c,d );
                     zt:= poliZ ( t,a,b,c,d );
                     if (i=0) and (t=0.0)
                          then Move3d (xt,yt,zt)
                          else Risca3d (xt,yt,zt);
                     t:= t + dt;
                 end;
         end;
  end;

{-------------------------------------------------------------}
{           calcula pontos da superficie de Bezier            }
{-------------------------------------------------------------}
procedure CalcSupBezier3d ( var pc; nptu,nptv:integer;
                            du, dv: real;
                            var sup; var iu,iv:integer );

var
    u, v,
    bim, bnj, b,
    cor, x, y, z,
    nx, ny, nz : real;
    i, j: integer;

    {---------------------------------------------------------}
    {       Gera uma posicao da matriz de pontos              }
    {       de controle.                                      }
    {---------------------------------------------------------}
    function PCtrl ( var P; i,j:integer ) : PCOORDPTO;
       begin
          PCtrl:= addr (PCONTR(P)[i * nptv + j]);
       end;

    {---------------------------------------------------------}

    procedure CalculaNormal ( var nx, ny, nz: real );
       var
         i, j : integer;
         dxu, dyu, dzu, dxv, dyv, dzv : real;
         b, bim, bnj: real;

       begin

          { Derivada parcial com relacao a u }
          dxu := 0.0;
          dyu := 0.0;
          dzu := 0.0;
          for i := 0 to nptu-2 do
              begin
                bim := bernstein (i, nptu-2, u);
                for j := 0 to nptv-1 do
                    begin
                      bnj := bernstein (j, nptv-1, v);
                      b := bim * bnj;
                      dxu := dxu + ( PCtrl(pc,i+1,j)^.x -
                                     PCtrl(pc,i,j)^.x ) * b;
                      dyu := dyu + ( PCtrl(pc,i+1,j)^.y -
                                     PCtrl(pc,i,j)^.y ) * b;
                      dzu := dzu + ( PCtrl(pc,i+1,j)^.z -
                                     PCtrl(pc,i,j)^.z ) * b;
                    end;
              end;
          dxu:= dxu*(nptu-1);
          dyu:= dyu*(nptu-1);
          dzu:= dzu*(nptu-1);

          { Derivada parcial com relacao a v }
          dxv := 0.0;
          dyv := 0.0;
          dzv := 0.0;
          for i := 0 to nptu-1 do
              begin
                bim := bernstein (i, nptu-1, u);
                for j := 0 to nptv-2 do
                    begin
                      bnj := bernstein (j, nptv-2, v);
                      b := bim * bnj;
                      dxv := dxv + ( PCtrl(pc,i,j+1)^.x -
                                     PCtrl(pc,i,j)^.x ) * b;
                      dyv := dyv + ( PCtrl(pc,i,j+1)^.y -
                                     PCtrl(pc,i,j)^.y ) * b;
                      dzv := dzv + ( PCtrl(pc,i,j+1)^.z -
                                     PCtrl(pc,i,j)^.z ) * b;
                    end;
              end;
          dxv:= dxv*(nptv-1);
          dyv:= dyv*(nptv-1);
          dzv:= dzv*(nptv-1);

          { Calculo da normal atraves do produto vetorial }
          nx := dyu*dzv - dzu*dyv;
          ny := dzu*dxv - dxu*dzv;
          nz := dxu*dyv - dyu*dxv;
          Normaliza (nx,ny,nz);
       end;

{-------------------------------------------------------------}

{ Corpo da Procedure CalcSupBezier3d }


begin
    TamJ:= round(1/dv) + 1;

    u := 0.0;               { ref. livro do Farin }
    iu:= 0;
    while u < 1.0001 do
        begin

            v := 0.0;
            iv:= 0;
            while v < 1.0001 do
                begin
                    x := 0.0;
                    y := 0.0;
                    z := 0.0;

                    for i := 0 to nptu-1 do
                        begin
                            bim := bernstein (i, nptu-1, u);
                            for j := 0 to nptv-1 do
                                begin
                                    bnj := bernstein (j, nptv-1, v);
                                    b := bim * bnj;
                                    x := x + PCtrl(pc,i,j)^.x * b;
                                    y := y + PCtrl(pc,i,j)^.y * b;
                                    z := z + PCtrl(pc,i,j)^.z * b;
                                end;
                        end;

                    CalculaNormal ( nx, ny, nz );
                    CalcCor3d ( nx, ny, nz, cor );

                    SaiPonto ( sup, x, y, z, cor, iu,iv );

                    v := v + dv;
                    Inc(iv);
                end;


            u := u + du;
            Inc(iu);
        end;

end;


{-------------------------------------------------------------}
{          Calcula pontos da superficie BSpline               }
{-------------------------------------------------------------}

procedure CalcSupBSpline3d ( var pc; nptu,nptv:integer;
                             du,dv:real;
                             var sup; var iu,iv:integer );

var
    nu, nv: integer;     { Numero de pontos a calcular por retalho }

    {---------------------------------------------------------}
    {        Gera uma posicao da matriz de pontos             }
    {        de controle.                                     }
    {---------------------------------------------------------}
    function PCtrl ( var P; i,j:integer ) : PCOORDPTO;
       begin
          PCtrl:= addr(PCONTR(P)[i * nptv + j]);
       end;


{-------------------------------------------------------------}

function b ( i:integer; t:real ) : real;
   begin
     case i of
         0: b:= t*t*t / 6;
        -1: b:= ( 1 + 3*t + 3*t*t - 3*t*t*t ) / 6;
        -2: b:= ( 4 - 6*t*t + 3*t*t*t ) / 6;
        -3: b:= ( 1 - 3*t + 3*t*t - t*t*t ) / 6;
     end;
   end;

{-------------------------------------------------------------}

function blinha ( i:integer; t:real ) : real;
   begin
     case i of
         0: blinha:= t*t / 2;
        -1: blinha:= ( 3 + 6*t - 9*t*t ) / 6;
        -2: blinha:= ( - 12*t + 9*t*t ) / 6;
        -3: blinha:= ( - 3 + 6*t - 3*t*t ) / 6;
     end;
   end;

{-------------------------------------------------------------}

procedure CalculaPonto ( i,j: integer; u,v: real; var x,y,z: real );

    var
       r, s: integer;
       base, bru: real;
       Paux: PCOORDPTO;

    begin
       x:=0.0; y:=0.0; z:=0.0;
       for r:= -3 to 0 do
          begin
             bru:= b(r,u);
             for s:= -3 to 0 do
                 begin
                    base:= bru * b(s,v);
                    Paux:= PCtrl(pc,i+r,j+s);
                    x := x + Paux^.x * base;
                    y := y + Paux^.y * base;
                    z := z + Paux^.z * base;
                 end;
          end;

    end;

{-------------------------------------------------------------}

procedure CalculaNormal ( i,j: integer; u,v: real; var nx,ny,nz: real );

   var
      r, s : integer;
      base, bru,
      dxu, dyu, dzu,
      dxv, dyv, dzv: real;
      Paux: PCOORDPTO;

   begin

      { Derivada Parcial com relacao a u }
      dxu:= 0; dyu:= 0; dzu:= 0;
      for r:= -3 to 0 do
         begin
            bru:= blinha(r,u);
            for s:= -3 to 0 do
                begin
                   base:= bru * b(s,v);
                   Paux:= PCtrl(pc,i+r,j+s);
                   dxu := dxu + Paux^.x * base;
                   dyu := dyu + Paux^.y * base;
                   dzu := dzu + Paux^.z * base;
                end;
         end;

      { Derivada Parcial com relacao a v }
      dxv:= 0; dyv:= 0; dzv:= 0;
      for r:= -3 to 0 do
         begin
            bru:= b(r,u);
            for s:= -3 to 0 do
                begin
                   base:= bru * blinha(s,v);
                   Paux:= PCtrl(pc,i+r,j+s);
                   dxv := dxv + Paux^.x * base;
                   dyv := dyv + Paux^.y * base;
                   dzv := dzv + Paux^.z * base;
                end;
         end;

      { Calculo da normal atraves do produto vetorial }
      nx := dyu*dzv - dzu*dyv;
      ny := dzu*dxv - dxu*dzv;
      nz := dxu*dyv - dyu*dxv;

   end;

{-------------------------------------------------------------}

procedure GeraRetalho (i,j:integer);
   var
      u, v,
      x,y,z,cor,
      nx,ny,nz: real;

   {----------------------------------------------------------}

   procedure InicU;
     begin
       u:= 0.0;
       if i=3 then
          iu:= 0
       else
          iu:= (i-3)*(nu-1);
     end;

   procedure InicV;
     begin
        v:= 0.0;
        if j=3 then
           iv:= 0
        else
           iv:= (j-3)*(nv-1);
     end;

   {----------------------------------------------------------}

   begin
     InicU;
     while u<=1.001 do
         begin
             InicV;
             while v<=1.001 do
                 begin
                     CalculaPonto ( i,j, u,v, x,y,z );
                     CalculaNormal ( i,j, u,v, nx,ny,nz );
                     CalcCor3d ( nx, ny, nz, cor );

                     SaiPonto ( sup, x,y,z,cor, iu,iv );
                     v:= v + dv;
                     Inc(iv);
                 end;
             u:= u + du;
             Inc(iu);
         end;
    end;

{-------------------------------------------------------------}

   { Procedure CalcSupBSpline3d }

   var
     i,j: integer;

   begin

     if ( nptu<4 ) or ( nptv<4 ) then
         begin
            iu:= 0; iv:= 0;
         end
     else
         begin
            nu:= round(1.0 / du) + 1;
            nv:= round(1.0 / dv) + 1;
            TamJ := (nv-1) * (nptv-3) + 1;

            for i:= 3 to nptu-1 do
                for j:= 3 to nptv-1 do
                    GeraRetalho (i,j);
         end;

   end;

{-------------------------------------------------------------}
{                                                             }
{   SUPERFICIE HERMITIANA:                                    }
{   Passa por todos os seus pontos de controle                }
{                                                             }
{   Para cada retalho da malha de pontos calculam-se as       }
{   derivadas parciais e mistas dos quatro vertices e         }
{   calcula-se a superficie hermitiana nesse retalho.         }
{   Os retalhos sao guardados na malha unica Sup[i,j].        }
{                                                             }
{   Autor: Joao Paulo - Set/92.                               }
{                                                             }
{-------------------------------------------------------------}


 Procedure CalcSupHermit3d ( var Pc; Nptu,Nptv :INTEGER;
                             Du,Dv :REAL;
                             Var Sup; Var iu,iv :INTEGER );
 TYPE
   VERTS3D   = Array[0..1,0..1] of COORDPTO;
   MAT4X4    = Array[0..3,0..3] of COORDPTO;



 VAR
   I,J          : INTEGER;
   Pu, Pv, Puv  : VERTS3D;




    {---------------------------------------------------------}
    {       Gera uma posicao da matriz de pontos              }
    {       de controle.                                      }
    {---------------------------------------------------------}
    function PCtrl ( var P; i,j:integer ) : PCOORDPTO;
       begin
         PCtrl:= addr(PCONTR(P)[i * nptv + j]);
       end;



{-------------------------------------------------------------}
{                       Base Hermitiana                       }
{-------------------------------------------------------------}
  Function Herm(i : INTEGER; t: REAL) :REAL;

  Var
    t_13, t_12, t3, t2    : real;

  Begin
     t_12 := (1-t)*(1-t);
     t_13 := (1-t)*(1-t)*(1-t);
     t2   := t*t;
     t3   := t*t*t;
     Case i of
        0: Herm := t_13 + 3*t*t_12;
        1: Herm := t * t_12;
        2: Herm := -t2*(1-t);
        3: Herm := 3*t2*(1-t) + t3;
     end;
  End;


{-------------------------------------------------------------}
{         Calcula vetor normal a U e V e guarda em W          }
{-------------------------------------------------------------}
  Procedure Normal( U,V : COORDPTO; Var W :COORDPTO);

  Begin
    W.x := U.y * V.z - V.y * U.z;
    W.y := V.x * U.z - U.x * V.z;
    W.z := U.x * V.y - V.x * U.y;
    Normaliza ( W.x, W.y, W.z );
  End;

{-------------------------------------------------------------}
{     CalDerParMis                                            }
{                                                             }
{     Dado o ponto ptos[i,j] calcula suas derivadas parciais  }
{     Pu,Pv e derivada mista Puv.                             }
{                                                             }
{     1. Calculo de Pu (Pv):                                  }
{      - Se na direcao u(v) ele for interior, ie, existir     }
{        Ptos[i-1,j] e Ptos[i+1,j] (Ptos[i,j-1] e             }
{        Ptos[i,j+1]), entao:                                 }
{        Pu (Pv) = Diferenca entre o pto anterior e           }
{        posterior na dircao u(v).                            }
{                                                             }
{      - Se na direcao u(v) ele nao for interior, entao:      }
{        Pu(Pv) = Diferenca dele mesmo com o mais proximo     }
{        na direcao u(v).                                     }
{                                                             }
{     2. Calculo de Puv:                                      }
{        Puv = Normal aos dois vetores Pu e Pv, ou seja       }
{        Puv e' o produto vetorial Pu X Pv  normalizado.      }
{                                                             }
{-------------------------------------------------------------}

 Procedure CalDerParMis( I,J :Integer; Var Pu,Pv,Puv  :COORDPTO);
 var
    Paux1, Paux2: PCOORDPTO;

 Begin
   If (I = 0) then
     Begin
       Paux1:= PCtrl (Pc,i+1,j);
       Paux2:= PCtrl (Pc,i,j);
       Pu.x := Paux1^.x - Paux2^.x;
       Pu.y := Paux1^.y - Paux2^.y;
       Pu.z := Paux1^.z - Paux2^.z;
     End
   Else
     If (I = Nptu-1) then
       Begin
         Paux1:= PCtrl (Pc,i,j);
         Paux2:= PCtrl (Pc,i-1,j);
         Pu.x := Paux1^.x - Paux2^.x;
         Pu.y := Paux1^.y - Paux2^.y;
         Pu.z := Paux1^.z - Paux2^.z;
       End
         Else
            Begin
               Paux1:= PCtrl (Pc,i+1,j);
               Paux2:= PCtrl (Pc,i-1,j);
               Pu.x := (Paux1^.x - Paux2^.x)/2;
               Pu.y := (Paux1^.y - Paux2^.y)/2;
               Pu.z := (Paux1^.z - Paux2^.z)/2;
            End;
   If (J = 0) then
     Begin
       Paux1:= PCtrl (Pc,i,j+1);
       Paux2:= PCtrl (Pc,i,j);
       Pv.x := Paux1^.x - Paux2^.x;
       Pv.y := Paux1^.y - Paux2^.y;
       Pv.z := Paux1^.z - Paux2^.z;
     End
   Else
     If (J = Nptv-1) then
       Begin
          Paux1:= PCtrl (Pc,i,j);
          Paux2:= PCtrl (Pc,i,j-1);
          Pv.x := Paux1^.x - Paux2^.x;
          Pv.y := Paux1^.y - Paux2^.y;
          Pv.z := Paux1^.z - Paux2^.z;
       End
     Else
       Begin
          Paux1:= PCtrl (Pc,i,j+1);
          Paux2:= PCtrl (Pc,i,j-1);
          Pv.x := (Paux1^.x - Paux2^.x)/2;
          Pv.y := (Paux1^.y - Paux2^.y)/2;
          Pv.z := (Paux1^.z - Paux2^.z)/2;
       End;
     { Faz Puv = Pu X Pv e normaliza Puv }
     Normal(Pu,Pv,Puv)
End;


{-------------------------------------------------------------}
{     CalDerMalha                                             }
{                                                             }
{     Dada  malha [i,j] calcula derivadas parciais pu,pv e    }
{     a derivada puv para cada pto dos vertices da malha:     }
{                  Ptos[i,j]      Ptos[i,j+1]                 }
{                  Ptos[i+1,j]    Ptos[i+1,j+1]               }
{     tendo o cuidado de nao calcular duas vezes derivadas    }
{     de ptos ja calculadas na malha anterior (adjacente      }
{     pela esquerda em relacao a j).                          }
{                                                             }
{     Atencao:                                                }
{     Pu[0,0], Pv[0,0] e Puv[0,0] correspondem a Ptos[i,j]    }
{     Pu[1,1], Pv[1,1] e Puv[1,1] correspondem a Ptos[i+1,j+1]}
{-------------------------------------------------------------}

 Procedure CalDerMalha( I,J :INTEGER; Var Pu,Pv,Puv: VERTS3D);

 Begin
   if (J = 0) then
     Begin
         {   Calcula todas as derivadas da malha [i,j]   }
         CalDerParMis(I,J,Pu[0,0],Pv[0,0],Puv[0,0]);
         CalDerParMis(I+1,J,Pu[1,0],Pv[1,0],Puv[1,0]);
         CalDerParMis(I,J+1,Pu[0,1],Pv[0,1],Puv[0,1]);
         CalDerParMis(I+1,J+1,Pu[1,1],Pv[1,1],Puv[1,1]);
     End
   else
     Begin
        { as derivadas Pu[0,1] e Pu[1,1] da malha anterior sao Pu[0,0] e
              Pu[1,0] da malha atual. O mesmo ocorre p/ Pv e Puv  }
        Pv[0,0] := Pv[0,1];
        Pu[0,0] := Pu[0,1];
        Puv[0,0]:= Puv[0,1];
        Pv[1,0] := Pv[1,1];
        Pu[1,0] := Pu[1,1];
        Puv[1,0]:= Puv[1,1];

        {  calcula Pu[0,1],Pu[1,1] (Pv e Puv)  ainda nao calculados  }
        CalDerParMis(I,J+1,Pu[0,1],Pv[0,1],Puv[0,1]);
        CalDerParMis(I+1,J+1,Pu[1,1],Pv[1,1],Puv[1,1]);
     end

 End;

{-------------------------------------------------------------}
{      Determina matriz Math[i,j] cujos coeficientes sao      }
{      os pontos de vertice da malha e suas derivadas         }
{      parciais e mistas.                                     }
{-------------------------------------------------------------}
 Procedure MatMalha(I,J :INTEGER; Var Mat :MAT4x4);

 Begin

   { matriz em x: }
   Mat[0,0].x := PCtrl(Pc,i,j)^.x;   Mat[1,0].x := Pu[0,0].x;
   Mat[0,1].x := Pv[0,0].x;          Mat[1,1].x := Puv[0,0].x;
   Mat[0,2].x := Pv[0,1].x;          Mat[1,2].x := Puv[0,1].x;
   Mat[0,3].x := PCtrl(Pc,i,j+1)^.x; Mat[1,3].x := Pu[0,1].x;

   Mat[2,0].x := Pu[1,0].x;          Mat[3,0].x := PCtrl(Pc,i+1,j)^.x;
   Mat[2,1].x := Puv[1,0].x;         Mat[3,1].x := Pv[1,0].x;
   Mat[2,2].x := Puv[1,1].x;         Mat[3,2].x := Pv[1,1].x;
   Mat[2,3].x := Pu[1,1].x;          Mat[3,3].x := PCtrl(Pc,i+1,j+1)^.x;

   { matriz em y: }
   Mat[0,0].y := PCtrl(Pc,i,j)^.y;   Mat[1,0].y := Pu[0,0].y;
   Mat[0,1].y := Pv[0,0].y;          Mat[1,1].y := Puv[0,0].y;
   Mat[0,2].y := Pv[0,1].y;          Mat[1,2].y := Puv[0,1].y;
   Mat[0,3].y := PCtrl(Pc,i,j+1)^.y; Mat[1,3].y := Pu[0,1].y;

   Mat[2,0].y := Pu[1,0].y;          Mat[3,0].y := PCtrl(Pc,i+1,j)^.y;
   Mat[2,1].y := Puv[1,0].y;         Mat[3,1].y := Pv[1,0].y;
   Mat[2,2].y := Puv[1,1].y;         Mat[3,2].y := Pv[1,1].y;
   Mat[2,3].y := Pu[1,1].y;          Mat[3,3].y := PCtrl(Pc,i+1,j+1)^.y;

   { matriz em z: }
   Mat[0,0].z := PCtrl(Pc,i,j)^.z;   Mat[1,0].z := Pu[0,0].z;
   Mat[0,1].z := Pv[0,0].z;          Mat[1,1].z := Puv[0,0].z;
   Mat[0,2].z := Pv[0,1].z;          Mat[1,2].z := Puv[0,1].z;
   Mat[0,3].z := PCtrl(Pc,i,j+1)^.z; Mat[1,3].z := Pu[0,1].z;

   Mat[2,0].z := Pu[1,0].z;          Mat[3,0].z := PCtrl(Pc,i+1,j)^.z;
   Mat[2,1].z := Puv[1,0].z;         Mat[3,1].z := Pv[1,0].z;
   Mat[2,2].z := Puv[1,1].z;         Mat[3,2].z := Pv[1,1].z;
   Mat[2,3].z := Pu[1,1].z;          Mat[3,3].z := PCtrl(Pc,i+1,j+1)^.z;

End;

{-------------------------------------------------------------}
{                 Calcula Malha [Indi,Indj]                   }
{-------------------------------------------------------------}

 Procedure  CalculaMalha(Indi,Indj       :INTEGER;
                         Var iuaux,ivaux :INTEGER);

 Var

   Iu,Iv,
   i,j                : INTEGER;
   MatH               : MAT4x4;
   u,v,
   Hermiu,
   Hermitiana,
   Somax,Somay,Somaz  : REAL;

   Begin
      CalDerMalha(IndI,IndJ, Pu,Pv,Puv);
      MatMalha(Indi,IndJ, MatH);
      u := 0.0;
      iu:= iuaux;
      Repeat
         v := 0.0;
         iv:= ivaux;
         Repeat
            Somax := 0.0;
            Somay := 0.0;
            Somaz := 0.0;
            For i:=0 to 3 do
              Begin
                 Hermiu := Herm(i,u);
                 for j:=0 to 3 do
                    begin
                      Hermitiana := Hermiu * Herm(j,v);
                      Somax := Somax + MatH[i,j].x * Hermitiana;
                      Somay := Somay + MatH[i,j].y * Hermitiana;
                      Somaz := Somaz + MatH[i,j].z * Hermitiana;
                    end;
              End;
            With MALHA(Sup)[ind(iu,iv)] do
             Begin
                x := Somax;
                y := Somay;
                z := Somaz;
             End;
           v:= v + dv;
           iv := iv + 1;
       Until (v > 1.001);
       u := u + du;
       iu:= iu + 1;
    Until (u > 1.001);

    If (Indj < Nptv-2) then ivaux := iv-1
     else
       if (Indi<Nptu-2) then
         begin
           iuaux := iu-1;
           ivaux := 0;
         end
           else begin
                   iuaux := iu;
                   ivaux := iv;
                end;
 End;


{-------------------------------------------------------------}
{          Calcula Cor de cada pto da malha Sup[i,j]          }
{-------------------------------------------------------------}

 Procedure CalculaCor;

 Var
   maxu,maxv,
   i,j         : INTEGER;
   du,dv,duv   : COORDPTO;

 Begin

  maxu := iu-1;
  maxv := iv-1;
  For i:=0 to maxu do
     For j:=0 to maxv do
        Begin
            If (i=0) then
               begin
                  du.x := MALHA(Sup)[ind(i+1,j)].x - MALHA(Sup)[ind(i,j)].x;
                  du.y := MALHA(Sup)[ind(i+1,j)].y - MALHA(Sup)[ind(i,j)].y;
	          du.z := MALHA(Sup)[ind(i+1,j)].z - MALHA(Sup)[ind(i,j)].z
               end
                else
                  if (i=maxu) then
                    begin
                      du.x := MALHA(Sup)[ind(i,j)].x - MALHA(Sup)[ind(i-1,j)].x;
                      du.y := MALHA(Sup)[ind(i,j)].y - MALHA(Sup)[ind(i-1,j)].y;
	              du.z := MALHA(Sup)[ind(i,j)].z - MALHA(Sup)[ind(i-1,j)].z;
                    end
                      else
                        begin
                           du.x := ( MALHA(Sup)[ind(i+1,j)].x -
                                     MALHA(Sup)[ind(i-1,j)].x ) / 2;
                           du.y := ( MALHA(Sup)[ind(i+1,j)].y -
                                     MALHA(Sup)[ind(i-1,j)].y ) / 2;
	                   du.z := ( MALHA(Sup)[ind(i+1,j)].z -
                                     MALHA(Sup)[ind(i-1,j)].z ) / 2;
                        end;
            If (j=0) then
               begin
                  dv.x := MALHA(Sup)[ind(i,j+1)].x - MALHA(Sup)[ind(i,j)].x;
                  dv.y := MALHA(Sup)[ind(i,j+1)].y - MALHA(Sup)[ind(i,j)].y;
	          dv.z := MALHA(Sup)[ind(i,j+1)].z - MALHA(Sup)[ind(i,j)].z
               end
                else
                  if (j = maxv) then
                    begin
                      dv.x := MALHA(Sup)[ind(i,j)].x - MALHA(Sup)[ind(i,j-1)].x;
                      dv.y := MALHA(Sup)[ind(i,j)].y - MALHA(Sup)[ind(i,j-1)].y;
	              dv.z := MALHA(Sup)[ind(i,j)].z - MALHA(Sup)[ind(i,j-1)].z;
                    end
                      else
                        begin
                           dv.x := ( MALHA(Sup)[ind(i,j+1)].x -
                                     MALHA(Sup)[ind(i,j-1)].x ) / 2;
                           dv.y := ( MALHA(Sup)[ind(i,j+1)].y -
                                     MALHA(Sup)[ind(i,j-1)].y ) / 2;
	                   dv.z := ( MALHA(Sup)[ind(i,j+1)].z -
                                     MALHA(Sup)[ind(i,j-1)].z ) / 2;
                        end;

            Normal(du,dv,duv);
            CalcCor3d(duv.x,duv.y,duv.z,MALHA(Sup)[ind(i,j)].cor);
        End;
   End;



{-------------------------------------------------------------}
{          *** Corpo da procedure CalcSupHermit3d ***         }
{-------------------------------------------------------------}

 Begin
    iu:= 0;
    iv:= 0;
    TamJ:= round(1.0/dv) * (nptv-1) + 1;
    For I:=0 to (Nptu-2) do
      For J:=0 to (Nptv-2) do
         CalculaMalha(I,J,iu,iv);
    CalculaCor;
 End;


{-------------------------------------------------------------}
{                                                             }
{                 Desenha uma superficie                      }
{                                                             }
{-------------------------------------------------------------}

procedure DesenhaSuperf3d ( var sup; nptu, nptv: integer );
var
  i, j: integer;
  vert: VERTICES;

begin
    TamJ:= nptv;

    for i:= 0 to nptu-2 do
        for j:=0 to nptv-2 do
            begin
                vert[1]:= MALHA(sup)[ind(i,j)];
                vert[2]:= MALHA(sup)[ind(i,j+1)];
                vert[3]:= MALHA(sup)[ind(i+1,j+1)];
                vert[4]:= MALHA(sup)[ind(i+1,j)];
                PintaPolig3d ( 4, vert )
            end;
end;
