{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit UtNCE3D                                     }
{            Rotinas utilitarias para o NCE3D                 }
{                                                             }
{            Arquivo include UtQUANT.PAS                      }
{            Quantizador para imagens geradas pelo NCE3D      }
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
{                                                             }
{    Quantizador para imagens geradas pelo NCE3D              }
{                                                             }
{    - Escolha das melhores cores, por frequencia.            }
{    - Aplicacao de fator randomico na entrada.               }
{    - Numero de bits por cor ajustavel atraves das           }
{        constantes NBITS e MAXCOR.                           }
{                                                             }
{-------------------------------------------------------------}

{-------------------------------------------------------------}
{              aloca memoria para o Histograma                }
{-------------------------------------------------------------}

procedure alocaHistograma;
    var
        r,g,b: byte;
    begin
        new (histo);
        new (phisto);
        for r := 0 to MAXCOR do
            begin
                new (histo^[r]);
                for g := 0 to MAXCOR do
                    for b := 0 to MAXCOR do
                     histo^[r]^[g][b] := 0;
            end;
    end;

{-------------------------------------------------------------}
{                     monta Histograma                        }
{-------------------------------------------------------------}

procedure montaHistograma (nomearq: string);
label
    FimLoop;
var
    i, x, y, n: integer;
    r, g, b: byte;
    cor: integer;
    rnd: byte;

