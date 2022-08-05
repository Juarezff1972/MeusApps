program Truco;
uses Crt,Graph,bgifont,bgidriv;
type
  tNaipe   = (Ouro, Espadas, Copas, Paus);
  tValor   = (Quatro, Cinco, Seis, Sete, Dama, Valete, Rei, As, Dois, Tres);
  tCarta   = record
               Valor : tValor;
               Naipe : tNaipe;
               Status: char
             end;
  tBaralho = array [1..40] of tCarta;
  tMao     = array [1..3] of integer;


var
  gBaralho     : tBaralho;        gNum         : integer;
  gCartaJog    : tMao;            gContinua    : boolean;
  gCartaComp   : tMao;            gGraphMode   : integer;
  gCartaVira   : integer;         gGraphDriver : integer;
  gVira        : integer;         gX,gY        : integer;
  gChar        : char;            gCartaSobre  : integer;
  gFim         : boolean;         gNome        : string[13];
  gSeta        : integer;         gMoveSeta    : boolean;
  gCont        : boolean;         gCardNum     : integer;
  gWho         : tMao;            gPontComp    : integer;
  gPontJog     : integer;         gTotalJog    : integer;
  gTotalComp   : integer;         gJogada      : integer;
  gValor       : integer;         gTruco       : boolean;
  gCond        : boolean;         gChr         : char;
{--------------------------------------------------------------------------}
function ConteValor(sContValor:tValor):integer;
var
  G: integer;
begin
  Case  sContValor of
    As  : G:=65; Dois: G:=50; Tres: G:=51; Quatro: G:=52; Cinco: G:=53;
    Seis: G:=54; Sete: G:=55; Dama: G:=81; Valete: G:=74; Rei  : G:=75
  end;
  ConteValor:=G
end;

{--------------------------------------------------------------------------}
function ConteNaipe(sContNaipe:tNaipe):integer;
var
  B: integer;
begin
  Case sContNaipe of
    Ouro: B:= 4; Espadas: B:= 6; Copas: B:= 3; Paus: B:=5
  end;
  ConteNaipe:=B
end;

{-------------------------------------------------------------------------}
procedure MontaBaralho;
var
  sContValor : tValor;
  sContNaipe : tNaipe;
  sContCarta : integer;

begin
  sContCarta:=1;
  for sContValor:=Quatro to Tres do
    for sContNaipe:=Ouro to Paus do
      begin
        gBaralho[sContCarta].Valor:=sContValor;
        gBaralho[sContCarta].Naipe:=sContNaipe;
        gBaralho[sContCarta].Status:=' ';
        sContCarta:=Succ(sContCarta)
      end
end;

{--------------------------------------------------------------------------}
procedure Embaralhar;
var
  sCont   : integer;
begin
  for sCont:=1 to 40 do
    gBaralho[sCont].Status:=' '
end;

{--------------------------------------------------------------------------}
procedure Cartas(var pMao : tMao);
var
  sNum    : integer;
  sCarta  : integer;

begin
  Randomize;
  sCarta:=1;
  repeat
    sNum:=Random(40) + 1;
    if gBaralho[sNum].Status = ' ' then
      begin
        pMao[sCarta]:=sNum;
        gBaralho[sNum].Status:='*';
        sCarta:=Succ(sCarta)
      end;
  until sCarta > 3
end;
{--------------------------------------------------------------------------}
procedure Truque;
begin
  settextstyle(Triplexfont,horizdir,1);
  outtextxy(470,160,'TRUCO !!!');
  gTruco:=true;
  gValor:=gValor+2;
end;

{--------------------------------------------------------------------------}
procedure VerTruco(pMao:tMao);
var
  sNum: integer;
begin
  randomize;
  sNum:= random(3);
  if sNum= 1 then
  begin
    if(gBaralho[pMao[1]].Valor>=Rei) or (gBaralho[pMao[1]].Status='M') then
      if(gBaralho[pMao[2]].Valor>=Rei) or (gBaralho[pMao[2]].Status='M') then
        if(gBaralho[pMao[3]].Valor>=Rei) or (gBaralho[pMao[3]].Status='M') then
          Truque
  end
end;

{--------------------------------------------------------------------------}
procedure VerManilha(pMao:tMao);
begin
  if(gBaralho[pMao[1]].Status='M') and (gbaralho[pMao[2]].Status='M') then
    Truque;
  if(gBaralho[pMao[1]].Status='M') and (gbaralho[pMao[3]].Status='M') then
    Truque;
  if(gBaralho[pMao[2]].Status='M') and (gbaralho[pMao[3]].Status='M') then
    Truque
