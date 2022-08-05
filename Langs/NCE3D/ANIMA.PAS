{--------------------------------------------------------}
{                                                        }
{  ANIMA.PAS                                             }
{                                                        }
{  Le imagens do disco e grava em Video K7               }
{                                                        }
{  Autor: Mauricio Bomfim                                }
{                                                        }
{  Em agosto de 1992                                     }
{                                                        }
{--------------------------------------------------------}

program Anima;

uses
    crt, graph, ncemenu, nce3d;

const
    GRAVAR = FALSE;

var
    i,
    ntelas,
    codigo   : integer;
    resp,
    nome,
    extensao : string;


{--------------------------------------------------------}
{        Grava uma cena na ilha de edicao                }
{--------------------------------------------------------}

procedure GravaCena;

   begin
      if GRAVAR then
         begin
            if i = 0 then
                begin
                  GravaPrimeiraCena3d (120);
                  delay(40000);
                end
            else if i >= ntelas-1 then
                begin
                  GravaDemaisCenas3d (120,1);
                  delay(40000);
                end
            else
                begin
                  GravaDemaisCenas3d (1,1);
                  delay (24000);
                end;
         end;

   end;


{--------------------------------------------------------}

begin

    InicVideo3d (VGA256);
    nome:= LeTexto ( 20,20, 'Nome base dos arquivos ?' );
    resp:= LeTexto ( 50,20, 'Numero de arquivos ?' );
    val ( resp, ntelas, codigo );

    for i:= 0 to ntelas-1 do
        begin
            str (i:1, extensao);
            LeTela3d ( nome + '.' + extensao );
            GravaCena;
        end;

    Termina3d;

end.








