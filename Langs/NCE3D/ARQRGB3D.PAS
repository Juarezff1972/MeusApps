{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit ArqRGB3D                                    }
{            Geracao de arquivo RGB 24 bits                   }
{                                                             }
{    Importante:                                              }
{         Esta Unit e' compartilhada pela NCE3D e UtNCE3D.    }
{         Nao deve ser usada diretamente pelo usuario!        }
{                                                             }
{                                                             }
{    Versao 2.7 ( Junho de 1993 )                             }
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

Unit arqRGB3d;

interface

type
    RGB       = record
                   r,g,b : byte;
                end;

    LGRAV = array [1..800] of RGB;

const
    tabdither: array [0..15] of integer = (
             1, 9, 3,11,
            13, 5,15, 7,
             4,12, 2,10,
            16, 8,14, 6
     );

    NCORESRGB = 19;
    CoresRGB : array [0..NCORESRGB-1] of RGB =
               ( ( r:255; g:  0; b:  0 ),   { VERMELHO }
                 ( r:  0; g:255; b:  0 ),   { VERDE    }
                 ( r:  0; g:  0; b:255 ),   { AZUL     }
                 ( r:255; g:255; b:  0 ),   { AMARELO  }
                 ( r:  0; g:255; b:255 ),   { CIANO    }
                 ( r:255; g:  0; b:255 ),   { MAGENTA  }
                 ( r:128; g:128; b:128 ),   { CINZA    }
                 ( r:255; g:204; b:  0 ),   { DOURADO  }
                 ( r:255; g:128; b:  0 ),   { LARANJA  }
                 ( r:204; g:128; b: 51 ),   { MARROM   }
                 ( r:204; g: 51; b: 51 ),   { SALMAO   }
                 ( r:229; g:117; b:117 ),   { ROSA     }
                 ( r: 36; g: 36; b:143 ),   { MARINHO  }
                 ( r:117; g: 46; b:117 ),   { VIOLETA  }
                 ( r:  0; g:128; b:  0 ),   { MUSGO    }
                 ( r:  0; g:  0; b:  0 ),   { PRETO    }
                 ( r:255; g:255; b:255 ),   { BRANCO   }
                 ( r:192; g:192; b:192 ),   { CINZA CLARO }
                 ( r: 64; g: 64; b: 64 )    { CINZA ESCURO }
    );

var
    ModoDither : integer;      { Modo grafico, para a procedure de dither }
    trataMachBand: boolean;    { Usa randomico ou nao }
    arqRGB: file;              { controle da gravacao da imagem }
    ultYgrav, xgrav, ygrav, dxgrav: integer;
    CorGrav: ^LGRAV;
    ArqImagemAberto: boolean;
    NomeArqImagem: string;
    TabPal: array [0..255] of RGB; { Tabela de cores usada pela NCE3D }
    CorFundo: byte;                { Fundo utilizado na InicZBuffer3d }

{--------------------------------------------------------}
{                 Procedures Globais                     }
{--------------------------------------------------------}

procedure VGADither   (xt,yt: integer; corRabs,corGabs,corBabs: byte);
procedure ReInicBufRead;
procedure InicBufRead;
procedure LibBufRead;
function  xreadbyte   (var arq: file; var b: byte): boolean;
function  xreadint    (var arq: file; var x: integer): boolean;
procedure InicBufWrite;
procedure LibBufWrite (var arq: file);
procedure xwritebyte  (var arq: file; b: byte);
procedure xwriteint   (var arq: file; x: integer);

procedure  guardaCor24bits (x, y: integer; corR, corG, corB: byte);

{--------------------------------------------------------}

implementation
uses graph;

{--------------------------------------------------------}
{     Pinta um ponto em modo 256 utilizando dither       }
{--------------------------------------------------------}

procedure VGADither (xt, yt: integer; corRabs, corGabs, corBabs: byte);
const
    fat60 : longint = trunc(1.0 / 255.0 * (60-1) * 16 * 128);
var
    randomico: integer;
    cor, corR, corG, corB: integer;
    vermelho, verde, azul: integer;
    dit: integer;
    corC, cinza: longint;

    {--------------------------------------------------------}

    procedure calcCorDither;
    const
         fat1 : integer = trunc(1.0 / 255 * (7-1) * 16 * 128);
         fat2 : integer = trunc(1.0 / 255 * (4-1) * 16 * 128);
    begin
        corR := corRabs * fat1 + randomico;
        vermelho := corR shr (4+7);
        corR := corR - vermelho shl (4+7);
        if corR > dit then
            vermelho := vermelho+1;

        corG := corGabs * fat1 + randomico;
        verde := corG shr (4+7);
        corG := corG - verde shl (4+7);
        if corG > dit then
            verde := verde+1;

        corB := corBabs * fat2 + randomico;
        azul := corB shr (4+7);
        corB := corB - azul shl (4+7);
        if corB > dit then
            azul := azul+1;
    end;

    {--------------------------------------------------------}

begin
    if trataMachBand then
        randomico := random(128)
    else
        randomico := 0;
    dit := tabdither [((yt and 3) shl 2) or (xt and 3)] shl 7;

    if (corRabs = corGabs) and
       (corRabs = corBabs) then
        begin   { tons de cinza }
            corC := corRabs * fat60 + randomico;
            cinza := corC shr (4+7);
            corC := corC - cinza shl (4+7);
            if corC > dit then
                cinza := cinza+1;
            putpixel (xt, yt, 196+cinza);
        end
    else
        begin   { dither colorido }
            calcCorDither;
            putpixel (xt, yt, vermelho*28 + verde*4 + azul);
        end;
end;

{--------------------------------------------------------}
{        Rotinas de Entrada e Saida bufferizada          }
{--------------------------------------------------------}

const
   TAM_BUFFER = 4096;

type
   BUFFER = array [0..TAM_BUFFER-1] of byte;

var
   BufRead, BufWrite: ^BUFFER;
   NumRead, ReadPtr, WritePtr:  word;

{--------------------------------------------------------}

procedure ReInicBufRead;
begin
   NumRead:= 0;
   ReadPtr:= 0;
end;

{--------------------------------------------------------}

procedure InicBufRead;
begin
   new (BufRead);
   ReInicBufRead;
end;

{--------------------------------------------------------}

procedure LibBufRead;
begin
   Dispose (BufRead);
end;

{--------------------------------------------------------}

function xreadbyte (var arq: file; var b: byte): boolean;
begin
   if ReadPtr = NumRead then begin
           blockread ( arq, BufRead^, TAM_BUFFER, NumRead );
           ReadPtr:= 0;
           if NumRead = 0 then
               begin
                   xreadbyte:= TRUE;
                   exit;
               end;
   end;
   b:= BufRead^[ReadPtr];
   ReadPtr:= ReadPtr + 1;
   xreadbyte:= FALSE;
end;

{--------------------------------------------------------}

function xreadint (var arq: file; var x: integer): boolean;
var b1, b2: byte;
begin
    if xreadbyte(arq,b1) then
        begin
            xreadint:= true;
            exit;
        end;
    if xreadbyte(arq,b2) then
        begin
            xreadint:= true;
            exit;
        end;
    x := (b2 shl 8) or b1;
    xreadint:= false;
end;

{--------------------------------------------------------}

procedure InicBufWrite;
begin
   new (BufWrite);
   WritePtr:= 0;
end;

{--------------------------------------------------------}

procedure LibBufWrite (var arq: file);
begin
   if WritePtr > 0 then
        blockwrite ( arq, BufWrite^, WritePtr );
   Dispose (BufWrite);
end;

{--------------------------------------------------------}

procedure xwritebyte (var arq: file; b: byte);
begin
   BufWrite^[WritePtr]:= b;
   WritePtr:= WritePtr + 1;
   if WritePtr = TAM_BUFFER then
       begin
           blockwrite ( arq, BufWrite^, TAM_BUFFER );
           WritePtr:= 0;
       end;
end;

{--------------------------------------------------------}

procedure xwriteint (var arq: file; x: integer);
var b1, b2: byte;
begin
    b1:= x and $ff;          { parte baixa }
    b2:= (x shr 8) and $ff;  { parte alta }
    xwritebyte(arq,b1);
    xwritebyte(arq,b2);
end;

{--------------------------------------------------------}
{         guarda imagem rgb de 24 bits no disco          }
{   testar se o arquivo esta aberto antes de chamar!!!   }
{--------------------------------------------------------}

procedure  guardaCor24bits (x, y: integer; corR, corG, corB: byte);
var
    i: integer;
    rr, gg, bb: byte;

begin
    if ultYgrav = -1 then    { Inicio do arquivo }
        begin
            assign (arqRGB, NomeArqImagem);
            {$I-}  rewrite (arqRGB,1); {$I+}
            if ioresult <> 0 then
                begin
                    closegraph;
                    writeln ('*** Nome de arquivo invalido [',
                             NomeArqImagem, ']. ***');
                    writeln ( '*** Programa Cancelado. ***' );
                    halt;
                end;

            with TabPal[CorFundo] do
                begin
                    rr:= trunc(r*255/63);
                    gg:= trunc(g*255/63);
                    bb:= trunc(b*255/63);
                    xwritebyte (arqRGB, rr);
                    xwritebyte (arqRGB, gg);
                    xwritebyte (arqRGB, bb);
                end;
            ultYgrav := y;
            ygrav := y;
            xgrav := x;
            dxgrav := 0;
        end;

    if (ultYgrav <> y) or (x <> xgrav+dxgrav ) then
        begin
            { grava prefixo da linha }
            xwriteint (arqRGB, xgrav);
            xwriteint (arqRGB, ygrav);
            xwriteint (arqRGB, dxgrav);
            { grava linha de pixels }
            for i:= 1 to dxgrav do
                with corGrav^[i] do
                    begin
                        xwritebyte (arqRGB,r);
                        xwritebyte (arqRGB,g);
                        xwritebyte (arqRGB,b);
                    end;

            xgrav := x;
            ygrav := y;
            dxgrav := 1;
        end
    else
        dxgrav := dxgrav + 1;

    with corGrav^[dxgrav] do
        begin
            r := corR;
            g := corG;
            b := corB;
        end;

    ultYgrav := y;
{
    if y = -1 then
        begin
            close (arqRGB);
        end;
}
end;

{--------------------------------------------------------}
{                    Inicializacoes                      }
{--------------------------------------------------------}

begin
    { Default: nao gera arquivo de imagem 24 bits }
    ArqImagemAberto:= FALSE;
    NomeArqImagem:= '';
    ultYgrav := -1;
    CorFundo := 0;
end.