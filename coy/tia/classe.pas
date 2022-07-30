unit classe;

interface
 const
  MaxFila=40;
  MaxAtendente=3;

 type
  TUsuario = class
   private
    Problema:string; //define qual o problema do usuário
    Prioridade:string; //define qual a prioridade do seu problema
    MaxTempoEspera: real;//até quando o problema pode continuar
    TempoConsulta:real;//tempo para arrumar o sistema
   public
    constructor CUsuario;
    procedure LeValores;
    destructor DUsuario;
  end;

  TAtendente = class
   private
    Especialidade:string;//qual problema ele atende
    AtendePrioridade:boolean;//atende por prioridade
    Falta:boolean;//se atendente faltou ou não
    Fila:array [1..MaxFila] of TUsuario;
   public
    constructor CAtendente;
    procedure LeValores;
    destructor DAtendente;
  end;

  TSistema = class
   public
    NumAtendentes:array [1..4] of integer;
    TempoMedioConsulta:real;
    Faltas:real;
    constructor CSistema;
    procedure Executa;
    destructor DSistema;
  end;

implementation

uses Principal;
(*-------------------------------------------------------------*)
{                   Classe TUsuario                             }
////////////////////////////////////////////////////////////
// Instância o objeto
constructor TUsuario.CUsuario;
begin
 inherited Create;
 Problema:='Nenhum';
 Prioridade:='Nenhuma';
 MaxTempoEspera:=-1.0;
 TempoConsulta:=0.0;
end;
////////////////////////////////////////////////////////////
// Procedimento que permite o sistema acessar pra leitura os
// dados dos usuários
procedure TUsuario.LeValores;
begin
  //Form1.Edit1.Text:=Problema;
end;
////////////////////////////////////////////////////////////
// Desinstância o objeto
destructor TUsuario.DUsuario;
begin
  inherited Destroy;
end;
(*-------------------------------------------------------------*)

(*-------------------------------------------------------------*)
{                   Classe TAtendente                           }
////////////////////////////////////////////////////////////
// Instância o objeto
constructor TAtendente.CAtendente;
begin
  inherited Create;
  Especialidade:='Nenhuma';
  AtendePrioridade:=False;
  Falta:=False;
end;
////////////////////////////////////////////////////////////
// Procedimento que permite o sistema acessar pra leitura os
// dados dos atendentes
procedure TAtendente.LeValores;
begin
end;
////////////////////////////////////////////////////////////
// Desinstância o objeto
destructor TAtendente.DAtendente;
begin
  inherited Destroy;
end;
(*-------------------------------------------------------------*)
(*-------------------------------------------------------------*)
{                   Classe TSistema                             }
////////////////////////////////////////////////////////////
// Instância o objeto
constructor TSistema.CSistema;
var i:integer;
begin
  inherited Create;
  TempoMedioConsulta:=0;
  Faltas:=0;
  for i:=1 to 4 do
  begin
    NumAtendentes[i]:=0;
  end;
end;
////////////////////////////////////////////////////////////
// Procedimento que resolve o sistema
procedure TSistema.Executa;
begin
end;
////////////////////////////////////////////////////////////
// Desinstância o objeto
destructor TSistema.DSistema;
begin
  inherited Destroy;
end;
(*-------------------------------------------------------------*)

end.

