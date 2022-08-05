Program Partida;

uses crt;

Procedure boot;
begin
   inline($ba/$40/$00/            { mov dx,0040            }
          $8e/$da/                { mov ds,dx              }
          $bb/$72/$00/            { mov bx,0072            }
          $c7/$07/$34/$12/        { mov word ptr [bx],1234 }
          $ea/$00/$00/$ff/$ff/    { jmp ffff:0000          }
          $90);                   { nop                    }
end;

begin
   Writeln('Prestes a executar partida fria...');
   Writeln;
   Writeln('Aperte algo para continuar...');
   repeat until keypressed;
   Boot;
end.