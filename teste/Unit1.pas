unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  FileCtrl, StdCtrls, Grids, Outline, DirOutln;

type
  TForm1 = class(TForm)
    FileListBox1: TFileListBox;
    DriveComboBox1: TDriveComboBox;
    Label1: TLabel;
    Button1: TButton;
    Button2: TButton;
    procedure mostrar(Sender: TObject);
    procedure nome_arquivo(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.mostrar(Sender: TObject);
begin
   label1.Caption:=DriveComboBox1.Drive+' / '+DriveComboBox1.Text;
end;

procedure TForm1.nome_arquivo(Sender: TObject);
begin
   label1.caption:=FileListBox1.Drive+' / '+FileListBox1.Directory+' / '+FileListBox1.FileName;
end;

end.
