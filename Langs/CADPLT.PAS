{$M 8192,0,0}
uses dos;
var old : pointer;
    op : text;
Procedure int78(Flags, CS, IP, AX, BX, CX, DX, SI, DI, DS, ES, BP: Word);
interrupt;
begin
   assign(op,'prn');
   rewrite(op);
   if ax=1 then write(op,'PU;HO;RE;HO;');
   if ax=2 then write(op,'RE;');
   if ax=3 then write(op,'MA',bx,',',cx,';');
   if ax=4 then write(op,'DA',bx,',',cx,';');
   if ax=8 then write(op,'PU;');
   if ax=9 then write(op,'PU;HO;RE;');
   close(op);
   ax:=0;
end;
begin
   GetIntVec($78,old);
   SetIntVec($78,Addr(int78));
   swapvectors;
   Exec(GetEnv('COMSPEC'),'/E:480 /C cad.bat');
   swapvectors;
   SetIntVec($78,old);
end.