end;

{--------------------------------------------------------------------------}
procedure Help;
var
  sHelp :string[80];
  Arq   :text;
  sLine :integer;
begin
  sLine := 0;
  RestoreCrtMode;
  Assign(Arq,'Truco.txt');
  Reset(Arq);
  ClrScr;
  repeat
  readln(Arq,sHelp);
  writeln(sHelp);
  sLine:=sLine+1;
  if sLine=19 then
  begin
    writeln('-----------------------------');
    writeln('Aperte <ENTER> para continuar');
    readln;
    sLine := 0
  end;
  until eof(Arq);
  writeln('Aperte <ENTER> para voltar...');
  readln;
  SetGraphmode(GetGraphMode);
  gMoveSeta:=true
  end;

{--------------------------------------------------------------------------}
procedure Abort(Msg : string);
begin
  Writeln(Msg, ': ', GraphErrorMsg(GraphResult));
  Halt(1);
end;
{--------------------------------------------------------------------------}
procedure Apresentacao;
begin
  if RegisterBGIdriver(@CGADriverProc) < 0 then
    Abort('CGA');
  if RegisterBGIdriver(@EGAVGADriverProc) < 0 then
    Abort('EGA/VGA');
  if RegisterBGIdriver(@HercDriverProc) < 0 then
    Abort('Herc');
  if RegisterBGIdriver(@ATTDriverProc) < 0 then
    Abort('AT&T');
  if RegisterBGIdriver(@PC3270DriverProc) < 0 then
    Abort('PC 3270');
  if RegisterBGIfont(@GothicFontProc) < 0 then
    Abort('Gothic');
  if RegisterBGIfont(@SansSerifFontProc) < 0 then
    Abort('SansSerif');
  if RegisterBGIfont(@SmallFontProc) < 0 then
    Abort('Small');
  if RegisterBGIfont(@TriplexFontProc) < 0 then
    Abort('Triplex');
  gGraphdriver := detect;
  initgraph(gGraphdriver,gGraphmode,'');
    case gGraphdriver of
    CGA     : gGraphmode:= CGAhi;
    EGA     : gGraphmode:= EGAlo;
    VGA     : gGraphmode:= VGAlo;
    MCGA    : gGraphmode:= MCGAmed;
    HERCMONO: gGraphmode:= HERCMONOhi;
    ATT400  : gGraphmode:= ATT400med;
    PC3270  : gGraphmode:= PC3270hi;
    IBM8514 : gGraphmode:= IBM8514lo
  end;
  setgraphmode(gGraphMode);
  settextjustify(getmaxx div 2,getmaxy div 2);
  settextstyle(triplexfont,horizdir,9);
  setlinestyle(Solidln,15,Normwidth);
  setcolor(15);
  rectangle(0,0,639,199);
  outtextxy(170,40,'TRUCO');
  settextstyle(Gothicfont,horizdir,4);
  delay(2000)
end;

{--------------------------------------------------------------------------}
procedure Copyright;
begin
  settextstyle(triplexFont,horizdir,5);
  outtextxy(230,0,'TRUCO');
  settextstyle(triplexFont,Horizdir,2);
  outtextxy(0,40,'Produzido por :');
  outtextxy(0,60,'Adil Calomeno Junior');
  outtextxy(0,80,'Juarez Fioravanti Filho');
  outtextxy(0,100,'Mauricio Streisky');
  outtextxy(0,160,'Engenharia de Computacao');
  outtextxy(255,160,'~');
  outtextxy(248,161,',');
  outtextxy(0,180,'Pontificia Universidade Catolica do Parana''');
  delay(3000);
end;

{-------------------------------------------------------------------------}
procedure Cartavirada(x,y:integer);
begin
  setcolor(15);
  rectangle(x,y,x+100,y+70);
  setfillstyle(XHatchFill,15);
  bar(x+1,y+1,x+99,y+69);
end;

{--------------------------------------------------------------------------}
procedure ApagaCarta(x,y:integer);
begin
    setcolor(0);
    setfillstyle(solidfill,0);
    bar(x,y,x+100,y+70);
    setcolor(15)
end;

{--------------------------------------------------------------------------}
procedure numero(x,y,n:integer;pMao:tMao);
var
  G          : integer;
  sContValor : tValor;

