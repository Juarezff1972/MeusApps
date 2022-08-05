{$A+,B-,D+,E+,F-,I-,L+,N-,O-,R-,S-,V-}
{$M 8192,0,0}
PROGRAM FDFORMAT;

USES dos,baseconv,desqview;

  {Copyright (c) 1988, Christoph H. HochstÑtter}
  {Compiled in Turbo-Pascal 5.5}
  {Should Compile in Turbo-Pascal 5.0 also}
  {Last Updated: 03-Sep-1989}

  {$IFDEF L49}

CONST text01 = 'Fehler ';
CONST text02 = '(A)bbrechen (W)iederholen (I)gnorieren ? ';
CONST t3     = 'W';
CONST text04 = 'Kein gÅltiges Laufwerk.';
CONST text05 = 'SUBST/ASSIGN/Netzwerk-Laufwerk.';
CONST text06 = 'Kein Floppy-Laufwerk.';
CONST text07 = 'Vîllig unbekannte Laufwerksart';
CONST text08 = 'Ich formatiere Laufwerk ';
CONST text09 = ' Seite(n), ';
CONST text10 = ' Spuren, ';
CONST text11 = ' Sektoren/Spur, ';
CONST text12 = ' BasisverzeichniseintrÑge, ';
CONST text13 = ' Sektor(en)/Cluster, Sektoren-Versatz: ';
CONST text14 = 'Kopf: ';
CONST text15 = ', Zylinder: ';
CONST text17 = 'Formatierfehler im Systembereich: Programm abgebrochen.';
CONST text18 = 'Mehr als ';
CONST text19 = ' Sektoren nicht lesbar. Programm abgebrochen.';
CONST text20 = ' als schlecht markiert';
CONST text21 = 'Format-Identifizierung:          ';
CONST text22 = 'Gesamtsektoren auf der Diskette: ';
CONST text23 = 'Sektoren pro Spur:               ';
CONST text24 = 'Schreib-/Lesekîpfe:              ';
CONST text25 = 'Bytes pro Sektor:                ';
CONST text26 = 'Versteckte Sektoren:             ';
CONST text27 = 'Boot-Sektoren:                   ';
CONST text28 = 'Anzahl der FAT''s:                ';
CONST text29 = 'Sektoren pro FAT:                ';
CONST text30 = 'Cluster auf Diskette:            ';
CONST text31 = ' Bytes GesamtkapazitÑt';
CONST text32 = ' Bytes in schlechten Sektoren';
CONST text33 = ' Bytes auf der Diskette verfÅgbar';
CONST text34 = 'Dieses Laufwerk kann nicht formatiert werden.';
CONST text35 = 'Laufwerk ist physisch ';
CONST text36 = 'BIOS Umschaltung 40/80 Spuren: ';
CONST text37 = 'nach XT-Standard';
CONST text38 = 'nach EPSON QX-16 Standard';
CONST text39 = 'nach AT-Standard';
CONST text40 = 'wird nicht unterstÅtzt';
CONST text41 = 'Syntax Error beim Aufruf.';
CONST text42 = 'Format ist: FDFORMAT drive: [Optionen]';
CONST text43 = '  Beispiel: FDFORMAT a: t41 h2 s10 C1 D112';
CONST text44 = 'Parameter Bedeutung                              Voreinstellung';
CONST text45 = 'drive:    Laufwerk, das formatiert werden soll   ----';
CONST text46 = 'Tnn       Anzahl der Spuren je Seite             40/80 je nach Laufwerk';
CONST text47 = 'Hnn       Anzahl der Seiten                      2';
CONST text48 = 'Nnn       Anzahl der Sektoren je Spur            9/15/18 je nach Laufwerk';
CONST text49 = 'Cn        Anzahl der Sektoren je Cluster         1 bei HD, 2 bei DD';
CONST text50 = 'Dnnn      Anzahl der BasisverzeichniseintrÑge    224 bei HD, 112 bei DD';
CONST text51 = 'Inn       Interleave-Faktor                      1';
CONST text52 = 'Fnnn      Format festlegen';
CONST text53 = 'R         Formatierung nicht verifizieren';
CONST text69 = 'Bnnn      Diskettentypbyte festlegen             je nach Format';
CONST text70 = 'Gnnn      GAP-LÑnge festlegen                    je nach Format';
CONST text71 = 'Lesen Sie die READ.ME Datei fÅr weitere Optionen';
CONST text54 = 'Dieses Programm benîtigt mindestens DOS 3.20.';
CONST text55 = 'FDFORMAT -- Formatieren von Disketten mit erhîhter KapazitÑt';
CONST text56 = 'Copyright (c) 03.01.1990, Christoph H. HochstÑtter, Ver 1.60';
CONST text57 = 'Sie kînnen nur 1 oder 2 Seiten nehmen.';
CONST text58 = 'Sie sollten schon mindestens eine Spur formatieren.';
CONST text59 = 'Interleave mu· von 1-';
CONST text60 = ' sein.';
CONST text61 = 'WARNUNG! DOS verwaltet bei Disketten nur 1 oder 2 Sektoren/Cluster';
CONST text62 = 'WARNUNG! Zu viele Spuren. Das kann Ihr Laufwerk beschÑdigen';
CONST text63 = 'WARNUNG! DOS verwaltet bei Disketten maximal 240 BasisverzeichniseintrÑge';
CONST text64 = 'Neue Diskette in Laufwerk ';
CONST text65 = ': einlegen';
CONST text66 = 'Anschlie·end ENTER drÅcken (ESC=Abbruch)';
CONST text67 = 'öbertragungsrate: ';
CONST text68 = ', GAP-LÑnge: ';
CONST text79 = 'Kann alte Formatierung nicht finden.';
CONST text72 = 'EIN';
CONST text73 = 'AUS';
CONST text74 = 'Bitte Diskettennamen eingeben (max. 11 Zeichen): ';
CONST text75 = 'Fehler beim Erstellen des Namens.';
CONST text76 = 'Syntax-Fehler in der Datei FDFORMAT.CFG.';
CONST text77 = 'Lesefehler in der Datei FDFORMAT.CFG.';
CONST text78 = ', Sektoren: ';
CONST error01 = 'Falsches Disketten-Steuer-Kommando';
CONST error02 = 'Formatierung nicht gefunden';
CONST error03 = 'Diskette ist schreibgeschÅtzt';
CONST error04 = 'Sektor nicht gefunden';
CONST error06 = 'Unerlaubter Diskettenwechsel';
CONST error08 = 'DMA-Baustein Åbergelaufen';
CONST error09 = 'Mehr als 64 kByte im DMA Baustein';
CONST error0c = 'Format nicht kompatibel mit DatenÅbertragungsrate';
CONST error10 = 'Zyklische RedundanzprÅfung fehlerhaft';
CONST error20 = 'Diskettenadapter fehlerhaft';
CONST error40 = 'Laufwerkskopf konnte nicht positioniert werden';
CONST error80 = 'Keine Diskette im Laufwerk oder falsch eingelegt';
CONST errorxx = 'Fehlerursache unbekannt';

  {$ENDIF}
  {$IFDEF L1}

