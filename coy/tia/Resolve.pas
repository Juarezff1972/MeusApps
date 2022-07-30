unit Resolve;

interface
(*-------------------------------------------------------*)
(*                   Definição de Tipos                  *)
const
  MaxUsuarioFila=40;
  MaxAtendenteEspecial=3;

type
  Usuario=record
    Problema:string;
    Prioridade:byte;
    TempoEspera:integer;
    TempoConsulta:integer;
  end;

  Atendente=record
    Especialidade:string;
    AtendePrioridade:boolean;
    Falta:boolean;
    Atendendo:boolean;
    Fila:array [1..MaxUsuarioFila] of Usuario;
  end;
(*-------------------------------------------------------*)
(*-------------------------------------------------------*)
(*           Definição de Variáveis                      *)
var Atendentes:array[1..12] of Atendente;
(*-------------------------------------------------------*)
(*-------------------------------------------------------*)
(*           Definição de Procedimentos                  *)
procedure GeraAtendente(DiaSemana:integer);
procedure GeraUsuarios(DiaSemana:integer);
procedure VerificaFila;
procedure Dia(DiaSemana:integer);
procedure Executa;
(*-------------------------------------------------------*)
implementation
uses Dialogs,Principal,SysUtils,Sistema;
(*-------------------------------------------------------*)
///////////////////////////////////////////////////////
// Gera as faltas dos Atendentes conforme o Dia da
//  Semana
procedure GeraAtendente(DiaSemana:integer);
var i,soma,porcfalta,numero:integer;
begin
  soma:=StrToInt(frmSistema.A.Text)+StrToInt(frmSistema.B.Text)+
   StrToInt(frmSistema.C.Text)+StrToInt(frmSistema.D.Text);
  case DiaSemana of
    1:
     begin
     end;
    2:
     begin
     end;
    3:
     begin
     end;
    4:
     begin
     end;
    5:
     begin
     end;
    6:
     begin
     end;
    7:
     begin
     end;
  end;
  for i:=1 to porcfalta do
  begin
    randomize;
    numero:=random(soma);
    Atendentes[numero].Falta:=True;
  end;
end;
///////////////////////////////////////////////////////
// Gera a chegada dos usuários e seus problemas
//  conforme o dia da semana
procedure GeraUsuarios(DiaSemana:integer);
begin
  case DiaSemana of
    1:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
    2:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
    3:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
    4:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
    5:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
    6:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
    7:
     begin
       ShowMessage(IntToStr(DiaSemana));
     end;
  end;
end;
////////////////////////////////////////////////////////
// Verifica qual amenor fila de atendentes p/ o seu
//  problema livre e entra nesta fila
procedure VerificaFila;
begin
end;
///////////////////////////////////////////////////////
// Procedimento de controle do Dia
procedure Dia(DiaSemana:integer);
var i:integer;
begin
   GeraAtendente(DiaSemana);
   for i:=1 to 540 do
   begin
     GeraUsuarios(DiaSemana);
     VerificaFila;
     //Entra;
   end;
end;
///////////////////////////////////////////////////////
// Procedimento de controle do sistema
procedure Executa;
var i,DiaSemana:integer;
begin
  DiaSemana:=7;
  for i:=1 to 30 do
  begin
    if DiaSemana>7 then
       DiaSemana:=1;
    Dia(DiaSemana);
    Inc(DiaSemana);
  end;
  frmPrincipal.tbExecutar.ImageIndex:=4;
end;
(*-------------------------------------------------------*)

end.
