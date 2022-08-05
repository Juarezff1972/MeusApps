{-----------------------------------------------------------}
{                                                           }
{     Modelagem das letras NCE/UFRJ sob o arco iris.        }
{                                                           }
{     Autor: Mauricio Bomfim                                }
{     Em: abril de 1993                                     }
{                                                           }
{-----------------------------------------------------------}
program NCE;
uses
  crt, graph, nce3d, utnce3d;

const
     MODOVIDEO   = SVGA256;
     NLETRAS     = 7;

type
     POLIGONO    = record
                      np: integer;
                      letra: array[1..24] of PONTO;
                   end;

var
     carac       : array [1..NLETRAS] of POLIGONO;

const
     p : array [1..NLETRAS] of COORDPTO = (
                          ( x:-42; y:-25; z:  0 ),    { N }
                          ( x:-30; y:-25; z:  0 ),    { C }
                          ( x:-18; y:-25; z:  0 ),    { E }
                          ( x:  6; y:-25; z:  0 ),    { U }
                          ( x: 18; y:-25; z:  0 ),    { F }
                          ( x: 30; y:-25; z:  0 ),    { R }
                          ( x: 42; y:-25; z:  0 )     { J }
     );

{---------------------------------------------------------------}

procedure InicLetras;

   var
        arq : text;
        i,l : integer;
   begin
        assign (arq, 'NCEUFRJ.DAT' );
        reset (arq);

        for l:=1 to NLETRAS do
            with carac[l] do
               begin
                  readln (arq, np);
                  for i := 1 to np do
                     begin
                        readln (arq, letra[i].x, letra[i].y);
                        letra[i].z:= 0;
                     end;
               end;

        close (arq);
   end;

{---------------------------------------------------------------}

procedure Horizonte;
    var
        i, h:integer;
    begin
      if MODOVIDEO = VGA256 then
          h:= 125
      else
          h:= 300;

      InicVertDegrade ( 1,   0,   0, branco );
      InicVertDegrade ( 2, getmaxx,   0, branco );
      InicVertDegrade ( 3, getmaxx,   h, branco );
      InicVertDegrade ( 4,   0, h, branco );
      DegradeLinear ( 4, 0, 1, branco, azul );

      for i:= 0 to 100 do
          putpixel ( random(getmaxx), random(h), 255 );

      InicVertDegrade ( 1,   0, h, branco );
      InicVertDegrade ( 2, getmaxx, h, branco );
      InicVertDegrade ( 3, getmaxx, getmaxy, branco );
      InicVertDegrade ( 4,   0, getmaxy, branco );
      DegradeLinear ( 4, 0, 1, preto, cinza );
    end;

{---------------------------------------------------------------}

procedure ArcoIris;
    type
      linha = array [0..6] of record
                                x,y,c: integer;
                              end;
    var
       i: integer;

    const
      linha1 : linha = ( (x: 60; y:300; c:vermelho),
                         (x: 80; y:300; c:laranja),
                         (x:100; y:300; c:amarelo),
                         (x:120; y:300; c:verde),
                         (x:140; y:300; c:ciano),
                         (x:160; y:300; c:azul),
                         (x:180; y:300; c:violeta) );

      linha2 : linha = ( (x:439; y:  0; c:vermelho),
                         (x:489; y:  0; c:laranja),
                         (x:539; y:  0; c:amarelo),
                         (x:589; y:  0; c:verde),
                         (x:639; y:  0; c:ciano),
                         (x:639; y: 40; c:azul),
                         (x:639; y: 80; c:violeta) );

    begin
      if MODOVIDEO = VGA256 then
           for i:= 0 to 6 do
              begin
                 linha1[i].x := round (linha1[i].x * 320.0 / 640);
                 linha1[i].y := round (linha1[i].y * 200.0 / 480);
                 linha2[i].x := round (linha2[i].x * 320.0 / 640);
                 linha2[i].y := round (linha2[i].y * 200.0 / 480);
              end;

      for i:= 0 to 5 do
          begin
            InicVertDegrade ( 1, linha1[i].x, linha1[i].y, linha1[i].c );
            InicVertDegrade ( 2, linha1[i+1].x, linha1[i+1].y, linha1[i+1].c );
            InicVertDegrade ( 3, linha2[i+1].x, linha2[i+1].y, linha2[i+1].c );
            InicVertDegrade ( 4, linha2[i].x, linha2[i].y, linha2[i].c );
            PintaPoligDegrade (4);
          end;
    end;

{---------------------------------------------------------------}

procedure Desenha ( simb:POLIGONO );

   const
      PROF = 8;

   var
      i : integer;
      tras, vert: array [1..24] of PONTO;
      nx,ny,nz, cor: real;

   begin
      with simb do
        begin
           { Face da frente }
           CalcNormPlano3d ( np, letra, nx,ny,nz );
           CalcCor3d ( nx, ny, nz, cor );
           for i:= 1 to np do
               letra[i].cor:= cor;
           PintaPolig3d (np,letra);

           { Face de tras }
           CalcCor3d ( -nx, -ny, -nz, cor );
           for i:= 1 to np do
               begin
                   tras[i].x:= letra[i].x;
                   tras[i].y:= letra[i].y;
                   tras[i].z:= letra[i].z - PROF;
                   tras[i].cor:= cor;
               end;
           PintaPolig3d (np,tras);

           { Arestas laterais }
           for i:= 1 to np-1 do
               begin
                  vert[1]:= letra[i];
                  vert[2]:= letra[i+1];
                  vert[3]:= tras [i+1];
                  vert[4]:= tras [i];
                  CalcNormPlano3d ( 4, vert, nx,ny,nz );
                  CalcCor3d ( -nx, -ny, -nz, cor );
                  vert[1].cor := cor;
                  vert[2].cor := cor;
                  vert[3].cor := cor;
                  vert[4].cor := cor;
                  PintaPolig3d (4,vert);
               end;
           vert[1]:= letra[np];
           vert[2]:= letra[1];
           vert[3]:= tras [1];
           vert[4]:= tras [np];
           CalcNormPlano3d ( 4, vert, nx,ny,nz );
           CalcCor3d ( -nx, -ny, -nz, cor );
           vert[1].cor := cor;
           vert[2].cor := cor;
           vert[3].cor := cor;
           vert[4].cor := cor;
           PintaPolig3d (4,vert);
        end;
   end;

{---------------------------------------------------------------}

procedure DesenhaLetras;
   var
       i: integer;
       xb,yb,zb: real;

   begin
      for i:= 1 to NLETRAS do
         begin
             InicTransf3d;
             Translada3d ( p[i].x,p[i].y,p[i].z );
             Desenha (carac[i]);
         end;
   end;

{---------------------------------------------------------------}

begin
   InicVideo3d (MODOVIDEO);
   SelecLuz3d ( 0,-300,-300 );
   SelecLuzAmbiente3d ( 0.5 );
   SelecCorPolig3d ( Vermelho );
   Janela3d ( 40, 800, 20.0, 15.0,
              0, 0, getmaxx+1, getmaxy+1 );
   Observador3d ( 0, 0, -200, 0, 0, 0 );
   InicLetras;
   InicZBuffer3d ( getmaxy+1, 0, 'C:' );
   Horizonte;
   ArcoIris;
   DesenhaLetras;
   readln;
   Termina3d;
end.