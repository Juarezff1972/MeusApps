unit Principal;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ToolWin, ComCtrls, ExtCtrls, Menus,Resolve;

type
  TfrmPrincipal = class(TForm)
    MainMenu1: TMainMenu;
    StatusBar1: TStatusBar;
    Timer1: TTimer;
    ToolBar1: TToolBar;
    ImageList1: TImageList;
    tbUsuarios: TToolButton;
    tbAtendentes: TToolButton;
    tbSistema: TToolButton;
    ToolButton4: TToolButton;
    ToolButton5: TToolButton;
    tbExecutar: TToolButton;
    ToolButton7: TToolButton;
    ToolButton8: TToolButton;
    tbSair: TToolButton;
    Configuracao: TMenuItem;
    Executar: TMenuItem;
    Sair: TMenuItem;
    Usuarios: TMenuItem;
    Atendentes: TMenuItem;
    Sistema: TMenuItem;
    Ajuda: TMenuItem;
    Sobre1: TMenuItem;
    Bevel1: TBevel;
    Iniciar: TMenuItem;
    Parar: TMenuItem;
    procedure FormActivate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure tbSairClick(Sender: TObject);
    procedure SairClick(Sender: TObject);
    procedure tbUsuariosClick(Sender: TObject);
    procedure UsuariosClick(Sender: TObject);
    procedure tbAtendentesClick(Sender: TObject);
    procedure AtendentesClick(Sender: TObject);
    procedure tbSistemaClick(Sender: TObject);
    procedure SistemaClick(Sender: TObject);
    procedure tbExecutarClick(Sender: TObject);
    procedure IniciarClick(Sender: TObject);
    procedure PararClick(Sender: TObject);
    procedure Sobre1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
 frmPrincipal: TfrmPrincipal;
implementation

uses Usuarios, Atendentes, Sistema, Sobre;

{$R *.DFM}

procedure TfrmPrincipal.FormActivate(Sender: TObject);
begin
  StatusBar1.Panels.Items[1].Text:=FormatDateTime('dddddd',Date);
end;

procedure TfrmPrincipal.Timer1Timer(Sender: TObject);
begin
  StatusBar1.Panels.Items[2].Text:=FormatDateTime('tt',Time);
end;

procedure TfrmPrincipal.tbSairClick(Sender: TObject);
begin
  if MessageBox(frmPrincipal.Handle,'Deseja Realmente Sair',
    'Susto',MB_YesNo or MB_IconQuestion )=IDYes then
  begin
    Application.Terminate;
  end;
end;

procedure TfrmPrincipal.SairClick(Sender: TObject);
begin
  tbSairClick(Sender);
end;

procedure TfrmPrincipal.tbUsuariosClick(Sender: TObject);
begin
  frmUsuarios:=TfrmUsuarios.create(self);
  frmUsuarios.ShowModal;
end;

procedure TfrmPrincipal.UsuariosClick(Sender: TObject);
begin
  tbUsuariosClick(Sender);
end;

procedure TfrmPrincipal.tbAtendentesClick(Sender: TObject);
begin
  frmAtendentes:=TfrmAtendentes.create(self);
  frmAtendentes.ShowModal;
end;

procedure TfrmPrincipal.AtendentesClick(Sender: TObject);
begin
  tbAtendentesClick(Sender);
end;

procedure TfrmPrincipal.tbSistemaClick(Sender: TObject);
begin
  frmSistema:=TfrmSistema.create(self);
  frmSistema.ShowModal;
end;

procedure TfrmPrincipal.SistemaClick(Sender: TObject);
begin
  tbSistemaClick(Sender);
end;

procedure TfrmPrincipal.tbExecutarClick(Sender: TObject);
begin
 if tbExecutar.ImageIndex=4 then
 begin
  // Inicia a execução
  tbExecutar.ImageIndex:=3;
  StatusBar1.Panels.Items[0].Text:='Executando o algoritmo de filas';
  Resolve.Executa;
 end
 else
 begin
  // Para a execução
  tbExecutar.ImageIndex:=4;
 end;
end;

procedure TfrmPrincipal.IniciarClick(Sender: TObject);
begin
  tbExecutarClick(Sender);
end;

procedure TfrmPrincipal.PararClick(Sender: TObject);
begin
  tbExecutarClick(Sender);
end;

procedure TfrmPrincipal.Sobre1Click(Sender: TObject);
begin
  frmSobre:=TfrmSobre.Create(self);
  frmSobre.ShowModal;
end;

end.
