unit princ;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, StdCtrls, Buttons, ExtCtrls;

type
  TForm3 = class(TForm)
    MainMenu1: TMainMenu;
    Executar1: TMenuItem;
    Configurar1: TMenuItem;
    Sobre1: TMenuItem;
    Executar2: TMenuItem;
    Sair1: TMenuItem;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    BitBtn3: TBitBtn;
    BitBtn4: TBitBtn;
    Image1: TImage;
    procedure Configurar1Click(Sender: TObject);
    procedure Executar2Click(Sender: TObject);
    procedure Sair1Click(Sender: TObject);
    procedure Sobre1Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
    procedure BitBtn3Click(Sender: TObject);
    procedure BitBtn4Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form3: TForm3;

implementation

uses config, SUSTO1, sobre;

{$R *.DFM}
////////////////////////////////////////////////////////////////////////////
procedure TForm3.Configurar1Click(Sender: TObject);
begin
   Form2.Show;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.Executar2Click(Sender: TObject);
begin
   Form1.Show;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.Sair1Click(Sender: TObject);
begin
   Close;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.Sobre1Click(Sender: TObject);
begin
   form4.show;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.BitBtn1Click(Sender: TObject);
begin
   Form1.Show;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.BitBtn2Click(Sender: TObject);
begin
   Form2.Show;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.BitBtn3Click(Sender: TObject);
begin
   form4.show;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm3.BitBtn4Click(Sender: TObject);
begin
   close;
end;

end.
