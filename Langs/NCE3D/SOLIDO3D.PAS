{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include SOLIDO3D.PAS                     }
{            Modulo de Criacao de Solidos Canonicos           }
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

procedure InicVert ( var p:PONTO; x,y,z,cor:real );
    begin
      p.x:= x;
      p.y:= y;
      p.z:= z;
      p.cor:= cor;
    end;

{-------------------------------------------------------------}
{                     Definicao de um cubo                    }
{-------------------------------------------------------------}

procedure Cubo3d ( h1,h2,h3: real );

type
    PCUBO = array [1..8] of real;

const
    px: PCUBO = ( 0, 1, 1, 0, 0, 1, 1, 0 );
    py: PCUBO = ( 1, 1, 0, 0, 1, 1, 0, 0 );
    pz: PCUBO = ( 1, 1, 1, 1, 0, 0, 0, 0 );

var
    v: array[1..4] of PONTO;
    cor,
    x0, y0, z0,
    difx, dify, difz,
    ntx, nty, ntz: real;

begin

    AplicaTransf3d ( 0.0,0.0,0.0, x0,y0,z0 );

    AplicaTransf3d ( 0,0,1, ntx,nty,ntz );
    difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
    normaliza (difx,dify,difz);
    if difx*x0 + dify*y0 + difz*z0 < 0 then
        begin
            CalcCor3d ( 0,0,1, cor );
            inicvert (v[1],px[1]*h1,py[1]*h2,pz[1]*h3,cor);
            inicvert (v[2],px[2]*h1,py[2]*h2,pz[2]*h3,cor);
            inicvert (v[3],px[3]*h1,py[3]*h2,pz[3]*h3,cor);
            inicvert (v[4],px[4]*h1,py[4]*h2,pz[4]*h3,cor);
            PintaPolig3d (4,v);
        end;

    AplicaTransf3d ( 0,1,0, ntx,nty,ntz );
    difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
    normaliza (difx,dify,difz);
    if difx*x0 + dify*y0 + difz*z0 < 0 then
        begin
            CalcCor3d ( 0,1,0, cor );
            inicvert (v[1],px[1]*h1,py[1]*h2,pz[1]*h3,cor);
            inicvert (v[2],px[2]*h1,py[2]*h2,pz[2]*h3,cor);
            inicvert (v[3],px[6]*h1,py[6]*h2,pz[6]*h3,cor);
            inicvert (v[4],px[5]*h1,py[5]*h2,pz[5]*h3,cor);
            PintaPolig3d (4,v);
        end;

    AplicaTransf3d ( 0,0,-1, ntx,nty,ntz );
    difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
    normaliza (difx,dify,difz);
    if difx*x0 + dify*y0 + difz*z0 < 0 then
        begin
            CalcCor3d ( 0,0,-1, cor );
            inicvert (v[1],px[5]*h1,py[5]*h2,pz[5]*h3,cor);
            inicvert (v[2],px[6]*h1,py[6]*h2,pz[6]*h3,cor);
            inicvert (v[3],px[7]*h1,py[7]*h2,pz[7]*h3,cor);
            inicvert (v[4],px[8]*h1,py[8]*h2,pz[8]*h3,cor);
            PintaPolig3d (4,v);
        end;

    AplicaTransf3d ( 0,-1,0, ntx,nty,ntz );
    difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
    normaliza (difx,dify,difz);
    if difx*x0 + dify*y0 + difz*z0 < 0 then
        begin
            CalcCor3d ( 0,-1,0, cor );
            inicvert (v[1],px[8]*h1,py[8]*h2,pz[8]*h3,cor);
            inicvert (v[2],px[7]*h1,py[7]*h2,pz[7]*h3,cor);
            inicvert (v[3],px[3]*h1,py[3]*h2,pz[3]*h3,cor);
            inicvert (v[4],px[4]*h1,py[4]*h2,pz[4]*h3,cor);
            PintaPolig3d (4,v);
        end;

    AplicaTransf3d ( 1,0,0, ntx,nty,ntz );
    difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
    normaliza (difx,dify,difz);
    if difx*x0 + dify*y0 + difz*z0 < 0 then
        begin
            CalcCor3d ( 1,0,0, cor );
            inicvert (v[1],px[2]*h1,py[2]*h2,pz[2]*h3,cor);
            inicvert (v[2],px[3]*h1,py[3]*h2,pz[3]*h3,cor);
            inicvert (v[3],px[7]*h1,py[7]*h2,pz[7]*h3,cor);
            inicvert (v[4],px[6]*h1,py[6]*h2,pz[6]*h3,cor);
            PintaPolig3d (4,v);
        end;

    AplicaTransf3d ( -1,0,0, ntx,nty,ntz );
    difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
    normaliza (difx,dify,difz);
    if difx*x0 + dify*y0 + difz*z0 < 0 then
        begin
            CalcCor3d ( -1,0,0, cor );
            inicvert (v[1],px[1]*h1,py[1]*h2,pz[1]*h3,cor);
            inicvert (v[2],px[4]*h1,py[4]*h2,pz[4]*h3,cor);
            inicvert (v[3],px[8]*h1,py[8]*h2,pz[8]*h3,cor);
            inicvert (v[4],px[5]*h1,py[5]*h2,pz[5]*h3,cor);
            PintaPolig3d (4,v);
        end;

