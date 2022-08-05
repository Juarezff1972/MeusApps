Unit AltCrt;
  {By Rick Housh - CIS PIN 72466,212}
  {Alternate to CRT unit}
  {Does all it can without memory mapped screen writing}
  {Uses DOS rather than Video ROM where possible}
(**************************************************************************)
{
 I started this Unit because I couldn't read a single keystroke with
 just the System or the Dos Units.  ReadKey is in the CRT unit.  Many of
 the programs I write do not need the memory mapped screen writes used in
 the Crt unit, but if you use it at all, you get them, and about 2k
 additional size to your .EXE file.  Well, I started with just a simple
 inline function to read a keystroke and return its character.  It's in
 here, called GetKey.

 But, while I was at it, I ended up replacing most of the Crt unit, using
 only DOS (Interrupt 21H) calls and Video ROM BIOS (Interrupt 10H) calls.
 There is no memory mapped video writing, and the write routines have not
 been replaced.  TP version 4's regular Write and WriteLn routines are
 used, and they use DOS, which places a number of limitations on what
 one can do.  However, most of the useful functions are retained.

 There is one major limitation.  The window procedure is not supported,
 and the Unit will not work on anything other than an 80 column screen.
 It aborts with a semi-polite message if asked to do otherwise.

 The following CRT unit functions are supported as follows:
    AssignCrt      :   Not supported
    ClrEol         :   Fully supported
    ClrScr         :   Fully supported
    Delay          :   Not supported
    DelLine        :   Not supported    (Could easily be, but never used it)
    GotoXY         :   Fully supported
    HighVideo      :   Limited support
    InsLine        :   Not Supported    (See DelLine)
    LowVideo       :   Limited support
    NoSound        :   Not supported
    Sound          :   Not supported
    TextBackground :   Limited support
    TextColor      :   Limited support
    TextMode       :   Not supported
    Window         :   Not supported
    KeyPressed     :   Fully supported
    NormVideo      :   Limited support
    ReadKey        :   Fully supported
    WhereX         :   Fully supported
    WhereY         :   Fully supported

 From the above you can see that most of the important CRT Unit functions
 are available.  The video attribute functions which are only partially
 supported are because using the DOS Write and WriteLn functions does
 not make it possible.  However if you use TextBackground and/or
 TextColor (or NormVideo, HighVideo, or LowVideo) just before a ClrXxx
 call, they will work.  Ordinarily, DOS functions cannot do that.  For
 example, consider the following:
   TextBackground(Blue);
   TextColor(Yellow);
   ClrScr;
 This little bit of code will clear the screen and everything will there-
 after be yellow on blue.  ClrEol will do that also, but only on the
 line on which it is used.

 Those miscellaneous functions which are not supported are almost all
 available in Carley Phillip's CRTI unit, available in this DL as
 CRTI.ARC.  Combine some of these and some of those in one unit if you
 need the Sound, NoSound, Delay, etc.

 This unit supplies one unit not available in CRT, the GetKey function.
 Most of the time I just want a character returned.  I am not interested
 in function keys, etc.  GetKey does just that.  It first flushes the
 keyboard, in case you accidentally pressed something, ignores function
 keys, and returns the value of the keypress as a character.  Where the
 variable ch is a character, the appropriate syntax would be:
    ch := GetKey;
 It will then wait for the key.

        This program is dedicated to the public domain.
        No copyright is claimed.
        I would be interested in reports.
                    Rick Housh
                    5811 W. 85th Terr.
                    Overland Park, KS 66207
                    Tel. 913/341-7592
                    Compuserve PIN #72466,212

}




  Interface
  Const
    Black = 0; Blue = 1; Green = 2; Cyan = 3; Red = 4; Magenta = 5;
    Brown = 6; LightGray = 7; DarkGray = 8; LightBlue = 9;
    LightGreen = 10; LightCyan = 11; LightRed = 12; LightMagenta = 13;
    Yellow = 14; White = 15; Blink = 128;

    BW40 = 0; BW80 = 1; Mono = 7; CO40 = 1; CO80 = 3; Font8x8 = 256;
    C40 = CO40; C80 = CO80;

  Var
    CheckBreak, CheckEOF : Boolean;
    TextAttr             : Byte;
    LastMode             : Word;
    Function GetKey      : Char;
    Function ReadKey     : Char;
    Function WhereX      : Byte;
    Function WhereY      : Byte;
    Function Keypressed  : Boolean;
    Procedure NormVideo;
    Procedure LowVideo;
    Procedure HighVideo;
    Procedure ClrEol;
    Procedure ClrScr;
    Procedure GotoXY(XPos, YPos : Byte);
    Procedure TextBackGround(Back : Byte);
    Procedure TextColor(Fore : Byte);


  Implementation

  {There are two ClrEol procedures here, the first of which is
   commented out.  The second, currently operative, is a little
   slower, but can reset the video attribute on the line it
   clears.  The first is faster, but incapable of doing that.
   Use whichever you like, but remember to comment out the unused
   one.}

