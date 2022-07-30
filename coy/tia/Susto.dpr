program Susto;

uses
  Forms,
  Principal in 'Principal.pas' {frmPrincipal},
  Usuarios in 'Usuarios.pas' {frmUsuarios},
  Atendentes in 'Atendentes.pas' {frmAtendentes},
  Sistema in 'Sistema.pas' {frmSistema},
  Resolve in 'Resolve.pas',
  Sobre in 'Sobre.pas' {frmSobre};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TfrmPrincipal, frmPrincipal);
  Application.CreateForm(TfrmUsuarios, frmUsuarios);
  Application.CreateForm(TfrmAtendentes, frmAtendentes);
  Application.CreateForm(TfrmSistema, frmSistema);
  Application.CreateForm(TfrmSobre, frmSobre);
  Application.Run;
end.