end;


{-------------------------------------------------------------}
{                  Definicao de um Cilindro                   }
{-------------------------------------------------------------}

procedure Cilindro3d ( n:integer; r,h:real );

var
    i              : integer;
    t,x1,y1,x2,y2,
    ntx,nty,ntz,
    x0,y0,z0,cor   : real;
    v              : array[1..4] of PONTO;
    visivel        : boolean;

    {---------------------------------------------------------}

    procedure CalcCor ( var v:PONTO; var visivel:boolean );
        var
           nx,ny,nz,
           ntx,nty,ntz: real;
        begin
           nx := v.x;
           ny := v.y;
           nz := 0.0;
           CalcCor3d ( nx,ny,nz,v.cor );
           AplicaTransf3d ( nx,ny,nz, ntx,nty,ntz );
           visivel:= visivel OR ( ntz-z0 <= 0 );
        end;

    {---------------------------------------------------------}

begin

    if n<3 then n:= 3;
    x1:= r; y1:= 0;
    AplicaTransf3d ( 0,0,0, x0,y0,z0 );

    for i:= 1 to n do
        begin

    	   t := i*2*PI/n;
	   x2 := r*cos(t);
	   y2 := r*sin(t);

           { Pinta secao do cilindro }
           visivel:= false;
           inicvert ( v[1], x1, y1, 0, 0 );
           CalcCor  ( v[1], visivel );
           inicvert ( v[2], x2, y2, 0, 0 );
           CalcCor  ( v[2], visivel );
           inicvert ( v[3], x2, y2, h, 0 );
           CalcCor  ( v[3], visivel );
           inicvert ( v[4], x1, y1, h, 0 );
           CalcCor  ( v[4], visivel );
           if visivel then
               PintaPolig3d (4,v);

           { Pinta secao do fundo }
           CalcCor3d ( 0,0,-1,cor );
           AplicaTransf3d ( 0,0,-1, ntx,nty,ntz );
           if ntz-z0 <= 0 then
               begin
                  inicvert ( v[1], x1, y1, 0, cor );
                  inicvert ( v[2], x2, y2, 0, cor );
                  inicvert ( v[3],  0,  0, 0, cor );
                  PintaPolig3d (3,v);
               end;

           { Pinta secao da tampa }
           CalcCor3d ( 0,0,1,cor );
           AplicaTransf3d ( 0,0,1, ntx,nty,ntz );
           if ntz-z0 <= 0 then
               begin
                  inicvert ( v[1], x1, y1, h, cor );
                  inicvert ( v[2], x2, y2, h, cor );
                  inicvert ( v[3],  0,  0, h, cor );
                  PintaPolig3d (3,v);
               end;

           x1:= x2; y1:= y2;

        end;

end;


{-------------------------------------------------------------}
{                   Definicao de um anel                      }
{-------------------------------------------------------------}

procedure Anel3d ( n:integer; r,h: real );

var
    i: integer;
    t,x1,y1,x2,y2:real;
    v: array [1..4] of PONTO;

    {---------------------------------------------------------}

    procedure CalcCor ( var v:PONTO );
        var
           nx,ny,nz: real;
        begin
           nx := v.x;
           ny := v.y;
           nz := 0.0;
           CalcCor3d ( nx,ny,nz,v.cor );
        end;

    {---------------------------------------------------------}

begin

    if n<3 then n:= 3;
    x1:= r; y1:= 0;

    for i:= 1 to n do
        begin

    	   t := i*2*PI/n;
	   x2 := r*cos(t);
	   y2 := r*sin(t);

           { Pinta secao do Anel }
           inicvert ( v[1], x1, y1, 0, 0 );
           CalcCor  ( v[1] );
           inicvert ( v[2], x2, y2, 0, 0 );
           CalcCor  ( v[2] );
           inicvert ( v[3], x2, y2, h, 0 );
           CalcCor  ( v[3] );
           inicvert ( v[4], x1, y1, h, 0 );
           CalcCor  ( v[4] );
           PintaPolig3d (4,v);

           x1:= x2; y1:= y2;

        end;

