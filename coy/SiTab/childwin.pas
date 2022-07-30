unit Childwin;

interface

uses Windows, Classes, Graphics, Forms, Controls, ExtCtrls;

type
  TMDIChild = class(TForm)
    Imgchild: TImage;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormActivate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }

  end;

implementation

uses Main;

{$R *.DFM}

procedure TMDIChild.FormClose(Sender: TObject; var Action: TCloseAction);
begin
   mainform.sumatela(Sender);
   Action := caFree;
end;

procedure TMDIChild.FormActivate(Sender: TObject);
begin
   case tag of
      1 : mainform.lblzoom.caption:='Zoom: 6,25%';
      2 : mainform.lblzoom.caption:='Zoom: 12,5%';
      3 : mainform.lblzoom.caption:='Zoom: 25%';
      4 : mainform.lblzoom.caption:='Zoom: 50%';
      5 : mainform.lblzoom.caption:='Zoom: 100%';
      6 : mainform.lblzoom.caption:='Zoom: 200%';
      7 : mainform.lblzoom.caption:='Zoom: 300%';
      8 : mainform.lblzoom.caption:='Zoom: 400%';
      9 : mainform.lblzoom.caption:='Zoom: 500%';
   end;
end;

end.
