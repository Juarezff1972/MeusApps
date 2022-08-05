{$A+,B-,D+,E-,F-,I-,L+,N-,O-,R-,S-,V-}
{$M 4096,0,655360}
PROGRAM WIMAGE;

  {WIMAGE - Write Diskette to file / Read Diskette from file - Ver 1.10}
  {Compiled with Turbo-Pascal Ver 5.5}

USES dos,diskio;

VAR para   : String[50];
  fn       : String[50];
  f        : FILE;
  lw       : Byte;
  ok       : Boolean;
  trk      : Word;
  rest     : Word;
  t,rt     : Word;
  j        : Word;
  written  : Word;
  xsec     : LongInt;
  readdisk : Boolean;
  FileBoot : ^bpbtyp;
  ysec     : LongInt;
  ftrk     : Word;
  CylMem   : CmTyp;

  PROCEDURE SyntaxError;
  BEGIN
    WriteLn('Syntax Error.');
    WriteLn;
    WriteLn('Syntax is:   WIMAGE <drive>: <file>  (write Diskette to file)');
    WriteLn('             WIMAGE <file> <drive>:  (read Diskette from file)');
    WriteLn;
    WriteLn('Exapmles: WIMAGE A: C:\DISKS\MYDISK.360');
    WriteLn('          WIMAGE C:\DISKS\DOS330.144 A:');
    WriteLn;
    Halt(1);
  END;

  PROCEDURE BootError;
  BEGIN
    WriteLn('File ',fn,' is no diskette image.');
    Close(f);
    Halt(9);
  END;

  PROCEDURE DosError;
  BEGIN
    WriteLn('This program requires DOS 3.20 or higher.');
    Halt(10);
  END;

