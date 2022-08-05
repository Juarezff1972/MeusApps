' NULLMOD.BAS
' Forever Young Software
' (c)opyright 1984-1998
' All Rights Reserved
' Benjamin David Lunt
' blunt@zekes.com
' http://www.zekes.com/~blunt/index.html
'
' This is freeware.  You may do with it what you want,
'  as long as you don't do it for profit without my
'  knowledge.  As always, if this program in anyway
'  crashes your machine, I am not responsible.
'  Have any questions, give me an e-message.
'
' A routine to copy a file over a null modem connection
'  between two serial com ports.
' Make sure that both machines have this program running,
'  while one states sending and the other is receiving.
' To send a file: Enter S and when prompted; enter filename
' To receive a file, just enter R for receive and wait
'  for file to be received.  This routine does the rest.
'
' Make sure that your null modem cable is correctly made.
'  A null modem cable should be constructed like so:
'   - For a 9 pin (F) to 9 pin (F) connection:
'       RXD 2   <---->   TXD 3
'       TXD 3   <---->   RXD 2
'       GND 5   <---->   GND 5
'
'   - For a 9 pin (F) to 25 pin (F) connection:
'       RXD 2   <---->   TXD 2
'       TXD 3   <---->   RXD 3
'       GND 5   <---->   GND 7
'
'   - For a 25 pin (F) to 25 pin (F) connection:
'       TXD 2   <---->   RXD 3
'       RXD 3   <---->   RXD 2
'       GND 7   <---->   GND 7
'

ON ERROR GOTO errorhandler

DIM B AS STRING * 1

CLS
PRINT "Null modem file transfer routine   v1.00"
PRINT "Forever Young Software"
PRINT "Copyright 1998"
PRINT "All rights reserved"
PRINT : PRINT
PRINT "Receive or Send a file (R, S):  "
V$ = UCASE$(INPUT$(1))
IF V$ = "R" THEN
  PRINT "Receiving file:  ";
  File$ = ""
3 OPEN "COM2:1200,N,8,1,BIN" FOR RANDOM AS #2 LEN = 1
    GET #2, , B
    FOR R% = 1 TO ASC(B)
      GET #2, , B
      File$ = File$ + B
    NEXT
    PRINT File$
    OPEN "b" + File$ FOR OUTPUT AS #1
    CLOSE #1
    OPEN "b" + File$ FOR BINARY AS #1 LEN = 1
      GET #2, , B
      FileLen& = ASC(B)
      GET #2, , B
      FileLen& = FileLen& OR (ASC(B) * 256)
      GET #2, , B
      FileLen& = FileLen& OR (ASC(B) * 32768)
      GET #2, , B
      FileLen& = FileLen& OR (ASC(B) * 65536)
      PRINT
      FOR Curbytenum& = 1 TO FileLen&
        LOCATE CSRLIN - 1, 1
        PRINT "Receiving byte "; Curbytenum&; " of "; FileLen&
        GET #2, , B
        PUT #1, , B
      NEXT
    CLOSE #1
  CLOSE #2
  END
ELSEIF V$ = "S" THEN
  LINE INPUT "Please enter filename:  "; File$
1 OPEN File$ FOR BINARY AS #1
  CLOSE #1
2 OPEN "COM2:1200,N,8,1,BIN" FOR RANDOM AS #2 LEN = 1
    PRINT "Getting file info..."
    OPEN File$ FOR BINARY AS #1 LEN = 1
      FileLen& = LOF(1)
      PRINT "  Length of file to send:  "; FileLen&
      PRINT "  Sending File Name..."
      B = CHR$(LEN(File$))
      PUT #2, , B
      FOR R% = 1 TO LEN(File$)
        B = MID$(File$, R%, 1)
        PUT #2, , B
      NEXT
      PRINT "  Sending File Length..."
      TempLen& = FileLen&
      FOR R% = 1 TO 4
        B = CHR$(TempLen& AND &HFF&)
        PUT #2, , B
        TempLen& = TempLen& \ 256
      NEXT
      PRINT "  Sending File..."
      FOR R& = 1 TO FileLen&
        GET #1, , B
        PUT #2, , B
      NEXT
      PRINT "Done sending file."
    CLOSE #1
  CLOSE #2
  END
ELSE PRINT "Wrong response"
END IF
END

errorhandler:
  IF ERL = 1 THEN
    PRINT "File Not Found"
    END
  ELSEIF ERL = 2 OR ERL = 3 THEN
    PRINT "Com port error"
    END
  ELSE
    PRINT "undefined error"
    END
  END IF
