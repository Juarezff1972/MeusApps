{
  ***  ***   Thanks to Reinier van Loon
 *   **   *  for this port to Pascal
   **  **    19 September 1998
 **   .  **  [Edited by Forever Young Software]
 *********
}

Const
  Freq = 11000; { max. 29999 }

var
  buffer : array[0..32767] of byte;
  Base : Integer;
  Channel, Pg, Add, Len, ModeReg : Byte; 

function DSPReset : Boolean;
var
  Count, Junk : Integer;
begin
  Port[Base + 6] := 1;
  For Count := 1 To 50 Do
     Junk := Port[Base + 6];
  Port[Base + 6] := 0;
  For Count := 1 To 50 Do
     Junk := Port[Base + 6];
  If ( Port[Base + 14] And $80 = $80 ) And ( Port[Base + 10] = $AA ) Then
     DSPReset := True
  Else
     DSPReset := False;
end;


Procedure Write( aByte : Byte );
Begin
  While Port[Base + 12] And $80 > 0 Do;
  Port[Base + 12] := aByte;
End;


Procedure DMAPlay( aSegment, anOffset : Word ; aLength : Integer );
Var
  Page, Length : Word;
  MemLoc : LongInt;
Begin
  Length := aLength - 1;
  Page := 0;
  MemLoc := aSegment;
  MemLoc := MemLoc * 16;
  MemLoc := MemLoc + anOffset;

  Port[$A]  := $4 + Channel;
  Port[$C]  := $0;
  Port[$B]  := ModeReg;
  Port[Add] := MemLoc And $FF;
  Port[Add] := ( MemLoc And $FFFF ) Div $100;
  If (MemLoc And  65536) > 0 Then Page := Page + 1;
  If (MemLoc And 131072) > 0 Then Page := Page + 2;
  If (MemLoc And 262144) > 0 Then Page := Page + 4;
  If (MemLoc And 524288) > 0 Then Page := Page + 8;
  Port[Pg]  := Page;
  Port[Len] := Length And $FF;
  Port[Len] := ( Length And $FFFF ) Div $100;
  Port[$A]  := Channel;

  Write( $40 );
  Write( 256 - ( 1000000 Div Freq ) );
  Write( $14 );
  Write( Length And $FF );
  Write( ( Length And $FFFF ) Div $100 );
End;

Procedure SpeakerOn;
Begin
  Write( $D1 );
End;

Procedure SpeakerOff;
Begin
  Write( $D1 );
End;

Procedure SetBase( anAddress : Integer );
Begin
  Base := anAddress;
End;

Procedure SetChannel( aChannel : Integer );
Begin
  Channel := aChannel;
  If Channel = 0 Then
  Begin
    Pg      := $87;
    Add     := $00;
    Len     := $01;
    ModeReg := $48;
  End;
  If Channel = 1 Then
  Begin
    Pg      := $83;
    Add     := $02;
    Len     := $03;
    ModeReg := $49;
  End;
  If Channel = 2 Then
  Begin
    Pg      := $81;
    Add     := $04;
    Len     := $05;
    ModeReg := $4A;
  End;
  If Channel = 3 Then
  Begin
    Pg      := $82;
    Add     := $06;
    Len     := $07;
    ModeReg := $4B;
  End;
End;

Procedure SetVolume( aLevel : Word );
Begin
  Port[Base + 4] := $22; { set volume }
  Port[Base + 5] := $DD; { Left = HI Nibble, Right = LO nibble (0FF is max) }
End;

Procedure Test( aFileName : String );
Var
  WavFile   : File;
  WavBuffer : ^Byte;
  Length    : Word;
Begin
  WriteLn;
  WriteLn( 'PlayWav   A utility for Playing WAV files    Version  1.02b' );
  WriteLn( 'Forever Young Software(r)   (C)opyright 1984-1998' );
  WriteLn( 'TurboPascal version by 2SHARE' );
  WriteLn;

  If DSPReset Then
    WriteLn( 'DSP reset sucessfully!' )
  Else
    WriteLn( 'DSP failed to reset, try another port.' );

  SpeakerOn;
  SetVolume( $22 );

  GetMem( WavBuffer, 32767 ); { Make a 32k buffer for file }

  Assign( WavFile, aFileName );
  Reset( WavFile, 1 );
  Seek( WavFile, 44 ); { Skip header on WAV }
  BlockRead( WavFile, WavBuffer^, 32767, Length ); { Get max 32k from file }
  If Length > 32767 Then Length := 32767; { Adjust length if needed to 32k}
  DMAPlay( Seg( WavBuffer^ ), Ofs( WavBuffer^ ), Length );
  Close( WavFile );
  Dispose( WavBuffer );
End;

begin
  SetBase( $220 );
  SetChannel( 1 );
  Test( 'prop.wav' );
  readln;
End.