BEGIN
  WriteLn;
  WriteLn('WIMAGE-Write Disk to File/Read Disk from File-V1.10');
  WriteLn('Copyright (c) 1988 - 1989, Christoph H. Hochst„tter');
  WriteLn;
  IF Swap(DosVersion)<$314 THEN DosError;
  IF (Length(ParamStr(1))=2) AND (Length(ParamStr(2))=2) THEN SyntaxError;
  IF Length(ParamStr(1))=2 THEN BEGIN
    para:=ParamStr(1);
    fn:=ParamStr(2);
    readdisk:=True;
  END ELSE IF Length(ParamStr(2))=2 THEN BEGIN
    para:=ParamStr(2);
    fn:=ParamStr(1);
    readdisk:=False;
  END ELSE
    SyntaxError;
  IF fn='' THEN SyntaxError;
  FOR lw:=1 TO Length(fn) DO fn[lw]:=Upcase(fn[lw]);
  IF para[2]<>':' THEN SyntaxError;
  lw:=Ord(Upcase(para[1]))-$40;
  BootSec.init(ok);
  IF NOT(ok) THEN BEGIN
    WriteLn('Not enough Memory.');
    Halt(4);
  END;
  BootSec.Readx(lw);
  IF BootSec.UnknownDrive THEN BEGIN
    WriteLn('Drive does not exist.');
    Halt(3);
  END;
  IF (BootSec.Status AND $9200) <> 0 THEN BEGIN
    WriteLn('WIMAGE does not work with a SUBST/ASSIGN/NETWORK Drive.');
    Halt(2);
  END;
  IF BootSec.Media=5 THEN BEGIN
    WriteLn('WIMAGE does not handle fixed disks.');
    Halt(8);
  END;
  WITH BootSec.bpb^ DO BEGIN
    IF sec<>0 THEN
      xsec:=sec
    ELSE
      xsec:=lsc;
    rest:=xsec MOD (hds*spt);
    IF rest<>0 THEN BEGIN
      WriteLn('This disk has hidden sectors.');
      Halt(5);
    END;
    trk:=xsec DIV (hds*spt);
    WriteLn('Information from the floppy:');
    WriteLn('Tracks       : ',trk);
    WriteLn('Sectors/Track: ',spt);
    WriteLn('Sides        : ',hds);
    IF readdisk THEN BEGIN
      WriteLn('Bytes total  : ',DiskSize(lw));
      WriteLn('Bytes free   : ',DiskFree(lw));
    END ELSE ysec:=DiskSize(lw);
    WriteLn;
    Assign(f,fn);
    IF readdisk THEN
      Rewrite(f,1)
    ELSE
      Reset(f,1);
    IF IoResult<>0 THEN BEGIN
      WriteLn('File ',fn,' cannot be openend.');
      Close(f);
      Halt(6);
    END;
    IF NOT(readdisk) THEN BEGIN
      GetMem(FileBoot,512);
      BlockRead(f,FileBoot^,512,written);
      IF (written<>512) OR
      (FileBoot^.boot_code[511]<>$AA) OR
      (FileBoot^.boot_code[510]<>$55) THEN BEGIN
        BootError;
      END;
      IF FileBoot^.sec=0 THEN
        ysec:=FileBoot^.lsc
      ELSE
        ysec:=FileBoot^.sec;
      ftrk:=ysec DIV (FileBoot^.spt*FileBoot^.hds);
      WriteLn('Information from the Image-File');
      WriteLn('Tracks       : ',ftrk);
      WriteLn('Sectors/Track: ',FileBoot^.spt);
      WriteLn('Sides        : ',FileBoot^.hds);
      WriteLn;
      IF (ftrk<>trk) OR (FileBoot^.spt<>spt) OR (FileBoot^.hds<>hds) THEN BEGIN
        Close(f);
        WriteLn('Source File and Destination Disk have different formats.');
        Halt(11);
      END;
      Seek(f,0);
      FreeMem(FileBoot,512);
    END;
    maxcyl:=AllocCyl(CylMem,trk-1);
    WriteLn(Succ(maxcyl)*LongInt(CylMem^[0]^.Datalen),' Bytes available for ',maxcyl+1,' Cylinders.');
    WriteLn;
    t:=0;
    IF readdisk THEN BEGIN
      WHILE t<trk DO BEGIN
        IF t+maxcyl>trk-1 THEN rt:=trk-t ELSE rt:=maxcyl+1;
        Write('Reading',rt:3,' Cylinders from',t:3,' to',t+rt-1:3,' ');
        FOR j:=0 TO rt-1 DO BEGIN
          CylMem^[j]^.Readx(lw,t+j);
        END;
        WriteLn('- Writing to File ',fn);
        FOR j:=0 TO rt-1 DO BEGIN
          BlockWrite(f,CylMem^[j]^.data^,CylMem^[j]^.Datalen,written);
          IF IoResult<>0 THEN BEGIN
            WriteLn('Error writing to file ',fn);
            Close(f);
            Halt(13);
          END;
          IF written<>CylMem^[j]^.Datalen THEN BEGIN
            WriteLn('Disk Full - File: ',fn);
            Close(f);
            Halt(7);
          END;
        END;
        t:=t+rt;
      END;
    END ELSE BEGIN
      WHILE t<trk DO BEGIN
        IF t+maxcyl>trk-1 THEN rt:=trk-t ELSE rt:=maxcyl+1;
        Write('Reading from file ',fn,' ');
        FOR j:=0 TO rt-1 DO BEGIN
          BlockRead(f,CylMem^[j]^.data^,CylMem^[j]^.Datalen,written);
          IF (written<>CylMem^[j]^.Datalen) OR (IoResult<>0) THEN BEGIN
            WriteLn('Error reading from File ',fn);
            Close(f);
            Halt(12);
          END;
        END;
        WriteLn('- Writing',rt:3,' Cylinders from',t:3,' to',t+rt-1:3);
        FOR j:=0 TO rt-1 DO BEGIN
          CylMem^[j]^.Writex(lw,t+j);
        END;
        t:=t+rt;
      END;
    END;
    Close(f);
  END;
END.
