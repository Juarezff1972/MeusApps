{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit UtNCE3D                                     }
{            Rotinas utilitarias para o NCE3D                 }
{                                                             }
{            Arquivo include UtDEGRAD.PAS                     }
{            Pintura de poligonos em degrade                  }
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
{              Mauricio Nunes da Costa Bomfim                 }
{                                                             }
{                                                             }
{-------------------------------------------------------------}

{-------------------------------------------------------------}
{       Calcula as coordenadas do centro de um poligono       }
{-------------------------------------------------------------}

function XCentroDegrade (n: integer): integer;
var i: integer;
    x: real;
begin
    x := 0;
    for i := 1 to n do
        x := x + vert[i].x;
    XCentroDegrade := round (x / n);
end;

{--------------------------------------------------------}

function YCentroDegrade (n: integer): integer;
var i: integer;
    y: real;
begin
    y := 0;
    for i := 1 to n do
        y := y + vert[i].y;
    YCentroDegrade := round (y / n);
end;

{--------------------------------------------------------}
{       Inicializa um vertice de um poligono             }
{--------------------------------------------------------}

procedure InicVertDegradeRGB (num,x,y: integer; r,g,b: byte);
begin
    vert[num].x:= x;
    vert[num].y:= y;
    vert[num].r:= r;
    vert[num].g:= g;
    vert[num].b:= b;
end;

{--------------------------------------------------------}

procedure InicVertDegrade (num,x,y,cor: integer);
begin
    with CoresRGB[cor] do
        InicVertDegradeRGB (num,x,y, r,g,b);
end;

{--------------------------------------------------------}
{    Inicializa os vertices de um poligono circular      }
{--------------------------------------------------------}

procedure InicCirculoDegrade (xc,yc,raio,ndiv: integer);
var
    i, x, y: integer;
    delta, ang: real;
begin
    delta:= 2 * pi / ndiv;
    ang:= 0;
    for i:= 1 to ndiv do
        begin
            x:= round (xc + raio * cos (ang));
            y:= round (yc + raio * sin (ang));
            InicVertDegrade (i, x, y, 0);
            ang:= ang + delta;
        end
end;

{--------------------------------------------------------}

function intmax (a,b: real): integer;
begin
    intmax := trunc (a) + 1;
    if a < b then intmax := trunc(b)+1;
end;

function intmin (a,b: real): integer;
begin
    intmin := trunc (a) - 1;
    if a > b then intmin := trunc(b)-1;
end;

{--------------------------------------------------------}
{             Pinta um poligono em degrade               }
{--------------------------------------------------------}

procedure PintaPoligDegrade (nvert: integer);

    procedure montaBucket;
    var i, y, n, n1: integer;
        t: real;
        pbuc: PBUCKET;
        x1, x2: real;
        y1, y2: real;

    begin
        for y := 0 to getmaxy do
            buc [y] := NIL;

        for n := 1 to nvert do
            begin
                n1 := n+1;
                if n1 > nvert then n1 := 1;
                x1 := vert[n1].x;   y1 := vert[n1].y;
                x2 := vert[n].x;    y2 := vert[n].y;


                for y := intmin(y1,y2) to intmax(y1,y2) do
                    begin
                        t := -1;
                        if y1 <> y2 then
                             t := (y+0.5-y1) / (y2-y1);

                        if (t >= 0) and (t <= 1) then
                            begin
                                new (pbuc);
                                pbuc^.xb := x1 + (x2-x1) * t;
                                pbuc^.r := trunc (vert[n1].r +
                                           (vert[n].r-vert[n1].r) * t);
                                pbuc^.g := trunc (vert[n1].g +
                                           (vert[n].g-vert[n1].g) * t);
                                pbuc^.b := trunc (vert[n1].b +
                                           (vert[n].b-vert[n1].b) * t);
                                pbuc^.pxbuc := buc[y];
                                buc[y] := pbuc;
                            end;
                    end;
            end;
    end;

    {----------------------------------------------------}

    procedure pintaLinha (y: integer);
    var
        p, salva: PBUCKET;
        nl, xi, x1, x2,
        i, j,
        corR, corG, corB: integer;
        deltacorR, deltacorG, deltacorB: integer;
        delta: real;
        temp: BUCKET;
        xl: array [1..200] of BUCKET;

    begin
        nl := 0;
        p := buc[y];
        while p <> NIL do
            begin
                nl := nl + 1;
                xl[nl].xb := p^.xb;
                xl[nl].r := p^.r;
                xl[nl].g := p^.g;
                xl[nl].b := p^.b;

                salva := p;
                p := p^.pxbuc;
                dispose (salva);
            end;

        for i := 1 to nl-1 do
            for j := i to nl do
                if xl[i].xb > xl[j].xb then
                    begin
                        temp := xl [i];
                        xl [i] := xl[j];
                        xl [j] := temp;
                    end;

        i:= 1;
        while i < nl do
            begin
               x1:= trunc(xl[i].xb);
               x2:= trunc(xl[i+1].xb);
               delta := 0.0;
               if x1 <> x2 then
                   delta:= 1.0 / (x2-x1);

               deltaCorR := trunc (int(xl[i+1].r - xl[i].r) * 64 * delta);
               deltaCorG := trunc (int(xl[i+1].g - xl[i].g) * 64 * delta);
               deltaCorB := trunc (int(xl[i+1].b - xl[i].b) * 64 * delta);
               corR := trunc (xl[i].r * 64);
               corG := trunc (xl[i].g * 64);
               corB := trunc (xl[i].b * 64);

               for xi:= x1 to x2 do
                  begin
                      VGADither (xi, y, corR shr 6, corG shr 6, corB shr 6);
                      if ArqImagemAberto then
                          GuardaCor24bits (xi, y,
                                     corR shr 6, corG shr 6, corB shr 6);
                      corR := corR + deltaCorR;
                      corG := corG + deltaCorG;
                      corB := corB + deltaCorB;
                  end;
               i:= i + 2;
            end;
    end;

    {----------------------------------------------------}

