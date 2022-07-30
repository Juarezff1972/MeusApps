unit Sistema;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, ExtCtrls, Mask;

type
  TfrmSistema = class(TForm)
    btCancelar: TBitBtn;
    btOk: TBitBtn;
    Panel1: TPanel;
    RadioGroup1: TRadioGroup;
    A: TEdit;
    B: TEdit;
    C: TEdit;
    D: TEdit;
    Panel2: TPanel;
    Label2: TLabel;
    eTempo: TEdit;
    Label3: TLabel;
    meFaltas: TMaskEdit;
    procedure btCancelarClick(Sender: TObject);
    procedure btOkClick(Sender: TObject);
    procedure RadioGroup1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSistema: TfrmSistema;

implementation

uses Principal;

{$R *.DFM}

procedure TfrmSistema.btCancelarClick(Sender: TObject);
begin
 frmSistema.Close;
end;

procedure TfrmSistema.btOkClick(Sender: TObject);
begin
  frmPrincipal.Executar.Enabled:=True;
  frmPrincipal.TbExecutar.Enabled:=True;
  frmSistema.Close;
end;

procedure TfrmSistema.RadioGroup1Click(Sender: TObject);
var i:integer;
begin
  case RadioGroup1.ItemIndex of
    0:
    begin
     A.Enabled:=True;
     B.Enabled:=False;
     C.Enabled:=False;
     D.Enabled:=False;
    end;
    1:
    begin
     A.Enabled:=False;
     B.Enabled:=True;
     C.Enabled:=False;
     D.Enabled:=False;
    end;
    2:
    begin
     A.Enabled:=False;
     B.Enabled:=False;
     C.Enabled:=True;
     D.Enabled:=False;
    end;
    3:
    begin
     A.Enabled:=False;
     B.Enabled:=False;
     C.Enabled:=False;
     D.Enabled:=True;
    end;
  end;
end;

end.
