unit Tela;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ComCtrls, ToolWin, Menus;

type
  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    ToolBar1: TToolBar;
    ToolBar2: TToolBar;
    StatusBar1: TStatusBar;
    Timer1: TTimer;
    PaintBox1: TPaintBox;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    ToolButton3: TToolButton;
    ToolButton4: TToolButton;
    ToolButton5: TToolButton;
    ToolButton6: TToolButton;
    ToolButton7: TToolButton;
    ToolButton8: TToolButton;
    ToolButton9: TToolButton;
    ToolButton10: TToolButton;
    ToolButton11: TToolButton;
    ToolButton12: TToolButton;
    ToolButton13: TToolButton;
    ToolButton14: TToolButton;
    ToolButton15: TToolButton;
    ToolButton16: TToolButton;
    ToolButton17: TToolButton;
    ToolButton18: TToolButton;
    ToolButton19: TToolButton;
    ToolButton20: TToolButton;
    ToolButton21: TToolButton;
    ToolButton22: TToolButton;
    ToolButton23: TToolButton;
    ToolButton24: TToolButton;
    ToolButton25: TToolButton;
    ToolButton26: TToolButton;
    ToolButton27: TToolButton;
    ToolButton28: TToolButton;
    ToolButton29: TToolButton;
    ImageList1: TImageList;
    ImageList2: TImageList;
    ToolButton30: TToolButton;
    ToolButton31: TToolButton;
    ToolButton32: TToolButton;
    ToolButton33: TToolButton;
    ToolButton34: TToolButton;
    ColorDialog1: TColorDialog;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    Arquivo1: TMenuItem;
    Novo1: TMenuItem;
    Abrir1: TMenuItem;
    Salvar1: TMenuItem;
    Sair1: TMenuItem;
    Editar1: TMenuItem;
    Voltar1: TMenuItem;
    Recortar1: TMenuItem;
    MaisZoom1: TMenuItem;
    MenosZoom1: TMenuItem;
    PrintDialog1: TPrintDialog;
    procedure Timer1Timer(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure ToolButton1Click(Sender: TObject);
    procedure ToolButton2Click(Sender: TObject);
    procedure ToolButton3Click(Sender: TObject);
    procedure ToolButton4Click(Sender: TObject);
    procedure ToolButton5Click(Sender: TObject);
    procedure ToolButton6Click(Sender: TObject);
    procedure ToolButton7Click(Sender: TObject);
    procedure ToolButton8Click(Sender: TObject);
    procedure ToolButton9Click(Sender: TObject);
    procedure ToolButton10Click(Sender: TObject);
    procedure ToolButton11Click(Sender: TObject);
    procedure ToolButton12Click(Sender: TObject);
    procedure ToolButton13Click(Sender: TObject);
    procedure ToolButton14Click(Sender: TObject);
    procedure ToolButton15Click(Sender: TObject);
    procedure ToolButton16Click(Sender: TObject);
    procedure ToolButton17Click(Sender: TObject);
    procedure ToolButton31Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure ToolButton34Click(Sender: TObject);
    procedure PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure ToolButton19Click(Sender: TObject);
    procedure ToolButton20Click(Sender: TObject);
    procedure Novo1Click(Sender: TObject);
    procedure ToolButton18Click(Sender: TObject);
    procedure Abrir1Click(Sender: TObject);
    procedure Salvar1Click(Sender: TObject);
    procedure Sair1Click(Sender: TObject);
    procedure ToolButton24Click(Sender: TObject);
    procedure ToolButton21Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  TipoDesenho:Byte;
  Ponto:TPoint;

implementation

{$R *.DFM}

procedure TForm1.Timer1Timer(Sender: TObject);
begin
   StatusBar1.Panels.Items[3].Text:=FormatDateTime('tt',Time);
end;

procedure TForm1.FormActivate(Sender: TObject);
begin
  StatusBar1.Panels.Items[2].Text:=FormatDateTime('dddddd',Date);
end;

procedure TForm1.PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  StatusBar1.Panels.Items[1].Text:='X: '+IntToSTR(X)+' Y: '+IntToStr(Y);
end;

procedure TForm1.ToolButton1Click(Sender: TObject);
begin
  ToolButton1.Down:= True;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
  TipoDesenho:=1;
  StatusBar1.panels.Items[0].Text:='Ponto';
end;

procedure TForm1.ToolButton2Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= True;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
  TipoDesenho:=2;
  StatusBar1.panels.Items[0].Text:='Reta';
end;

procedure TForm1.ToolButton3Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= True;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
  TipoDesenho:=3;
  StatusBar1.panels.Items[0].Text:='Retângulo';
end;

procedure TForm1.ToolButton4Click(Sender: TObject);
begin
  ToolButton1.Down:=False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= True;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
  TipoDesenho:=4;
  StatusBar1.panels.Items[0].Text:='Elipse';
end;

procedure TForm1.ToolButton5Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= True;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;

end;

procedure TForm1.ToolButton6Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= True;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton7Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= True;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton8Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= True;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton9Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= True;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton10Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=True;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton11Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=True;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton12Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=True;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
end;

procedure TForm1.ToolButton13Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=True;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton14Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=True;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton15Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=True;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton16Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=True;
  ToolButton17.Down:=False;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton17Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=True;
  ToolButton21.Down:=False;
end;

procedure TForm1.ToolButton31Click(Sender: TObject);
begin
  if MessageBox(Form1.Handle,'Deseja Realmente Sair','Sair',
     MB_YESNO OR MB_ICONQUESTION)=IDYES then
  begin
    Application.Terminate;
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  TipoDesenho:=0;
end;

procedure TForm1.PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var b:string;
begin
  Canvas.Pen.Color:=ColorDialog1.Color;
  case TipoDesenho of
   1:
    begin
    end;
   2:
    begin
      Canvas.MoveTo(X,Y+55);
    end;
   3:
    begin
      Ponto.x:=X;
      Ponto.y:=Y+55;
    end;
    4:
    begin
      Ponto.x:=X;
      Ponto.y:=Y+55;
    end;
    5:
      begin
      end;
   21:
    begin
      Ponto.x:=X;
      Ponto.y:=Y+55;
    end;
   end;
end;

procedure TForm1.ToolButton34Click(Sender: TObject);
begin
  ColorDialog1.Execute;
end;

procedure TForm1.PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  case TipoDesenho of
    1:
      begin
         Canvas.Pixels[X,Y+55]:=ColorDialog1.Color;
      end;
    2:
     begin
       Canvas.LineTo(X,Y+55);
     end;
    3:
     begin
       Canvas.Rectangle(Ponto.x,Ponto.y,X,Y+55);
     end;
    4:
     begin
       Canvas.Ellipse(Ponto.x,Ponto.y,X,Y);
     end;
    5:
    begin
    end;
    21:
     begin
     end;
  end;
end;

procedure TForm1.ToolButton19Click(Sender: TObject);
begin
  OpenDialog1.Execute;
end;

procedure TForm1.ToolButton20Click(Sender: TObject);
begin
  SaveDialog1.Execute;
end;

procedure TForm1.Novo1Click(Sender: TObject);
begin
  ToolButton18Click(Sender);
end;

procedure TForm1.ToolButton18Click(Sender: TObject);
begin
  if MessageBox(Form1.Handle,'Deseja Salvar a Figura','Novo',
     MB_YESNO or MB_ICONQUESTION)=IDYES then
     SaveDialog1.Execute
  else
    PaintBox1.Refresh;
end;

procedure TForm1.Abrir1Click(Sender: TObject);
begin
  ToolButton19Click(Sender);
end;

procedure TForm1.Salvar1Click(Sender: TObject);
begin
  ToolButton20Click(Sender);
end;

procedure TForm1.Sair1Click(Sender: TObject);
begin
  ToolButton31Click(Sender);
end;

procedure TForm1.ToolButton24Click(Sender: TObject);
begin
  PrintDialog1.Execute;
end;

procedure TForm1.ToolButton21Click(Sender: TObject);
begin
  ToolButton1.Down:= False;
  ToolButton2.Down:= False;
  ToolButton3.Down:= False;
  ToolButton4.Down:= False;
  ToolButton5.Down:= False;
  ToolButton6.Down:= False;
  ToolButton7.Down:= False;
  ToolButton8.Down:= False;
  ToolButton9.Down:= False;
  ToolButton10.Down:=False;
  ToolButton11.Down:=False;
  ToolButton12.Down:=False;
  ToolButton13.Down:=False;
  ToolButton14.Down:=False;
  ToolButton15.Down:=False;
  ToolButton16.Down:=False;
  ToolButton17.Down:=False;
  ToolButton21.Down:=True;
  TipoDesenho:=21;
  StatusBar1.Panels.Items[0].Text:='Selecionar';
end;

end.