var
    y: integer;

begin
    montaBucket;
    for y := 0 to getmaxy do
        pintaLinha (y);
end;

{--------------------------------------------------------}
{     Pinta um poligono fazendo um degrade linear        }
{--------------------------------------------------------}

procedure DegradeLinearRGB ( nvert, dx,dy, r1,g1,b1, r2,g2,b2: integer );
   var
      i: integer;
      proj: array [1..MAXVERTSPOLIG] of real;
      projIni, projFim, r, nx, ny, t: real;

begin
   if (dx = 0) and (dy = 0) then
       dy:= 1;
   projIni := 9999; projFim := -9999;
   r:= sqrt ( 1.0*dx*dx + 1.0*dy*dy );
   nx := dx / r;   ny := dy / r;
   for i:= 1 to nvert do
       begin
          proj[i]:= vert[i].x * nx - vert[i].y * ny;
          if proj[i] > projFim then
                projFim:= proj[i];
          if proj[i] < projIni then
                projIni:= proj[i];
       end;
   if projIni = projFim then
       exit;
   for i:= 1 to nvert do
      begin
          t := (proj[i]-projIni) / (projFim-projIni);
          vert[i].r := round (r1 + t * (r2-r1));
          vert[i].g := round (g1 + t * (g2-g1));
          vert[i].b := round (b1 + t * (b2-b1));
      end;
   PintaPoligDegrade (nvert);
end;

{--------------------------------------------------------}

procedure DegradeLinear ( nvert, dx,dy, cor1, cor2: integer );
var
    r1,g1,b1, r2,g2,b2: integer;
begin
   with CoresRGB[cor1] do
       begin
           r1:= r; g1:= g; b1:= b;
       end;
   with CoresRGB[cor2] do
       begin
           r2:= r; g2:= g; b2:= b;
       end;
   DegradeLinearRGB ( nvert, dx,dy, r1,g1,b1, r2,g2,b2 );
end;

{--------------------------------------------------------}
{     Pinta um poligono fazendo um degrade circular      }
{--------------------------------------------------------}

procedure DegradeCircularRGB ( nvert, xc,yc, raio,
                                r1,g1,b1, r2,g2,b2: integer );
    procedure montaBucket;
    var i, y, n, n1: integer;
        t: real;
        pbuc: PBUCKET;
        x1, x2: real;
        y1, y2: real;

    begin
        for y := 0 to getmaxy do
            buc [y] := NIL;

        for n := 1 to nvert do
            begin
                n1 := n+1;
                if n1 > nvert then n1 := 1;
                x1 := vert[n1].x;   y1 := vert[n1].y;
                x2 := vert[n].x;    y2 := vert[n].y;

                for y := intmin(y1,y2) to intmax(y1,y2) do
                    begin
                        t := -1;
                        if y1 <> y2 then
                             t := (y+0.5-y1) / (y2-y1);

                        if (t >= 0) and (t <= 1) then
                            begin
                                new (pbuc);
                                pbuc^.xb := x1 + (x2-x1) * t;
                                pbuc^.pxbuc := buc[y];
                                buc[y] := pbuc;
                            end;
                    end;
            end;
    end;

    {----------------------------------------------------}

    procedure pintaLinha (y: integer);

    var
        p, salva: PBUCKET;
        nl, xi, x1, x2,
        i, j,
        corR, corG, corB: integer;
        t, dist: real;
        temp: BUCKET;
        xl: array [1..20] of BUCKET;

    begin
        nl := 0;
        p := buc[y];
        while p <> NIL do
            begin
                nl := nl + 1;
                xl[nl].xb := p^.xb;

                salva := p;
                p := p^.pxbuc;
                dispose (salva);
            end;

        for i := 1 to nl-1 do
            for j := i to nl do
                if xl[i].xb > xl[j].xb then
                    begin
                        temp := xl [i];
                        xl [i] := xl[j];
                        xl [j] := temp;
                    end;

        i:= 1;
        while i < nl do
            begin
               x1:= trunc(xl[i].xb);
               x2:= trunc(xl[i+1].xb);
               for xi:= x1 to x2 do
                  begin
                      dist := sqrt (sqr (1.0*(xi-xc)) + sqr (1.0*(y-yc)));
                      t := dist / raio;
                      if t > 1 then t := 1;
                      corR := round (r1 + t * (r2-r1));
                      corG := round (g1 + t * (g2-g1));
                      corB := round (b1 + t * (b2-b1));

                      VGADither (xi, y, corR, corG, corB);
                      if ArqImagemAberto then
                          GuardaCor24bits (xi, y,
                                           corR, corG, corB);
                  end;
               i:= i + 2;
            end;
    end;

    {----------------------------------------------------}

var y: integer;
begin
    montaBucket;
    for y := 0 to getmaxy do
        pintaLinha (y);

end;

{--------------------------------------------------------}

procedure DegradeCircular ( nvert, xc,yc, raio, cor1,cor2: integer );
var
    r1,g1,b1, r2,g2,b2: integer;
begin
   with CoresRGB[cor1] do
       begin
           r1:= r; g1:= g; b1:= b;
       end;
   with CoresRGB[cor2] do
       begin
           r2:= r; g2:= g; b2:= b;
       end;
   DegradeCircularRGB ( nvert, xc,yc, raio, r1,g1,b1, r2,g2,b2 );
end;
