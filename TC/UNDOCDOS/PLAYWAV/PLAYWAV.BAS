DECLARE FUNCTION ResetDSP% ()
DECLARE SUB WriteDSP (byte%)
DECLARE SUB DMAPlay (Segment&, Offset&, Length&)

CONST PgPort% = &H83, AddPort% = &H2, LenPort% = &H3, ModeReg% = &H49
CONST Channel% = 1, BasePort% = &H220, Freq% = 11000

PRINT
PRINT "PlayWav   A utility for Playing WAV files    Version  1.02b"
PRINT "Forever Young Software(r)   (C)opyright 1984-1998"
PRINT

IF ResetDSP% THEN 'resets DSP (returns true if sucessful)
   PRINT "DSP reset sucessfully!"
ELSE
   PRINT "DSP failed to reset, try another port."
END IF

WriteDSP &HD1  'turn the speaker on

OUT BasePort% + 4, &H22    ' set volume
OUT BasePort% + 5, &HDD    ' Left = HI Nibble, Right = LO nibble (0FF is max)

DIM SHARED WavBuffer(1 TO 1) AS STRING * 32767 'Make a 32k buffer for file.

File$ = "prop.wav"
OPEN File$ FOR BINARY AS #1
  GET #1, 44, WavBuffer(1) 'Get 32k from file (skip header on WAV)
  Length& = LOF(1) - 44
  IF Length& > 32767 THEN Length& = 32767 'Adjust length if needed to 32k
  DMAPlay VARSEG(WavBuffer(1)), VARPTR(WavBuffer(1)), Length&
CLOSE 1
END

SUB DMAPlay (Segment&, Offset&, Length&)
  
  Length& = Length& - 1
  Page% = 0
  MemLoc& = Segment& * 16 + Offset&

  OUT &HA, &H4 + Channel%
  OUT &HC, &H0
  OUT &HB, ModeReg%
  OUT AddPort%, MemLoc& AND &HFF
  OUT AddPort%, (MemLoc& AND &HFFFF&) \ &H100
  IF (MemLoc& AND 65536) THEN Page% = Page% + 1
  IF (MemLoc& AND 131072) THEN Page% = Page% + 2
  IF (MemLoc& AND 262144) THEN Page% = Page% + 4
  IF (MemLoc& AND 524288) THEN Page% = Page% + 8
  OUT PgPort%, Page%
  OUT LenPort%, Length& AND &HFF
  OUT LenPort%, (Length& AND &HFFFF&) \ &H100
  OUT &HA, Channel%

  WriteDSP &H40
  WriteDSP (256 - 1000000 \ Freq%)
  WriteDSP &H14
  WriteDSP (Length& AND &HFF)
  WriteDSP ((Length& AND &HFFFF&) \ &H100)
END SUB

FUNCTION ResetDSP%
  ' Resets the DSP
  OUT BasePort% + 6, 1
  FOR Count% = 1 TO 10
     Junk% = INP(BasePort% + 6)
  NEXT
  OUT BasePort% + 6, 0
  FOR Count% = 1 TO 10
     Junk% = INP(BasePort% + 6)
  NEXT
  IF (INP(BasePort% + 14) AND &H80 = &H80) AND (INP(BasePort% + 10) = &HAA) THEN
     ResetDSP% = -1
  ELSE
     ResetDSP% = 0
  END IF
END FUNCTION

SUB WriteDSP (byte%)
  ' Writes a byte to the DSP
  DO
  LOOP WHILE INP(BasePort% + 12) AND &H80
  OUT BasePort% + 12, byte%
END SUB

