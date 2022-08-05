{-------------------------------------------------------------}
{                                                             }
{    Modelagem de um balanco usando solidos primitivos        }
{                                                             }
{    Autor: Mauricio                                          }
{    Em:    Junho de 1993                                     }
{                                                             }
{-------------------------------------------------------------}
program balanco;
uses nce3d,graph,crt;

const
    { Caracteristicas do movimento }
    ANGMIN   = 120;
    ANGMAX   = 240;
    DELTA    = +30;
    NQUADROS =   5;

    { Regiao da tela que e' salva }
    X0 = 230;
    Y0 = 105;
    X1 = 390;
    Y1 = 235;

    { Dimensoes do objeto }
    XChao   = 120;
    YChao   =  2;
    ZChao   = 80;
    XBase   =  3;
    YBase   =  8;
    ZBase   = 16;
    XHasteV =  3;
    YHasteV = 40;
    ZHasteV =  2;
    XHasteH = 43;
    YHasteH =  2;
    ZHasteH =  2;
    XCorda  = 0.2;
    YCorda  = 32;
    ZCorda  = 0.2;
    XBanco  = 12;
    YBanco  = 0.5;
    ZBanco  = 12;
    AltBalanco = YHasteV-YCorda;

var
    rad, ang, incr: real;
    i : integer;
    c:char;
    p: array [1..NQUADROS] of pointer;
    size: word;
    arq: file;
    NumWritten, NumRead: word;

{-------------------------------------------------------------}

procedure DesenhaSuporte;
    begin
       { Chao }
       SelecCorPolig3d (VERDE);
       InicTransf3d;
       translada3d (-XChao/2,-YChao,-ZChao/2);
       cubo3d      (XChao,YChao,ZChao);

       { Base direita }
       SelecCorPolig3d (MARROM);
       inicTransf3d;
       translada3d (XHasteH/2,0,-Zbase/2);
       cubo3d      (XBase,YBase,ZBase);

       { Base esquerda }
       inicTransf3d;
       translada3d (-XHasteH/2,0,-ZBase/2);
       cubo3d      (XBase,YBase,ZBase);

       { Haste vertical direita }
       inicTransf3d;
       translada3d (XHasteH/2,0,-ZHasteV/2);
       cubo3d      (XHasteV,YHasteV,ZHasteV);

       { Haste vertical esquerda }
       inicTransf3d;
       translada3d (-XHasteH/2,0,-ZHasteV/2);
       cubo3d      (XHasteV,YHasteV,ZHasteV);

       { Haste superior }
       inicTransf3d;
       translada3d (-(XHasteH-XHasteV)/2,YHasteV,-ZHasteH/2);
       cubo3d      (XHasteH,YHasteH,ZHasteH);
    end;

{-------------------------------------------------------------}

procedure DesenhaBanco;
    begin
       { Angulo em radianos }
       rad:= ang*pi/180;

       { Corrente direita do balanco }
       inicTransf3d;
       RodaX3d     (rad);
       translada3d (XBanco/2,YCorda+AltBalanco,-ZHasteH/2);
       cubo3d      (XCorda,YCorda,ZCorda);

       { Corrente esquerda do balanco }
       inicTransf3d;
       RodaX3d     (rad);
       translada3d (-XBanco/2,YCorda+AltBalanco,-ZHasteH/2);
       cubo3d      (XCorda,YCorda,ZCorda);

       { Banco }
       SelecCorPolig3d (VERMELHO);
       inicTransf3d;
       translada3d (-XBanco/2,+YCorda,-ZBanco/2);
       RodaX3d     (rad);
       translada3d (0,YCorda+AltBalanco,0);
       cubo3d      (XBanco,YBanco,ZBanco);
    end;

{-------------------------------------------------------------}

procedure GravaQuadro;
    begin
        GetImage (X0,Y0,X1,Y1,P[1]^);
        BlockWrite (arq,p[1]^,size,NumWritten);
    end;

{-------------------------------------------------------------}

procedure CalcNovoAngulo;
    begin
        ang := ang + incr;
        if ang > ANGMAX then
           begin
             ang:= ANGMAX;
             incr:= -DELTA;
           end;
        if ang < ANGMIN then
           begin
             ang:= ANGMIN;
             incr:= DELTA;
           end;
    end;

{-------------------------------------------------------------}

procedure GeraOsQuadros;
    begin
        Assign (arq, 'temp.$$$');
        Rewrite (arq, 1);
        Size := ImageSize (X0,Y0,X1,Y1);
        if MemAvail < Size then
            begin
               Termina3d;
               writeln ('Memoria insuficiente.');
               c:= readkey;
               halt;
            end;
        GetMem(P[1], Size);

        ang:= ANGMIN; incr:= +DELTA;
        for i:= 1 to NQUADROS do
           begin
               inicZBuffer3D(getmaxy+1,255,'c:');
               DesenhaSuporte;
               DesenhaBanco;
               GravaQuadro;
               CalcNovoAngulo;
           end;
        close (arq);
        LiberaZBuffer3d;
    end;

{-------------------------------------------------------------}

procedure Anima;
    begin
        Assign (arq, 'temp.$$$');
        Reset (arq, 1);
        for i:=1 to NQUADROS do
            begin
                if i <> 1 then
                    begin
                        if MemAvail < Size then
                            begin
                               Termina3d;
                               writeln ('Memoria insuficiente.');
                               c:= readkey;
                               halt;
                            end;
                        GetMem(P[i], Size);
                    end;
                BlockRead (arq,p[i]^,size,NumRead);
            end;
        close (arq);
        erase (arq);

        while not keypressed do
            begin
                for i:=1 to NQUADROS do
                    begin
                        PutImage(X0,Y0, P[i]^, NormalPut);
                        delay(150);
                    end;
                for i:=NQUADROS downto 1 do
                    begin
                        PutImage(X0,Y0, P[i]^, NormalPut);
                        delay(150);
                    end;
            end;
    end;

{-------------------------------------------------------------}

begin
    inicVideo3D(SVGA256);
    Janela3d ( 40,
               600,
               20.0, 15.0,
               0, 0,
               getmaxx+1, getmaxy+1);
    Observador3d(150,150,-300, 0,0,0 );
    SelecLuz3d(80,100,-300);
    TiraMachBand3d(false);

    GeraOsQuadros;
    Anima;
    Termina3d;
end.