const text01 = 'Error ';
const text02 = '(A)bort (R)etry (I)gnore ? ';
const t3     = 'R';
const text04 = 'No valid drive.';
const text05 = 'SUBST/ASSIGN/Network-Drive.';
const text06 = 'Not a floppy drive.';
const text07 = 'Unknown drive type.';
const text08 = 'Formatting drive ';
const text09 = ' Head(s), ';
const text10 = ' Tracks, ';
const text11 = ' Sectors/track, ';
const text12 = ' Root Directory Entries, ';
const text13 = ' Sector(s)/Cluster, Sector-Shift: ';
const text14 = 'Head: ';
const text15 = ', Cylinder: ';
const text17 = 'Format error in system area: Program aborted.';
const text18 = 'More than ';
const text19 = ' sectors unreadable. Program aborted.';
const text20 = ' marked as bad';
const text21 = 'OEM-Entry:              ';
const text22 = 'Total sectors on disk:  ';
const text23 = 'Sectors per track:      ';
const text24 = 'Heads:                  ';
const text25 = 'Bytes per sector:       ';
const text26 = 'Hidden sectors:         ';
const text27 = 'Boot-sectors:           ';
const text28 = 'Number of FATs:         ';
const text29 = 'Sectors per FAT:        ';
const text30 = 'Total clusters on disk: ';
const text31 = ' total bytes on disk';
const text32 = ' bytes in bad sectors';
const text33 = ' bytes available';
const text34 = 'This drive cannot be formatted.';
const text35 = 'Drive is physical ';
const text36 = 'BIOS double-step support: ';
const text37 = 'XT-like';
const text38 = 'EPSON QX-16 like';
const text39 = 'AT-like';
const text40 = 'Not available or unknown';
const text41 = 'Syntax Error.';
const text42 = 'Usage is: FDFORMAT drive: [options]';
const text43 = ' Example: FDFORMAT a: t41 h2 s10 C1 D112';
const text44 = 'Option   Meaning                                 Default';
const text45 = 'drive:   drive to be formatted                   none';
const text46 = 'Tnn      Number of tracks                        40/80 depends on drive';
const text47 = 'Hnn      Number of heads                         2';
const text48 = 'Nnn      Number of sectors per track             9/15/18 depends on drive';
const text49 = 'Cn       Number of sectors per cluster           1 for HD, 2 for DD';
const text50 = 'Dnnn     Number of root directory entries        224 for HD, 112 for DDD';
const text51 = 'Inn      Interleave                              1';
const text52 = 'F        specify Diskette format';
const text53 = 'R        Skip verifying';
const text69 = 'Bnnn     Force a specified Format-Descriptor     depends on format';
const text70 = 'Gnnn     Use specified GAP-Length                depends on format';
const text71 = 'See the READ.ME file for other options';
const text54 = 'This program requires DOS 3.2 or higher.';
const text55 = 'FDFORMAT - Disk Formatter for High Capacity Disks - Ver 1.60';
const text56 = 'Copyright (c) 03-Jan-1990, Christoph H. HochstÑtter, Germany';
const text57 = 'Heads must be 1 or 2.';
const text58 = 'At least one track should be formatted.';
const text59 = 'Interleave must be from 1 to ';
const text60 = '.';
const text61 = 'WARNING! DOS supports only 1 or 2 sectors per cluster.';
const text62 = 'WARNING! That many tracks could cause damage to your drive.';
const text63 = 'WARNING! DOS supports a maximum of 240 root directory entries.';
const text64 = 'Insert new Diskette in drive ';
const text65 = ':';
const text66 = 'Press ENTER when ready (ESC=QUIT)';
const text67 = 'Data Transfer Rate: ';
const text68 = ', GAP-Length: ';
const text79 = 'Cannot find old format of diskette.';
const text72 = 'ON';
const text73 = 'OFF';
const text74 = 'Enter Volume Name (max. 11 characters): ';
const text75 = 'Error creating volume label.';
const text76 = 'Syntax Error in FDFORMAT.CFG.';
const text77 = 'Error reading FDFORMAT.CFG.';
const text78 = ', Sectors: ';
CONST error01 = 'Illegal Command. Bug in FDFORMAT';
CONST error02 = 'Address mark not found';
CONST error03 = 'Disk is write protected';
CONST error04 = 'Sector not found';
CONST error06 = 'Illegal disk change';
CONST error08 = 'DMA overrun';
CONST error09 = 'DMA accross 64 kB boundary';
CONST error0c = 'Format not compatible with data transfer rate';
CONST error10 = 'CRC error';
CONST error20 = 'controller/adapter error';
CONST error40 = 'seek error';
CONST error80 = 'No disk in drive';
CONST errorxx = 'Unknown error';

{$ENDIF}

CONST maxform = 20;

