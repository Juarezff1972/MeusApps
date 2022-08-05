{$A+,B-,D+,E+,F-,I-,L+,N-,O-,R-,S-,V-}
{$M 8192,10000,655360 }

uses dos;

type boottyp   = array[30..511] of byte;
     fattyp    = array[0..4095] of byte;

     bpbtyp  =  record
		  jmp: array[1..3] of byte;  {Die ersten drei Bytes fÅr JUMP}
		  oem: array[1..8] of char;  {OEM-Eintrag}
		  bps: word;                 {Bytes pro Sektor}
		  spc: byte;                 {Sektoren pro Cluster}
		  res: word;                 {BOOT-Sektoren}
		  fat: byte;                 {Anzahl der FAT's}
		  rde: word;                 {BasisverzeichniseintrÑge}
		  sec: word;                 {Gesamtsektoren der Diskette}
		  mds: byte;                 {Media-Deskriptor}
		  spf: word;                 {Sektoren pro FAT}
		  spt: word;                 {Sektoren pro Spur}
		  hds: word;                 {Seiten}
		  shh: word;                 {Versteckte Sektoren}
		  boot_code: boottyp;        {Puffer fÅr BOOT-Code}
		end;

var regs:        registers;                  {Prozessor-Register}
    x:           word;                       {Hilfsvariable}
    old58:       pointer;                    {Speicher fÅr Interrupt 58}
    src:	 bpbtyp;                     {BOOT-Sektor Quelldiskette}
    dst:         bpbtyp;                     {BOOT-Sektor Zieldiskette}
    chx:         Char;                       {Hilfsvariable}
    para:	 String[5];                  {Kommandozeileneingabe}
    i:           Word;                       {Hilfsvariable}
    fat:         ^fattyp;                    {FAT der Quelldiskette}
    lwsrc,lwdst: byte;                       {Quell/Ziellaufwerksnummer}
    driveinfo:   array[0..30] of byte;       {Laufwerksbeschreibung}
    Sector:      Word;                       {Sektor, ab dem kopiert wird}
    memory:      Pointer;                    {Hilfsvariable}
    memfree:     Word;                       {Freier Speicher in Sektoren}
    StartSeg:    Word;                       {Segment des freien Speichers}
    StartOfs:    Word;                       {Offset des freien Speichers}
    SourceIn:    Boolean;                    {True, wenn Quelldisk eingelegt}
    WantSource:  Boolean;                    {True, wenn Quelldisk benîtigt}
    CopyAll:     Boolean;                    {True, wenn /A Parameter}
    First:       Boolean;                    {True, wenn Zieldisk 1.Mal kommt}
    BadCluster:  Array[1..20] of Word;       {Tabelle der schlechten Cluster}
    BCCount:     Byte;                       {Counter fÅr BadCluster}

const Read58: Array[0..5] of Byte =(

            $CD,$25,           {  INT  25H            }
            $59,               {  POP  CX             }
            $CA,$02,$00);      {  RETF 2              }

      Write58: Array[0..5] of Byte =(

            $CD,$26,           {  INT  26H            }
            $59,               {  POP  CX             }
            $CA,$02,$00);      {  RETF 2              }


Function ReadKey:Char;
Var r:Registers;
begin
  with r do begin
    ah:=7;
    intr($21,r);
    if al in [3,27] then begin writeln; halt end;
    ReadKey:=chr(al);
  end;
end;

Function Cluster(Sector: Word):Word;
Var h: byte;
begin
  Cluster:=((Sector-(src.rde shr 4)
            -(src.spf shl 1)-1)
           div Word(src.spc))+2;
end;

Function ClusterCont(Cluster:Word):Word;
Var Offset: Word;
begin
  Offset:=Cluster*3 shr 1;
  if Cluster and 1 = 0 then
    ClusterCont:=(fat^[offset]+Word(fat^[offset+1]) shl 8) and $fff
  else
    ClusterCont:=((fat^[offset]+Word(fat^[offset+1]) shl 8) and $fff0) shr 4;
end;

Procedure FreeBadCluster(Cluster:Word);
Var Offset:Word;
begin
  if (Cluster>1) and (Cluster<4096) and (ClusterCont(Cluster)=$ff7) then begin
    Offset:=Cluster*3 shr 1;
    if Cluster and 1 = 0 then begin
      fat^[offset]:=0;
      fat^[offset+1]:=fat^[offset+1] and $F0;
    end else begin
      fat^[offset+1]:=0;
      fat^[offset]:=fat^[offset] and $F;
    end;
  end;
end;

Procedure MarkBadClusters;
Var i,j,Offset:Word;
begin
  for i:=1 to BCCount do begin
    j:=BadCluster[i];
    if ClusterCont(j)=0 then begin
      Offset:=j*3 shr 1;
      writeln('Cluster ',j,' auf der Zieldiskette als schlecht markiert');
      if j and 1 = 0 then begin
        fat^[offset]:=$F7;
        fat^[offset+1]:=fat^[offset+1] or $F
      end else begin
        fat^[offset+1]:=$FF;
        fat^[offset]:=fat^[offset] or $70
      end;
    end;
  end;
  BCCount:=0;
end;

Procedure Insert;
begin
  writeln('Anschlie·end die Eingabetaste betÑtigen (ESC=Abbruch)');
  repeat until Readkey=#13;
  writeln;
end;


Procedure InsertSource;
begin
  writeln;
  writeln('Quelldiskette in Laufwerk ',chr(lwsrc+$40),': einlegen.');
  SourceIn:=true;
  Insert;
end;

Procedure InsertDest;
begin
  writeln;
  writeln('Zieldiskette in Laufwerk ',chr(lwdst+$40),': einlegen.');
  Insert;
  SourceIn:=false;
end;

Procedure error(rw:byte; Var er:boolean; Sector:Word);
Var c,d: Word;
begin
  writeln;
  write('Fehler ',regs.al,': Sektor: ',Sector,' ');
  c:=Cluster(Sector);
  if Sector=0 then writeln('BOOT-Sektor');
  if Sector in [1..src.spf] then writeln('FAT 1');
  if Sector in [src.spf+1..src.spf shl 1] then writeln('FAT 2');
  if (Sector > src.spf shl 1) and ((c<2) or (c>4096)) then
    writeln('Basisverzeichnis');
  if (c>1) and (c<4096) then begin
    write('Cluster ',c,': ');
    d:=ClusterCont(c);
    case d of
      0: writeln('Freier Cluster');
      $ff7: writeln('Schlechter Cluster');
      else writeln('Belegter Cluster');
    end;
  end;
  repeat
    write('(A)bbrechen, (W)iederholen, (I)gnorieren ? ');
    chx:=UpCase(ReadKey); writeln(chx);
  until chx in ['A','I','W'];
  case chx of
    'A': halt;
    'I': begin
           er:=false;
           if (rw=1) and (c>1) and (c<4096) then begin
             inc(BCCount);
             BadCluster[BCCount]:=c;
           end;
         end;
    'W': er:=true;
  end;
end;

Procedure DiskReadWrite(rw,lw:Byte; Sector:Word; Count:Byte; Transfer:Pointer);
Var er:boolean;
begin
  if (lwdst=lwsrc) then begin
    if not(WantSource) and SourceIn then InsertDest;
    if WantSource and not(SourceIn) then InsertSource;
  end;
  with regs do begin
  GetIntVec($58,old58);
  al:=lw-1;
  dx:=Sector;
  cx:=Count;
  bx:=LongInt(transfer) and $ffff;
  ds:=LongInt(transfer) shr 16;
  if rw=0 then SetIntVec($58,@read58) else SetIntVec($58,@write58);
  intr($58,regs);
  if (FCarry and Flags) <> 0 then
    for i:=0 to Count-1 do
      repeat
        al:=lw-1;
        dx:=Sector+i;
        cx:=1;
        bx:=LongInt(transfer) and $ffff;
        ds:=(LongInt(transfer) shr 16)+(i shl 5);
        if rw=0 then SetIntVec($58,@read58) else SetIntVec($58,@write58);
        intr($58,regs);
        SetIntVec($58,old58);
        er:=false;
        if (FCarry and Flags) <> 0 then error(rw,er,Sector+i);
      until not er;
  SetIntVec($58,old58);
  end;
end;

Procedure ReadSystemArea(lw:Byte);
begin
  with regs do begin
    DiskReadWrite(0,lw,0,1,@src);
    writeln;
    writeln('Gesamtsektoren: ',src.sec);
    writeln('Seiten:         ',src.hds);
    writeln('Spuren:         ',src.sec div src.hds div src.spt);
    writeln('Sektoren/Spur:  ',src.spt);
    GetMem(fat,src.spf shl 9);
    DiskReadWrite(0,lw,1,src.spf,fat);
  end;
end;

Procedure CheckDrive(lw:Byte);
begin
  with regs do begin
    ax:=$4409;
    bl:=lw;
    bh:=0;
    intr($21,regs);
    if (FCarry and Flags) <> 0 then begin
      writeln(chr(lw+$40),': ist kein gÅltiges Laufwerk');
      halt;
    end;
    if (dx and $9200)<>0 then begin
      writeln(chr(lw+$40),': SUBST oder ASSIGN wurde zur Umleitung verwendet.');
      halt;
    end;
    ax:=$440d;
    cx:=$860;
    bl:=lw;
    bh:=0;
    dx:=ofs(driveinfo);
    ds:=seg(driveinfo);
    intr($21,regs);
    if not(driveinfo[1] in [0,1,2,7]) then begin
      writeln(chr(lw+$40),': ist kein Floppy-Laufwerk');
      halt;
    end;
  end;
end;

Procedure SyntaxError;
begin
  writeln('Syntax Fehler bei der Eingabe!'#10#13);
  writeln('Korrektes Format ist: '#10#13);
  writeln('FDCOPY Quellaufwerk: Ziellaufwerk: [/A]'#10#13);
  writeln('Die /A Option bewirkt, da· auch freie Cluster kopiert werden'#10#13);
  writeln('Beispiel FDCOPY a: b: /A'#10#13);
  halt;
end;

Procedure CheckMemory;
begin
  memfree:=(MaxAvail shr 9);
  GetMem(Memory,1);
  StartSeg:=LongInt(Memory) shr 16;
  StartOfs:=LongInt(Memory) and $ffff;
  FreeMem(Memory,1);
  writeln('Im Speicher ist Platz fÅr ',memfree,' Sektoren.');
  writeln;
  if memfree=0 then begin
    writeln('Zu wenig Speicher');
    halt;
  end;
  if StartOfs>$8000 then begin
    StartOfs:=StartOfs-$8000;
    StartSeg:=StartSeg+$800;
  end;
end;

Function MustCopy(i:Word):Byte;
Var j:Word;
begin
  if i<=src.spf shl 1 then
    MustCopy:=0
  else begin
    j:=Cluster(i);
    if (j<2) or (j>4095) then
      MustCopy:=2
    else begin
      j:=ClusterCont(j);
      if (j=0) or (j=$ff7) then
        MustCopy:=1
      else
        MustCopy:=2;
    end;
  end;
end;

Procedure DoOneBlock(rw:Byte);
Var SectorSave:  Word;            {Speicher fÅr Startsektor}
    i,j:         Word;            {Hilfsvariablen}
    SectorsLeft: Word;            {Noch verbleibender Speicherplatz}
    Segment:     Word;            {Segment fÅr aktuelle Sektoren}
    cnt:         Byte;            {Anzahl der zu lesenden Sektoren}
    StartSec:    Word;            {Startsektor eines Lese/Schreibvorgangs}
    EndSec:      Word;            {Endsektor eines Lese/Schreibvorgangs}
    Copy:        Byte;            {1=Alle Sektoren/2=Nur belegte Sektoren}
begin
  SectorSave:=Sector;
  Segment:=StartSeg;
  SectorsLeft:=memfree;
  repeat
    if CopyAll then Copy:=1 else Copy:=2;
    StartSec:=$ffff;
    for i:=Sector to Sector+src.spt-1 do begin
      if MustCopy(i)>=Copy then EndSec:=i;
      j:=(Sector shl 1)+src.spt-i-1;
      if MustCopy(j)>=Copy then StartSec:=j;
    end;
    if StartSec<>$ffff then begin
      cnt:=EndSec-StartSec+1;
      write('Speicher: ',SectorsLeft-cnt,', Sektoren: ',Startsec,'-',EndSec);
      x:=Cluster(Startsec);
      if (x>1) and (x<4096) then
        write(', Cluster: ',x,'-',Cluster(EndSec),'         ')
      else
        write(' Basisverzeichnis       ');
      write(#13);
      Case rw of
        0: begin
             WantSource:=true;
             DiskReadWrite(0,lwsrc,Startsec,cnt,ptr(Segment,StartOfs));
           end;
        1: begin
             WantSource:=false;
             DiskReadWrite(1,lwdst,Startsec,cnt,ptr(Segment,StartOfs))
           end;
      end;
      SectorsLeft:=SectorsLeft-cnt;
      Segment:=Segment+(cnt shl 5);
    end;
    for i:=Sector to Sector+src.spt-1 do
      FreeBadCluster(Cluster(i));
    MarkBadClusters;
    Sector:=Sector+src.spt;
  until (Sector>=src.sec) or (SectorsLeft<src.spt);
  if rw=0 then Sector:=SectorSave;
end;

begin
  writeln;
  writeln('FDCOPY - Diskcopy Programm fÅr alle Formate -- Ver 1.00');
  writeln('Copyright (c) 30. August 1988, Christoph H. HochstÑtter');
  writeln;
  SourceIn:=false;
  WantSource:=true;
  CopyAll:=false;
  BCCount:=0;
  para:=ParamStr(1);
  if length(para)<>2 then SyntaxError;
  if para[2]<>':' then SyntaxError;
  lwsrc:=ord(UpCase(para[1]))-$40;
  para:=ParamStr(2);
  if length(para)<>2 then SyntaxError;
  if para[2]<>':' then SyntaxError;
  lwdst:=ord(UpCase(para[1]))-$40;
  if (ParamStr(3)='/A') or (ParamStr(3)='/a') then
    CopyAll:=true
  else
    if ParamStr(3)<>'' then SyntaxError;
  InsertSource;
  CheckDrive(lwsrc);
  ReadSystemArea(lwsrc);
  if lwsrc<>lwdst then begin
    WantSource:=false;
    InsertDest;
    CheckDrive(lwdst);
  end;
  CheckMemory;
  Sector:=0;
  first:=true;
  repeat
    DoOneBlock(0);
    if first then begin
      WantSource:=false;
      first:=false;
      DiskReadWrite(0,lwdst,0,1,@dst);
      if src.sec <> dst.sec then begin
        writeln(#10#10'Zieldiskette ist nicht kompatibel');
        halt;
      end;
    end;
    DoOneBlock(1);
  until Sector>=src.sec;
  src.oem:='CH-COPY1';
  src.spt:=dst.spt;
  src.hds:=dst.hds;
  if src.spc=dst.spc then begin
    src.mds:=dst.mds;
    fat^[0]:=dst.mds;
  end;
  WantSource:=false;
  DiskReadWrite(1,lwdst,0,1,@src);
  DiskReadWrite(1,lwdst,1,src.spf,fat);
  DiskReadWrite(1,lwdst,1+src.spf,src.spf,fat);
  writeln(#13#10#10'Kopie erstellt');
end.
