USES baseconv;

VAR c: Byte;
  f:   FILE OF Byte;
  g:   Text;
  i:   Word;

BEGIN
  Assign(f,ParamStr(1));
  Assign(g,'BOOT.DAT');
  Reset(f);
  Rewrite(g);
  FOR i:=1 TO 512 DO BEGIN
    Read(f,c);
    Write(g,'$',hexf(c,2),',');
    IF i MOD 16=0 THEN WriteLn(g);
  END;
  Close(f);
  Close(g);
END.