(*
  Procedure ClrEol;  {Just write spaces to EOL and put cursor back}
  Begin
  Inline(
   $B4/$03/               {MOV AH,$03    ;Set for read curs. pos.}
   $30/$FF/               {XOR BH,BH     ;Set page zero}
   $CD/$10/               {INT $10       ;Call ROM BIOS}
   $52/                   {PUSH DX       ;save cursor pos}
   $31/$C9/               {XOR CX,CX     ;Zero CX}
   $B1/$4F/               {MOV CL,$79    ;put column 79 in CL}
   $28/$D1/               {SUB CL,DL     ;subtract current curs. column}
                          {              ;CX now has count of}
                          {              ;columns to EOL}
   $80/$F9/$00/           {CMP CL,$0     ;Is the cursor at EOL?}
   $75/$03/               {JNZ  AGAIN    ;If not clreol}
   $5A/                   {POP  DX       ;else get DX}
   $EB/$0F/               {JMP short GETOUT ;and exit}
                          {AGAIN:}
   $B4/$02/               {MOV AH,$02    ;Set for write a char}
   $B2/$20/               {MOV DL,$20    ;make it a space}
   $CD/$21/               {INT $21       ;and write it}
   $E2/$F8/               {LOOP AGAIN    ;until at EOL}
   $5A/                   {POP DX        ;get original cursor pos.}
   $30/$FF/               {XOR BH,BH     ;Set at page zero}
   $B4/$02/               {MOV AH,$02    ;Set for position curs.}
   $CD/$10);              {INT $10       ;Call ROM BIOS}
                          {GETOUT:       ;Finished}
   end; {Inline procedure ClrEol}
*)

  Procedure ClrEol;  { Replacement for CRT.ClrEol }
   Var ATR : Byte;
   Begin
     ATR := TextAttr;
      Inline(
      $B4/$03/               {MOV   AH,$03      ;Set up for curs. pos. report}
      $31/$DB/               {XOR   BX,BX       ;Page to zero}
      $CD/$10/               {INT   $10         ;Call Video ROM}
      $31/$C9/               {XOR   CX,CX       ;CX to 0}
      $52/                   {PUSH  DX          ;Y in DH, X in CL, Savem}
                             {AGAIN:            ;Label for loop}
      $B4/$09/               {MOV   AH,$09      ;Set to write char w/attribute}
      $B0/$20/               {MOV   AL,$20      ;Char will be a space}
      $8A/$5E/<ATR/          {MOV   BL,<ATR[BP] ;Attribute is TextAttr}
      $B1/$50/               {MOV   CL,$50      ;80 column line}
      $28/$D1/               {SUB   CL,DL       ;Count is 80 - Curs. Pos.}
      $CD/$10/               {INT   $10         ;Call Video ROM, write 1st space}
      $B4/$02/               {MOV   AH,$02      ;Set cursor position}
      $B7/$00/               {MOV   BH,$00      ;on page zero}
      $80/$C2/$01/           {ADD   DL,$01      ;increment by one}
      $CD/$10/               {INT   $10         ;call Video ROM to mov it}
      $E2/$E8/               {LOOP  AGAIN       ;Do it again, until CX = 0}
      $5A/                   {POP   DX          ;Recover original cursor pos.}
      $31/$DB/               {XOR   BX,BX       ;Set to page zero}
      $CD/$10);              {INT   $10         ;Call Video ROM to put it back}
   end;  {Inline Procedure ClrEol}

