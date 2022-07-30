unit Main;

interface

uses Windows, SysUtils, Classes, Graphics, Forms, Controls, Menus,
  StdCtrls, Dialogs, Buttons, Messages, ExtCtrls, ComCtrls, ExtDlgs, ClipBrd;

type
  TMainForm = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    FileOpenItem: TMenuItem;
    FileCloseItem: TMenuItem;
    Window1: TMenuItem;
    Help1: TMenuItem;
    N1: TMenuItem;
    FileExitItem: TMenuItem;
    WindowCascadeItem: TMenuItem;
    WindowTileItem: TMenuItem;
    WindowArrangeItem: TMenuItem;
    HelpAboutItem: TMenuItem;
    FileSaveItem: TMenuItem;
    FileSaveAsItem: TMenuItem;
    Edit1: TMenuItem;
    CopyItem: TMenuItem;
    PasteItem: TMenuItem;
    WindowMinimizeItem: TMenuItem;
    SpeedPanel: TPanel;
    OpenBtn: TSpeedButton;
    SaveBtn: TSpeedButton;
    CopyBtn: TSpeedButton;
    PasteBtn: TSpeedButton;
    ExitBtn: TSpeedButton;
    StatusBar: TStatusBar;
    OpenPictureDialog: TOpenPictureDialog;
    SavePictureDialog: TSavePictureDialog;
    Contedo1: TMenuItem;
    Deteco1: TMenuItem;
    ClulasdaTabela1: TMenuItem;
    DetBtn: TSpeedButton;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    Visualizar1: TMenuItem;
    Toolbar1: TMenuItem;
    MaisZoom1: TMenuItem;
    MenosZoom1: TMenuItem;
    N2: TMenuItem;
    lblzoom: TLabel;
    Imagem: TImage;
    procedure FormCreate(Sender: TObject);
    procedure FileNewItemClick(Sender: TObject);
    procedure WindowCascadeItemClick(Sender: TObject);
    procedure UpdateMenuItems(Sender: TObject);
    procedure WindowTileItemClick(Sender: TObject);
    procedure WindowArrangeItemClick(Sender: TObject);
    procedure FileCloseItemClick(Sender: TObject);
    procedure FileOpenItemClick(Sender: TObject);
    procedure FileExitItemClick(Sender: TObject);
    procedure FileSaveItemClick(Sender: TObject);
    procedure FileSaveAsItemClick(Sender: TObject);
    procedure CopyItemClick(Sender: TObject);
    procedure PasteItemClick(Sender: TObject);
    procedure WindowMinimizeItemClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure HelpAboutItemClick(Sender: TObject);
    procedure ClulasdaTabela1Click(Sender: TObject);
    procedure DetBtnClick(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure MenosZoom1Click(Sender: TObject);
    procedure MaisZoom1Click(Sender: TObject);
    procedure Toolbar1Click(Sender: TObject);
    procedure CopyBtnClick(Sender: TObject);
    procedure PasteBtnClick(Sender: TObject);
    procedure SumaTela(Sender: TObject);
  private
    { Private declarations }
    procedure CreateMDIChild(const Name: string);
    procedure ShowHint(Sender: TObject);
  public
    { Public declarations }
    function telaativa : word;
  end;

var
 MainForm: TMainForm;

implementation

{$R *.DFM}

uses ChildWin, About, erro1, temo;

var tela : array[1..2000] of TMDIChild;
    bmpx : array[1..2000] of word;
    bmpy : array[1..2000] of word;
 zoomfact: array[1..9] of real;
   zoomat: byte;
   conta : word;

function TMainForm.telaativa : word;
var i : word;
begin
   for i:=1 to conta do if tela[i].Active then break;
   case tela[i].tag of
      1 : lblzoom.caption:='Zoom: 6,25%';
      2 : lblzoom.caption:='Zoom: 12,5%';
      3 : lblzoom.caption:='Zoom: 25%';
      4 : lblzoom.caption:='Zoom: 50%';
      5 : lblzoom.caption:='Zoom: 100%';
      6 : lblzoom.caption:='Zoom: 200%';
      7 : lblzoom.caption:='Zoom: 300%';
      8 : lblzoom.caption:='Zoom: 400%';
      9 : lblzoom.caption:='Zoom: 500%';
   end;
   telaativa:=i;
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  Application.OnHint := ShowHint;
  Screen.OnActiveFormChange := UpdateMenuItems;
  zoomat:=5;
  conta:=0;
  zoomfact[1]:=0.0625; // 6,25%
  zoomfact[2]:=0.125;  // 12,5%
  zoomfact[3]:=0.25;   // 25%
  zoomfact[4]:=0.5;    // 50%
  zoomfact[5]:=1;      // 100%
  zoomfact[6]:=2;      // 200%
  zoomfact[7]:=3;      // 300%
  zoomfact[8]:=4;      // 400%
  zoomfact[9]:=5;      // 500%
end;

procedure TMainForm.ShowHint(Sender: TObject);
begin
  StatusBar.SimpleText := Application.Hint;
end;

procedure TMainForm.CreateMDIChild(const Name: string);
var
  Child: TMDIChild;
begin
  { create a new MDI child window }
  Child := TMDIChild.Create(Application);
  Child.Caption := Name;
  inc(conta);
  tela[conta]:=Child;
  child.helpcontext:=conta;
end;

procedure TMainForm.FileNewItemClick(Sender: TObject);
begin
  CreateMDIChild('FIGURA' + IntToStr(MDIChildCount + 1));
end;

procedure TMainForm.FileOpenItemClick(Sender: TObject);
var filename : string;
    bitmap   : TBitmap;
    w,h      : longint;
    i        : longint;
begin
  if OpenPictureDialog.Execute then
  begin
     filename:=OpenPictureDialog.FileName;
     CreateMDIChild(filename);
     bitmap:=TBitmap.create;
     bitmap.LoadFromFile(filename);
     with bitmap do
     begin
        W:=Width;
        H:=Height;
     end;
     i:=telaativa;
     tela[i].imgchild.picture.LoadFromFile(filename);
     tela[i].imgchild.width:=w;
     tela[i].imgchild.height:=h;
     ActiveMDIChild.clientWidth:=W;
     ActiveMDIChild.clientHeight:=h;
     bmpx[i]:=w;
     bmpy[i]:=h;
  end;
  UpdateMenuItems(Sender);
end;

procedure TMainForm.SumaTela(Sender: TObject);
var i,ii : longint;
begin
   ii:=telaativa;
   i:=tela[ii].helpcontext;
   for ii:=i to conta do
   begin
      tela[ii]:=tela[ii+1];
      bmpx[ii]:=bmpx[ii+1];
      bmpy[ii]:=bmpy[ii+1];
   end;
   dec(conta);
   UpdateMenuItems(Sender);
end;

procedure TMainForm.FileCloseItemClick(Sender: TObject);
var i,ii : longint;
begin
   i:=telaativa;
   if ActiveMDIChild <> nil then ActiveMDIChild.Close;
   for ii:=i to conta do
   begin
      tela[ii]:=tela[ii+1];
      bmpx[ii]:=bmpx[ii+1];
      bmpy[ii]:=bmpy[ii+1];
   end;
   dec(conta);
   UpdateMenuItems(Sender);
end;

procedure TMainForm.FileSaveItemClick(Sender: TObject);
var i:longint;
filename:string;
begin
   i:=telaativa;
   filename:=tela[i].caption;
   tela[i].imgchild.picture.SaveToFile(filename);
end;

procedure TMainForm.FileSaveAsItemClick(Sender: TObject);
var filename : string;
    i        : longint;
begin
  if SavePictureDialog.Execute then
  begin
     filename:=SavePictureDialog.FileName;
     i:=telaativa;
     tela[i].imgchild.picture.SaveToFile(filename);
  end;
end;

procedure TMainForm.FileExitItemClick(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.CopyItemClick(Sender: TObject);
begin
  CopyBtnClick(Sender);
end;

procedure TMainForm.PasteItemClick(Sender: TObject);
begin
  PasteBtnClick(Sender);
end;

procedure TMainForm.WindowCascadeItemClick(Sender: TObject);
begin
  Cascade;
end;

procedure TMainForm.WindowTileItemClick(Sender: TObject);
begin
  Tile;
end;

procedure TMainForm.WindowArrangeItemClick(Sender: TObject);
begin
  ArrangeIcons;
end;

procedure TMainForm.WindowMinimizeItemClick(Sender: TObject);
var
  I: Integer;
begin
  { Must be done backwards through the MDIChildren array }
  for I := MDIChildCount - 1 downto 0 do
    MDIChildren[I].WindowState := wsMinimized;
end;

procedure TMainForm.UpdateMenuItems(Sender: TObject);
begin
  FileCloseItem.Enabled := MDIChildCount > 0;
  FileSaveItem.Enabled := MDIChildCount > 0;
  FileSaveAsItem.Enabled := MDIChildCount > 0;
  CopyItem.Enabled := MDIChildCount > 0;
  PasteItem.Enabled := Clipboard.HasFormat(CF_BITMAP);
  SaveBtn.Enabled := MDIChildCount > 0;
  CopyBtn.Enabled := MDIChildCount > 0;
  PasteBtn.Enabled := Clipboard.HasFormat(CF_BITMAP);
  WindowCascadeItem.Enabled := MDIChildCount > 0;
  WindowTileItem.Enabled := MDIChildCount > 0;
  WindowArrangeItem.Enabled := MDIChildCount > 0;
  WindowMinimizeItem.Enabled := MDIChildCount > 0;
  detbtn.enabled := MDIChildCount > 0;
  speedbutton1.enabled := MDIChildCount > 0;
  speedbutton2.enabled := MDIChildCount > 0;
  deteco1.enabled := MDIChildCount > 0;
  clulasdatabela1.enabled := MDIChildCount > 0;
  maiszoom1.enabled := MDIChildCount > 0;
  menoszoom1.enabled := MDIChildCount > 0;
  edit1.enabled := Clipboard.HasFormat(CF_BITMAP);
  if conta=0 then lblzoom.caption:='Zoom:';
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
  Screen.OnActiveFormChange := nil;
end;

procedure TMainForm.HelpAboutItemClick(Sender: TObject);
begin
  AboutBox.ShowModal;
end;

procedure TMainForm.ClulasdaTabela1Click(Sender: TObject);
var i,ant,novo : longint;
    x,y,z,w,h  : longint;
    aux,aux1,t : longint;
    x1,y1,aux2 : longint;
    numptos    : longint; // número de pixels em uma célula
    celvaz     : longint;
    minx,miny  : longint;
    maxx,maxy  : longint;
    rdest,rorig: trect;
    cor : byte;
    novonome,filename:string;
    bitmap : tbitmap;
    f : file;
begin
   celvaz:=0;
   if conta>0 then
   begin
     i:=telaativa;
     tela[i].imgchild.picture.savetofile('temp.bmp');
     ant:=i;
     filename:=tela[i].caption;
     CreateMDIChild(filename);
     bitmap:=TBitmap.create;
     bitmap.LoadFromFile('temp.bmp');
     with bitmap do
     begin
        W:=Width;
        H:=Height;
     end;
     bitmap.destroy;
     i:=telaativa;
     tela[i].imgchild.picture.LoadFromFile('temp.bmp');
     tela[i].imgchild.width:=w;
     tela[i].imgchild.height:=h;
     ActiveMDIChild.clientWidth:=W;
     ActiveMDIChild.clientHeight:=h;
     bmpx[i]:=w;
     bmpy[i]:=h;
     novonome:='';
     for z:=1 to length(filename)-4 do novonome:=novonome+filename[z];
     novonome:=novonome+'_SiTab.bmp';
     tela[i].caption:=novonome;
     i:=telaativa;
     aux:=w*h;
     aux1:=0;
     tela[i].imgchild.canvas.brush.Color:=clBlack;
     tela[i].imgchild.canvas.floodfill(0,0,clWhite,fsSurface);
     tela[i].imgchild.canvas.pen.color:=clWhite;
     tela[i].imgchild.canvas.pen.mode:=pmNot;
     tela[i].imgchild.canvas.moveto(0,0);
     for x:=0 to w do
     begin
        tela[i].imgchild.canvas.moveto(x,0);
        tela[i].imgchild.canvas.lineto(x,h);
     end;
     tela[i].repaint;
     termometro.show;
     termometro.caption:='Detecção - Passo 1';
     termometro.label1.caption:='Gravando célula:';
     termometro.gauge1.Progress:=0;
     termometro.gauge2.progress:=0;
     termometro.label2.caption:='Número de células:';
     termometro.label3.caption:='Células vazias:';
     termometro.Repaint;
     for x:=0 to w do
     begin
        for y:=0 to h do
        begin
           if tela[i].imgchild.canvas.pixels[x,y]=clBlack then
           begin
              tela[i].imgchild.canvas.brush.Color:=clBlue;
              tela[i].imgchild.canvas.floodfill(0,0,clWhite,fsSurface);
              tela[i].imgchild.canvas.brush.Color:=clRed;
              tela[i].imgchild.canvas.floodfill(x,y,clBlue,fsBorder);
              tela[i].imgchild.canvas.brush.Color:=clWhite;
              tela[i].imgchild.canvas.floodfill(0,0,clBlue,fsSurface);
              tela[i].repaint;
           end;
           inc(aux1);
           termometro.gauge1.progress:=trunc(100*aux1/aux);
        end;
     end;
     tela[i].repaint;
     aux1:=0;
     t:=0;
     termometro.caption:='Detecção - Passo 2';
     termometro.repaint;
     cor:=1;
     for x:=0 to w do
     begin
        for y:=0 to h do
        begin
           if tela[i].imgchild.canvas.pixels[x,y]=clRed then
           begin
              tela[i].imgchild.canvas.brush.Color:=clBlack;
              tela[i].imgchild.canvas.floodfill(x,y,clRed,fsSurface);
              imagem.canvas.brush.Color:=clBlack;
              inc(t);
              termometro.label2.caption:='Número de células: '+InttoStr(t);
              termometro.repaint;
              tela[i].repaint;
              imagem.width:=w;
              imagem.height:=h;
              aux2:=0;
              imagem.picture.loadfromfile('temp.bmp');
              imagem.canvas.pen.color:=clWhite;
              case cor of
                 1 : tela[i].imgchild.canvas.brush.Color:=clmaroon;
                 2 : tela[i].imgchild.canvas.brush.Color:=clgreen;
                 3 : tela[i].imgchild.canvas.brush.Color:=clolive;
                 4 : tela[i].imgchild.canvas.brush.Color:=clnavy;
                 5 : tela[i].imgchild.canvas.brush.Color:=clpurple;
                 6 : tela[i].imgchild.canvas.brush.Color:=clteal;
                 7 : tela[i].imgchild.canvas.brush.Color:=clgray;
                 8 : tela[i].imgchild.canvas.brush.Color:=clsilver;
                 9 : tela[i].imgchild.canvas.brush.Color:=cllime;
                 10: tela[i].imgchild.canvas.brush.Color:=clblue;
                 11: tela[i].imgchild.canvas.brush.Color:=clfuchsia;
              end;
              imagem.canvas.brush.Color:=clBlack;
              imagem.canvas.floodfill(x,y,clBlue,fsBorder);
              imagem.canvas.brush.Color:=clWhite;
              imagem.canvas.floodfill(x,y,clBlack,fsSurface);
              ///////////////////
              numptos:=0;
              ///////////////////
              minx:=w;
              miny:=h;
              maxx:=0;
              maxy:=0;
              for x1:=1 to w do
              begin
                 for y1:=1 to h do
                 begin
                    if tela[i].imgchild.canvas.pixels[x1,y1]=clBlack then
                    begin
                       if x1>maxx then maxx:=x1;
                       if y1>maxy then maxy:=y1;
                       if x1<minx then minx:=x1;
                       if y1<miny then miny:=y1;
                       imagem.canvas.pixels[x1,y1]:=
                                        tela[ant].imgchild.canvas.pixels[x1,y1];
                       if imagem.canvas.pixels[x1,y1]<>clWhite then
                          inc(numptos);
                    end;
                    inc(aux2);
                    termometro.gauge2.progress:=trunc(100*aux2/aux);
                 end;
              end;
              tela[i].imgchild.canvas.floodfill(x,y,clBlack,fsSurface);
              inc(cor);
              if cor=12 then cor:=1;
              /////////////
              if numptos>0 then
              begin
                 novonome:='';
                 for z:=1 to length(filename)-4 do
                    novonome:=novonome+filename[z];
                 novonome:=novonome+'_celula'+InttoStr(t)+'.bmp';
                 bitmap:=TBitmap.create;
                 bitmap.Width:=maxx-minx;
                 bitmap.Height:=maxy-miny;
                 rdest.left:=0;
                 rdest.top:=0;
                 rdest.right:=bitmap.width;
                 rdest.bottom:=bitmap.Height;
                 rorig.left:=minx;
                 rorig.top:=miny;
                 rorig.right:=maxx;
                 rorig.bottom:=maxy;
                 bitmap.canvas.CopyRect(rdest,imagem.canvas,rorig);
                 bitmap.savetofile(novonome);
                 bitmap.destroy;
              end
              else
              begin
                 inc(celvaz);
                 termometro.label3.caption:='Células vazias: '+inttostr(celvaz);
                 termometro.repaint;
              end;
              //////////////
           end;
           inc(aux1);
           termometro.gauge1.progress:=trunc(100*aux1/aux);
        end;
     end;
     aux2:=0;
     termometro.caption:='Detecção - Passo 3';
     termometro.label1.caption:='Redesenhando tabela:';
     termometro.repaint;
     for x1:=1 to w do
     begin
        for y1:=1 to h do
        begin
           if tela[i].imgchild.canvas.pixels[x1,y1]<>clWhite then
           begin
              if tela[ant].imgchild.canvas.pixels[x1,y1]=clBlack then
                           tela[i].imgchild.canvas.pixels[x1,y1]:=clBlue;
           end;
           termometro.gauge2.progress:=trunc(100*aux2/aux);
           inc(aux2);
        end;
     end;
     termometro.close;
   end
   else form1.showmodal;
   AssignFile(F,'temp.bmp');
   Reset(F);
   CloseFile(F);
   Erase(F);
end;

procedure TMainForm.DetBtnClick(Sender: TObject);
begin
   ClulasdaTabela1Click(Sender);
end;

procedure TMainForm.SpeedButton1Click(Sender: TObject);
var i,w,h : word;
begin
   i:=telaativa;
   w:=bmpx[i];
   h:=bmpy[i];
   zoomat:=tela[i].tag;
   if zoomat>1 then dec(zoomat);
   tela[i].tag:=zoomat;
   tela[i].imgchild.width:=trunc(w*zoomfact[zoomat]);
   tela[i].imgchild.height:=trunc(h*zoomfact[zoomat]);
   ActiveMDIChild.clientWidth:=trunc(w*zoomfact[zoomat]);
   ActiveMDIChild.clientHeight:=trunc(h*zoomfact[zoomat]);
   case zoomat of
      1 : lblzoom.caption:='Zoom: 6,25%';
      2 : lblzoom.caption:='Zoom: 12,5%';
      3 : lblzoom.caption:='Zoom: 25%';
      4 : lblzoom.caption:='Zoom: 50%';
      5 : lblzoom.caption:='Zoom: 100%';
      6 : lblzoom.caption:='Zoom: 200%';
      7 : lblzoom.caption:='Zoom: 300%';
      8 : lblzoom.caption:='Zoom: 400%';
      9 : lblzoom.caption:='Zoom: 500%';
   end;
end;

procedure TMainForm.SpeedButton2Click(Sender: TObject);
var i,w,h : word;
begin
   i:=telaativa;
   w:=bmpx[i];
   h:=bmpy[i];
   zoomat:=tela[i].tag;
   if zoomat<9 then inc(zoomat);
   tela[i].tag:=zoomat;
   tela[i].imgchild.width:=trunc(w*zoomfact[zoomat]);
   tela[i].imgchild.height:=trunc(h*zoomfact[zoomat]);
   ActiveMDIChild.clientWidth:=trunc(w*zoomfact[zoomat]);
   ActiveMDIChild.clientHeight:=trunc(h*zoomfact[zoomat]);
   case zoomat of
      1 : lblzoom.caption:='Zoom: 6,25%';
      2 : lblzoom.caption:='Zoom: 12,5%';
      3 : lblzoom.caption:='Zoom: 25%';
      4 : lblzoom.caption:='Zoom: 50%';
      5 : lblzoom.caption:='Zoom: 100%';
      6 : lblzoom.caption:='Zoom: 200%';
      7 : lblzoom.caption:='Zoom: 300%';
      8 : lblzoom.caption:='Zoom: 400%';
      9 : lblzoom.caption:='Zoom: 500%';
   end;
end;

procedure TMainForm.MenosZoom1Click(Sender: TObject);
begin
   SpeedButton1Click(Sender);
end;

procedure TMainForm.MaisZoom1Click(Sender: TObject);
begin
   SpeedButton2Click(Sender);
end;

procedure TMainForm.Toolbar1Click(Sender: TObject);
begin
   toolbar1.checked:=not toolbar1.checked;
   speedpanel.visible:=toolbar1.checked;
end;

procedure TMainForm.CopyBtnClick(Sender: TObject);
var i : byte;
begin
   i:=telaativa;
   Clipboard.Assign(tela[i].imgchild.picture);
   UpdateMenuItems(Sender);
end;

procedure TMainForm.PasteBtnClick(Sender: TObject);
var Bitmap: TBitmap;
    filename : string;
    i,w,h : word;
begin
  if Clipboard.HasFormat(CF_BITMAP) then
  begin
     filename:='Figura'+InttoStr(conta+1)+'.BMP';
     CreateMDIChild(filename);
     bitmap:=TBitmap.create;
     bitmap.Assign(Clipboard);
     with bitmap do
     begin
        W:=Width;
        H:=Height;
     end;
     i:=telaativa;
     tela[i].imgchild.picture.Assign(Clipboard);;
     tela[i].imgchild.width:=w;
     tela[i].imgchild.height:=h;
     ActiveMDIChild.clientWidth:=W;
     ActiveMDIChild.clientHeight:=h;
     bmpx[i]:=w;
     bmpy[i]:=h;
  end;
end;


end.
