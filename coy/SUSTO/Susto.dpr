program susto;

uses
  Forms,
  SUSTO1 in 'SUSTO1.pas' {Form1},
  config in 'config.pas' {Form2},
  princ in 'princ.pas' {Form3},
  pausa in 'pausa.pas' {Form5},
  sobre in 'sobre.pas' {Form4};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Simulador de Filas do INAMPS';
  Application.CreateForm(TForm3, Form3);
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm5, Form5);
  Application.CreateForm(TForm4, Form4);
  Application.Run;
end.
