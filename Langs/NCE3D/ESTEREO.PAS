{-------------------------------------------------------------}
{                                                             }
{             Cubos Estereoscopicos                           }
{                                                             }
{-------------------------------------------------------------}

program Cubos;

uses graph, nce3d, crt;

var
    arq: file;

{-------------------------------------------------------------}

procedure guardaImagem;
var p: pointer;
    tam, y: integer;
begin
    assign (arq, 'imagem.$$$');
    rewrite (arq, 1);
    tam := imagesize (0,0,getmaxx,0);
    getmem (p, tam);

    for y := 0 to getmaxy do
        begin
            getimage (0, y, getmaxx, y, p^);
            blockwrite (arq, p^, tam);
        end;

    close (arq);
    freemem (p, tam);
end;

{-------------------------------------------------------------}

procedure superpoeImagem;
var p: pointer;
    tam, y: integer;
begin
    assign (arq, 'imagem.$$$');
    reset (arq, 1);
    tam := imagesize (0,0,getmaxx,0);
    getmem (p, tam);

    for y := 0 to getmaxy do
        begin
            blockread (arq, p^, tam);
            putimage (0, y, p^, XORPUT);
        end;

    close (arq);
    freemem (p, tam);
end;

{-------------------------------------------------------------}

procedure cena;
var i, j, k: integer;
begin
    for i := 1 to 3 do
       for j := 1 to 3 do
           for k := 1 to 3 do
           begin
               inictransf3d;
               translada3d( (i-2)*30, (j-2)*30, (k-2)*30 );
               rodaY3d(-pi/6);
               cubo3d(10,10,10);
           end;
end;

{-------------------------------------------------------------}

begin
    InicVideo3d (svga256);
    Janela3d (10, 400, 20.0, 15.0,
               0, 0, getmaxx+1, getmaxy+1);
    SelecLuz3d (120,120,-80);
    SelecLuzAmbiente3d ( 0.2 );

    { Gera a imagem vermelha }
    Observador3d ( -2, 50, -100, 0, -140, 200 );
    InicZbuffer3d ( getmaxy+1, 0, 'c:' );
    SelecRGBPolig3d (0.7,0,0);
    cena;
    guardaimagem;

    { Gera a imagem azul }
    Observador3d (  2, 50, -100, 0, -140, 200 );
    InicZbuffer3d ( getmaxy+1, 0, 'c:' );
    SelecRGBPolig3d (0,0,1);
    cena;
    superpoeimagem;

    textcolor (255);
    directvideo := false;
    writeln ('Use oculos 3d (azul(d)/vermelho(e))');
    erase (arq);
    readln;
    Termina3d;
end.

