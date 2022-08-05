Type  C   = Record
             Caractere:Char;
             Atributo :Byte;
            end;

      Mat = Array[1..25,1..80] of C;

Var   Mem_Video:Mat  Absolute $B800:0000;
      Arq      :File of Mat;

Begin
      Assign (Arq,'SCREEN.SCR');
      {$I-}
              Reset(Arq);
              if IoResult <> 0 then
                 Begin
                     WriteLn('SCREEN.SCR nao encontrado');
                     Exit;
                 end;
      {$I+}

      Read (Arq,Mem_Video);
      Close(Arq);
      Repeat until keypressed;
End.