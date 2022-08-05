{$A+,B-,D+,E-,F+,I-,L+,N-,O+,R-,S-,V-}

UNIT diskio;

INTERFACE

USES dos;

CONST Read58: ARRAY[0..5] OF Byte =(

  $CD,$25,                                                                 {  INT  25H            }
  $59,                                                                     {  POP  CX             }
  $CA,$02,$00);                                                            {  RETF 2              }

  Write58: ARRAY[0..5] OF Byte =(

    $CD,$26,                                                               {  INT  26H            }
    $59,                                                                   {  POP  CX             }
    $CA,$02,$00);                                                          {  RETF 2              }

TYPE Split      = RECORD
                    O: Word;
                    S: Word;
                  END;

TYPE filtyp     = FILE OF ARRAY[0..511] OF Byte;
     fileptr    = ^filtyp;
     boottyp    = ARRAY[36..511] OF Byte;

TYPE bpbtyp     =  RECORD
                     jmp: ARRAY[1..3] OF Byte;                     {Die ersten drei Bytes fr JUMP}
                     oem: ARRAY[1..8] OF Char;                                        {OEM-Eintrag}
                     bps: Word;                                                  {Bytes pro Sektor}
                     spc: Byte;                                              {Sektoren pro Cluster}
                     res: Word;                                                     {BOOT-Sektoren}
                     fat: Byte;                                                  {Anzahl der FAT's}
                     rde: Word;                                          {Basisverzeichniseintr„ge}
                     sec: Word;                                       {Gesamtsektoren der Diskette}
                     mds: Byte;                                                  {Media-Deskriptor}
                     spf: Word;                                                  {Sektoren pro FAT}
                     spt: Word;                                                 {Sektoren pro Spur}
                     hds: Word;                                                            {Seiten}
                     shh: Longint;                                            {Versteckte Sektoren}
                     lsc: Longint;                     {Anzahl der Sektoren bei groáen Partitionen}
                     boot_code: boottyp;                                     {Puffer fr BOOT-Code}
                   END;

      dos4rw    = RECORD                                                   {Disk Read/Write Packet}
                    sector   : LongInt;                                     {fr Partitionen >=32M}
                    count    : Word;
                    Transfer : Pointer;
                  END;

TYPE  SectorTyp = Object
                    data: Pointer;
                    Start: LongInt;
                    datalen: Word;
                    Constructor init(VAR allocated: Boolean);
                    PROCEDURE Error(lw,rw,err:Byte; VAR er:Boolean; Sector:Longint); virtual;
                    PROCEDURE DiskRw(rw,lw:Byte; Sector:LongInt; Count:Byte; Transfer:Pointer);
                    PROCEDURE Readx(lw: Byte; x: LongInt);
                    PROCEDURE Writex(lw: Byte; x: LongInt);
                    Destructor Done;
                  END;


TYPE CylTyp    = Object (SectorTyp)
                   Constructor init(spcyl: Word; VAR allocated: Boolean);
                   PROCEDURE Readx(lw: Byte; x: Word);
                   PROCEDURE Writex(lw: Byte; x: Word);
                 END;


TYPE BootSecTyp = Object(SectorTyp)
                    bpb: ^bpbtyp;
                    status: Word;
                    Media: Byte;
                    UnknownDrive: Boolean;
                    dos4: Boolean;
                    Constructor init(VAR allocated: Boolean);
                    PROCEDURE Readx(lw: Byte);
                    PROCEDURE Writex(lw: Byte);
                  END;

TYPE STyp       = ARRAY[0..0] OF ^SectorTyp;
     CTyp       = ARRAY[0..0] OF ^CylTyp;
     Smtyp      = ^Styp;
     Cmtyp      = ^CTyp;



VAR BootSec         : BootSecTyp;
  old58             : Pointer;
  maxsec            : Word;
  maxcyl            : Word;


  PROCEDURE CheckDrive(lw:Byte; VAR Status:Word; VAR error1:Boolean; VAR Media:Byte);
  PROCEDURE DeallocCyl(Var Cylmem:Cmtyp; Stop:Word);
  PROCEDURE DeallocSec(Var Secmem:Smtyp; Stop:Word);
  FUNCTION AllocCyl(VAR Cylmem:Cmtyp; Stop:Word): Word;
  FUNCTION AllocSec(VAR secmem:Smtyp; stop:Word): Word;
  FUNCTION ReadKey: Char;

IMPLEMENTATION

  FUNCTION ReadKey:Char;
  VAR r: Registers;
  BEGIN
    WITH r DO BEGIN
      ah:=8;
      msdos(r);
      ReadKey:=chr(r.al);
    END;
  END;

  PROCEDURE Sectortyp.error(lw,rw,err:Byte; VAR er:Boolean; Sector:Longint);
  VAR chx: Char;
  BEGIN
    WITH BootSec DO BEGIN
      WriteLn;
      IF rw=0 THEN
        Write('Read')
      ELSE
        Write('Write');
      Write('-Error Drive ',chr(lw+$40),': ');
      CASE err OF
        $00: Write('Disk is write protected');
        $01: Write('Unknown unit');
        $02: Write('Drive not ready');
        $03: Write('Unknown command');
        $04: Write('Bad CRC');
        $05: Write('Bad request structure length');
        $06: Write('Seek error');
        $07: Write('Unknown media type');
        $08: Write('Sector not found');
        $09: Write('Printer out of paper');
        $0A: Write('Write fault');
        $0B: Write('Read fault');
        $0C: Write('General failure');
        $0D: Write('Sharing violation');
        $0E: Write('Lock violation');
        $0F: Write('Invalid disk change');
        $10: Write('FCB unavailable');
        $11: Write('Sharing buffer overflow');
        ELSE Write('Unknown error');
      END;
      Writeln('.');
      Write('Error ',err,': Sector: ',Sector,' ');
      IF Sector=0 THEN
        WriteLn('BOOT-Sector')
      ELSE BEGIN
        IF (Sector>=1) and (Sector<=bpb^.spf) THEN
          WriteLn('FAT 1');
        IF (Sector>=bpb^.spf+1) and (sector<=Longint(bpb^.spf) shl 1) THEN
          WriteLn('FAT 2');
      END;
      REPEAT
        Write('(A)bort, (R)etry, (I)gnore ? ');
        chx:=Upcase(ReadKey); WriteLn(chx);
      UNTIL chx IN ['A','I','R'];
      CASE chx OF
        'A': Halt(255);
        'I': BEGIN
               er:=False;
             END;
        'R': er:=True;
      END;
    END;
  END;

  Constructor SectorTyp.init(VAR allocated: Boolean);
  BEGIN
    allocated:=True;
    IF MaxAvail<512 THEN allocated:=False;
    IF allocated THEN BEGIN
      GetMem(self.data,512);
      datalen:=512;
    END;
  END;

  PROCEDURE SectorTyp.DiskRw(rw,lw:Byte; Sector:Longint; Count:Byte; Transfer:Pointer);
  VAR regs: registers;

  VAR er    : Boolean;
    i       :  Word;
    rwpacket:  dos4rw;

  BEGIN
    WITH regs DO BEGIN
      GetIntVec($58,old58);
      al:=lw-1;
      IF NOT(BootSec.dos4) THEN BEGIN                                      {Parameter fr DOS 2.00-3.30}
        dx:=sector;
        cx:=count;
        bx:=LongInt(Transfer) AND $ffff;
        ds:=LongInt(Transfer) SHR 16;
      END ELSE BEGIN                                    {Parameter ab DOS 4.00 und COMPAQ DOS 3.31}
        cx:=$FFFF;
        rwpacket.sector:=sector;
        rwpacket.count:=count;
        rwpacket.Transfer:=Transfer;
        ds:=Seg(rwpacket);
        bx:=Ofs(rwpacket);
      END;
      IF rw=0 THEN SetIntVec($58,@Read58) ELSE SetIntVec($58,@Write58);
      intr($58,regs);
      IF (FCarry AND Flags) <> 0 THEN
        FOR i:=0 TO Count-1 DO
          REPEAT
            al:=lw-1;
            IF NOT(BootSec.dos4) THEN BEGIN
              dx:=Sector+i;
              cx:=1;
              bx:=LongInt(Transfer) AND $ffff;
              ds:=(LongInt(Transfer) SHR 16)+(i SHL 5);
            END ELSE BEGIN
              cx:=$FFFF;
              rwpacket.sector:=Sector+i;
              rwpacket.count:=1;
              rwpacket.Transfer:=ptr((Longint(Transfer) SHR 16)+(i SHL 5),
                                     LongInt(Transfer) and $ffff);
              ds:=Seg(rwpacket);
              bx:=Ofs(rwpacket);
            END;
            IF rw=0 THEN SetIntVec($58,@Read58) ELSE SetIntVec($58,@Write58);
            intr($58,regs);
            SetIntVec($58,old58);
            er:=False;
            IF (FCarry AND Flags) <> 0 THEN error(lw,rw,regs.al,er,Sector+i);
          UNTIL NOT er;
      SetIntVec($58,old58);
    END;
  END;

  PROCEDURE SectorTyp.Readx(lw: Byte; x: LongInt);
  BEGIN
    self.DiskRw(0,lw,x,1,self.data);
  END;

  PROCEDURE SectorTyp.Writex(lw: Byte; x: LongInt);
  BEGIN
    self.DiskRw(1,lw,x,1,self.data);
  END;

  Constructor CylTyp.init(spcyl: Word; VAR allocated: Boolean);
  BEGIN
    allocated:=True;
    datalen:=spcyl SHL 9;
    IF MaxAvail<datalen THEN allocated:=False;
    IF allocated THEN BEGIN
      GetMem(self.data,datalen);
    END;
  END;

  PROCEDURE CylTyp.Readx(lw: Byte; x:Word);
  BEGIN
    self.DiskRw(0,lw,LongInt(x)*(datalen SHR 9),datalen SHR 9,self.data);
  END;

  PROCEDURE CylTyp.Writex(lw: Byte; x:Word);
  BEGIN
    self.DiskRw(1,lw,LongInt(x)*(datalen SHR 9),datalen SHR 9,self.data);
  END;

  Constructor BootSecTyp.init(VAR allocated: Boolean);
  BEGIN
    allocated:=True;
    IF MaxAvail<512 THEN allocated:=False;
    IF allocated THEN BEGIN
      GetMem(self.data,512);
      self.bpb:=self.data;
      datalen:=512;
    END;
  END;

  PROCEDURE BootSecTyp.Readx(lw: Byte);
  BEGIN
    CheckDrive(lw,self.status,self.UnknownDrive,self.Media);
    self.dos4:=false;
    if not(UnknownDrive) and ((self.status and $9202)=2) then
      self.dos4:=true;
    if not(UnknownDrive) and ((self.status and $9200)=0) then
    self.DiskRw(0,lw,0,1,self.data);
  END;

  PROCEDURE BootSecTyp.Writex(lw: Byte);
  BEGIN
    self.DiskRw(1,lw,0,1,self.data);
  END;

  Destructor SectorTyp.Done;
  BEGIN
    FreeMem(self.data,datalen);
  END;

  FUNCTION AllocSec(VAR secmem:Smtyp; Stop:Word): Word;
  VAR i: Word;
    ok: Boolean;
  BEGIN
    GetMem(secmem,(Stop+1)*4);
    FOR i:=0 to Stop do Secmem^[i]:=NIL;
    i:=0;
    REPEAT
      IF (4512>MaxAvail) OR (secmem^[i]<>NIL) THEN
        ok:=False
      ELSE BEGIN
        New(secmem^[i],init(ok));
        IF ok THEN Inc(i);
      END;
    UNTIL NOT(ok) OR (i>stop);
    Dec(i);
    AllocSec:=i;
  END;

  FUNCTION AllocCyl(VAR Cylmem:Cmtyp; Stop:Word): Word;
  VAR i: Word;
    ok: Boolean;
  BEGIN
    GetMem(cylmem,(Stop+1)*4);
    FOR i:=0 to Stop do Cylmem^[i]:=NIL;
    i:=0;
    REPEAT
      IF (((BootSec.bpb^.spt*BootSec.bpb^.hds) SHL 9)+4000>MaxAvail) OR
      (cylmem^[i]<>NIL) THEN
        ok:=False
      ELSE BEGIN
        New(cylmem^[i],init(BootSec.bpb^.spt*BootSec.bpb^.hds,ok));
        IF ok THEN Inc(i);
      END;
    UNTIL NOT(ok) OR (i>stop);
    Dec(i);
    AllocCyl:=i;
  END;

  PROCEDURE DeallocCyl(Var Cylmem:Cmtyp; Stop:Word);
  VAR i: Word;
  BEGIN
    FOR i:=0 TO stop DO BEGIN
      Dispose(cylmem^[i],Done);
    END;
    FreeMem(cylmem,(Stop+1)*4);
  END;

  PROCEDURE DeallocSec(Var Secmem:Smtyp; Stop:Word);
  VAR i: Word;
  BEGIN
    FOR i:=0 TO stop DO BEGIN
      Dispose(secmem^[i],Done);
    END;
    FreeMem(secmem,(Stop+1)*4);
  END;

  PROCEDURE CheckDrive(lw:Byte; VAR Status:Word; VAR error1:Boolean; VAR Media:Byte);
  VAR regs: registers;
    driveinfo : ARRAY[0..48] OF Byte;
  BEGIN
    WITH regs DO BEGIN
      ax:=$4409;
      bl:=lw;
      bh:=0;
      intr($21,regs);
      error1:=(FCarry AND Flags) <> 0;
      Status:=dx;
      ax:=$440d;
      cx:=$860;
      bl:=lw;
      bh:=0;
      dx:=Ofs(driveinfo);
      ds:=Seg(driveinfo);
      intr($21,regs);
      Media:=driveinfo[1];
    END;
  END;

END.