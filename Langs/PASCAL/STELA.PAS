Type  C   = Record
             Caractere:Char;
             Atributo :Byte;
            end;

      Mat = Array[1..25,1..80] of C;

Var   Mem_Video:Mat  Absolute $B800:0000;
      Arq      :File of Mat;

Begin
      Assign (Arq,'SCREEN.SCR');
      Rewrite(Arq);
      Write(Arq,Mem_Video);
      Close(Arq);
End.