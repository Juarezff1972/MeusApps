{-------------------------------------------------------------}
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit NCE3D                                       }
{                                                             }
{            Arquivo include INIC3D.PAS                       }
{            Modulo de Inicializacao                          }
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
{                                                             }
{       Inicializa o video para uso das funcoes 3D            }
{                                                             }
{       Opcoes:                                               }
{                                                             }
{         CGAC0      320X200     4 cores                      }
{                                                             }
{         CGAHI      640X200     2 cores                      }
{                                                             }
{         HERCULES   720X348     2 cores                      }
{                                                             }
{         EGAHI      640X350     4 tons de cinza              }
{                                                             }
{         VGAHI      640X480     4 tons de cinza              }
{                                                             }
{         VGA256     320x200     7 tons R e G e 4 B +         }
{                               60 tons de cinza.             }
{                                                             }
{         VGADefault 320X200   64 cores                       }
{                       ( usando a paleta default )           }
{                                                             }
{         SVGA256    640X480     7 tons R e G e 4 B +         }
{                               60 tons de cinza.             }
{                                                             }
{         SVGADefault640X480   64 cores                       }
{                       ( usando a paleta default )           }
{                                                             }
{         SVGAHI256  800X600     7 tons R e G e 4 B +         }
{                               60 tons de cinza.             }
{                                                             }
{-------------------------------------------------------------}

procedure InicVideo3d ( opcao: integer );

     const
         tabr: array [0..6] of byte = (0,19,27,35,44,53,63);
         tabg: array [0..6] of byte = (0,19,27,35,44,53,63);
         tabb: array [0..3] of byte = (0,27,44,63);

     var
         i, ir, ig, ib, cor: integer;
         Palette : PaletteType;


{-------------------------------------------------------------}

     {$F+}
     function VGA320X200: Integer;
     begin
         VGA320X200 := 1;
     end;

     function SVGA640X480: Integer;
     begin
         SVGA640X480 := 2;
     end;

     function SVGA800X600: Integer;
     begin
         SVGA800X600 := 3;
     end;
     {$F-}

{-------------------------------------------------------------}

     procedure TrataErro ( CodErro:shortint );
     begin
         clrscr;
         case CodErro of
           grFileNotFound:
                 begin
                     writeln ('*** Driver BGI nao foi encontrado. ' +
                              'Copie-o para este diretorio ***');
                 end;
           grInvalidMode:
                 begin
                    writeln ('*** Sua placa de video nao permite ' +
                             'a utilizacao deste modo grafico. ***');
                 end;
           grNoLoadMem:
                 begin
                    writeln ('*** Nao ha memoria suficiente para ' +
                             'carregar o driver BGI. ***');
                    writeln ('*** Procure a DAU/NCE ***');
                 end;
           else
                 begin
                    writeln ('*** Erro nao identificado na ' +
                             'inicializacao do modo grafico. ***');
                    writeln ('*** Procure a DAU/NCE ***');
                 end;
         end;

         writeln ('*** Programa Cancelado... ***');
         halt;

     end;

{-------------------------------------------------------------}

     procedure InicVideo ( placa,modo : integer );
     var
        CodErro: shortint;
     begin
         initgraph (placa,modo,'');
         CodErro:= GraphResult;
         if CodErro <> grOk then
             TrataErro ( CodErro );
     end;

{-------------------------------------------------------------}

     procedure InicVideo256 (opcao:integer);
     var modo, placa, i: integer;
         regs: registers;
         CodErro: shortint;

     begin
         case opcao of
            VGA256:    placa := installuserdriver ('VGA256', @VGA320X200);
            SVGA256:   placa := installuserdriver ('SVGA256',@SVGA640X480);
            SVGAHI256: placa := installuserdriver ('SVGA256',@SVGA800X600);
         end;
         placa := detect;

         initgraph(placa,modo,'');
         CodErro:= shortint(GraphResult);
         if CodErro <> grOk then
             TrataErro ( CodErro );

         if opcao = VGA256 then
            begin
              regs.ax := $13;
              intr( $10, regs);
            end;

     end;

{-------------------------------------------------------------}

