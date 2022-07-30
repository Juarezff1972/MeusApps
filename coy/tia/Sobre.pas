unit Sobre;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, ExtCtrls;

type
  TfrmSobre = class(TForm)
    Image1: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Bevel1: TBevel;
    BitBtn1: TBitBtn;
    Bevel2: TBevel;
    procedure FormActivate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSobre: TfrmSobre;

implementation

{$R *.DFM}

procedure TfrmSobre.FormActivate(Sender: TObject);
var a:string;
    b:PChar;
    c:integer;
begin
  Beep;
  end;


end.
