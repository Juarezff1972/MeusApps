'**FileDir**
'Fill an array with the current dir contents:
'  RC = Amount of dir contents returned
'Parameters:
'   PathName$ = ASCiiZ string


DECLARE FUNCTION FileDir% (PathName$, FSeg%, FOff%, FAttr%)

DIM MyArray(75) AS STRING * 12

' get all dirs
CLS
FAttr% = &H10
DFiles$ = "*."
FilTot% = FileDir%(DFiles$ + CHR$(0), VARSEG(MyArray(0)), VARPTR(MyArray(0)), FAttr%)
PRINT "All files in that directory with the name: "; DFiles$
FOR R% = 0 TO FilTot% - 1
  PRINT "[" + MyArray(R%) + "]"
NEXT

FAttr% = 32
DFiles$ = "*.*"
FilTot% = FileDir%(DFiles$ + CHR$(0), VARSEG(MyArray(0)), VARPTR(MyArray(0)), FAttr%)
FOR R% = 0 TO FilTot% - 1
  PRINT "[" + MyArray(R%) + "]"
NEXT
PRINT

