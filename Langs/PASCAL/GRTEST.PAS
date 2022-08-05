uses graph,dos;
var gd,gm : integer;
    regs  : registers;

function getgraphmode : word;
begin
   regs.ah:=$f;
   intr($10,regs);
   getgraphmode:=regs.al;
end;

procedure setgraphmode(mode : integer);
begin
   regs.ah:=0;
   regs.al:=mode;
   intr($10,regs);
end;

function getmaxx : word;
begin
   case getgraphmode of
      $d,$4,$5,$13             : getmaxx:=320;
      $e,$6,$f,$10,$11,$12,$72 : getmaxx:=640;
      $54,$63                  : getmaxx:=800;
      $67,$64                  : getmaxx:=1024;
      $0,$1                    : getmaxx:=40;
      $2,$3,$7                 : getmaxx:=80;
   end;
end;

function getmaxy : word;
begin
   case getgraphmode of
      $0,$1,$2,$3,$7     : getmaxy:=25;
      $4,$5,$6,$d,$e,$13 : getmaxy:=200;
      $f,$10             : getmaxy:=350;
      $11,$12,$72        : getmaxy:=480;
      $54,$63            : getmaxy:=600;
      $64,$67            : getmaxy:=768;
   end;
end;

function getmaxcolor : word;
begin
   case getgraphmode of
      $0,$1,$2,$3,$d,$e,$10,$12,$54 : getmaxcolor:=16;
      $4,$5,$67                     : getmaxcolor:=4;
      $6,$11,$f                     : getmaxcolor:=2;
      $13,$63,$64                   : getmaxcolor:=256;
      $72                           : getmaxcolor:=32768;
   end;
end;

begin
   gd:=;
   initgraph(gd,gm,'');
   cleardevice;
   setgraphmode($64);
   setcolor(215);
   line(0,0,1024,768);
   closegraph;
end.