{-----------------------------------------------------------}
{                                                           }
{   QUANTPCX - Quantizador de imagens geradas pelo NCE3D    }
{                                                           }
{   Entrada: Arquivo .RGB ( gerado com AbreArqImagem3d )    }
{   Saida (opcional): Arquivo .PCX                          }
{                                                           }
{-----------------------------------------------------------}

program QuantPCX;
uses crt, graph, NCE3D, UtNCE3D;

label
    FIM;
var
    nomeArq, nomeArqPCX: string;
    c: char;


begin
    InicVideo3d (SVGADefault);
    directvideo:= false;

    while TRUE do
        begin
            cleardevice;
            setrgbpalette (15,63,63,63);
            textcolor (15);
            gotoxy (1,1);
            writeln ('QuantPCX');
            writeln ('Quantiza imagens geradas pelo NCE3D');
            writeln;

            if ParamCount = 0 then
                begin
                    write('Nome do arquivo de entrada (.RGB): ');
                    readln(nomearq);
                    if nomearq = '' then
                        begin
                            Termina3d;
                            halt;
                        end;

                    write ('Nome do arquivo de saida (.PCX): ' );
                    readln (nomearqPCX);
                end
            else
                begin
                    nomearq := ParamStr(1);
                    nomearqPCX := ParamStr(2);
                end;

            writeln ('Aguarde...');
            QuantizaImagem (nomearq);

            if NomeArqPCX <> '' then
                gravaPCX (nomeArqPCX,0,0,getmaxx,getmaxy);

            if ParamCount = 0 then
                c:= readkey
            else
                goto FIM;
        end;

FIM:
    Termina3d;
end.
