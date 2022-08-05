{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit UtNCE3D                                     }
{            Rotinas utilitarias para o NCE3D                 }
{                                                             }
{            Arquivo include UtPCX.PAS                        }
{            Leitura e gravacao de arquivos em padrao PCX     }
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
{       Seleciona a palette que deve ser usada na leitura     }
{       de um arquivo PCX. Opcoes:                            }
{       UsaPaletteNCE3D, UsaPalettePCX e ConvertePalette.     }
{-------------------------------------------------------------}

procedure SelecModoLeituraPCX (modo: integer);
begin
    ModoLeituraPCX:= modo;
end;

{-------------------------------------------------------------}
{                 Rotinas de Gravacao de PCX                  }
{-------------------------------------------------------------}

procedure gravaPCX (nomearq: string; x0,y0,x1,y1: integer );
var
    header: headerPCX;
    vgaPal: palArray;

{-------------------------------------------------------------}

    procedure gravaHeaderPCX ( x0,y0,x1,y1: integer );
    const
        colorMapDefault: cmap = (
            (R:  0; G:  0; B:  0),
            (R:  0; G:  0; B:170),
            (R:  0; G:170; B:  0),
            (R:  0; G:170; B:170),
            (R:170; G:  0; B:  0),
            (R:170; G:  0; B:170),
            (R:170; G: 85; B:  0),
            (R:170; G:170; B:170),
            (R: 85; G: 85; B: 85),
            (R: 85; G: 85; B:255),
            (R: 85; G:255; B: 85),
            (R: 85; G:255; B:255),
            (R:255; G: 85; B: 85),
            (R:255; G: 85; B:255),
            (R:255; G:255; B: 85),
            (R:255; G:255; B:255) );

    var
        i: integer;
    begin
        with header do
            begin
                Manufacturer := 10;
	        Version := 5;
	        Encoding := 1;
	        BitsPerPixel := 8;
	        Wxmin := x0;
                Wymin := y0;
                Wxmax := x1;
                Wymax := y1;
  	        HRes := getmaxx+1;
	        VRes := getmaxy+1;
                Colormap := ColorMapDefault;
	        Reserved := 0;
	        NPlanes := 1;
	        BytesPerLine := x1-x0+1;
                PaletteInfo := 0;
                for i := 0 to 57 do
                    filler[i] := 0;
            end;

        blockwrite (PCXFileOut, header, sizeof (header));
    end;

    {-------------------------------------------------------------}

    procedure gravaCorpoPCX (x0,y0,x1,y1: integer);
    var nrep: integer;
        byteant, lido: byte;
        x,y: integer;

        procedure descarrega (b: byte; n: integer);
        var codrep: byte;
            rep:integer;
        begin
            while n > 0 do
                begin
                    if (n > 1) or ((b and $c0) = $c0) then
                        begin
                            rep := n;
                            if rep > 63 then rep := 63;
                            codrep := $c0 or rep;
                            xwritebyte (PCXFileOut, codrep);
                        end;

                    xwritebyte (PCXFileOut, b);
                    n := n - 63;
                end;
        end;

    begin
        InicBufWrite;
        for y := y0 to y1 do
            begin
                byteant := getPixel (x0, y);
                nrep := 1;

                for x := x0+1 to x1 do
                  begin
                    lido := GetPixel (x, y);
                    if lido = byteant then
                        nrep := nrep + 1
                    else
                        begin
                            descarrega (byteant, nrep);
                            byteant := lido;
                            nrep := 1;
                        end;
                  end;

                descarrega (byteant, nrep);
            end;
        LibBufWrite (PCXFileOut);
    end;

    {-------------------------------------------------------------}

    procedure gravaPalettePCX;
    var
        i: integer;
        b: byte;
        reg: registers;
    begin
        b := 12;
        blockwrite (pcxFileOut, b, 1);

        with reg do
            begin
                cx := 256;
                dx := ofs (vgaPal);
                es := seg (vgaPal);
                ax := $1017;
                bx := 0;
                intr ($10, reg);
            end;

        for i := 0 to 255 do
            with vgaPal[i] do
                begin
                    R := R shl 2;
                    G := G shl 2;
                    B := B shl 2;
                end;

        blockwrite (pcxFileOut, vgaPal, sizeof(palArray));
    end;

{-------------------------------------------------------------}
begin
    assign (PCXFileOut, nomearq+'.PCX');
    {$I-}  rewrite (pcxfileOut,1); {$I+}
    if ioresult <> 0 then
        begin
            Termina3d;
            clrscr;
            writeln ('*** Nome de arquivo invalido [', nomearq, ']. ***');
            writeln ( '*** Programa Cancelado. ***' );
            halt;
        end;

    gravaHeaderPCX (x0,y0,x1,y1);
    gravaCorpoPCX (x0,y0,x1,y1);
    gravaPalettePCX;

    close (PCXFileOut);
end;

{-------------------------------------------------------------}
{                 Rotinas de Leitura de PCX                   }
{-------------------------------------------------------------}

