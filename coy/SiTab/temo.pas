unit temo;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Gauges;

type
  TTermometro = class(TForm)
    Gauge1: TGauge;
    Gauge2: TGauge;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Termometro: TTermometro;

implementation

{$R *.DFM}


end.