TYPE tabletyp = ARRAY[1..25] OF RECORD
                                  t,h,s,f:Byte;
                                END;

  paratyp =  ARRAY[0..10] OF Byte;
  boottyp =  ARRAY[30..511] OF Byte;

  btttyp  =  ARRAY[1..20] OF RECORD
                               head:  Byte;
                               track: Byte;
                             END;
  ftabtyp = ARRAY[1..maxform] OF RECORD
                                   fmt: Word;
                                   trk: Byte;
                                   sec: Byte;
                                   hds: Byte;
                                 END;

  bpbtyp  =  RECORD
               jmp: ARRAY[1..3] OF Byte;                                     {Die ersten drei Bytes fÅr JUMP}
               oem: ARRAY[1..8] OF Char;                                                        {OEM-Eintrag}
               bps: Word;                                                                  {Bytes pro Sektor}
               spc: Byte;                                                              {Sektoren pro Cluster}
               res: Word;                                                                     {BOOT-Sektoren}
               fat: Byte;                                                                  {Anzahl der FAT's}
               rde: Word;                                                          {BasisverzeichniseintrÑge}
               sec: Word;                                                       {Gesamtsektoren der Diskette}
               mds: Byte;                                                                  {Media-Deskriptor}
               spf: Word;                                                                  {Sektoren pro FAT}
               spt: Word;                                                                 {Sektoren pro Spur}
               hds: Word;                                                                            {Seiten}
               shh: Word;                                                               {Versteckte Sektoren}
               boot_code: boottyp;                                                     {Puffer fÅr BOOT-Code}
             END;

VAR regs:     registers;                                                                 {Prozessor-Register}
  track:      Byte;                                                                           {Aktuelle Spur}
  head:       Byte;                                                                          {Aktuelle Seite}
  table:      tabletyp;                                                               {Formatierungs-Tabelle}
  table2:     ARRAY[1..25] OF Byte;                                                      {Interleave-Tabelle}
  x:          Word;                                                                           {Hilfsvariable}
  buffer:     ARRAY[0..18432] OF Byte;                                      {Puffer fÅr eingelesene Sektoren}
  old1E:      Pointer;                                                  {Alter Zeiger auf die Parameterliste}
  new1E:      ^paratyp;                                                 {Neuer Zeiger auf die Parameterliste}
  old13:      Pointer;                                                        {Alter Zeiger auf Interrupt 13}
  old58:      Pointer;                                                   {Alter Zeiger auf Hilfsinterrupt 58}
  bpb:        bpbtyp;                                                   {Boot-Sektor mit BIOS-Parameterblock}
  chx:        Char;                                                                           {Hilfsvariable}
  lw:         Byte;                                                                   {AusgewÑhltes Laufwerk}
  hds,sec:    Word;                                                             {Anzahl der Seiten, Sektoren}
  trk:        Word;                                                                       {Anzahl der Spuren}
  hd,lwhd:    Boolean;                                                                   {High-Density Flags}
  lwtrk:      Byte;                                                           {maximale Spuren des Laufwerks}
  lwsec:      Byte;                                                         {maximale Sektoren des Laufwerks}
  para:       ARRAY[1..50] OF String[20];                                   {Parameter von der Kommandozeile}
  rde:        Byte;                                                                {BasisverzeichniseintrÑge}
  spc:        Byte;                                                                    {Sektoren pro Cluster}
  i:          Byte;                                                                          {Hilfsvariablen}
  j,n:        Integer;                                                                        {Hilfsvariable}
  again:      Boolean;                                                   {Flag, ob INT 13 nochmal kommen mu·}
  bttCount:   Word;                                                            {Anzahl der schlechten Spuren}
  btt:        btttyp;                                                         {Tabelle der schlechten Spuren}
  Offset:     Word;                                                                {Relative Position im FAT}
  Mask:       Word;                                                            {Maske fÅr schlechten Cluster}
  bytes:      LongInt;                                                                {Bytes GesamtkapazitÑt}
  bad:        LongInt;                                                         {Bytes in schlechten Sektoren}
  at80:       Boolean;                                                   {TRUE, wenn 80/40 Spur nach AT-BIOS}
  noverify:   Boolean;                                               {TRUE, wenn Verify nicht verlangt wurde}
  DiskId:     Byte;                                               {Disketten-Format-Beschreibung fÅr AT-BIOS}
  il:         Byte;                                                                       {Interleave-Faktor}
  gpl:        Byte;                                                                               {GAP-LÑnge}
  shiftt:     Byte;                                                              {Sektor-Shifting fÅr Spuren}
  shifth:     Byte;                                                               {Sektor-Shifting fÅr Kîpfe}
  ModelByte:  Byte ABSOLUTE $F000:$FFFE;                                                          {XT/AT/386}
  ForceType:  Byte;                                                                      {Gezwungener Diskid}
  ForceMedia: Byte;                                                            {Erzwungener Media-Deskriptor}
  dosdrive:   Byte;                                                            {DOS-Laufwerks-Identifizierer}
  PCount:     Byte;                                                                    {Anzahl der Parameter}
  found:      Boolean;                                                                      {Format gefunden}
  sys:        Boolean;                                                                {System initialisieren}
  lwtab:      ARRAY[0..3] OF Byte ABSOLUTE $40:$90;                                   {Tabelle der Laufwerke}
  dlabel:     String[15];                                                                   {Disketten-Label}
  setlabel:   Boolean;                                                                         {Label setzen}
  batch:      Boolean;                                                                {Ohne Tastatur-Abfrage}
  cfgat80:    Boolean;                                              {TRUE, wenn Laufwerk fÅr AT konfiguriert}
  cfgpc80:    Boolean;                                              {TRUE, wenn Laufwerk fÅr XT konfiguriert}
  noformat:   Boolean;                                     {TRUE, wenn nicht physisch formatiert werden soll}
  cfgdrive:   Byte;                                                          {Laufwerksart aus Konfiguration}
  bios:       Boolean;                                                          {TRUE, wenn nur BIOS-Aufrufe}
  pc80:       Byte;                                                         {Maske, fÅr 80 Spur nach XT-BIOS}
  pc40:       Byte;                                                         {Maske, fÅr 80 Spur nach XT-BIOS}
  v720:       Byte;                                                                 {Media-Typ fÅr 720 kByte}
  v360:       Byte;                                                                 {Media-Typ fÅr 360 kByte}
  v12:        Byte;                                                                 {Media-Typ fÅr 1.2 MByte}
  v144:       Byte;                                                                {Media-Typ fÅr 1.44 MByte}

CONST para17: paratyp =($df,$02,$25,$02,17,$02,$ff,$23,$f6,$0f,$08);
  para18a:    paratyp =($df,$02,$25,$02,18,$02,$ff,$02,$f6,$0f,$08);
  para18:     paratyp =($df,$02,$25,$02,18,$02,$ff,$6c,$f6,$0f,$08);
  para10:     paratyp =($df,$02,$25,$02,10,$02,$ff,$2e,$f6,$0f,$08);                              {GPL 26-36}
  para11:     paratyp =($df,$02,$25,$02,11,$02,$ff,$02,$f6,$0f,$08);
  para15:     paratyp =($df,$02,$25,$02,15,$02,$ff,$54,$f6,$0f,$08);
  para09:     paratyp =($df,$02,$25,$02,09,$02,$ff,$50,$f6,$0f,$08);
  para08:     paratyp =($df,$02,$25,$02,08,$02,$ff,$58,$f6,$0f,$08);
  para20:     paratyp =($df,$02,$25,$02,20,$02,$ff,$2a,$f6,$0f,$08);                              {GPL 17-33}
  para21:     paratyp =($df,$02,$25,$02,21,$02,$ff,$0c,$f6,$0f,$08);
  para22:     paratyp =($df,$02,$25,$02,22,$02,$ff,$01,$f6,$0f,$08);

  ftab:    ftabtyp = ((fmt:160;trk:40;sec:8;hds:1),                                {Requires 180 kByte Drive}
                      (fmt:180;trk:40;sec:9;hds:1),                                {Requires 180 kByte Drive}
                      (fmt:200;trk:40;sec:10;hds:1),                               {Requires 180 kByte Drive}
                      (fmt:205;trk:41;sec:10;hds:1),                               {Requires 180 kByte Drive}
                      (fmt:320;trk:40;sec:8;hds:2),                                {Requires 360 kByte Drive}
                      (fmt:360;trk:40;sec:9;hds:2),                                {Requires 360 kByte Drive}
                      (fmt:400;trk:40;sec:10;hds:2),                               {Requires 360 kByte Drive}
                      (fmt:410;trk:41;sec:10;hds:2),                               {Requires 360 kByte Drive}
                      (fmt:720;trk:80;sec:9;hds:2),                                {Requires 720 kByte Drive}
                      (fmt:800;trk:80;sec:10;hds:2),                               {Requires 720 kByte Drive}
                      (fmt:820;trk:82;sec:10;hds:2),                               {Requires 720 kByte Drive}
                      (fmt:120;trk:80;sec:15;hds:2),                               {Requires 1.2 MByte Drive}
                      (fmt:12;trk:80;sec:15;hds:2),                                {Requires 1.2 MByte Drive}
                      (fmt:144;trk:80;sec:18;hds:2),                               {Requires 1.2 MByte Drive}
                      (fmt:14;trk:80;sec:18;hds:2),                                {Requires 1.2 MByte Drive}
                      (fmt:148;trk:82;sec:18;hds:2),                               {Requires 1.2 MByte Drive}
                      (fmt:16;trk:80;sec:20;hds:2),                                {Requires 1.4 MByte Drive}
                      (fmt:164;trk:82;sec:20;hds:2),                               {Requires 1.4 MByte Drive}
                      (fmt:168;trk:80;sec:21;hds:2),                               {Requires 1.4 MByte Drive}
                      (fmt:172;trk:82;sec:21;hds:2));                              {Requires 1.4 MByte Drive}

  GetPhys: ARRAY[0..14] OF Byte =(

    $1E,                                                                             {  PUSH DS             }
    $B8,$40,$00,                                                                     {  MOV  AX,40H         }
    $8E,$D8,                                                                         {  MOV  DS,AX          }
    $88,$16,$41,$00,                                                                 {  MOV  [41H],DL       }
    $1F,                                                                             {  POP  DS             }
    $B8,$01,$01,                                                                     {  MOV  AX,101H        }
    $CF);                                                                            {  IRET                }

  Help58: ARRAY[0..3] OF Byte =(

    $CD,$25,                                                                         {  INT  25H            }
    $59,                                                                             {  POP  CX             }
    $CF);                                                                            {  IRET                }

  boot: boottyp=(

    {$IFDEF L49}

    $00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$8C,$C8,$3D,$00,$7C,$74,$08,$BE,$82,$02,$E8,$6A,$00,$CD,
    $20,$FA,$33,$C0,$8E,$D0,$BC,$00,$7C,$B8,$B0,$07,$50,$50,$1F,$07,
    $BE,$00,$01,$BF,$00,$03,$B9,$00,$01,$F3,$A5,$B8,$D0,$07,$50,$50,
    $50,$1F,$07,$B8,$78,$01,$50,$CB,$FB,$BE,$C9,$01,$E8,$3A,$00,$B8,
    $01,$02,$B9,$01,$00,$BA,$80,$00,$33,$DB,$53,$07,$BB,$00,$7C,$06,
    $53,$CD,$13,$72,$0A,$26,$81,$3E,$FE,$7D,$55,$AA,$75,$01,$CB,$BE,
    $0F,$02,$E8,$14,$00,$B4,$01,$CD,$16,$74,$06,$32,$E4,$CD,$16,$EB,
    $F4,$32,$E4,$CD,$16,$33,$D2,$CD,$19,$8A,$04,$0A,$C0,$75,$01,$C3,
    $56,$B4,$0E,$CD,$10,$5E,$46,$EB,$F0,$46,$44,$46,$4F,$52,$4D,$41,
    $54,$20,$56,$65,$72,$73,$69,$6F,$6E,$20,$31,$2E,$36,$0A,$0D,$4B,
    $65,$69,$6E,$65,$20,$53,$79,$73,$74,$65,$6D,$64,$69,$73,$6B,$65,
    $74,$74,$65,$2E,$20,$53,$74,$61,$72,$74,$65,$6E,$20,$76,$6F,$6E,
    $20,$46,$65,$73,$74,$70,$6C,$61,$74,$74,$65,$2E,$0A,$0D,$00,$4B,
    $61,$6E,$6E,$20,$6E,$69,$63,$68,$74,$20,$76,$6F,$6E,$20,$64,$65,
    $72,$20,$46,$65,$73,$74,$70,$6C,$61,$74,$74,$65,$20,$73,$74,$61,
    $72,$74,$65,$6E,$2E,$0A,$0D,$53,$79,$73,$74,$65,$6D,$2D,$44,$69,
    $73,$6B,$65,$74,$74,$65,$20,$69,$6E,$20,$4C,$61,$75,$66,$77,$65,
    $72,$6B,$20,$41,$3A,$20,$65,$69,$6E,$6C,$65,$67,$65,$6E,$0A,$0D,
    $41,$6E,$73,$63,$68,$6C,$69,$65,$E1,$65,$6E,$64,$20,$65,$69,$6E,
    $65,$20,$54,$61,$73,$74,$65,$20,$64,$72,$81,$63,$6B,$65,$6E,$0A,
    $0D,$00,$44,$69,$65,$73,$65,$20,$44,$61,$74,$65,$69,$20,$6E,$69,
    $63,$68,$74,$20,$61,$75,$73,$66,$81,$68,$72,$65,$6E,$2E,$07,$0A,
    $0D,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$55,$AA);

  {$ENDIF}
    {$IFDEF L1}

$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$8C,$C8,$3D,$00,$7C,$74,$08,$BE,$4A,$02,$E8,$6A,$00,$CD,
$20,$FA,$33,$C0,$8E,$D0,$BC,$00,$7C,$B8,$B0,$07,$50,$50,$1F,$07,
$BE,$00,$01,$BF,$00,$03,$B9,$00,$01,$F3,$A5,$B8,$D0,$07,$50,$50,
$50,$1F,$07,$B8,$78,$01,$50,$CB,$FB,$BE,$C9,$01,$E8,$3A,$00,$B8,
$01,$02,$B9,$01,$00,$BA,$80,$00,$33,$DB,$53,$07,$BB,$00,$7C,$06,
$53,$CD,$13,$72,$0A,$26,$81,$3E,$FE,$7D,$55,$AA,$75,$01,$CB,$BE,
$07,$02,$E8,$14,$00,$B4,$01,$CD,$16,$74,$06,$32,$E4,$CD,$16,$EB,
$F4,$32,$E4,$CD,$16,$33,$D2,$CD,$19,$8A,$04,$0A,$C0,$75,$01,$C3,
$56,$B4,$0E,$CD,$10,$5E,$46,$EB,$F0,$46,$44,$46,$4F,$52,$4D,$41,
$54,$20,$56,$65,$72,$73,$69,$6F,$6E,$20,$31,$2E,$36,$0A,$0D,$4E,
$6F,$20,$53,$79,$73,$74,$65,$6D,$64,$69,$73,$6B,$2E,$20,$42,$6F,
$6F,$74,$69,$6E,$67,$20,$66,$72,$6F,$6D,$20,$68,$61,$72,$64,$64,
$69,$73,$6B,$2E,$0A,$0D,$00,$43,$61,$6E,$6E,$6F,$74,$20,$6C,$6F,
$61,$64,$20,$66,$72,$6F,$6D,$20,$68,$61,$72,$64,$64,$69,$73,$6B,
$2E,$0A,$0D,$49,$6E,$73,$65,$72,$74,$20,$53,$79,$73,$74,$65,$6D,
$64,$69,$73,$6B,$20,$61,$6E,$64,$20,$70,$72,$65,$73,$73,$20,$61,
$6E,$79,$20,$6B,$65,$79,$2E,$0A,$0D,$00,$44,$6F,$20,$6E,$6F,$74,
$20,$65,$78,$65,$63,$75,$74,$65,$20,$74,$68,$69,$73,$20,$43,$4F,
$4D,$2D,$46,$69,$6C,$65,$2E,$0A,$0D,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,
$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$55,$AA);


  {$ENDIF}

    FUNCTION ReadKey:Char;
      VAR r:registers;
    BEGIN
      GiveUpIdle;
      WITH r DO BEGIN
        ah:=7;
        intr($21,r);
        IF al IN [3,27] THEN BEGIN
          WriteLn;
          Halt(4);
        END;
        ReadKey:=Chr(al);
      END;
    END;

    PROCEDURE ConfigError;
    BEGIN
      WriteLn;
      WriteLn(text76);
      Halt(16);
    END;

    PROCEDURE GetValue(x,y:String;VAR Value:Byte);
      VAR i,k: Byte;
      j:   Integer;
    BEGIN
      y:=' '+y+'=';
      i:=pos(y,x);
      IF i<>0 THEN BEGIN
        i:=i+Length(y);
        WHILE x[i]=' ' DO Inc(i);
        IF i>Length(x) THEN ConfigError;
        k:=i;
        WHILE x[k]<>' ' DO Inc(k);
        IF x[i]<>'$' THEN BEGIN
          Val(Copy(x,i,k-i),Value,j);
          IF j<>0 THEN ConfigError;
        END ELSE BEGIN
          Value:=dezh(Copy(x,i+1,k-i-1));
          IF BaseError<>0 THEN ConfigError;
        END;
      END;
    END;

    PROCEDURE CfgRead;
      VAR f: Text;
      x: String;
      i: Byte;
    BEGIN
      cfgat80:=False;
      cfgpc80:=False;
      cfgdrive:=255;
      bios:=False;
      pc80:=0;
      pc40:=0;
      v720:=0;
      v360:=0;
      v12:=0;
      v144:=0;
      x:=FSearch('FDFORMAT.CFG',GetEnv('PATH'));
      IF x<>'' THEN BEGIN
        Assign(f,x);
        {$I-} Reset(f); {$I+}
        IF IoResult=0 THEN BEGIN
          WHILE NOT eof(f) DO BEGIN
            ReadLn(f,x);
            x:=x+' ';
            FOR i:=1 TO Length(x) DO x[i]:=Upcase(x[i]);
            IF Copy(x,1,2)=para[1] THEN BEGIN
              IF pos(' BIOS ',x)<>0 THEN bios:=True;
              IF pos(' AT ',x)<>0 THEN cfgat80:=True;
              GetValue(x,'F',cfgdrive);
              IF NOT(cfgdrive IN [0,1,2,7,255]) THEN ConfigError;
              IF pos(' XT ',x)<>0 THEN cfgpc80:=True;
              GetValue(x,'40',pc40);
              GetValue(x,'80',pc80);
              GetValue(x,'360',v360);
              GetValue(x,'720',v720);
              GetValue(x,'1.2',v12);
              GetValue(x,'1.44',v144);
              GetValue(x,'X',shifth);
              GetValue(x,'Y',shiftt);
            END;
            IF cfgat80 AND cfgpc80 THEN ConfigError;
          END;
          {$I-} Close(f); {$I+}
        END ELSE BEGIN
          WriteLn;
          WriteLn(text77);
          Halt(8);
        END;
      END;
    END;

    PROCEDURE int13error;
    BEGIN
      WriteLn;
      CASE regs.ah OF
        $01: Write(error01);
        $02: Write(error02);
        $03: Write(error03);
        $04: Write(error04);
        $06: Write(error06);
        $08: Write(error08);
        $09: Write(error09);
        $0c: Write(error0c);
        $10: Write(error10);
        $20: Write(error20);
        $40: Write(error40);
        $80: Write(error80);
        ELSE Write(errorxx);
      END;
      WriteLn('.');
    END;

    PROCEDURE int13;
      VAR axs: Word;
      chx: Char;
      er:  Boolean;
    BEGIN
      again:=False;
      WITH regs DO BEGIN
        axs:=ax;
        REPEAT
          GiveUpCPU;
          ax:=axs;
          IF ah IN [2,3,5] THEN SetIntVec($1E,new1E);
          IF trk>43 THEN dl:=dl OR pc80 ELSE dl:=dl OR pc40;
          IF NOT(bios) THEN lwtab[dl]:=DiskId;
          intr($13,regs);
          SetIntVec($1E,old1E);
          GiveUpCPU;
          er:=ah>1;
        UNTIL ah<>6;
        IF er THEN BEGIN
          WriteLn;
          WriteLn(text01,regs.ah,'  ',text14,dh,text15,ch,text78,cl,'-',cl+Lo(axs)-1);
          int13error;
          WriteLn(text02);
          REPEAT
            chx:=Upcase(ReadKey);
            CASE chx OF
              'A': Halt(4);
              'I': er:=False;
              t3 : BEGIN er:=False; again:=True; END;
            END;
          UNTIL chx IN ['A','I',t3];
        END;
        ax:=axs;
      END;
    END;

    PROCEDURE parse;
      VAR j:    Byte;
      argstr: String[80];
    BEGIN
      argstr:='';
      FOR j:=1 TO 50 DO para[j]:='';
      FOR j:=1 TO ParamCount DO argstr:=argstr+' '+ParamStr(j);
      FOR j:=1 TO Length(argstr) DO argstr[j]:=Upcase(argstr[j]);
      PCount:=0;
      FOR j:=1 TO Length(argstr) DO BEGIN
        CASE argstr[j] OF
          ' ','-','/': Inc(PCount);
          ELSE         IF (NOT(argstr[j] IN [':','.'])) OR (PCount=1) THEN
              para[PCount]:=para[PCount]+argstr[j];
        END;
      END;
    END;


    PROCEDURE GetPhysical(VAR lw:Byte);
    BEGIN
      WITH regs DO BEGIN
        GetIntVec($58,old58);
        GetIntVec($13,old13);
        SetIntVec($58,@Help58);
        SetIntVec($13,@GetPhys);
        al:=lw; cx:=1; dx:=0;
        ds:=Seg(buffer); bx:=Ofs(buffer);
        intr($58,regs);
        SetIntVec($58,old58);
        SetIntVec($13,old13);
        lw:=Mem[$40:$41];
      END;
    END;

    PROCEDURE DriveTyp(VAR lw:Byte;VAR hd:Boolean;VAR trk,sec:Byte);
    BEGIN
      WITH regs DO BEGIN
        ax:=$4409; bx:=lw+1;
        intr($21,regs);
        IF (FCarry AND Flags) <> 0 THEN BEGIN
          WriteLn(text04);
          trk:=0;
          Exit;
        END;
        IF (dx AND $9200)<>0 THEN BEGIN
          WriteLn(text05);
          trk:=0;
          Exit;
        END;
        ax:=$440f; bx:=lw+1;
        intr($21,regs);
        IF (FCarry AND Flags)<>0 THEN BEGIN
          WriteLn(text04);
          trk:=0;
          Exit;
        END;
        ax:=$440d; cx:=$860; bl:=lw+1;
        bh:=0; dx:=Ofs(buffer); ds:=Seg(buffer);
        buffer[0]:=0;
        intr($21,regs);
        dosdrive:=buffer[1];
        IF cfgdrive<>255 THEN
          dosdrive:=cfgdrive;
        CASE dosdrive OF
          0: BEGIN trk:=39; sec:= 9; hd:=False; END;
          1: BEGIN trk:=79; sec:=15; hd:=True ; END;
          2: BEGIN trk:=79; sec:= 9; hd:=False; END;
          7: BEGIN trk:=79; sec:=18; hd:=True ; END;
          ELSE
            BEGIN
              WriteLn(text06);
              trk:=0;
              Exit;
            END
        END;
        GetPhysical(lw);
        lw:=lw AND $9f;
        IF NOT(lw IN [0..3]) THEN BEGIN
          WriteLn(text07);
          trk:=0;
          Exit;
        END;
        IF cfgat80 THEN
          at80:=cfgat80
        ELSE
          at80:=(ModelByte=$f8) OR (ModelByte=$fc);
      END;
    END;

    PROCEDURE ATSetDrive(lw:Byte; trk,sec,Disk2,Disk,SetUp:Byte);
    BEGIN
      WITH regs DO BEGIN
        dh:=lw; ah:=$18; ch:=trk; cl:=sec;
        intr($13,regs);
        IF ah>1 THEN BEGIN
          ah:=$17; al:=SetUp; dl:=lw;
          intr($13,regs);
        END;
        IF ForceType<>0 THEN BEGIN
          lwtab[lw]:=ForceType;
          bios:=False;
        END ELSE IF Disk2<>0 THEN BEGIN
          bios:=False;
          lwtab[lw]:=Disk2;
        END ELSE
          IF NOT(bios) THEN lwtab[lw]:=Disk;
        DiskId:=lwtab[lw];
      END;
    END;

    PROCEDURE SectorAbsolute(sector:Word;VAR hds,trk,sec:Byte);
      VAR h:Word;
    BEGIN
      sec:=(sector MOD bpb.spt)+1;
      h:=sector DIV bpb.spt;
      trk:=h DIV bpb.hds;
      hds:=h MOD bpb.hds;
    END;

    FUNCTION SectorLogical(hds,trk,sec:Byte):Word;
    BEGIN
      SectorLogical:=trk*bpb.hds*bpb.spt+hds*bpb.spt+sec-1;
    END;

    FUNCTION Cluster(sector: Word):Word;
    BEGIN
      Cluster:=((sector-(bpb.rde SHR 4)
                 -(bpb.spf SHL 1)-1)
                DIV Word(bpb.spc))+2;
    END;

    PROCEDURE ClusterOffset(Cluster:Word; VAR Offset,Mask:Word);
    BEGIN
      Offset:=Cluster*3 SHR 1;
      IF Cluster AND 1 = 0 THEN
        Mask:=$ff7
      ELSE
        Mask:=$ff70;
    END;

    PROCEDURE GetOldParms;
    BEGIN
      WITH regs DO BEGIN
        ax:=$201;
        dx:=lw;
        cx:=1;
        es:=Seg(bpb);
        bx:=Ofs(bpb);
        REPEAT int13 UNTIL NOT again;
        IF ((FCarry AND Flags) = 0) AND (bpb.hds<>0) AND (bpb.spt<>0)
        AND (bpb.sec MOD (bpb.hds*bpb.spt)=0) THEN BEGIN
          sec:=bpb.spt;
          hds:=bpb.hds;
          trk:=bpb.sec DIV bpb.hds DIV bpb.spt;
        END ELSE BEGIN
          WriteLn(text79);
          Halt(4);
        END;
      END;
    END;

    PROCEDURE format;
      VAR i:Byte;
    BEGIN
      IF rde AND 15 <> 0 THEN Inc(rde,16);
      rde:=rde SHR 4;
      IF (spc=2) AND (rde AND 1 = 0) THEN Inc(rde);
      bpb.rde:=rde SHL 4;
      CASE sec OF
        0..8:   new1E:=@para08;
        9:      new1E:=@para09;
        10:     new1E:=@para10;
        11:     new1E:=@para11;
        12..15: new1E:=@para15;
        17:     new1E:=@para17;
        18:     IF lwsec>17 THEN
                  new1E:=@para18
                ELSE
                  new1E:=@para18a;
        19..20: new1E:=@para20;
        21:     new1E:=@para21;
        22..255:new1E:=@para22;
      END;
      IF gpl<>0 THEN
        new1E^[7]:=gpl
      ELSE
        gpl:=new1E^[7];
      WriteLn;
      Write(text08,Chr(lw+$41),': ');
      IF hd THEN WriteLn('High-Density') ELSE WriteLn('Double-Density');
      WriteLn(hds,text09,trk,text10,sec,text11,'Interleave: ',il,text68,gpl);
      WriteLn(bpb.rde,text12,spc,text13,shiftt,':',shifth);
      bttCount:=0;
      WITH regs DO BEGIN
        FOR i:=1 TO 25 DO BEGIN
          table[i].f:=2;
          table2[i]:=0;
        END;
        i:=1;
        n:=1;
        REPEAT
          REPEAT
            WHILE table2[n]<>0 DO Inc(n);
            IF n>sec THEN n:=1;
          UNTIL table2[n]=0;
          table2[n]:=i;
          n:=n+il;
          Inc(i);
        UNTIL i>sec;
        ax:=0;
        bx:=0;
        dl:=lw;
        IF at80 THEN BEGIN
          IF (trk>43) AND (sec>11) AND (sec<16) THEN ATSetDrive(lw,79,lwsec,v12,$14,5);
          IF (trk>43) AND (sec>11) AND (sec>15) THEN ATSetDrive(lw,79,lwsec,v144,$14,5);
          IF (dosdrive IN [0,1]) AND (trk>43) AND (sec<12) THEN ATSetDrive(lw,79,9,v720,$53,4);
          IF (dosdrive IN [2,7]) AND (trk>43) AND (sec<12) THEN ATSetDrive(lw,79,9,v720,$97,4);
          IF (trk<44) AND (sec>11) THEN ATSetDrive(lw,39,lwsec,0,$34,3);
          IF (dosdrive IN [2,7]) AND (trk<44) AND (sec<12) THEN ATSetDrive(lw,39,9,v360,$B7,2);
          IF (dosdrive IN [0,1]) AND (trk<44) AND (sec<12) THEN ATSetDrive(lw,39,9,v360,$73,2);
        END;
        IF at80 AND NOT(bios) THEN BEGIN
          Write(text67);
          CASE (DiskId AND $C0) OF
            $00: Write('500');
            $40: Write('300');
            $80: Write('250');
            $C0: Write('???');
          END;
          Write(' kBaud, Double-Stepping: ');
          IF (DiskId AND 32)=0 THEN
            Write(text73,', ')
          ELSE
            Write(text72,', ');
        END;
        bpb.jmp[1]:=$EB;
        bpb.jmp[2]:=$40;
        bpb.jmp[3]:=144;
        bpb.spt:=sec;
        bpb.hds:=hds;
        bpb.shh:=0;
        bpb.bps:=512;
        bpb.spc:=spc;
        bpb.res:=1;
        bpb.fat:=2;
        bpb.sec:=sec*bpb.hds*trk;
        bpb.boot_code:=boot;
        IF ForceMedia=0 THEN BEGIN
          CASE bpb.spc OF
            1:   IF (trk>44) AND (bpb.spt IN [12..17]) THEN
                   bpb.mds:=$f9
                 ELSE
                   bpb.mds:=$f0;
            2:   IF trk IN [1..43] THEN bpb.mds:=$fd ELSE bpb.mds:=$f9;
            ELSE bpb.mds:=$f8;
          END;
        END
        ELSE bpb.mds:=ForceMedia;
        bpb.spf:=Trunc(bpb.sec*1.5/512/bpb.spc)+1;
        WHILE Trunc((1.5*(((bpb.sec-bpb.res-(bpb.rde DIV 16)
                            -bpb.fat*(bpb.spf-1)) DIV bpb.spc)+2)-1)/bpb.bps)+1<bpb.spf DO
          Dec(bpb.spf);
        WriteLn('Media-Byte: ',hexf(bpb.mds,2));
        WriteLn;
        dl:=lw;
        ax:=0;
        REPEAT int13 UNTIL NOT again;
        n:=0;
        FillChar(buffer,SizeOf(buffer),#0);
        FOR track:=trk-1 DOWNTO 0 DO BEGIN
          IF track<>trk-1 THEN n:=n+shiftt;
          FOR head:=hds-1 DOWNTO 0 DO BEGIN
            IF head<>hds-1 THEN n:=n+shifth;
            n:=n MOD sec;
            FOR i:=1 TO sec DO
              table[i].s:=table2[(i+n-1) MOD sec+1];
            Write(text14,head,text15,track);
            x:=SectorLogical(head,track,1);
            x:=Cluster(x);
            FOR i:=1 TO sec DO BEGIN
              table[i].t:=track;
              table[i].h:=head;
            END;
            REPEAT
              IF noformat THEN BEGIN
                again:=False;
                Write('  ');
              END ELSE BEGIN
                ah:=5;
                al:=sec;
                dl:=lw;
                dh:=head;
                ch:=track;
                cl:=1;
                es:=Seg(table);
                bx:=Ofs(table);
                Write('  F   '#8#8#8);
                Mem[$40:$41]:=0;
                int13;
              END;
              Write(#8,'V                         ');Write(#13);
              IF NOT(again OR noverify) OR (track<3) THEN BEGIN
                ah:=3;
                al:=sec;
                dl:=lw;
                dh:=head;
                ch:=track;
                cl:=1;
                es:=Seg(buffer);
                bx:=Ofs(buffer);
                int13;
              END;
            UNTIL NOT again;
            IF (FCarry AND Flags) <> 0 THEN BEGIN
              IF (x<2) OR (x>10000) THEN BEGIN
                WriteLn(text17);
                Halt(2);
              END;
              Inc(bttCount);
              IF bttCount>20 THEN BEGIN
                WriteLn(text18,20*sec,text19);
                Halt(2);
              END;
              btt[bttCount].track:=track;
              btt[bttCount].head:=head;
              WriteLn(text14,head,text15,track,text20,#10#13);
            END;
          END;
        END;
      END;
    END;

    PROCEDURE WriteBootSect;
    BEGIN
      WITH regs DO BEGIN
        WriteLn; bpb.oem:='CH-FOR16'; WriteLn;
        WriteLn(text21,bpb.oem); WriteLn(text22,bpb.sec);
        WriteLn(text23,bpb.spt); WriteLn(text24,bpb.hds);
        WriteLn(text25,bpb.bps); WriteLn(text26,bpb.shh);
        WriteLn(text27,bpb.res); WriteLn(text28,bpb.fat);
        WriteLn(text29,bpb.spf); WriteLn(text30,Cluster(bpb.sec)-2);
        dh:=0; dl:=lw; ch:=0; cl:=1;
        al:=1; ah:=3; es:=Seg(bpb);
        bx:=Ofs(bpb);
        REPEAT int13 UNTIL NOT again;
        FillChar(buffer[3],18430,#0);
        buffer[0]:=bpb.mds;
        buffer[1]:=$ff;
        buffer[2]:=$ff;
        bad:=0;
        FOR i:=1 TO bttCount DO
          FOR j:=1 TO sec DO BEGIN
            x:=SectorLogical(btt[i].head,btt[i].track,j);
            x:=Cluster(x);
            ClusterOffset(x,Offset,Mask);
            IF buffer[Offset] AND Lo(Mask)=0 THEN Inc(bad,bpb.spc*512);
            buffer[Offset]:=buffer[Offset] OR Lo(Mask);
            buffer[Offset+1]:=buffer[Offset+1] OR Hi(Mask);
          END;
        es:=Seg(buffer);
        bx:=Ofs(buffer);
        Inc(cl);
        al:=bpb.spf;
        REPEAT int13 UNTIL NOT again;
        SectorAbsolute(bpb.spf+1,dh,ch,cl);
        ah:=3;
        dl:=lw;
        IF bpb.spf+cl>sec+1 THEN al:=sec-cl+1;
        REPEAT int13 UNTIL NOT again;
        IF bpb.spf+cl>sec+1 THEN BEGIN
          bx:=bx+al*512;
          al:=bpb.spf-al;
          Inc(dh);
          cl:=1;
          REPEAT int13 UNTIL NOT again;
        END;
        bytes:=LongInt(Cluster(bpb.sec)-2)*512*LongInt(bpb.spc);
        WriteLn;
        WriteLn(bytes:9,text31);
        IF bad<>0 THEN WriteLn(bad:9,text32);
        WriteLn(bytes-bad:9,text33);
        WriteLn;
        IF NOT(bios) THEN lwtab[lw]:=0;
        dl:=lw;
        ah:=0;
        REPEAT int13 UNTIL NOT again;
        ax:=$440f; bx:=lw+1;
        intr($21,regs);
      END;
    END;

    PROCEDURE WriteSys;
      VAR comspec: String[40];
    BEGIN
      comspec:=GetEnv('COMSPEC');
      exec(comspec,'/C SYS '+Chr(lw+$41)+':');
      exec(comspec,'/C COPY '+comspec+' '+Chr(lw+$41)+':\ >NUL');
    END;

    PROCEDURE WriteLabel(x:String);
      VAR i: Byte;
    BEGIN
      WITH regs DO BEGIN
        IF x='' THEN BEGIN
          REPEAT
            Write(text74);
            ReadLn(x);
          UNTIL Length(x)<12;
        END;
        IF x<>'' THEN BEGIN
          IF Length(x)>8 THEN Insert('.',x,9);
          x:=Chr(lw+$41)+':\'+x;
          x[Length(x)+1]:=#0;
          cx:=8;
          ds:=Seg(x);
          dx:=Ofs(x)+1;
          ah:=$3c;
          msdos(regs);
          IF (FCarry AND Flags) <> 0 THEN BEGIN
            WriteLn(text75);
            Exit;
          END;
          bx:=ax;
          ah:=$3e;
          msdos(regs);
          IF (FCarry AND Flags) <> 0 THEN BEGIN
            WriteLn(text75);
            Halt(32);
          END;
        END;
      END;
    END;

    PROCEDURE DrivePrt;
    BEGIN
      WriteLn;
      IF lwtrk=0 THEN BEGIN
        WriteLn(text34);
        Exit;
      END;
      Write(text35,Chr(lw+$41));
      IF lwhd THEN
        Write(': High-Density, ')
      ELSE
        Write(': Double-Density, ');
      WriteLn(lwtrk+1,text10,lwsec,text11);
      Write(text36);
      IF pc80=$20 THEN WriteLn(text37);
      IF pc80=$40 THEN WriteLn(text38);
      IF at80 THEN WriteLn(text39);
      IF NOT(at80) AND (pc80=0) THEN WriteLn(text40);
      WriteLn;
    END;

    PROCEDURE SyntaxError;
    BEGIN
      WriteLn; WriteLn(text41); WriteLn;
      WriteLn(text42); WriteLn(text43); WriteLn;
      WriteLn(text44); WriteLn; WriteLn(text45);
      WriteLn(text46); WriteLn(text47); WriteLn(text48);
      WriteLn(text49); WriteLn(text50); WriteLn(text51);
      WriteLn(text52); WriteLn(text53);
      WriteLn(text69); WriteLn(text70); WriteLn;
      WriteLn(text71);
      Halt(1);
    END;

    PROCEDURE CheckDos;
      VAR Version: Word;
    BEGIN
      Version:=Swap(DosVersion);
      IF Version<$314 THEN BEGIN
        WriteLn(text54);
        Halt(128);
      END;
    END;

    BEGIN
      WriteLn;
      WriteLn(text55);
      WriteLn(text56);
      CheckDos;
      GetIntVec($1E,old1E);
      new1E:=old1E;
      parse;
      noverify:=False;
      noformat:=False;
      IF (Length(para[1])<>2) OR (para[1,2]<>':') THEN SyntaxError;
      lw:=Ord(Upcase(para[1,1]))-$41;
      shiftt:=0;
      shifth:=0;
      CfgRead;
      DriveTyp(lw,lwhd,lwtrk,lwsec);
      DrivePrt;
      IF (lwtrk=0) AND (para[1]<>'') THEN Halt(1);
      rde:=0;
      il:=0;
      spc:=0;
      gpl:=0;
      setlabel:=False;
      sys:=False;
      ForceType:=0;
      ForceMedia:=0;
      batch:=False;
      trk:=lwtrk+1;
      sec:=lwsec;
      hds:=2;
      FOR i:=2 TO PCount DO
        IF para[i]<>'' THEN BEGIN
          chx:=para[i,1];
          IF Upcase(chx)='V' THEN BEGIN
            dlabel:='';
            setlabel:=True;
            dlabel:=Copy(para[i],2,11);
          END ELSE
            IF Length(para[i])=1 THEN BEGIN
              CASE Upcase(chx) OF
                'P': BEGIN END;
                'R': noverify:=True;
                'A': noformat:=True;
                'O': BEGIN
                       trk:=80;
                       sec:=9;
                       rde:=144;
                     END;
                '4': BEGIN
                       trk:=40;
                       sec:=9;
                     END;
                '1': BEGIN
                       hds:=1;
                     END;
                '8': BEGIN
                       sec:=8;
                     END;
                'S': BEGIN
                       sys:=True;
                     END;
                'K': BEGIN
                       batch:=True;
                     END;
                ELSE SyntaxError;
              END;
            END ELSE BEGIN
              IF para[i,2]='$' THEN BEGIN
                n:=dezh(Copy(para[i],3,255));
                j:=BaseError
              END ELSE
                Val(Copy(para[i],2,255),n,j);
              IF j<>0 THEN SyntaxError;
              CASE Upcase(para[i,1]) OF
                'T':trk:=n;
                'H':hds:=n;
                'N':sec:=n;
                'S':sec:=n;
                'M':ForceMedia:=n;
                'D':rde:=n;
                'C':spc:=n;
                'I':il:=n;
                'G':gpl:=n;
                'X':shifth:=n;
                'Y':shiftt:=n;
                'B':ForceType:=n;
                'F':BEGIN
                      found:=False;
                      FOR j:=1 TO maxform DO
                        IF NOT(found) AND (n=ftab[j].fmt) THEN BEGIN
                          trk:=ftab[j].trk;
                          sec:=ftab[j].sec;
                          hds:=ftab[j].hds;
                          found:=True;
                        END;
                      IF NOT(found) THEN SyntaxError;
                    END;
                ELSE SyntaxError;
              END;
            END;
        END;
      IF noformat THEN GetOldParms;
      IF sec>11 THEN hd:=True ELSE hd:=False;
      IF rde=0 THEN
        CASE hd OF
          True:  rde:=224;
          False: rde:=112;
        END;
      IF spc=0 THEN
        CASE hd OF
          True:  spc:=1;
          False: spc:=2;
        END;
      IF il=0 THEN
        IF sec-lwsec IN [3..8] THEN il:=2 ELSE il:=1;
      IF NOT(hds IN [1..2]) THEN BEGIN
        WriteLn(text57);
        Halt(1);
      END;
      IF trk<1 THEN BEGIN
        WriteLn(text58);
        Halt(1);
      END;
      IF il>=Pred(sec) THEN BEGIN
        WriteLn(text59,Pred(sec),text60);
        Halt(1);
      END;
      IF NOT(spc IN [1..2]) THEN
        WriteLn(text61);
      IF ShortInt(trk-lwtrk)>4 THEN
        WriteLn(text62);
      IF rde>240 THEN
        WriteLn(text63);
      IF NOT(batch) THEN BEGIN
        WriteLn;
        WriteLn(text64,Chr(lw+$41),text65);
        WriteLn(text66);
        chx:=ReadKey;
      END;
      format;
      WriteBootSect;
      IF sys THEN WriteSys;
      IF setlabel THEN WriteLabel(dlabel);
    END.
