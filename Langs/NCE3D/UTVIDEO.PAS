{-------------------------------------------------------------}
{                                                             }
{                                                             }
{    NCE3D - Rotinas de Manuseio Tridimensional               }
{                                                             }
{            Unit UtNCE3D                                     }
{            Rotinas utilitarias para o NCE3D                 }
{                                                             }
{            Arquivo include UtVIDEO.PAS                      }
{            Rotinas de controle da placa para a              }
{            ilha de edicao do NCE                            }
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

procedure envia ( botao:byte );
begin
   port[PORTA]:= botao;
   delay(TEMPO);
end;


procedure marca_entrada;
begin
   envia(ENTRY);
   envia(ENTRY and REC_IN);
   envia(SOLTA);
end;


procedure marca_saida;
begin
   envia(ENTRY);
   envia(ENTRY and REC_OUT);
   envia(SOLTA);
end;


procedure incrementa_saida ( incr:integer );
var
   i: integer;
begin
   for i:=0 to incr-1 do
      begin
        envia(REC_OUT);
        envia(TRIM_MAIS and REC_OUT);
      end;
   envia(REC_OUT);
   envia(SOLTA);
end;


procedure decrementa_entrada ( decr:integer );
var
   i: integer;
begin
   for i:=0 to decr-1 do
      begin
        envia(REC_IN);
        envia(TRIM_MENOS and REC_IN);
      end;
   envia(REC_IN);
   envia(SOLTA);
end;


procedure autoedit;
begin
   envia(AUTO_EDIT);
   envia(SOLTA);
end;

{---------------------------------------------------------}

procedure GravaPrimeiraCena ( incr:integer );
begin
   marca_entrada;
   marca_saida;
   incrementa_saida(incr);
   autoedit;
end;

{---------------------------------------------------------}

procedure GravaDemaisCenas ( incr, decr:integer );
begin
   marca_entrada;
   marca_saida;
   incrementa_saida(incr);
   decrementa_entrada(decr);
   autoedit;
end;
