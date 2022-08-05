{  this utilitie opens/closes the CD door.  It doesn't print anything
   to the screen so that it can be 'cleanly' used in a batch file.

   util to open/close the CD door.
   call cddoor with:
     job (0 to open, 5 to close)
     drive (3 = D, 4 = E, etc.)

   Turbo Pascal 6.0
}


program cddoordemo;

{$G+}

type
  myarray = array[0..12] of word;

var  jobtodo   : byte;
     buffer    : myarray;

procedure cddoor (job:byte; drive:word);
begin

  jobtodo := job;

  buffer[0]  := $000D;   { fill buffer with data }
  buffer[1]  := $000C;
  buffer[2]  := $0000;
  buffer[3]  := $0000;
  buffer[4]  := $0000;
  buffer[5]  := $0000;
  buffer[6]  := $0000;
  buffer[7]  := ofs(jobtodo);
  buffer[8]  := seg(jobtodo);
  buffer[9]  := $0001;
  buffer[10] := $0000;
  buffer[11] := $0000;
  buffer[12] := $0000;

  asm
    pusha
    push ds
    pop  es
    mov  bx,offset buffer[0]
    mov  cx,drive
    mov  ax,1510h
    int  2Fh
    popa
  end;
end;


begin
  writeln ('Opening Door');
  cddoor (0, 3);
  writeln ('Press ENTER to continue...');
  readln;
  writeln ('Closing Door');
  cddoor (5, 3);
end.
