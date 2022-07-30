unit mil_bug;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls, Grids, Calendar, OleCtrls, graphsv3, Gauges;

type
  TForm1 = class(TForm)
    Timer1: TTimer;
    Label1: TLabel;
    Label3: TLabel;
    Label2: TLabel;
    ProgressBar1: TProgressBar;
    TrackBar1: TTrackBar;
    ScrollBar1: TScrollBar;
    Gauge1: TGauge;
    procedure Timer1Timer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  incr : integer;


implementation

{$R *.DFM}

procedure TForm1.Timer1Timer(Sender: TObject);
var data1 : tdatetime;
    Year, Month, Day, Hour, Min, Sec, MSec: Word;
    stringue : string;
begin
   Label1.Caption := 'Data/Hora Atual: ' + DateTimeToStr(Now);
   data1:=StrToDateTime('31/12/1999 23:59:59');
   data1:=data1-now;
   DecodeDate(data1, Year, Month, Day);
   DecodeTime(data1, Hour, Min, Sec, MSec);
   month:=month-1;
   day:=day+1;
   if year-1900<=1 then stringue:=inttostr(year-1900)+' ano' else stringue:=inttostr(year-1900)+' anos';
   if month<=1 then stringue:=stringue+', '+inttostr(month)+' mês' else stringue:=stringue+', '+inttostr(month)+' meses';
   if day<=1 then stringue:=stringue+', '+inttostr(day)+' dia' else stringue:=stringue+', '+inttostr(day)+' dias';
   if hour<=1 then stringue:=stringue+', '+inttostr(hour)+' hora' else stringue:=stringue+', '+inttostr(hour)+' horas';
   if min<=1 then stringue:=stringue+', '+inttostr(min)+' minuto' else stringue:=stringue+', '+inttostr(min)+' minutos';
   if sec<=1 then stringue:=stringue+', '+inttostr(sec)+' segundo' else stringue:=stringue+', '+inttostr(sec)+' segundos';
   progressbar1.position:=sec;
   trackbar1.position:=min;
   scrollbar1.position:=scrollbar1.position+incr;
   if scrollbar1.position=1000 then incr:=-1;
   if scrollbar1.position=0 then incr:=1;
   gauge1.progress:=1000-msec;
   label2.caption:=stringue;
end;


procedure TForm1.FormCreate(Sender: TObject);
begin
   incr:=1;
   scrollbar1.position:=0;
end;

end.
