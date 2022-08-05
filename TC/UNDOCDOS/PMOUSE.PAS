{ this simple routine shows how to use the mouse in your
  pascal programs. }

PROGRAM mouse;

uses crt;

var
  HPos, VPos, BStatus : word;

procedure GetMouseInfo;
begin
   asm
      mov  ax,03
      int  33h
      mov  ax,cx
      mov  cl,03
      shr  ax,cl
      shr  dx,cl
      mov  HPos,ax
      mov  VPos,dx
      mov  BStatus,bx
   end;
end;

Begin
  clrscr;

  {mouse cursor on}
  asm
    mov  ax,01
    int  33h
  end;

  repeat
     GetMouseInfo;
     gotoxy(10,10);
     writeln('Horz Pos:  ', HPos,'    ');
     writeln('Vert Pos:  ', VPos,'    ');
     if (BStatus AND $01) = 1 then
       writeln('Left Button Pressed')
     else
       writeln('                   ');
     if (BStatus AND $02) = 2 then
       writeln('Right Button Pressed')
     else
       writeln('                    ');
  until keypressed;

  {mouse cursor off}
  asm
    mov  ax,02
    int  33h
  end;
end.
