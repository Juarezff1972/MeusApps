unit config;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Spin, Buttons, ComCtrls, Tabnotbk, ExtCtrls;

type
  TForm2 = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    SpinEdit1: TSpinEdit;
    SpinEdit2: TSpinEdit;
    SpinEdit3: TSpinEdit;
    SpinEdit4: TSpinEdit;
    SpinEdit5: TSpinEdit;
    SpinEdit6: TSpinEdit;
    SpinEdit7: TSpinEdit;
    BitBtn1: TBitBtn;
    TabbedNotebook1: TTabbedNotebook;
    Label8: TLabel;
    Label9: TLabel;
    Label18: TLabel;
    Label19: TLabel;
    Label21: TLabel;
    Label20: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    TabbedNotebook2: TTabbedNotebook;
    Label10: TLabel;
    SpinEdit8: TSpinEdit;
    Label17: TLabel;
    Label11: TLabel;
    SpinEdit9: TSpinEdit;
    Label12: TLabel;
    SpinEdit10: TSpinEdit;
    Label13: TLabel;
    SpinEdit13: TSpinEdit;
    Label27: TLabel;
    GroupBox2: TGroupBox;
    Label29: TLabel;
    SpinEdit15: TSpinEdit;
    Label30: TLabel;
    Label31: TLabel;
    SpinEdit16: TSpinEdit;
    Label32: TLabel;
    SpinEdit17: TSpinEdit;
    Label36: TLabel;
    SpinEdit20: TSpinEdit;
    Label37: TLabel;
    Label40: TLabel;
    SpinEdit22: TSpinEdit;
    Label41: TLabel;
    Label42: TLabel;
    SpinEdit23: TSpinEdit;
    Label43: TLabel;
    SpinEdit24: TSpinEdit;
    Label47: TLabel;
    SpinEdit27: TSpinEdit;
    Label48: TLabel;
    TabbedNotebook4: TTabbedNotebook;
    Label84: TLabel;
    Label85: TLabel;
    Label86: TLabel;
    Label87: TLabel;
    Label91: TLabel;
    Label92: TLabel;
    SpinEdit50: TSpinEdit;
    SpinEdit51: TSpinEdit;
    SpinEdit52: TSpinEdit;
    SpinEdit55: TSpinEdit;
    Label95: TLabel;
    Label96: TLabel;
    Label97: TLabel;
    Label98: TLabel;
    Label102: TLabel;
    Label103: TLabel;
    SpinEdit57: TSpinEdit;
    SpinEdit58: TSpinEdit;
    SpinEdit59: TSpinEdit;
    SpinEdit62: TSpinEdit;
    Label106: TLabel;
    Label107: TLabel;
    Label108: TLabel;
    Label109: TLabel;
    Label113: TLabel;
    Label114: TLabel;
    SpinEdit64: TSpinEdit;
    SpinEdit65: TSpinEdit;
    SpinEdit66: TSpinEdit;
    SpinEdit69: TSpinEdit;
    TabbedNotebook5: TTabbedNotebook;
    Label117: TLabel;
    Label118: TLabel;
    Label119: TLabel;
    Label120: TLabel;
    Label124: TLabel;
    Label125: TLabel;
    SpinEdit71: TSpinEdit;
    SpinEdit72: TSpinEdit;
    SpinEdit73: TSpinEdit;
    SpinEdit76: TSpinEdit;
    Label128: TLabel;
    Label129: TLabel;
    Label130: TLabel;
    Label131: TLabel;
    Label135: TLabel;
    Label136: TLabel;
    SpinEdit78: TSpinEdit;
    SpinEdit79: TSpinEdit;
    SpinEdit80: TSpinEdit;
    SpinEdit83: TSpinEdit;
    Label139: TLabel;
    Label140: TLabel;
    Label141: TLabel;
    Label142: TLabel;
    Label146: TLabel;
    Label147: TLabel;
    SpinEdit85: TSpinEdit;
    SpinEdit86: TSpinEdit;
    SpinEdit87: TSpinEdit;
    SpinEdit90: TSpinEdit;
    Label150: TLabel;
    SpinEdit92: TSpinEdit;
    SpinEdit93: TSpinEdit;
    Label151: TLabel;
    TabbedNotebook3: TTabbedNotebook;
    Label51: TLabel;
    Label52: TLabel;
    Label53: TLabel;
    Label54: TLabel;
    Label58: TLabel;
    Label59: TLabel;
    SpinEdit29: TSpinEdit;
    SpinEdit30: TSpinEdit;
    SpinEdit31: TSpinEdit;
    SpinEdit34: TSpinEdit;
    Label62: TLabel;
    Label63: TLabel;
    Label64: TLabel;
    Label65: TLabel;
    Label69: TLabel;
    Label70: TLabel;
    SpinEdit36: TSpinEdit;
    SpinEdit37: TSpinEdit;
    SpinEdit38: TSpinEdit;
    SpinEdit41: TSpinEdit;
    Label73: TLabel;
    Label74: TLabel;
    Label75: TLabel;
    Label76: TLabel;
    Label80: TLabel;
    Label81: TLabel;
    SpinEdit43: TSpinEdit;
    SpinEdit44: TSpinEdit;
    SpinEdit45: TSpinEdit;
    SpinEdit48: TSpinEdit;
    RadioGroup1: TRadioGroup;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    RadioButton3: TRadioButton;
    RadioButton4: TRadioButton;
    procedure BitBtn1Click(Sender: TObject);
    procedure SpinEdit1Change(Sender: TObject);
    procedure SpinEdit2Change(Sender: TObject);
    procedure SpinEdit3Change(Sender: TObject);
    procedure SpinEdit4Change(Sender: TObject);
    procedure SpinEdit5Change(Sender: TObject);
    procedure SpinEdit6Change(Sender: TObject);
    procedure SpinEdit7Change(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    chegam: array[1..7] of byte;
  end;

var
  Form2: TForm2;

implementation

{$R *.DFM}
////////////////////////////////////////////////////////////////////////////
procedure TForm2.BitBtn1Click(Sender: TObject);
begin
   Close;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit1Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[1]:=SpinEdit1.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit2Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[2]:=SpinEdit2.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit3Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[3]:=SpinEdit3.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit4Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[4]:=SpinEdit4.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit5Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[5]:=SpinEdit5.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit6Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[6]:=SpinEdit6.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.SpinEdit7Change(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[7]:=SpinEdit7.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;
////////////////////////////////////////////////////////////////////////////
procedure TForm2.FormCreate(Sender: TObject);
var aux : real;
    aux1: string;
begin
   aux:=(SpinEdit1.Value+SpinEdit2.Value+SpinEdit3.Value+SpinEdit4.Value+
         SpinEdit5.Value+SpinEdit6.Value+SpinEdit7.Value)/7;
   chegam[1]:=SpinEdit1.Value;
   chegam[2]:=SpinEdit2.Value;
   chegam[3]:=SpinEdit3.Value;
   chegam[4]:=SpinEdit4.Value;
   chegam[5]:=SpinEdit5.Value;
   chegam[6]:=SpinEdit6.Value;
   chegam[7]:=SpinEdit7.Value;
   str(aux:4:2,aux1);
   label9.caption:=aux1;
end;

end.