begin
    assign (arqrgb, nomearq+'.rgb');
    {$I-}  reset (arqrgb,1); {$I+}
    if ioresult <> 0 then
        begin
            Termina3d;
            clrscr;
            writeln ('*** O arquivo ', nomearq,
                     '.rgb nao foi encontrado. ***');
            writeln ( '*** Programa Cancelado. ***' );
            halt;
        end;
    InicBufRead;

    { background e' forcado pertencer `a tabela }
    with phisto^[0] do
        begin
            if xreadbyte (arqRGB,pr) then
                goto FimLoop;
            if xreadbyte (arqRGB,pg) then
                goto FimLoop;
            if xreadbyte (arqRGB,pb) then
                goto FimLoop;
            pr:= pr shr BITS_IGN;
            pg:= pg shr BITS_IGN;
            pb:= pb shr BITS_IGN;
            histo^[pr]^[pg][pb] := 255;
        end;
    ncores := 0;

    randSeed := 477;
    while TRUE do
        begin
            if xreadint(arqrgb,x) then
                goto FimLoop;
            if xreadint(arqrgb,y) then
                goto FimLoop;
            if xreadint(arqrgb,n) then
                goto FimLoop;
            for i:= 1 to n do
                begin
                    if xreadbyte (arqrgb,r) then
                        goto FimLoop;
                    if xreadbyte (arqrgb,g) then
                        goto FimLoop;
                    if xreadbyte (arqrgb,b) then
                        goto FimLoop;

                    rnd := random (RANDOMICO);
                    r := (r+rnd) shr BITS_IGN;
                    if r > MAXCOR then r := MAXCOR;
                    g := (g+rnd) shr BITS_IGN;
                    if g > MAXCOR then g := MAXCOR;
                    b := (b+rnd) shr BITS_IGN;
                    if b > MAXCOR then b := MAXCOR;

                    if histo^[r]^[g][b] <> 255 then
                        begin
                            histo^[r]^[g][b] := histo^[r]^[g][b] + 1;
                            if histo^[r]^[g][b] = 1 then
                                begin
                                    ncores := ncores + 1;
                                    phisto^[ncores].pr := r;
                                    phisto^[ncores].pg := g;
                                    phisto^[ncores].pb := b;
                                end;
                        end;
                end;
        end;
FimLoop:
    LibBufRead;
    close (arqrgb);
end;

{-------------------------------------------------------------}
{                         quick sort                          }
{-------------------------------------------------------------}

procedure sort (first, last: word);
var i, j, k: integer;
    pivot: integer;
    temp: PRGB;

begin
    if first < (last-1) then
        begin
            i := first;
            j := last;
            k := (first + last) shr 1;
            pivot := (histo^[phisto^[i].pr]^[phisto^[i].pg][phisto^[i].pb] +
                      histo^[phisto^[j].pr]^[phisto^[j].pg][phisto^[j].pb] +
                      histo^[phisto^[k].pr]^[phisto^[k].pg][phisto^[k].pb]) div 3;

            while i < j do
                begin
                    while histo^[phisto^[i].pr]^[phisto^[i].pg][phisto^[i].pb]
                             > pivot do
                        i := i + 1;
                    while histo^[phisto^[j].pr]^[phisto^[j].pg][phisto^[j].pb]
                             < pivot do
                        j := j - 1;
                    if i < j then
                        begin
                            temp := phisto^[i];
                            phisto^[i] := phisto^[j];
                            phisto^[j] := temp;

                            i := i + 1;
                            j := j - 1;
                        end;
                end;

            if j < last then
                begin
                    sort (first, j);
                    sort (j+1, last);
                end;
        end;

    if histo^[phisto^[last].pr]^[phisto^[last].pg][phisto^[last].pb] >
       histo^[phisto^[first].pr]^[phisto^[first].pg][phisto^[first].pb]  then
        begin
            temp := phisto^[first];
            phisto^[first] := phisto^[last];
            phisto^[last] := temp;
        end;
end;

{-------------------------------------------------------------}
{                    localiza melhor cor                      }
{-------------------------------------------------------------}

function localizaMelhorCor (corR,corG,corB: byte): integer;
var
    d, menorDist: longint;
    i: integer;
begin
    menorDist := 64*64*64;
    for i := 0 to 255 do
        begin
            with phisto^[i] do
                d := sqr(corR - pr) + sqr(corG - pg) + sqr(corB - pb);

            if d < menorDist then
                begin
                    menorDist := d;
                    localizaMelhorCor := i;
                end;
        end;
end;

{-------------------------------------------------------------}
{                      ordena histograma                      }
{-------------------------------------------------------------}

procedure ordenaHistograma;
var i, n: integer;
begin
    n := ncores;
    if n > 255 then
        begin
            sort (1, ncores);
            n := 255;
        end;

    for i := 0 to n do
        with phisto^[i] do
            histo^[pr]^[pg][pb] := i;

    for i := 256 to ncores do
        with phisto^[i] do
            histo^[pr]^[pg][pb] := localizaMelhorCor (pr,pg,pb);
end;

{-------------------------------------------------------------}
{                      monta palette                          }
{-------------------------------------------------------------}

procedure montaPalette;
var i: integer;
begin
    for i := 0 to 255 do
        setrgbpalette (i, phisto^[i].pr shl SHIFT_PAL,
                          phisto^[i].pg shl SHIFT_PAL,
                          phisto^[i].pb shl SHIFT_PAL);
end;

{-------------------------------------------------------------}
{                    poe a imagem na tela                     }
{-------------------------------------------------------------}

procedure poeNaTela;
label FimLoop;
var i, x, y, n: integer;
    r, g, b: byte;
    cor: integer;
    rnd: byte;

begin
    randSeed := 477;
    reset (arqrgb,1);
    InicBufRead;
    if xreadbyte (arqRGB,r) then
        goto FimLoop;
    if xreadbyte (arqRGB,g) then
        goto FimLoop;
    if xreadbyte (arqRGB,b) then
        goto FimLoop;

    setfillstyle (SolidFill,0);
    bar (0,0,getmaxx,getmaxy);
    while TRUE do
        begin
            if xreadint(arqrgb,x) then
                goto FimLoop;
            if xreadint(arqrgb,y) then
                goto FimLoop;
            if xreadint(arqrgb,n) then
                goto FimLoop;
            for i:= 1 to n do
                begin
                    if xreadbyte (arqrgb,r) then
                        goto FimLoop;
                    if xreadbyte (arqrgb,g) then
                        goto FimLoop;
                    if xreadbyte (arqrgb,b) then
                        goto FimLoop;

                    rnd := random (RANDOMICO);
                    r := (r+rnd) shr BITS_IGN;
                    if r > MAXCOR then r := MAXCOR;
                    g := (g+rnd) shr BITS_IGN;
                    if g > MAXCOR then g := MAXCOR;
                    b := (b+rnd) shr BITS_IGN;
                    if b > MAXCOR then b := MAXCOR;

                    putpixel (x, y, histo^[r]^[g][b]);
                    x := x + 1;
                end;
        end;
FimLoop:
    LibBufRead;
    close (arqrgb);
end;

{-------------------------------------------------------------}
{     libera memoria alocada para processar o histograma      }
{-------------------------------------------------------------}

procedure liberaHistograma;
var i: integer;
begin
    for i := 0 to MAXCOR do
        dispose (histo^[i]);
    dispose (histo);
    dispose (phisto);
end;

{-------------------------------------------------------------}
{      quantiza uma imagem de 24 bits gerada pelo NCE3D       }
{-------------------------------------------------------------}

procedure QuantizaImagem (nomearq: string);
begin
    alocaHistograma;
    montaHistograma (nomearq);
    ordenaHistograma;

    montaPalette;
    poeNaTela;
    liberaHistograma;
end;