procedure LePCX (nomearq: string; x0, y0: integer);
var
    header: headerPCX;
    vgaPal: palArray;

{-------------------------------------------------------------}

    procedure ErroFatal;
    begin
        LibBufRead;
        Termina3d;
        clrscr;
        writeln ('*** Arquivo PCX nao reconhecido. ***');
        writeln ( '*** Programa Cancelado. ***' );
        halt;
    end;

    {-------------------------------------------------------------}

    procedure openFile (nomearq:string);
    begin
        assign (pcxfileIn, nomearq+'.PCX');
        {$I-}  reset (pcxfileIn,1); {$I+}
        if ioresult <> 0 then
            begin
                Termina3d;
                clrscr;
                writeln ('*** O arquivo ', nomearq, '.PCX nao foi encontrado. ***');
                writeln ( '*** Programa Cancelado. ***' );
                halt;
            end;
    end;

    {--------------------------------------------------------}

    procedure decodHeader;
    var
        i: integer;

    begin
        with header do
           begin
              if xreadbyte (pcxfileIn, manufacturer) then
                  ErroFatal;
              if xreadbyte (pcxfileIn, version) then
                  ErroFatal;
              if xreadbyte (pcxfileIn, encoding) then
                  ErroFatal;
              if xreadbyte (pcxfileIn, bitsPerPixel) then
                  ErroFatal;

              if xreadint(pcxFileIn,Wxmin) then
                  ErroFatal;
              if xreadint(pcxFileIn,Wymin) then
                  ErroFatal;
              if xreadint(pcxFileIn,Wxmax) then
                  ErroFatal;
              if xreadint(pcxFileIn,Wymax) then
                  ErroFatal;
              if xreadint(pcxFileIn,hres) then
                  ErroFatal;
              if xreadint(pcxFileIn,vres) then
                  ErroFatal;

              for i := 0 to 15 do
                  with ColorMap[i] do
                      begin
                          if xreadbyte (pcxfileIn, r) then
                              ErroFatal;
                          if xreadbyte (pcxfileIn, g) then
                              ErroFatal;
                          if xreadbyte (pcxfileIn, b) then
                              ErroFatal;
                      end;

              if xreadbyte (pcxfileIn, reserved) then
                   ErroFatal;
              if xreadbyte (pcxfileIn, nplanes) then
                   ErroFatal;
              if xreadint (pcxFileIn,bytesPerLine) then
                   ErroFatal;
              if xreadint (pcxFileIn,paletteInfo) then
                   ErroFatal;
           end;
    end;

    {--------------------------------------------------------}

    procedure decod256palette;
    var
        i: integer;
        fsize: longint;
        tag: byte;
    begin
        fsize := filesize (pcxfileIn);
        seek (pcxfileIn, fsize - 769);
        ReInicBufRead;
        if xreadbyte (pcxfileIn, tag) then
            ErroFatal;

        if tag = 12 then
            begin
                for i := 0 to 255 do
                    with vgaPal[i] do
                        begin
                            if xreadbyte (pcxfileIn, r) then
                                ErroFatal;
                            if xreadbyte (pcxfileIn, g) then
                                ErroFatal;
                            if xreadbyte (pcxfileIn, b) then
                                ErroFatal;
                            r := r shr 2;
                            g := g shr 2;
                            b := b shr 2;
                            if ModoLeituraPCX = UsaPalettePCX then
                                SetRGBPalette (i,r,g,b);
                        end;
            end;
    end;

    {--------------------------------------------------------}

    procedure decodFile (x0,y0: integer);
    var
        x, y: word;
        b, count, value: byte;
        i, j,
        YSize, TotalBytes: integer;

    begin
        seek (pcxfileIn, 128);
        ReInicBufRead;

        with header do
           begin
              YSize:= Wymax - Wymin + 1;
              TotalBytes := NPlanes * BytesPerLine;
              for y:= 1 to YSize do
                 begin
                    i:= 0;
                    while i < TotalBytes do
                       begin
                         if xreadbyte (pcxfileIn,b) then
                             ErroFatal;

                         if (b and $c0) = $c0 then
                             begin
                                 count := b and $3f;
                                 if xreadbyte (pcxfileIn,value) then
                                     ErroFatal;
                             end
                         else
                             begin
                                 count := 1;
                                 value := b;
                             end;

                         for j:= 0 to count-1 do
                            begin
                            if (x0+i <= getmaxx) and (y0+y <= getmaxy) then
                                begin
                                   if ModoLeituraPCX = ConvertePalette then
                                       VGADither (x0+i,y0+y,
                                                  VGAPal[value].r shl 2,
                                                  VGAPal[value].g shl 2,
                                                  VGAPal[value].b shl 2)
                                   else
                                       PutPixel (x0+i,y0+y,value);
                                end;
                            i:= i + 1;
                            end;
                       end;
                 end;
           end;
    end;

{--------------------------------------------------------}

begin
    openFile (nomearq);
    InicBufRead;
    decodHeader;

    if header.version = 5 then
        decod256palette;

    decodFile (x0,y0);
    LibBufRead;
    close (pcxfileIn);
end;
