uses dos,crt,drivers;
type chtb = array[1..8192] of byte;
var regs : registers;
    i,j,k: word;
    x,y  : byte;
    t    : chtb;
    r,g,b: array[1..7] of byte;
    asci : file of byte;
    mouse: tevent;
    chrt : char;
{----------------------------------------------------------------}
Procedure MostraMapa(z : word);
var i : word;
    y : byte;
begin
   for i:=z*16 to z*16+15 do
   begin
      y:=t[i];
      if y>=128 then
      begin
	 write('OO');
	 y:=y-128;
      end
      else write('..');
      if y>=64 then
      begin
	 write('OO');
	 y:=y-64;
      end
      else write('..');
      if y>=32 then
      begin
	 write('OO');
	 y:=y-32;
      end
      else write('..');
      if y>=16 then
      begin
	 write('OO');
	 y:=y-16;
      end
      else write('..');
      if y>=8 then
      begin
	 write('OO');
	 y:=y-8;
      end
      else write('..');
      if y>=4 then
      begin
	 write('OO');
	 y:=y-4;
      end
      else write('..');
      if y>=2 then
      begin
	 write('OO');
	 y:=y-2;
      end
      else write('..');
      if y>=1 then
      begin
	 writeln('OO');
	 y:=y-1;
      end
      else writeln('..');
   end;
end;
{----------------------------------------------------------------}
begin
   initevents;
   for i:=1 to 7 do
   begin
      regs.ah:=$10;   regs.al:=$15;   regs.bx:=i;    intr($10,regs);
      r[i]:=regs.dh;  g[i]:=regs.ch;  b[i]:=regs.cl;

      regs.ah:=$10;   regs.al:=$15;   regs.bx:=i+56;
      intr($10,regs);

      regs.ah:=$10;   regs.al:=$10;   regs.bx:=i;
      intr($10,regs);

   end;
   assign(asci,'char437');
   reset(asci);
   regs.ah:=$11;   regs.al:=4;   regs.bl:=0;   intr($10,regs);
   regs.ah:=$11;   regs.al:=$30; regs.bh:=6;   intr($10,regs);
   for i:=0 to 255*16+15 do read(asci,t[i]);
   close(asci);
   assign(asci,'char004');
   reset(asci);
   for i:=4096 to (255*16+15)+4096 do read(asci,t[i]);
   close(asci);
   regs.ah:=$11; regs.al:=$0;     regs.cx:=512;      regs.dx:=0;
   regs.bh:=16;  regs.es:=seg(t); regs.bp:=ofs(t)-1; intr($10,regs);
   regs.ah:=$11; regs.al:=$3;     regs.bl:=16;       intr($10,regs);
   clrscr;
   x:=1;y:=1;
   for k:=0 to 255 do if k<>7 then
   begin
      gotoxy(x,y);
      inc(x);if x>79 then begin x:=1; inc(y); end; regs.cx:=1;
      regs.ah:=$9; regs.al:=k; regs.bl:=11; regs.bh:=0; intr($10,regs);
   end;
   for k:=0 to 255 do if k<>7 then
   begin
      gotoxy(x,y);
      inc(x);if x>79 then begin x:=1; inc(y); end; regs.cx:=1;
      regs.ah:=$9; regs.al:=k; regs.bl:=3; regs.bh:=0; intr($10,regs);
   end;
   writeln;
   regs.ah:=1; regs.cx:=6400; intr($10,regs);
   showmouse;
   repeat
      textattr:=yellow;
      getmouseevent(mouse);
      gotoxy(mouse.where.x+1,mouse.where.y+1);
      if mouse.what=evmousemove then
      begin
	 regs.ah:=$8; regs.bh:=0; intr($10,regs);
      end;
      gotoxy(43,17); write('Caracter: ');
      if ((127-regs.ah)>7) then
      begin
	 write((regs.al+256):4);
	 gotoxy(1,8);
	 mostramapa(regs.al+256);
      end else
      begin
	 write((regs.al):4);
	 gotoxy(1,8);
	 mostramapa(regs.al);
      end;
   until (mouse.buttons=mbrightbutton);
   hidemouse;
   regs.ah:=1; regs.cx:=$607; intr($10,regs);
   for i:=1 to 7 do
   begin
      regs.ah:=$10;   regs.al:=$10;
      regs.bx:=i;     regs.dh:=r[i];
      regs.ch:=g[i];  regs.cl:=b[i];
      intr($10,regs);
   end;
   regs.ah:=$11;
   regs.al:=4;
   regs.bl:=0;
   intr($10,regs);
   regs.ah:=$11;
   regs.al:=$3;
   regs.bl:=0;
   intr($10,regs);
   doneevents;
end.


{ Arquivos CHAR: 437 - Normal        }
{                860 - Brasil        }
{                850 - Internacional }
{ CHARXXX        001 - It lico       }
{                002 - Sublinhado    }
{                003 - Small Caps    }
{                004 - Acentua‡„o    }

