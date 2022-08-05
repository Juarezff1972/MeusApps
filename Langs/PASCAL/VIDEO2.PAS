{$M $800,0,0 }   { 2K stack, no heap }
uses Crt, Dos;
var
  VidIntVec : Procedure(Flags,CS,IP,AX,BX,CX,DX,SI,DI,DS,ES,BP: Word);
  x,y : byte;
{$F+}
procedure ShowInfo(Flags,CS,IP,AX,BX,CX,DX,SI,DI,DS,ES,BP: Word); interrupt;
begin
  x:=wherex;
  y:=wherey;
  gotoxy(1,1); write(AX);
  gotoxy(1,2); write(BX);
  gotoxy(1,3); write(CX);
  gotoxy(1,4); write(DX);
  gotoxy(x,y);
  inline ($9C);
  VidIntVec(Flags,CS,IP,AX,BX,CX,DX,SI,DI,DS,ES,BP);
end;
{$F-}
begin
  GetIntVec($10,@VidIntVec);
  SetIntVec($10,Addr(ShowInfo));
  Keep(0);
end.



