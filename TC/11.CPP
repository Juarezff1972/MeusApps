typedef struct { unsigned char Red,Grn,Blu } rgb;
typedef rgb PalRegs[255];


void GetPal(PalRegs Pal)
    regs : registers;
{
   unsigned char i;
   for(i=0;i<=255;i++)
   {
      _AH=0x10;
      _AL=0x15;

   }
   for i:=0 to 255 do
   begin
      regs.ah:=$10;
      regs.al:=$15;
      regs.bx:=i;
      intr($10,regs);
      Pal[i].Red:=regs.dh;
      Pal[i].Grn:=regs.ch;
      Pal[i].Blu:=regs.cl;
   end;
}
{-------------------------------------------------------------------}
Procedure SetPal(Pal : PalRegs);
var i    : byte;
    regs : registers;
begin
   regs.ah:=$10;
   regs.al:=$12;
   regs.bx:=0;
   regs.cx:=256;
   regs.dx:=Ofs(Pal[0]);
   regs.es:=Seg(Pal[0]);
   intr($10,regs);
end;
{-------------------------------------------------------------------}
Procedure CyclePal(var Hue : PalRegs);
var tmp : RGB;
    i   : byte;
begin
   tmp:=Hue[1];
   move(Hue[2],Hue[1],762);
   Hue[255]:=tmp;
   SetPal(Hue);
end;
{-------------------------------------------------------------------}
