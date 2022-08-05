{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit UtNCE3D                                     }
{            Rotinas utilitarias para o NCE3D                 }
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

Unit UtNCE3D;

interface
uses dos, crt, graph, ArqRGB3d, nce3d;

const
     {  Constantes para SelecModoLeituraPCX  }
     UsaPaletteNCE3D = 0;
     UsaPalettePCX   = 1;
     ConvertePalette = 2;

{-------------------------------------------------------------}
{           Pintura de poligonos em degrade                   }
{-------------------------------------------------------------}
function  XCentroDegrade     (n: integer): integer;
function  YCentroDegrade     (n: integer): integer;
procedure InicVertDegrade    (num,x,y,cor: integer);
procedure InicVertDegradeRGB (num,x,y: integer; r,g,b: byte);
procedure InicCirculoDegrade (xc,yc,raio,ndiv: integer);
procedure PintaPoligDegrade  (nvert: integer);
procedure DegradeLinear      (nvert, dx,dy, cor1, cor2: integer);
procedure DegradeLinearRGB   (nvert, dx,dy, r1,g1,b1, r2,g2,b2: integer);
procedure DegradeCircular    (nvert, xc,yc, raio, cor1,cor2: integer);
procedure DegradeCircularRGB (nvert, xc,yc, raio,
                              r1,g1,b1, r2,g2,b2: integer);

{-------------------------------------------------------------}
{          Gravacao e leitura de arquivos PCX                 }
{-------------------------------------------------------------}
procedure SelecModoLeituraPCX (modo: integer);
procedure gravaPCX (nomearq: string; x0,y0,x1,y1: integer);
procedure LePCX    (nomearq: string; x0,y0: integer);

{-------------------------------------------------------------}
{          Quantizacao de imagens geradas pelo NCE3D          }
{-------------------------------------------------------------}
procedure QuantizaImagem (nomearq: string);

{-------------------------------------------------------------}
{          Controle da placa para a ilha de edicao            }
{-------------------------------------------------------------}
procedure GravaPrimeiraCena ( incr:integer );
procedure GravaDemaisCenas  ( incr, decr:integer );


implementation

{-------------------------------------------------------------}
{    Rotinas de preenchimento de poligonos em Degrade         }
{-------------------------------------------------------------}
const
    MAXVERTSPOLIG = 30;
    MAXY = 599;

type
    PTO_DEGRADE = record
                     x,y : integer;
                     r,g,b : byte;
                   end;

    POLIG_DEGRADE = array [1..MAXVERTSPOLIG] of PTO_DEGRADE;

    PBUCKET = ^BUCKET;

    BUCKET = record
        xb: real;
        r,g,b: byte;
        pxbuc: PBUCKET;
    end;

var
    buc: array [0..MAXY] of PBUCKET;
    vert: POLIG_DEGRADE;

{-------------------------------------------------------------}
{    Rotinas de Gravacao e Leitura de Arquivos PCX            }
{-------------------------------------------------------------}
type
  palArray = array [0..255] of RGB;

  cmap = array [0..15] of RGB;

  headerpcx = record
	Manufacturer: byte;	{ Constant Flag  10 = ZSoft .PCX           }
	Version: byte;          { Version information:                     }
				{ 0 = Version 2.5                          }
				{ 2 = Version 2.8 w/palette information    }
				{ 3 = Version 2.8 w/o palette information  }
				{ 5 = Version 3.0                          }
	Encoding: byte;	        { 1 = .PCX run length encoding             }
	BitsPerPixel: byte;	{ Number of bits/pixel per plane           }
	Wxmin, Wymin,
        Wxmax, Wymax: integer;	{ Picture Dimensions                       }
				{ (Xmin, Ymin) - (Xmax - Ymax)             }
				{ in pixels, inclusive                     }
  	HRes: integer;          { Horizontal Resolution of creating device }
	VRes: integer;          { Vertical Resolution of creating device   }
	Colormap: cmap;         { Color palette setting, see text          }
	Reserved: byte;
	NPlanes: byte;  	{ Number of color planes                   }
	BytesPerLine: integer;	{ Number of bytes per scan line per        }
				{ color plane (always even for .PCX files) }
        PaletteInfo: integer;   { How to interpret palette: 1 - color/BW   }
				{ 2 = grayscale                            }
	Filler: array [0..57] of byte;
        	                { blank to fill out 128 byte header        }
end;

var
    PCXFileIn, PCXFileOut: file;
    ModoLeituraPCX: integer;

{-------------------------------------------------------------}
{    Quantizador para imagens geradas pelo NCE3D              }
{-------------------------------------------------------------}
const
    NBITS     = 5;      { Numero de bits por tom }
    MAXCOR    = 31;     { 2 ** NBITS - 1 }

    BITS_IGN  = 8 - NBITS;

    SHIFT_PAL =  32 div (MAXCOR+1);
    RANDOMICO = 256 div (MAXCOR+1);

type
    GBhisto = array [0..MAXCOR, 0..MAXCOR] of byte;
    RGBhisto = array [0..MAXCOR] of ^GBhisto;

    PRGB = record
               pr, pg, pb: byte;
           end;
    PRGBArray = array [0..10000] of PRGB;

var
    histo:  ^RGBhisto;
    phisto: ^PRGBArray;
    arqrgb: file;
    ncores: integer;

{-------------------------------------------------------------}
{      Rotinas de controle da placa para a ilha de edicao     }
{-------------------------------------------------------------}
const
   TEMPO  = 500;
   PORTA  = $378;     {  PARALELA  }

   TRIM_MAIS  = $df;
   TRIM_MENOS = $ef;
   ENTRY      = $f7;
   REC_OUT    = $bf;  { alterado em 11/05/93 }
   AUTO_EDIT  = $fd;
   REC_IN     = $fe;
   SOLTA      = $ff;

{-------------------------------------------------------------}
{            Implementacao (arquivos de include)              }
{-------------------------------------------------------------}

{$I UTDEGRAD.PAS   }
{$I UTQUANT.PAS    }
{$I UTPCX.PAS      }
{$I UTVIDEO.PAS    }

{-------------------------------------------------------------}
{                   Inicializacoes gerais                     }
{-------------------------------------------------------------}

begin
    SelecModoLeituraPCX (UsaPaletteNCE3D);
end.