Procedure ClrScr;    {Replacement for CRT.ClrScr}
  Var ATR : Byte;
  Begin
    ATR := TextAttr;
    Inline(
      $B8/$00/$07/           {MOV  AX,$0700   ;Ready to clear screen}
      $8A/$7E/<ATR/          {MOV  BH,<ATR[BP] ;Assigned attribute}
      $31/$C9/               {XOR  CX,CX      ;Whole Screen}
      $BA/$4F/$18/           {MOV  DX,$184F   ;clear to bottom right}
      $CD/$10/               {INT  $10        ;Call ROM BIOS}
      $B4/$02/               {MOV  AH,$02     ;Ready to pos. cursor}
      $B7/$00/               {MOV  BH,$00     ;Page zero}
      $31/$D2/               {XOR  DX,DX      ;Cursor top left}
      $CD/$10);              {INT  $10        ;Call ROM BIOS}
  end;

  Function GetKey : char;     { Additional function.  Not in CRT Unit }
    Var CH : char;
    Begin
      Inline(
                     {; Function GetKey : Char}
                     {; Clears the keyboard buffer then waits until}
                     {; a key is struck.  If the key is a special, e.g.}
                     {; function key, goes back and reads the next}
                     {; byte in the keyboard buffer.  Thus does}
                     {; nothing special with function keys.}
      $B4/$0C        {       MOV  AH,$0C      ;Set up to clear buffer}
      /$B0/$08       {       MOV  AL,8        ;then to get a char}
      /$CD/$21       {SPCL:  INT  $21         ;Call DOS}
      /$3C/$00       {       CMP  AL,0        ;If it's a 0 byte}
      /$75/$04       {       JNZ  CHRDY       ;is spec., get second byte}
      /$B4/$08       {       MOV  AH,8        ;else set up for another}
      /$EB/$F6       {       JMP  SHORT SPCL  ;and get it}
      /$88/$46/<CH   {CHRDY: MOV  <CH[BP],AL  ;else put into function return}
       );
      If CheckBreak and (Ch = #3) then
        Begin        {If CheckBreak is true and it's a ^C}
          Inline(    {then execute Ctrl_Brk}
          $CD/$23);
        end;
      GetKey := Ch;
    end; {Inline function GetKey}


  Function ReadKey : char;  { Replacement for CRT.ReadKey }
    Var chrout : char;
    Begin
                         {  ;Just like ReadKey in CRT unit}
      Inline(
      $B4/$07/               {  MOV AH,$07          ;Char input w/o echo}
      $CD/$21/               {  INT $21             ;Call DOS}
      $88/$86/<CHROUT);      {  MOV <chrout[bp],AL  ;Put into variable}
      If CheckBreak and (chrout = #3) then  {If it's a ^C and CheckBreak true}
        Begin                             {then execute Ctrl_Brk}
          Inline(
          $CD/$23);           {     INT $23}
        end;
      ReadKey := chrout;                    {else return character}
    end;

  Function WhereX : Byte;  {Returns x coordinate of cursor}
  Var XVAL : Byte;         { Replacement for CRT.WhereX }
  Begin
    Inline(
      $B4/$03/               { MOV     AH,+$03       ;Set for GetCursPos}
      $B7/$00/               { MOV     BH,+$00       ;page zero}
      $CD/$10/               { INT     $10           ;Call Video ROM}
      $88/$D0/               { MOV     AL,DL         ;Move x coordinate}
      $04/$01/               { ADD     AL,$01        ;increment by one}
      $88/$46/<XVAL);        { MOV     <XVAL[BP],AL  ;to adjust for TP}
      WhereX := XVAL;        {                        And return it}
  end;

  Function WhereY : Byte;  {Returns Y coordinate of cursor}
  Var YVAL : Byte;         { Replacment for CRT.WhereY }
  Begin
    Inline(
      $B4/$03/               {      MOV     AH,+$03  ;Same as WhereX}
      $B7/$00/               {      MOV     BH,+$00  ;except returns DH}
      $CD/$10/               {      INT     $10      ;rather then DL}
      $88/$F0/               {      MOV     AL,DH}
      $04/$01/               {      ADD     AL,$01}
      $88/$86/>YVAL);        {      MOV     [BP+>YVAL],AL}
      WhereY := YVAL;
  end;


  Function KeyPressed : boolean;   { Replacement for CRT.KeyPressed }
                         {  ;Detects whether a key is pressed}
                         {  ;Does nothing with the key}
                         {  ;Returns true if key is pressed}
                         {  ;Otherwise, false}
                         {  ;Key remains in kbd buffer}
    Var IsThere : Byte;
    Begin
      KeyPressed := False;
      Inline(
      $B4/$0B/               {    MOV AH,+$0B         ;Get input status}
      $CD/$21/               {    INT $21             ;Call DOS}
      $88/$86/<ISTHERE);     {    MOV <IsThere[BP],AL ;Move into variable}
      If IsThere = $FF then Keypressed := True;
    end;

  Procedure GoToXY(Xpos, Ypos : Byte);    { Replacement for CRT.GoToXY }
    Begin
      If (Xpos > 80) or (Xpos < 1) then exit;  {If impossible do nothing}
      If (Ypos > 25) or (Ypos < 1) then exit;
      dec(Xpos);                               {Convert to DOS format}
      dec(Ypos);
      Inline(
        $B4/$02/               {MOV  AH,$02        ;Ready to move cursor}
        $B7/$00/               {MOV  BH,$00        ;on page zero}
        $8A/$76/<YPOS/         {MOV  DH,<YPOS[BP]  ;Set Y coordinate}
        $8A/$56/<XPOS/         {MOV  DL,<XPOS[BP]  ;Set X coordinate}
        $CD/$10);              {INT  $10           ;and call ROM BIOS}
    end; {Procedure GoToXY}

   Procedure TextBackGround(Back : Byte);{Replacement for CRT.TextBackground}
     Begin
       If Back > 7 then exit;     { No illegal values allowed }
       TextAttr := (TextAttr AND $8F) + Back * 16;
     end;

   Procedure TextColor(Fore : Byte);
     Begin
       If not ((Fore in [0..15]) or (Fore in [128..143])) then exit;
       TextAttr := (TextAttr AND $70) + Fore;
     end;

   Procedure NormVideo;   { Replacement for CRT.NormVideo }
     Begin
       TextAttr := $07;
     end;

   Procedure LowVideo;    { Replacement for CRT.LowVideo }
     Begin
       TextAttr := TextAttr AND $0F;
     end;

   Procedure HighVideo;   { Replacement for CRT.HighVideo }
     Begin
       TextAttr := TextAttr OR $0F;
     end;

  Function GetCols : Byte;    { Return number of screen columns }
    Var COL : Byte;
    Begin
      Inline(
      $B4/$0F/           {    MOV  AH,$0F        ;Setup to get Video Mode}
      $CD/$10/           {    INT  $10           ;Call Video ROM}
      $88/$A6/>COL);     {    MOV  >COL[BP],AH   ;Interested only in columns}
      GetCols := COL;
    end;  {Procedure GetCols}

   Begin    { Setup }
    If GetCols <> 80 then
      Begin
        WriteLn(#7);
        WriteLn('You must be in an 80 column mode to run this program');
        WriteLn('Aborting...');
        Halt;
      end;
    CheckBreak := True;
    CheckEOF := False;
    TextAttr := 7;
   end.


