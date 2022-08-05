{--------------------------------------------------------}
{                                                        }
{  GRAVA.PAS                                             }
{                                                        }
{  Le imagens do disco e grava em Video                  }
{                                                        }
{  Autores: Mauricio e Antonio                           }
{                                                        }
{  Em fevereiro de 1993                                  }
{                                                        }
{--------------------------------------------------------}
program Grava;
uses
    crt, graph, nce3d, utnce3d;

var
    NomeArqPCX: string;
    nframes: integer;
    PrimeiraCena: boolean;

{--------------------------------------------------------}
{                Acusa erro de parametros                }
{--------------------------------------------------------}

procedure Aborta;
begin
  clrscr;
  writeln ('      GRAVA - Grava uma imagem PCX em video');
  writeln ('      -------------------------------------');
  writeln;
  writeln ('Uso:');
  writeln ('  grava <arq>                -> mostra arquivo na tela');
  writeln ('  grava <arq> <#frames>      -> grava <#frames> em video');
  writeln ('  grava <arq> <#frames> <P>  -> grava primeira cena em video');
  halt;
end;

{--------------------------------------------------------}
{           Critica os parametros do programa            }
{--------------------------------------------------------}

procedure CriticaParams;
var
    erro: integer;
begin
    nframes:= 0;
    PrimeiraCena:= FALSE;

    if ParamCount < 1 then
        Aborta;

    NomeArqPCX:= ParamStr(1);
    if ParamCount = 1 then
        exit;

    val (paramstr(2), nframes, erro);
    if erro <> 0 then
        Aborta;
    if ParamCount = 2 then
        exit;

    if ParamCount = 3 then
        if (ParamStr(3) = 'P') or (ParamStr(3) = 'p') then
            PrimeiraCena:= TRUE
        else
            Aborta
    else
        Aborta;
end;

{--------------------------------------------------------}
{        Grava uma cena na ilha de edicao                }
{--------------------------------------------------------}

procedure GravaCena;
begin
   if nframes = 0 then
       begin
           readln;
           exit;
       end;
   if PrimeiraCena then
       GravaPrimeiraCena (nframes)
   else
       GravaDemaisCenas (nframes,1);
   delay (28000 +  nframes * 100);
end;

{--------------------------------------------------------}

begin
    CriticaParams;
    InicVideo3d (SVGA256);
    LePCX ( NomeArqPCX, 0,0 );
    GravaCena;
    Termina3d;
end.
