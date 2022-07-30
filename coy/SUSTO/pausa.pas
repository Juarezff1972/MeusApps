unit pausa;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TForm5 = class(TForm)
    Label1: TLabel;
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form5: TForm5;

implementation

uses SUSTO1;

{$R *.DFM}

procedure TForm5.Button1Click(Sender: TObject);
begin
   form1.timer1.enabled:=true;
   Close;
end;

end.