begin

    case opcao of

       SVGA256,
       SVGAHI256,
       VGA256:    begin
                     InicVideo256(opcao);

                     { Inicializa os tons RGB }
                     for ir:= 0 to 6 do
                         for ig:= 0 to 6 do
                             for ib:= 0 to 3 do
                                 begin
                                     i:= ir*28 + ig*4 + ib;
                                     with TabPal[i] do
                                         begin
                                             r:= tabr[ir];
                                             g:= tabg[ig];
                                             b:= tabb[ib];
                                             setrgbpalette (i, r,g,b);
                                         end;
                                 end;


                     { inicializa os 60 tons de cinza }
                     for i := 196 to 199 do
                         begin
                             cor := (i - 196) * 2;
                             with TabPal[i] do
                                 begin
                                     r:= cor;
                                     g:= cor;
                                     b:= cor;
                                     setrgbpalette (i, r,g,b);
                                 end;
                         end;
                     for i := 200 to 255 do
                         begin
                             cor := i - 192;
                             with TabPal[i] do
                                 begin
                                     r:= cor;
                                     g:= cor;
                                     b:= cor;
                                     setrgbpalette (i, r,g,b);
                                 end;
                         end;
                  end;

       VGADEFAULT:
                  begin
                     InicVideo256 (VGA256);
                     setcolor (100);
                  end;

       SVGADEFAULT:
                  begin
                     InicVideo256 (SVGA256);
                     setcolor (100);
                  end;

       VGAHI: begin
                 InicVideo ( VGA,VGAHi );

                 with Palette do
                    begin
                      Size      := 4;
                      Colors[0] := $00;
                      Colors[1] := $07;
                      Colors[2] := $38;
                      Colors[3] := $3F;
                      SetAllPalette(Palette);
                    end;

              end;

       EGAHI: begin
                 InicVideo ( EGA,EGAHi );

                 with Palette do
                    begin
                      Size      := 4;
                      Colors[0] := $00;
                      Colors[1] := $07;
                      Colors[2] := $38;
                      Colors[3] := $3F;
                      SetAllPalette(Palette);
                    end;

              end;

       EGADEFAULT:
              InicVideo ( EGA,EGAHi );

       CGAC0: InicVideo ( CGA,CGAC0 );

       CGAHI: InicVideo ( CGA,CGAHI );

       HERCULES: InicVideo ( HercMono, HercMonoHi );

       ELSE  begin
               clrscr;
               writeln ('*** Modo Grafico Incorreto. ***');
               writeln ('*** Programa cancelado... ***');
               halt;
             end;
    end;


    { Salva modo grafico utilizado, }
    { para fazer dither convenientemente }
    ModoDither:= opcao;

    { Cria janela default }
    Janela3d ( 40,            { distancia do olho a lente }
               160,           { distancia do olho ao fundo }
               20.0, 15.0,    { tamanho real da janela }
               0, 0,          { posicao da janela }
               getmaxx+1,
               getmaxy+1 );   { numero de pontos da janela }

end;


{-------------------------------------------------------------}
{           Finaliza modo grafico, liberando memoria          }
{-------------------------------------------------------------}

procedure Termina3d;

var
   p: PPILHA;
   yt: integer;

begin
   while pilhaMat <> NIL do
       begin
          p:= pilhaMat^.ptr;
          dispose(pilhaMat);
          pilhaMat:= p;
       end;

   FechaArqImagem3d;
   LiberaZbuffer3d;
   closegraph;
end;


{-------------------------------------------------------------}
{               Seleciona uma janela                          }
{-------------------------------------------------------------}

procedure Janela3d (nD, nF, dimRealX, dimRealY:real;
                     X, Y, nptX, nptY: integer);
begin

    D   := nD;
    F   := nF;
    TamX:= nptX;
    TamY:= nptY;
    Sx  := dimRealX / 2.0;
    Sy  := dimRealY / 2.0;

    Vsx :=  (nptX-1) / 2;
    Vsy := -(nptY-1) / 2;

    Vcx := X + (nptX-1) / 2;
    Vcy := Y + (nptY-1) / 2;

end;