begin
  sContValor:= gBaralho[pMao[n]].Valor;
  G:= ConteValor(sContValor);
  settextstyle(DefaultFont,horizdir,2);
  outtextxy(x,y,chr(G));
  outtextxy(x+72,y+51,chr(G))
end;

{--------------------------------------------------------------------------}
procedure simbolo(x,y,n:integer;pMao:tMao);
var
  B          :integer;
  sContNaipe :tNaipe;
  sContValor :tValor;
begin
  sContNaipe:= gBaralho[pMao[n]].Naipe;
  sContValor:= gBaralho[pMao[n]].Valor;
  B:=ConteNaipe(sContNaipe);
  settextstyle(DefaultFont,horizdir,2);
  if (sContValor <> Dois) and (sContValor <> Quatro)
    and (sContValor<>Seis) and (sContValor <> Valete)
    and (sContValor<>Dama) and (sContValor <> Rei)
    then outtextxy(x,y,chr(B));
  if sContValor=Dois then begin
    outtextxy(x,y-9,chr(b));
    outtextxy(x,y+9,chr(b));
  end;
  if sContValor=Tres then begin
    outtextxy(x+20,y-17,chr(b));
    outtextxy(x-20,y+17,chr(b));
  end;
  if (sContValor=Quatro) or (sContValor=Cinco) then begin
    outtextxy(x+20,y-15,chr(b));
    outtextxy(x+20,y+15,chr(b));
    outtextxy(x-20,y+15,chr(b));
    outtextxy(x-20,y-15,chr(b));
  end;
  if (sContValor=Seis) or (sContValor=Sete) then begin
    outtextxy(x+20,y-17,chr(b));  outtextxy(x-20,y-17,chr(b));
    outtextxy(x+20,y,chr(b));     outtextxy(x-20,y,chr(b));
    outtextxy(x+20,y+17,chr(b));  outtextxy(x-20,y+17,chr(b));
  end;
  if (sContValor=Valete) or (sContValor=Dama) or (sContValor=Rei) then begin
    settextstyle(DefaultFont,Horizdir,3);
    outtextxy(x-4,y-5,chr(2));
    settextstyle(DefaultFont,horizdir,2);
    outtextxy(x-16,y-24,chr(B));
    outtextxy(x+14,y+26,chr(B));
  end;
end;

{--------------------------------------------------------------------------}
procedure DesenhaFace(x,y,n:integer;pMao:tMao);
begin
  rectangle(x,y,x+100,y+70);
  numero(x+7,y+3,n,pMao);
  simbolo(x+45,y+27,n,pMao)
end;

{--------------------------------------------------------------------------}
procedure vira(x,y,z,t:integer);
var
  G          : integer;
  B          : integer;
  sNum       : integer;
  sFim       : boolean;
  sValor     : tValor;
  sCont      : integer;
  sContValor : tValor;
  sContNaipe : tNaipe;
begin
  rectangle(x-7,y-3,x+93,y+67);
  Randomize;
  sFim:=false;
  repeat
    if gContinua=true then sNum:=Random(40) + 1;
    sContValor:= gBaralho[sNum].Valor;
    sContNaipe:= gBaralho[sNum].Naipe;
    if gBaralho[sNum].Status = ' ' then
    begin
      G:=ConteValor(sContValor);
      B:=ConteNaipe(sContNaipe);
      settextstyle(DefaultFont,horizdir,2);
      outtextxy(x,y,chr(G));
      outtextxy(x+72,y+51,chr(G));
      settextstyle(DefaultFont,horizdir,2);
      if (sContValor <> Dois) and (sContValor <> Quatro)
         and (sContValor<>Seis) and (sContValor <> Valete)
         and (sContValor<>Dama) and (sContValor <> Rei)
        then outtextxy(z,t,chr(B));
      if sContValor=Dois then begin
        outtextxy(z,t-9,chr(b));
        outtextxy(z,t+9,chr(b));
      end;
      if sContValor=Tres then begin
        outtextxy(z+20,t-17,chr(b));
        outtextxy(z-20,t+17,chr(b));
      end;
      if (sContValor=Quatro) or (sContValor=Cinco) then begin
        outtextxy(z+20,t-15,chr(b));
        outtextxy(z+20,t+15,chr(b));
        outtextxy(z-20,t+15,chr(b));
        outtextxy(z-20,t-15,chr(b));
      end;
      if (sContValor=Seis) or (sContValor=Sete) then begin
        outtextxy(z+20,t-17,chr(b));  outtextxy(z-20,t-17,chr(b));
        outtextxy(z+20,t,chr(b));     outtextxy(z-20,t,chr(b));
        outtextxy(z+20,t+17,chr(b));  outtextxy(z-20,t+17,chr(b));
      end;
      if (sContValor=Valete) or (sContValor=Dama) or (sContValor=Rei) then begin
        settextstyle(DefaultFont,Horizdir,3);
        outtextxy(z-4,t-5,chr(2));
        settextstyle(DefaultFont,horizdir,2);
        outtextxy(z-16,t-24,chr(B));
        outtextxy(z+14,t+26,chr(B));
      end;
    end;
    if gBaralho[sNum].Status <> ' ' then vira(x,y,z,t);
    sValor:=gBaralho[sNum].Valor;
    repeat
      sNum:=Succ(sNum);
      if sNum = 41 then
      sNum:=1;
    until gBaralho[sNum].Valor <> sValor;
    for sCont:=sNum to sNum + 3 do
    gBaralho[sCont].Status:='M';
    sFim:=true;
  until sFim
