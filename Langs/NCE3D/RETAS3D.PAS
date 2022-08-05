{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include RETAS3D.PAS                      }
{            Modulo de Tracado de Retas com recorte           }
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
{                                                             }
{-------------------------------------------------------------}

{-------------------------------------------------------------}
{       Seleciona uma cor RGB para o tracado de retas         }
{-------------------------------------------------------------}

procedure SelecRGBTraco3d (r,g,b: real);
begin
    SetColor ( CalcCorAproximada3d ( r,g,b) );
end;

{-------------------------------------------------------------}
{         Seleciona uma cor para o tracado de retas           }
{-------------------------------------------------------------}

procedure SelecCorTraco3d (cor:integer);
begin
    with CoresRGB[cor] do
        SetColor ( CalcCorAproximada3d (r/255,g/255,b/255) );
end;

{-------------------------------------------------------------}
{                    Recorte de retas 3d                      }
{-------------------------------------------------------------}

procedure RecorteCanonico (var x1,y1,z1, x2,y2,z2: real;
                                        var sobrouReta: boolean);
const
    Cima  = 1;
    Baixo = 2;
    Esq   = 4;
    Dir   = 8;

var
    t,x,y,z: real;
    c, c1, c2: integer;

label fim;

{-------------------------------------------------------------}

    procedure formulaEsq;
    begin
        t := (z1 + x1) / ((x1 - x2) - (z2 - z1));
        z := t * (z2-z1) + z1;
        x := -z;
        y := t * (y2 - y1) + y1;
    end;

{-------------------------------------------------------------}

    procedure formulaDir;
    begin
        t := (z1 - x1) / ((x2 - x1) - (z2 - z1));
        z := t * (z2-z1) + z1;
        x := z;
        y := t * (y2 - y1) + y1;
    end;

{-------------------------------------------------------------}

    procedure formulaBaixo;
    begin
        t := (z1 + y1) / ((y1 - y2) - (z2 - z1));
        z := t * (z2-z1) + z1;
        y := -z;
        x := t * (x2 - x1) + x1;
    end;

{-------------------------------------------------------------}

    procedure formulaCima;
    begin
        t := (z1 - y1) / ((y2 - y1) - (z2 - z1));
        z := t * (z2-z1) + z1;
        y := z;
        x := t * (x2 - x1) + x1;
    end;

{-------------------------------------------------------------}

    procedure codPonto (x,y,z: real; var cod: integer);
    begin
        cod := 0;
        if x < -z then cod := Esq  else
        if x >  z then cod := Dir;
        if y < -z then cod := cod + Baixo  else
        if y >  z then cod := cod + Cima;
    end;

{-------------------------------------------------------------}

{ Corpo da Procedure RecorteCanonico }