end;


{-------------------------------------------------------------}
{                     Definicao de um cone                    }
{-------------------------------------------------------------}

procedure Cone3d ( n:integer; r,h:real );

var
    i            : integer;
    dist,
    cor, t1, t2,
    x1,y1,x2,y2,
    ntx,nty,ntz,
    x0,y0,z0     : real;
    v            : array [1..4] of PONTO;
    visivel      : boolean;
    n1, n2  : array [0..36+1] of COORDPTO;

{-------------------------------------------------------------}

procedure CalcNormPonto ( cos_ang1, ang2: real; var nx, ny, nz: real );
    var
        matrot : MATTRANSF;
    begin
        SalvaTransf3d;
        InicTransf3d;
        matrot:= MatAtual;           { ***** RodaY3d (-ang1); ***** }
        matrot[1,1] := cos_ang1;
        matrot[3,1] := -sqrt (1-sqr(cos_ang1));
        matrot[1,3] := -matrot[3,1];
        matrot[3,3] := matrot[1,1];
        MultMat3d (matrot);
        RodaZ3d (ang2);
        AplicaTransfObjeto3d (1,0,0, nx,ny,nz);
        RecuperaTransf3d;
    end;

{-------------------------------------------------------------}

procedure CriaVertice ( num: integer; xp,yp,zp: real; normal:COORDPTO;
                        var visivel: boolean );
    begin
           inicvert ( v[num], xp,yp,zp, 0 );
           with normal do
               begin
                   CalcCor3d ( x,y,z, v[num].cor );
                   AplicaTransf3d ( x,y,z, ntx,nty,ntz );
               end;
           if ntz-z0 <= 0 then
               visivel:= true;
    end;

{-------------------------------------------------------------}

begin
    if n < 3 then n:= 3;
    if n > 36 then n:= 36;

    dist:= sqrt ( r*r + h*h );
    AplicaTransf3d ( 0,0,0, x0,y0,z0 );

{----- Pinta a base -----}
    CalcCor3d ( 0,0,-1, cor );
    AplicaTransf3d ( 0,0,-1, ntx,nty,ntz );

    if ntz-z0 <= 0 then
        begin
            x1:= r; y1:= 0; t1:= 0;
            for i:= 1 to n do
                begin
                     t2 := i*2*PI/n;
                     x2 := r*cos(t2);
                     y2 := r*sin(t2);

                     inicvert ( v[1], x1, y1, 0, cor );
                     inicvert ( v[2], x2, y2, 0, cor );
                     inicvert ( v[3], 0, 0, 0, cor );
                     PintaPolig3d (3,v);
                     t1:= t2; x1:= x2; y1:= y2;
                end;
        end;

{----- Calculo das normais ao cone -----}
    { Normais do cone nos vertices da base }
    for i:= 0 to n+1 do
        begin
    	   t2 := i*2*PI/n;
           with n1[i] do
               CalcNormPonto ( h/dist, t2, x,y,z );
        end;

    { Normais do cone no vertice superior }
    for i:= 1 to n do
        with n2[i] do
             begin
                 x:= (n1[i-1].x + n1[i+1].x) / 2.0;
                 y:= (n1[i-1].y + n1[i+1].y) / 2.0;
                 z:= (n1[i-1].z + n1[i+1].z) / 2.0;
             end;
    n2[0]:= n2[n];

{----- Pinta o cone -----}
    x1:= r; y1:= 0; t1:= 0;
    for i:= 1 to n do
        begin
           t2 := i*2*PI/n;
           x2 := r*cos(t2);
           y2 := r*sin(t2);
           visivel:= false;

           CriaVertice ( 1, x1,y1,0, n1[i-1], visivel);
           CriaVertice ( 2, x2,y2,0, n1[i], visivel);
           CriaVertice ( 3, 0,0,h, n2[i], visivel);
           CriaVertice ( 4, 0,0,h, n2[i-1], visivel);

           if visivel then
                 PintaPolig3d (4,v);

           x1:= x2; y1:= y2; t1:= t2;
        end;

end;


{-------------------------------------------------------------}
{                  Definicao de uma esfera                    }
{-------------------------------------------------------------}

procedure Esfera ( nivel:integer; raio:real; Ngomos:integer );

