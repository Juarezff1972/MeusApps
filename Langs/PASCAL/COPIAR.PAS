Procedure Copiar(coli,lini,colf,linf,PagFon,Pagdes:integer);
var col,lin:integer;
begin
  for lin:=Lini-1 to Linf-1 do
   for col:=Coli-1 to Colf-1 do
    MemW[seg_tela:(Col*2+Lin*160+PagDes*4000)]:=
    MemW[seg_tela:(Col*2+Lin*160+PagFon*4000)]
end;
