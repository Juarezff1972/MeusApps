program sitab;

uses
  Forms,
  Main in 'MAIN.PAS' {MainForm},
  Childwin in 'CHILDWIN.PAS' {MDIChild},
  About in 'about.pas' {AboutBox},
  erro1 in 'erro1.pas' {Form1},
  temo in 'temo.pas' {Termometro};

{$R *.RES}

begin
  Application.Title := 'SiTab - Sistema de Interpretação de Tabelas';
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TAboutBox, AboutBox);
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TTermometro, Termometro);
  Application.Run;
end.
