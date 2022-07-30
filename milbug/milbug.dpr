program milbug;

uses
  Forms,
  mil_bug in 'mil_bug.pas' {Form1};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