end;

{--------------------------------------------------------------------------}
procedure RecebeNome;
var
  sCh  : char;
  sQl  : integer;
  sTam : integer;
begin
  ClearDevice;
  gNome:='';
  settextstyle(DefaultFont,Horizdir,2);
  outtextxy(2,2,'            ');
  outtextxy(0,90,'Qual seu nome parceiro?');
  outtextxy(0,120,'             <---');
  sQl:=1;
  repeat
  sCh:=readkey;
  if sCh > #31 then gNome:=gNome+sCh;
  if sCh = #8  then
  begin
    sQl:=sQl-1;
    setcolor(0);
    outtextxy(0,120,gNome);
    setcolor(15);
    sTam:=length(gNome);
    delete(gNome,sTam,1)
  end;
  sQl:=sQl+1;
  outtextxy(0,120,gNome);
  until((sCh=#13)or(sQl=13));
  ClearDevice
end;
{--------------------------------------------------------------------------}
procedure ContaPonto;
var
  sPontComp: string;
  sPontJog : string;
begin
  if gWho[gJogada]=1 then begin
    str(gTotalComp,sPontComp);
    setcolor(0);
    outtextxy(600,118,sPontComp);
    setcolor(15);
    gPontComp:=gPontComp+1;
    str(gTotalComp,sPontComp);
    outtextxy(600,118,sPontComp);
  end;
  if gWho[gJogada]=2 then begin
    str(gTotalJog,sPontJog);
    setcolor(0);
    outtextxy(600,143,sPontJog);
    setcolor(15);
    gPontJog:=gPontJog+1;
    str(gTotalJog,sPontJog);
    outtextxy(600,143,sPontJog);
  end;
  if (gWho[gJogada]<>1) and (gWho[gJogada]<>2) then begin
    str(gTotalJog,sPontJog);
    outtextxy(600,143,sPontJog);
    str(gTotalComp,sPontComp);
    outtextxy(600,118,sPontComp);
  end;
end;
{-----------------------------------------------------------------------}
procedure Score;
begin
  rectangle(639,199,380,110);
  rectangle(639,199,380,160);
  gWho[gJogada]:=0;
  ContaPonto;
  settextstyle(TriplexFont,horizdir,1);
  outtextxy(385,115,'Computador');
  outtextxy(385,140,gNome);
end;
{--------------------------------------------------------------------------}
procedure Partida;
var
  sPontComp: string;
  sPontJog : string;
begin
  SetColor(0);
  str(gTotalJog,sPontJog);
  outtextxy(600,143,sPontJog);
  str(gTotalComp,sPontComp);
  outtextxy(600,118,sPontComp);
  SetColor(15);
  if gPontJog > gPontComp then gTotalJog:=gTotalJog+gValor;
  if gPontJog < gPontComp then gTotalComp:=gTotalComp+gValor;
  if gPontJog = gPontComp then
    begin
      if (gWho[3]=0)and(gWho[2]<>gWho[1]) then
        begin
          if gWho[1]=1 then gTotalComp:=gTotalComp+gValor;
          if gWho[1]=2 then gTotalJog:=gTotalJog+gValor;
        end;
      if (gWho[2]=0)and(gWho[1]<>gWho[3]) then
        begin
          if gWho[1]=1 then gTotalComp:=gTotalComp+gValor;
          if gWho[1]=2 then gTotalJog:=gTotalJog+gValor;
        end;
      if (gWho[1]=0)and(gWho[2]<>gWho[3]) then
        begin
          if gWho[2]=1 then gTotalComp:=gTotalComp+gValor;
          if gWho[2]=2 then gTotalJog:=gTotalJog+gValor;
        end;
    end;
  gPontJog:=0;
  gPontComp:=0;
end;
{--------------------------------------------------------------------------}
procedure ganhador(n,m:integer);
begin
  gWho[gJogada]:=0;
   if (gBaralho[gCartaJog[n]].Valor = gBaralho[gCartaComp[m]].Valor) then
   begin
     if (gBaralho[gCartaJog[n]].Status = ' ') and
        (gBaralho[gCartaComp[m]].Status = ' ') then gWho[gJogada]:=0;
     if (gBaralho[gCartaJog[n]].Status = 'M') and
        (gBaralho[gCartaComp[m]].Status = 'M') then
     begin
       if (gBaralho[gCartaJog[n]].Naipe > gBaralho[gCartaComp[m]].Naipe)
       then gWho[gJogada]:=2;
       if (gBaralho[gCartaJog[n]].Naipe < gBaralho[gCartaComp[m]].Naipe)
       then gWho[gJogada]:=1;
     end;
   end;
   if (gBaralho[gCartaJog[n]].Valor > gBaralho[gCartaComp[m]].Valor) then
     gWho[gJogada]:=2;
   if (gBaralho[gCartaJog[n]].Valor < gBaralho[gCartaComp[m]].Valor) then
     gWho[gJogada]:=1;
   if (gBaralho[gCartaJog[n]].Status = 'M') then gWho[gJogada]:=2;
   if (gBaralho[gCartaComp[m]].Status = 'M') then gWho[gJogada]:=1;
   gCardNum:=m
end;
{--------------------------------------------------------------------------}
procedure Final(Termino:integer);
var y:integer;
begin
  setcolor(0);
  for y:=163 to 198 do line(385,y,638,y);
  settextstyle(TriplexFont,Horizdir,2);
  if Termino=1 then begin
  setcolor(15);
  outtextxy(440,165,'Perdeu Panaca !!!');
  repeat until keypressed;
  end;
  if Termino=2 then begin
  setcolor(15);
  outtextxy(440,165,'Ganhou Babaca !!!');
  repeat until keypressed;
  end;
end;
{--------------------------------------------------------------------------}
procedure MoveCarta(n:integer;pMao:tMao);
var
  x,y,i,m :integer;
  sCounter:integer;
begin
  sCounter:=0;
  case n of
    1: begin x:=10;  y:=120  end;
    2: begin x:=250; y:=120  end;
    3: begin x:=130; y:=120  end;
  end;
  SetColor(0);
  ApagaCarta(x,y);
  SetColor(15);
  gJogada:=gJogada+1;
    repeat
      randomize;
      m:=random(3)+1;
      if gBaralho[gCartaJog[n]].Status='J' then exit;
      gCardNum:=m;
      Ganhador(n,m);
      if (gWho[gJogada]=0) or (gWho[gJogada]=2) then begin
      m:=random(3)+1;
      if gBaralho[gCartaJog[n]].Status='J' then exit;
      gCardNum:=m;
      Ganhador(n,m)
      end;
      if ((gBaralho[gCartaComp[1]].Status='J') and
         (gBaralho[gCartaComp[2]].Status='J') and
         (gBaralho[gCartaComp[3]].Status='J')) then exit;
    until (gBaralho[gCartaComp[m]].Status<>'J');
    gBaralho[gCartaComp[m]].Status:='J';
    sCounter:=0;
   DesenhaFace(10,14,n,pMao);
     if gWho[gJogada]=0 then
       outtextxy(390,175,'embuchou');
     if gWho[gJogada]=1 then
       outtextxy(390,175,'vitoria minha');
     if gWho[gJogada]=2 then
       outtextxy(390,175,'vitoria sua');
  ContaPonto;
  if ((gBaralho[gCartaComp[1]].Status='J') and
      (gBaralho[gCartaComp[2]].Status='J') and
      (gBaralho[gCartaComp[3]].Status='J')) then
      begin
        Partida;
        Score;
      end;
  if (gWho[1]=gWho[2]) and (gWho[2] <> 0) then
   begin
     gJogada:=3;
     gWho[3]:=0;
     if gWho[1]=1 then gTotalComp:=gTotalComp+gValor;
     if gWho[2]=2 then gTotalJog :=gTotalJog +gValor;
   end;
 if (gJogada=2) and (gWho[1]=0) and (gWho[2] <> 0) then
   begin
     gJogada:=3;
     gWho[3]:=0;
     if gWho[2]=1 then gTotalComp :=gTotalComp +gValor;
     if gWho[2]=2 then gTotalJog :=gTotalJog +gValor;
   end;
 if (gJogada=2) and (gWho[1]<>0) and (gWho[2] = 0) then
   begin
     gJogada:=3;
     gWho[3]:=0;
     if gWho[1]=1 then gTotalComp:=gTotalComp+gValor;
     if gWho[1]=2 then gTotalJog :=gTotalJog +gValor;
   end;

  DesenhaFace(150,14,gCardNum,gCartaComp);
  setcolor(0);
  delay(1000);
  for y:=163 to 198 do line(385,y,638,y);
  setcolor(15);
  Setcolor(0);
  ApagaCarta(150,14);
  ApagaCarta(10,14);
  setcolor(15);
  gBaralho[gCartaJog[n]].Status:='J';
  if (gTotalComp=12) then Final(1);
  if (gTotalJog =12) then Final(2);
end;
{--------------------------------------------------------------------------}
procedure MoveSeta(n: integer);
var
  x: integer;
begin
  case n of
    1: x:= 55;   2: x:=295;   3: x:=175
  end;
  line(x-7,199,x+7,199);
  line(x  ,195,x-7,199);
  line(x  ,195,x+7,199)
end;
{-------------------------------------------------------------------------}
begin
  ClrScr;
  gPontComp :=0;
  gPontJog  :=0;
  gTotalJog :=0;
  gTotalComp:=0;
  gSeta:=3;
  gMoveSeta:=true;
  Apresentacao;
  ClearDevice;
  Copyright;
  ClearDevice;
  RecebeNome;
  MontaBaralho;
  gFim:=false;
  gContinua:=true;
  repeat
  if gMoveSeta=true then begin
    ClearDevice;
    if gContinua = true then begin
    gJogada:=0;
    gWho[1]:=3;  gWho[2]:=3; gWho[3]:=3;
    gTruco:=true;
    gValor:=1;
    Embaralhar;
    Cartas(gCartaComp);
    Cartas(gCartaJog)
    end;
    Score;
    setfillstyle(solidfill,15);
    bar(0,0,639,10);
    setcolor(0);
    settextstyle(DefaultFont,Horizdir,1);
    outtextxy(10,2,'F1 - Help');
    outtextxy(130,2,'F2 - Trucar');
    outtextxy(260,2,'F3 - Desiste');
    outtextxy(400,2,'F4 - Recomeca');
    outtextxy(560,2,'F10 - DOS');
    setcolor(15);
    DesenhaFace(10,120,1,gCartaJog);
    DesenhaFace(250,120,2,gCartaJog);
    DesenhaFace(130,120,3,gCartaJog);
    vira(537,18,575,42);
    VerTruco(gCartaComp);
    VerManilha(gCartaComp);
    gX:=460;
    gY:=25;
    gCartaSobre:=1;
    while(gCartaSobre<5) do
    begin
      gCartaSobre:=gCartaSobre +1;
      ApagaCarta(gX,gY);
      Cartavirada(gX,gY);
      gX:=gX-5;
      gY:=gY+2
    end
  end;
   gMoveSeta:=true;
   if gSeta >3 then gSeta:=1;
   if gSeta <1 then gSeta:=3;
   MoveSeta(gSeta);
   gContinua := true;
   if gJogada < 3 then begin
   gContinua:=false;
   gCond:=false;
   repeat
     gChar:=Readkey;
     case ord(gChar) of
     32 : gCond:=true;
     00 : begin
             gChr:=readkey;
             case ord(gChr) of
               59, 60, 61, 68, 75, 77 : gCond:=true;
             end
           end
     end;
   until gCond=true;
   setcolor(0);
   MoveSeta(gSeta);
   setcolor(15);
   gContinua:=true;
   if gChar = #32 then begin
     MoveCarta(gSeta,gCartaJog);
     gContinua:=false;
     gMoveSeta:=False;
   end;
   if gChar = #00 then
   begin
     gContinua:=false;
     gMoveSeta:=False;
     case Ord(gChr) of
       59 : Help;
       60 : begin
              if gValor=1 then gValor:=0;
              if gTruco then gValor:=gValor+3;
              gTruco:=false;
             end;
       61 : gContinua:=true;
       68 : gFim:=true;
       75 : gSeta:=gSeta+1;
       77 : gSeta:=gSeta-1
     end
   end;
  end;
 until gFim;
 closegraph
end.
