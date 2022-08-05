{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
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
{              Mauricio Nunes da Costa Bomfim                 }
{                                                             }
{                                                             }
{-------------------------------------------------------------}

unit NCE3D;

interface

uses graph, crt, dos, arqRGB3d;

const
    { Modos de video disponiveis - InicVideo3d }
    { Driver CGA.BGI }
    CGAC0       =  0;   { 320 x 200 - 4 cores }
    CGAHI       =  4;   { 640 x 200 - 2 cores }

    { Driver EGAVGA.BGI }
    EGAHI       =  1;   { 640 x 350 -  4 tons de cinza }
    VGAHI       =  2;   { 640 x 480 -  4 tons de cinza }
    EGADEFAULT  = 93;   { 640 x 350 - 16 cores }

    { Driver HERC.BGI }
    HERCULES    = 94;   { 720 x 348 - 2 cores }

    { Driver VGA256.BGI }
    VGADEFAULT  = 95;   { 320 x 200 - 256 cores }
    VGA256      = 96;   { 320 x 200 - 256 cores }

    { Driver SVGA256.BGI }
    SVGADEFAULT = 97;   { 640 x 480 - 256 cores }
    SVGA256     = 98;   { 640 x 480 - 256 cores }

    SVGAHI256   = 99;   { 800 x 600 - 256 cores }

    { Cores disponiveis para SelecCorPolig3d, SelecCorTraco3d e }
    { pintura em degrade }
    VERMELHO    =  0;
    VERDE       =  1;
    AZUL        =  2;
    AMARELO     =  3;
    CIANO       =  4;
    MAGENTA     =  5;
    CINZA       =  6;
    DOURADO     =  7;
    LARANJA     =  8;
    MARROM      =  9;
    SALMAO      = 10;
    ROSA        = 11;
    MARINHO     = 12;
    VIOLETA     = 13;
    MUSGO       = 14;
    PRETO       = 15;
    BRANCO      = 16;
    CINZACLARO  = 17;
    CINZAESCURO = 18;

    { Modos de Pintura }
    PINTASOLIDO   = 0;
    PINTAARAMADO  = 1;

    { Modos de Transformacoes }
    NORMAL        = 0;
    HIERARQUICO   = 1;

type
    MATTRANSF   = array [1..4, 1..4] of real;

    PONTO       = record
                      x, y, z, cor: real;
                  end;

    COORDPTO    = record
                      x, y, z: real;
                  end;

    { Mantido apenas por compatibilidade com as versoes anteriores }
    VERTICES    = array [1..12] of PONTO;

var
    MatAtual: MATTRANSF;

{-------------------------------------------------------------}
{    Arquivo INIC3D.PAS                                       }
{-------------------------------------------------------------}

procedure InicVideo3d     (opcao: integer);
procedure Janela3d        (nD, nF, dimRealX, dimRealY: real;
                           X, Y, nptX, nptY: integer);
procedure Termina3d;

{-------------------------------------------------------------}
{    Arquivo TRANSF3D.PAS                                     }
{-------------------------------------------------------------}

procedure MultMat3d            (m: MATTRANSF);
procedure InicTransf3d;
procedure SelecModoTransf3d    (modo: integer);
procedure AplicaTransf3d       (x,y,z: real; var xm,ym,zm: real);
procedure AplicaTransfObjeto3d (x,y,z: real; var xm,ym,zm: real);
procedure SalvaTransf3d;
procedure RecuperaTransf3d;
procedure Translada3d          (tx,ty,tz: real);
procedure RodaX3d              (ang: real);
procedure RodaY3d              (ang: real);
procedure RodaZ3d              (ang: real);
procedure Escala3d             (sx,sy,sz: real);
procedure Observador3d         (x1,y1,z1, x2,y2,z2: real);

{-------------------------------------------------------------}
{    Arquivo RETAS3D.PAS                                      }
{-------------------------------------------------------------}

procedure SelecRGBTraco3d     (r,g,b: real);
procedure SelecCorTraco3d     (cor: integer);
procedure Reta3d              (x1,y1,z1, x2,y2,z2: real);
procedure Move3d              (x,y,z: real);
procedure Risca3d             (x,y,z: real);
function  ProjetaPonto3d      (x,y,z: real; var xt,yt: integer) : boolean;
procedure PutPixel3d          (x,y,z: real; cor: word);
procedure Xis3d               (x,y,z: real);
function  MoveCursor3d        (var x,y,z: real) : char;
procedure AbreArqAnimacao3d   (nomearq: string);
procedure FechaArqAnimacao3d;
procedure FechaCenaAnimacao3d;

{-------------------------------------------------------------}
{    Arquivo ZBUF3D.PAS                                       }
{-------------------------------------------------------------}

function  CalcCorAproximada3d (r,g,b: real) : word;
procedure InicZBuffer3d       (MaxLinMemoria:integer; fundo:word;
                               path:string);
procedure LiberaZBuffer3d;
procedure AbreArqImagem3d     (nomeArq: string);
procedure FechaArqImagem3d;
procedure TiraMachBand3d      (op: boolean);
procedure SelecModoPintura3d  (modo: integer);
procedure PintaPolig3d        (nvert: integer; var vert);
procedure PintaPonto3d        (pto: PONTO);

{-------------------------------------------------------------}
{    Arquivo LUZ3D.PAS                                        }
{-------------------------------------------------------------}

procedure SelecLuz3d         (lx,ly,lz: real);
procedure SelecLuzAmbiente3d (luz: real);
procedure CalcNormPlano3d    (nv: integer; var v; var nx,ny,nz: real);
procedure CalcCor3d          (nx,ny,nz: real; var cor: real);
procedure SelecCorPolig3d    (cor: integer);
procedure SelecRGBPolig3d    (corR,corG,corB: real);

{-------------------------------------------------------------}
{    Arquivo SUPERF3D.PAS                                     }
{-------------------------------------------------------------}

          {----- Curvas -----}
procedure PtoBezier3d      (t:real; np:integer; var p; var xb,yb,zb:real);
procedure CurvaBezier3d    (np:integer; var p);
procedure PtoBSpline3d     (t: real; i: integer; var p;
                            var xb,yb,zb: real);
procedure CurvaBSpline3d   (np: integer; var p);
procedure PtoHermit3d      (t: real; i,np: integer; var p;
                            var xb,yb,zb: real);
procedure CurvaHermit3d    (np: integer; var p);

          {----- Superficies -----}
procedure CalcSupBezier3d  (var pc; nptu,nptv: integer; du, dv: real;
                            var sup; var iu,iv: integer);
procedure CalcSupBSpline3d (var pc; nptu,nptv: integer; du,dv: real;
                            var sup; var iu,iv: integer);
Procedure CalcSupHermit3d  (var pc; nptu,nptv: integer; du,dv: real;
                            var sup; var iu,iv: integer);
procedure DesenhaSuperf3d  (var sup; nptu,nptv: integer);

{-------------------------------------------------------------}
{    Arquivo SOLIDO3D.PAS                                     }
{-------------------------------------------------------------}

procedure Cubo3d       (h1,h2,h3: real);
procedure Anel3d       (n: integer; r,h: real);
procedure Cilindro3d   (n: integer; r,h: real);
procedure Cone3d       (n: integer; r,h: real);
procedure Esfera3d     (nivel: integer; raio: real);
procedure SemiEsfera3d (nivel: integer; raio: real);
procedure Toro3d       (n1,n2: integer; r1,r2: real);

{-------------------------------------------------------------}

implementation

const
    NBYTESZBUF = SizeOf(word);   { Tamanho de cada elemento do ZBuffer }
    YMAX       = 600;            { Dimensao maxima da tela em Y }
    XMAX       = 800;            { Dimensao maxima da tela em X }
    MAX_VERT_POLIG  = 255;
    MAX_VERT_RECORT = MAX_VERT_POLIG + 30;  { Suposicao... }

type
    PLZBUF    = ^LZBUF;
    LZBUF     = array [0..XMAX] of word;
    ZBUFFER   = array [0..YMAX] of PLZBUF;

    PPILHA    = ^PILHA;
    PILHA     = record
                   m: MATTRANSF;
                   ptr: PPILHA;
                end;

    PCOORDPTO = ^ COORDPTO;

    MALHA     = array [0..2000] of PONTO;

    PCONTR    = array [0..600] of COORDPTO;

    POLIG     = array [1..MAX_VERT_POLIG] of PONTO;
    POLIG_AUX = array [1..MAX_VERT_RECORT] of PONTO;
    PPOLIG_AUX= ^ POLIG_AUX;

const
    matident: MATTRANSF =
                 ( (1, 0, 0, 0),
                   (0, 1, 0, 0),
                   (0, 0, 1, 0),
                   (0, 0, 0, 1) );

var
    MatOlho      : MATTRANSF;   { Matriz de transformacoes do olho }
    ModoTransf   : integer;     { Modo de transformacoes corrente: }
                                { NORMAL ou HIERARQUICO }
    EMDISCO      : PLZBUF;
    jaAlocouZbuf : boolean;
    zbuf         : ZBUFFER;     { Buffer que armazena a }
                                { coordenada zt de cada pixel da tela }
    arqZOvl      : file;        { Arquivo de overlay do ZBuffer. }
    ovlNovo      : boolean;     { Ja' fez overlay de zbuffer antes }
    ovlSorteado  : integer;     { Parte do zbuffer a ser tirada da mem. }

    luzAmbiente,                { Intensidade da luz ambiente }
    luzx, luzy, luzz,           { Posicao do ponto de luz }
    ultx, ulty, ultz  : real;   { Ultimo ponto usado por Move3d e Risca3d }

    corRpoli,
    corGpoli,
    corBpoli     : byte;       { Cor em RGB corrente }

    ModoPintura  : integer;    { Modo de preenchimento de poligonos }
                               { corrente ( aramado ou solido ). }

    D,                    { Distancia do primeiro plano de recorte (lente) }
    F,                    { Distancia do ultimo plano de recorte (fundo) }
    Sx, Sy,               { Tamanho de meia janela em cm }
    Vsx, Vsy,             { Pontos por meia janela }
    Vcx, Vcy   : real;    { Coordenadas do centro da janela }
    TamX, TamY : integer; { Dimensoes da janela em numero de pixels }

    PilhaMat   : PPILHA;       { Pilha para matrizes de transformacao. }
    emArquivo  : boolean;      { Indica se deve ser gerado preview da }
                               { animacao em arquivo. }
    arqAnima   : file of byte; { Arquivo para preview da animacao. }


function  LongMul          (a,b: integer): longint;             forward;
procedure JogaEmArquivo    (x1,y1, x2,y2:integer);              forward;
procedure ConvParaTela     (var xt,yt,zt:real; vert:PONTO);     forward;
procedure PintaComDither   (xt, yt:integer; cor:real);          forward;
procedure Normaliza        (var nx,ny,nz: real);                forward;
procedure AplicaTransfOlho (x,y,z: real;  var xm,ym,zm: real);  forward;

{$I INIC3D.PAS   }
{$I TRANSF3D.PAS }
{$I RETAS3D.PAS  }
{$I ZBUF3D.PAS   }
{$I LUZ3D.PAS    }
{$I SUPERF3D.PAS }
{$I SOLIDO3D.PAS }

{-------------------------------------------------------------}
{                                                             }
{                  Inicializacao Geral                        }
{                                                             }
{-------------------------------------------------------------}

var
    i: integer;

begin
    { Pilha de matrizes de transformacao }
    PilhaMat:= NIL;

    { ZBuffer nao alocado }
    jaAlocouZbuf:= FALSE;
    for i:= 0 to YMAX do
        zbuf[i] := NIL;

    { Variaveis de controle de overlay do Zbuffer }
    ovlNovo := true;
    EMDISCO := addr(mem [$0000:$0001]);

    { Nao gera preview da animacao em arquivo }
    emArquivo := FALSE;

    { Inicializa matriz de transformacoes }
    InicTransf3d;

    { Inicializa modo de transformacoes }
    SelecModoTransf3d ( NORMAL );

    { Posicao da luz default }
    SelecLuz3d ( 100, 100, -100 );

    { Posicao do observador default }
    Observador3d ( 0, 0, -100, 0, 0, 0 );

    { Cor RGB default para preenchiento de poligonos }
    SelecCorPolig3d ( CINZA );

    { Intensidade de luz ambiente default }
    SelecLuzAmbiente3d ( 0.2 );

    { Modo de pintura de poligono default }
    SelecModoPintura3d ( PINTASOLIDO );

    { Gera randomico para reduzir efeito MachBand }
    TiraMachBand3d ( TRUE );
end.