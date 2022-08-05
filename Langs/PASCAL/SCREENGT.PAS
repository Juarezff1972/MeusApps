{$M 8192,0,0}
uses dos,crt;
var x : ^byte;
    y : file of byte;
    i : word;
    tecla : byte;
    abcd : pointer;
procedure salvar;
interrupt;
begin
   inline($b4/$02/           { mov ah,0002  }
          $cd/$16/           { int 16       }
          $a2/>tecla);       { mov tecla,al }
   if tecla>=128 then tecla:=tecla-128; { Insert ligado }
   if tecla>=64 then  tecla:=tecla-64;  { Caps Lock     }
   if tecla>=32 then  tecla:=tecla-32;  { Num Lock      }
   if tecla>=16 then  tecla:=tecla-16;  { Scroll Lock   }
   if tecla>=8 then   tecla:=tecla-8;   { Alt           }
   if tecla=6 then
   begin
      assign(y,'screen.bmt');
      rewrite(y);
      for i:=0 to 32768 do
      begin
         x:=ptr($b800,I);
         write(y,x^);
      end;
      close(y);
   end;
{   clrscr;
   readln;
   reset(y);
   for i:=0 to 32768 do
   begin
      x:=ptr($b800,I);
      read(y,x^);
   end;
   close(y);}
   if tecla>=4 then   tecla:=tecla-4;   { Ctrl          }
   if tecla>=2 then   tecla:=tecla-2;   { Shift esq.    }
   if tecla>=1 then   tecla:=tecla-1;   { Shift dir.    }
end;
begin
   getintvec($1b,abcd);
   setintvec($1b,SaveInt1B);
   getintvec($1c,abcd);
   setintvec($1c,@salvar);
   keep(768);
end.