begin
    codPonto (x1, y1, z1, c1);
    codPonto (x2, y2, z2, c2);

    sobrouReta := true;

    { o algoritmo repete ate que os dois pontos  }
    { tenham codigos 0 e 0, o que indica que     }
    { estao ambos dentro da piramide.            }

    while (c1 <> 0) or (c2 <> 0) do
        begin

        { se ambos os pontos estao fora, e do mesmo }
        { lado, abandona tracado.  E' facil ver que }
        { o "and logico" da um resultado diferente  }
        { de zero se algum bit e' igual             }

             if (c1 and c2) <> 0 then
                 begin
                     sobrouReta := false;
                     goto fim;
                 end;

             { escolhe um ponto que esteja fora }

             c := c1;
             if c = 0 then c := c2;

             { calcula as intersecoes usando as formulas }

             if (c and Esq) <> 0   then formulaEsq
             else
             if (c and Dir) <> 0   then formulaDir
             else
             if (c and Baixo) <> 0 then formulaBaixo
             else
             if (c and Cima) <> 0  then formulaCima;

             { substitui um dos pontos }

             if c = c1 then
                 begin
                     x1 := x;  y1 := y;  z1 := z;
                     codponto (x1, y1, z1, c1);
                 end
             else
                 begin
                     x2 := x;  y2 := y;  z2 := z;
                     codponto (x2, y2, z2, c2);
                 end;
        end;
fim:

end;


{-------------------------------------------------------------}

procedure  CanonParaTela (xc, yc, zc: real;
                          var xtela, ytela: integer);
begin
    xtela := round (xc/zc * VSx + VCx);
    ytela := round (yc/zc * VSy + VCy);
end;


{-------------------------------------------------------------}
{           Traca uma reta entre dois pontos 3d               }
{-------------------------------------------------------------}

procedure Reta3d ( x1,y1,z1, x2,y2,z2: real );

var
    xc1, yc1, zc1,
    xc2, yc2, zc2: real;
    xt1, yt1, xt2, yt2: integer;
    sobrouReta: boolean;

begin
    { transforma no sistema canonico }

    AplicaTransf3d (x1, y1, z1, xc1, yc1, zc1);
    xc1 := xc1 * D / Sx;
    yc1 := yc1 * D / Sy;
    zc1 := zc1;

    AplicaTransf3d (x2, y2, z2, xc2, yc2, zc2);
    xc2 := xc2 * D / Sx;
    yc2 := yc2 * D / Sy;
    zc2 := zc2;

    RecorteCanonico (xc1, yc1, zc1, xc2, yc2, zc2, sobrouReta);

    if sobrouReta then
        begin
            CanonParaTela (xc1, yc1, zc1, xt1, yt1);
            CanonParaTela (xc2, yc2, zc2, xt2, yt2);
            line (xt1, yt1, xt2, yt2);
            if emArquivo then
                jogaEmArquivo (xt1, yt1, xt2, yt2);
        end;
end;

{-------------------------------------------------------------}
{                 Move para um ponto 3d                       }
{-------------------------------------------------------------}

procedure Move3d  (x, y, z: real);
begin
    ultx := x;
    ulty := y;
    ultz := z;
end;

{-------------------------------------------------------------}
{                 Traca reta ate um ponto 3d                  }
{-------------------------------------------------------------}

procedure Risca3d (x, y, z: real);
begin
    reta3D (ultx, ulty, ultz, x, y, z);
    move3d (x, y, z);
end;

{-------------------------------------------------------------}
{                 Projeta um ponto XYZ na tela                }
{-------------------------------------------------------------}

function  ProjetaPonto3d  ( x,y,z:real; var xt,yt:integer ) : boolean;

var
    x1,y1,z1: real;
    SobrouReta: boolean;

begin
    AplicaTransf3d ( x,y,z, x1,y1,z1 );
    x1 := x1 * D / Sx;
    y1 := y1 * D / Sy;
    z1 := z1;

    RecorteCanonico (x1, y1, z1, x1, y1, z1, sobrouReta);

    if SobrouReta then
         CanonParaTela (x1, y1, z1, xt, yt);
    ProjetaPonto3d := SobrouReta;

end;

{-------------------------------------------------------------}
{      Pinta o ponto xyz com a cor especificada,              }
{      sem utilizar o ZBuffer.                                }
{-------------------------------------------------------------}

procedure PutPixel3d ( x,y,z:real; cor:word );
var
    xt, yt: integer;
begin
    if ProjetaPonto3d ( x,y,z, xt,yt ) then
        PutPixel (xt,yt,cor);
end;


{-------------------------------------------------------------}
{           Desenha um X centrado num ponto dado              }
{-------------------------------------------------------------}

procedure xis3d ( x,y,z: real );
   var
      xt, yt: integer;
   begin                   { Esta' meio errado ... }
      if ProjetaPonto3d ( x,y,z, xt,yt ) then
          begin
             line ( xt-2,yt-2, xt+2,yt+2 );
             line ( xt-2,yt+2, xt+2,yt-2 );
          end;
   end;

{-------------------------------------------------------------}
{                     ImageSize modificada                    }
{-------------------------------------------------------------}

function TamImagem (x1,y1, x2,y2: integer): longint;
    begin
        TamImagem:= LongMul (x2-x1+1, y2-y1+1) + 6;
    end;

{-------------------------------------------------------------}
{       Aguarda movimentacao do cursor em 3D.                 }
{       Setas andam no plano XY, <+> e <-> andam em Z.        }
{       F1 diminui o passo, F2 aumenta o passo.               }
{       Qualquer tecla diferente para.                        }
{-------------------------------------------------------------}

function MoveCursor3d ( var x,y,z: real ) : char;

   const
      TECCIMA  = #72;
      TECBAIXO = #80;
      TECESQ   = #75;
      TECDIR   = #77;
      F1       = #59;
      F2       = #60;

   var
      c: char;
      passo: real;
      acabou: boolean;

{-------------------------------------------------------------}

   procedure eixos;
     var
         xt,yt: integer;
     begin
         reta3d ( -20,0,0, 20,0,0 );
         if ProjetaPonto3d (20,0,0,xt,yt) then
            OutTextXY (xt+3,yt-3,'X');
         reta3d ( 0,-20,0, 0,20,0 );
         if ProjetaPonto3d (0,20,0,xt,yt) then
            OutTextXY (xt+3,yt-3,'Y');
         reta3d ( 0,0,-20, 0,0,20 );
         if ProjetaPonto3d (0,0,20,xt,yt) then
            OutTextXY (xt+3,yt-3,'Z');
     end;

{-------------------------------------------------------------}

   procedure cursor;
     var
         corAnt: word;
     begin
         corAnt:= GetColor;
         if (ModoDither = VGA256) or (ModoDither = SVGA256) or
            (ModoDither = SVGAHI256) then
             selecCorTraco3d (vermelho)
         else
             setColor (red);

         xis3d ( x,y,z );
         SetLineStyle (DottedLn,0,NormWidth);
         reta3d ( x,0,z, x,0,0 );
         reta3d ( x,0,z, 0,0,z );
         reta3d ( x,0,z, x,y,z );
         SetLineStyle (SolidLn,0,NormWidth);
         SetColor (corAnt);
     end;

{-------------------------------------------------------------}

   procedure legenda;
     var
         s: string;
         posy: integer;

     begin
         setviewport (0,0, TextWidth('X =88888'), 3*TextHeight('X'), ClipOn);
         clearviewport;

         posy:= 0;
         str ( x:5:1, s );
         OutTextXY ( 0,posy,'X ='+s );

         posy:= posy + TextHeight('X');
         str ( y:5:1, s );
         OutTextXY ( 0,posy,'Y ='+s );

         posy:= posy + TextHeight('Y');
         str ( z:5:1, s );
         OutTextXY ( 0,posy,'Z ='+s );

         setviewport ( 0, 0, getmaxx, getmaxy, ClipOn );
     end;

{-------------------------------------------------------------}

var
   P: pointer;
   size: longint;
   TextInfo: TextSettingsType;

begin
(*
   { Salva a tela atual }
   Size := TamImagem(0,0,getmaxx,getmaxy);
   if Size > MemAvail then
       begin
          Termina3d;
          clrscr;
          writeln ('*** Memoria insuficiente para usar MoveCursor3d ',
                   'neste modo grafico. ***');
          writeln ('*** Programa cancelado... ***');
          Halt;
       end;
   GetMem(P, Size);
   { obs: driver VGA256 da' erro se der PutImage na ultima coluna da tela }
   GetImage(0,0,getmaxx-1,getmaxy-1,P^);
*)
   { Salva parametros de escrita de texto }
   GetTextSettings (TextInfo);
   SetTextStyle ( DefaultFont, Horizdir, 1 );

   acabou:= false;
   passo:= 1;
   setwritemode (XORPUT);
   eixos;
   repeat
        cursor;
        legenda;
        c:= readkey;
        cursor;
        if c=#00 then    { Analisa caracteres especiais. }
             begin
                 c:= readkey;
                 case c of
                     TECCIMA:  y:= y+passo;
                     TECBAIXO: y:= y-passo;
                     TECESQ:   x:= x-passo;
                     TECDIR:   x:= x+passo;
                     F1:       if passo>0.5 then
                                  passo:= passo/2;
                     F2:       if passo<4 then
                                  passo:= passo*2;

                     else
                         begin
                           MoveCursor3d := c;
                           acabou:= true;
                         end;
                 end;
             end
        else
             case c of
                 '+': z:= z+passo;
                 '-': z:= z-passo;
                 else
                     begin
                       MoveCursor3d := c;
                       acabou:= true;
                     end;
             end;
    until acabou;
    eixos;
    setwritemode (NORMALPUT);

    { Restaura parametros de escrita de texto }
    with TextInfo do
        SetTextStyle (Font, Direction, CharSize );
(*
    { Restaura a tela original }
    PutImage(0, 0, P^, NormalPut);
    FreeMem(P, Size);
*)
end;

{---------------------------------------------------------}
{                                                         }
{    Rotinas para saida em arquivo do preview da          }
{    animacao em wireframe.                               }
{    ( Arquivo .FRA usado pelo programa sh )              }
{                                                         }
{---------------------------------------------------------}

procedure AbreArqAnimacao3D (nomearq: string);
begin
    emArquivo := true;
    assign ( arqAnima, nomearq + '.fra' );
    rewrite (arqAnima);
end;

{-------------------------------------------------------------}

procedure FechaArqAnimacao3D;
begin
    {$I-} close(arqAnima);  {$I+}
    if ioresult <> 0 then;
    emArquivo := false;
end;

{-------------------------------------------------------------}

procedure JogaEmArquivo (x1, y1, x2, y2: integer);
var b: byte;
begin
    b := x1 and $ff;
    write (arqAnima, b);
    b := (x1 shr 8) and $ff;
    write (arqAnima, b);
    b := y1;
    write (arqAnima, b);
    b := x2 and $ff;
    write (arqAnima, b);
    b := (x2 shr 8) and $ff;
    write (arqAnima, b);
    b := y2;
    write (arqAnima, b);
end;

{-------------------------------------------------------------}

procedure FechaCenaAnimacao3d;
begin
    jogaEmArquivo (-1,0,0,0);
end;