const
    MAXNIVEL = 3;
    MAXOBJ   = 8*4*4*4;
    MAXPTOS  = MAXOBJ-2;

    indice: array [1..8,1..3] of integer =
                        ( ( 1, 5, 3 ),
                          ( 3, 5, 2 ),
                          ( 2, 5, 4 ),
                          ( 4, 5, 1 ),
                          ( 1, 3, 6 ),
                          ( 3, 2, 6 ),
                          ( 2, 4, 6 ),
                          ( 4, 1, 6 )
                        );

type

    TRIANGULO = record
                  pt1, pt2, pt3: integer;
                end;


var

    npoly: integer;
    poly: array [1..MAXOBJ] of TRIANGULO;

    nptos: integer;
    pto: array [1..MAXPTOS] of COORDPTO;


    {---------------------------------------------------------}

    procedure InicOctaedro;

       const
          oct: array [1..6] of COORDPTO =
                              ( ( x: 1; y: 0; z: 0 ),
                                ( x:-1; y: 0; z: 0 ),
                                ( x: 0; y: 1; z: 0 ),
                                ( x: 0; y:-1; z: 0 ),
                                ( x: 0; y: 0; z: 1 ),
                                ( x: 0; y: 0; z:-1 )
                              );

       var
          i: integer;

       begin

          { Inicializa vetor de pontos }
          nptos:= 6;
          for i:= 1 to nptos do
             begin
                pto[i].x:= oct[i].x * raio;
                pto[i].y:= oct[i].y * raio;
                pto[i].z:= oct[i].z * raio;
             end;

       end;

    {---------------------------------------------------------}

    procedure PontoMedio ( var pm: COORDPTO; p1,p2: COORDPTO );
       begin
         pm.x := ( p1.x + p2.x ) /2;
         pm.y := ( p1.y + p2.y ) /2;
         pm.z := ( p1.z + p2.z ) /2;
       end;

    {---------------------------------------------------------}

    procedure Afasta ( var pm:COORDPTO );
       var
          fator:real;
          dist: real;

       begin
          dist:= sqrt ( pm.x*pm.x + pm.y*pm.y + pm.z*pm.z );
          fator:= raio/dist;
          pm.x := pm.x * fator;
          pm.y := pm.y * fator;
          pm.z := pm.z * fator;
       end;

    {---------------------------------------------------------}

    procedure DivideTriang ( pt1, pt2, pt3: integer; nivel:integer );

       var
         pm1, pm2, pm3: COORDPTO;
         i1, i2, i3,
         t1, t2, t3: integer;

       begin

          if nivel = 0 then
              begin
                  { Inclui triangulos gerados na tabela de poligonos }
                  npoly := npoly + 1;
                  poly[npoly].pt1:= pt1;
                  poly[npoly].pt2:= pt2;
                  poly[npoly].pt3:= pt3;
              end
          else
              begin
                  { Calcula pontos medios }
                  PontoMedio ( pm1, pto[pt1], pto[pt2] );
                  PontoMedio ( pm2, pto[pt2], pto[pt3] );
                  PontoMedio ( pm3, pto[pt3], pto[pt1] );

                  { Afasta pontos medios para que distancia `a origem
                                         seja igual ao raio }
                  Afasta (pm1); Afasta (pm2); Afasta (pm3);

                  { Inclui os pontos medios na tabela de pontos }
                  i1 := nptos + 1;
                  i2 := nptos + 2;
                  i3 := nptos + 3;

                  pto[i1]:= pm1;
                  pto[i2]:= pm2;
                  pto[i3]:= pm3;
                  nptos:= nptos + 3;

                  { Redivide recursivamente os triangulos gerados }
                  DivideTriang ( i1, i2, i3,  nivel-1 );
                  DivideTriang ( i1, i3, pt1, nivel-1 );
                  DivideTriang ( i1, pt2, i2, nivel-1 );
                  DivideTriang ( i3, i2, pt3, nivel-1 );
              end;

       end;

    {---------------------------------------------------------}

    procedure AtribuiVert ( var P1:PONTO; P2:COORDPTO );

       begin
          p1.x:= p2.x;
          p1.y:= p2.y;
          p1.z:= p2.z;
          CalcCor3d ( p1.x,p1.y,p1.z, p1.cor );
       end;

    {---------------------------------------------------------}

var
      i: integer;
      v: array [1..3] of PONTO;
      x0,y0,z0,
      x,y,z,
      ntx,nty,ntz,
      difx,dify,difz,
      nx,ny,nz: real;
{      c: char;}

begin
      if nivel > MAXNIVEL then nivel:= MAXNIVEL;

      InicOctaedro;
      npoly := 0;

      { Calcula conjunto de poligonos que aproxima a esfera }
      for i:= 1 to Ngomos do
          DivideTriang ( indice[i,1], indice[i,2], indice[i,3], nivel );

      { Desenha a Esfera }
      if Ngomos = 8 then
         begin
            AplicaTransf3d (0,0,0,x0,y0,z0);
            for i:= 1 to npoly do
               begin
                  AtribuiVert (v[3], pto[poly[i].pt1]);
                  AtribuiVert (v[2], pto[poly[i].pt2]);
                  AtribuiVert (v[1], pto[poly[i].pt3]);

                  CalcNormPlano3d ( 3,v, nx,ny,nz );

                  AplicaTransf3d ( nx,ny,nz, ntx,nty,ntz );
       difx:= ntx-x0; dify:= nty-y0; difz:= ntz-z0;
       normaliza (difx,dify,difz);
       if difx*x0 + dify*y0 + difz*z0 < 0 then
{                  if ( ntz-z0 < 0 ) then}
                      PintaPolig3d ( 3, v );
               end;
         end
      else
         for i:= 1 to npoly do
            begin
               AtribuiVert (v[1], pto[poly[i].pt1]);
               AtribuiVert (v[2], pto[poly[i].pt2]);
               AtribuiVert (v[3], pto[poly[i].pt3]);
               PintaPolig3d ( 3, v );
            end;
end;

{-------------------------------------------------------------}

procedure Esfera3d ( nivel:integer; raio:real );
   begin
      esfera ( nivel, raio, 8 );
   end;

procedure SemiEsfera3d ( nivel:integer; raio:real );
   begin
      esfera ( nivel, raio, 4 );
   end;

{-------------------------------------------------------------}
{                     Definicao de um toro                    }
{-------------------------------------------------------------}

procedure Toro3d ( n1,n2:integer; r1,r2:real );

const
    MAXN1 = 36;
    MAXN2 = 36;

type
    CIRCUNF = array [0..MAXN1] of PONTO;

var
    x0,y0,z0,
    ntx,nty,ntz,
    xc,yc,zc,
    t, nx,ny,nz      : real;
    i, j             : integer;
    MatAux1, MatAux2 : MATTRANSF;
    c, c1, c2        : CIRCUNF;
    visivel1,
    visivel2         : array[0..MAXN1] of boolean;
    v                : array [1..4] of PONTO;

    {---------------------------------------------------------}

    procedure CalcCentro ( var xc,yc,zc: real );
        begin
            AplicaTransf3d ( r1,0,0, xc,yc,zc );
        end;

    {---------------------------------------------------------}

begin
    if n1<3 then n1:=3;
    if n1>MAXN1 then n1:= MAXN1;

    if n2<3 then n2:=3;
    if n2>MAXN2 then n2:= MAXN2;

    AplicaTransf3d ( 0,0,0, x0,y0,z0 );

    MatAux1:= MatAtual;
    MatAux2:= MatOlho;

    for i:= 0 to n2 do
        begin
           t:= i*2*pi / n2;
           nx:= r2*cos(t);
           ny:= r2*sin(t);
           nz:= 0;
           InicVert ( c[i], nx+r1, ny, nz, 0 );
           CalcCor3d ( nx,ny,nz, c[i].cor );
           AplicaTransf3d ( nx,ny,nz, ntx,nty,ntz );
           visivel1[i]:= ( ntz-z0 <= 0 );
        end;

    c1:= c;

    for j:= 1 to n1 do
        begin
           InicTransf3d;
           MatOlho:= MatIdent;
           RodaY3d ( j*2*pi/n1 );
           CalcCentro ( xc,yc,zc );
           for i:= 0 to n2 do
                AplicaTransf3d ( c[i].x,  c[i].y,  c[i].z,
                                   c2[i].x, c2[i].y, c2[i].z );

           MatAtual:= MatAux1;
           MatOlho:= MatAux2;
           for i:= 0 to n2 do
              begin
                nx:= c2[i].x - xc;
                ny:= c2[i].y - yc;
                nz:= c2[i].z - zc;
                CalcCor3d ( nx,ny,nz, c2[i].cor );

                AplicaTransf3d ( nx,ny,nz, ntx,nty,ntz );
                visivel2[i]:= ( ntz-z0 <= 0 );
              end;

           for i:= 0 to n2-1 do
               if ( visivel1[i] or visivel1[i+1] or
                    visivel2[i+1] or visivel2[i] ) then
                   begin
                      v[1]:= c1[i];
                      v[2]:= c1[i+1];
                      v[3]:= c2[i+1];
                      v[4]:= c2[i];
                      PintaPolig3d ( 4, v );
                   end;

           c1:= c2;
           visivel1:= visivel2;
        end;
end;

