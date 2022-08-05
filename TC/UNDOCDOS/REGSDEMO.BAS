' This is a demo to show how to use the registers
'  in Quick Basic 4.5
' This demo shows three different ways to do it.
'  (Well 2 1/2 ways)
' 1.  Use CALL INTERRUPT
'   This method lets you use the following registers:
'   AX, BX, CX, DX, BP, SI, DI, & FLAGS
' 2.  Use CALL INTERRUPTX
'   This method lets you use the following registers:
'   AX, BX, CX, DX, BP, SI, DI, FLAGS, DS, & ES
' 3.  Use CALL ABSOLUTE
'   This method lets you actually put assembled code
'   in DATA lines and then call it.

' Please note:  You MUST load the QB.QLB to use these
'   Functions.  At the command line do a:
'     QB yourprog.bas /l

DEFINT A-Z
'$INCLUDE: 'QB.BI'

DIM InRegs AS RegType, OutRegs AS RegType      ' CALL INTERRUPT
DIM InRegsX AS RegTypeX, OutRegsX AS RegTypeX  ' CALL INTERRUPTX
DIM AsmProg(1 TO 5)                            ' 10 bytes to hold our
																							 '  assembled code
TEXT$ = CHR$(13) + CHR$(10) + "Hello World$"

CLS

InRegs.AX = &H200          ' ah = 02h, al = 00h
InRegs.DX = &H41           ' dh = 00h, dl = 41h (ascii A)
CALL INTERRUPT(&H21, InRegs, OutRegs)

RESTORE AsmBytes                          ' point QB to start at AsmBytes
P = VARPTR(AsmProg(1))                    ' point to our array
DEF SEG = VARSEG(AsmProg(1))              '
	FOR I = 0 TO 8                          ' put 9 bytes in to our array
		 READ J                               '  .
		 POKE (P + I), J                      '  .
	NEXT I                                  '  .
	CALL ABSOLUTE(VARPTR(AsmProg(1)))       ' call it
DEF SEG                                   '

InRegsX.AX = &H900                        ' print "HELLO WORLD"
InRegsX.DX = SADD(TEXT$)                  '
InRegsX.DS = VARSEG(TEXT$)                '
CALL INTERRUPTX(&H21, InRegsX, OutRegsX)  '

END

' Our assembled code.  Use DEBUG or something else.
AsmBytes:
DATA &HB4, &H02        : 'MOV  AH,02h
DATA &HB2, &H42        : 'MOV  DL,42h
DATA &HCD, &H21        : 'INT  21H
DATA &HCA, &H00, &H00  : 'RET  0          ' make sure to far ret

