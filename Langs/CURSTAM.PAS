var inicio,fim : word;
begin
   Inicio:=0;
   Fim:=15;
   asm
      mov    dx,$3D4
      mov    ax,$A
      out    dx,ax
      inc    dx
      mov    ax,Inicio
      out    dx,ax
      dec    dx
      mov    ax,$B
      out    dx,ax
      inc    dx
      mov    ax,Fim
      out    dx,ax
  end;
end.