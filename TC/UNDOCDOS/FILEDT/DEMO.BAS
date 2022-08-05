DECLARE FUNCTION OpenEFile% (File$, Attrb%, Handle%)
DECLARE FUNCTION GetFDtTm% (Handle%, FDate%, FTime%)
DECLARE FUNCTION CloseFile% (Handle%)

CLS
'**OpenEFile**
'Openes an existing file
'RC = 0 = OK
'     2 = File not found
'     3 = Path not found
'     4 = No more handles available
'     5 = Access denied
'   0Ch = Invalid access code
'Returns a handle in Handle% if successful
'Parameters:
'   File$ = ASCiiZ string
'   Attrb% = 0 = read-only
'            1 = write-only
'            2 = either (both)
'   Handle% = Returns a handle in Handle% if successful
File$ = "files.qlb" + CHR$(0)
Attrb% = 0
IF OpenEFile%(File$, Attrb%, Handle%) = 0 THEN
  ThisHandle% = Handle%
ELSE
  PRINT "Error opening file"
  END
END IF

'**GetFDtTm**
'Gets the specified files date and time that is was created or modified
'RC = 0 = OK
'    else = error
'Parameters:
'    Handle% = Handle of open file
'    FDate% = Date Var
'       Date Var = ((YEAR - 1980) * 512) + (MONTH * 32) + DAY
'    FTime% = Time Var
'       Time Var = (HOUR * 2048) + (MINUTE * 32) + (SECOND / 2)
IF GetFDtTm%(ThisHandle%, FDate%, FTime%) = 0 THEN
  PRINT "Files' Date  (MM/DD/YYYY):   ";
  PRINT (FDate% AND &H1E0&) \ 32; "/";
  PRINT (FDate% AND &H1F&); "/";
  PRINT ((FDate% AND &HFE00&) \ 512) + 1980
  PRINT "  Files' Time  (HH:MM:SS):   ";
  PRINT (FTime% AND &HF800&) \ 2048; ":";
  PRINT (FTime% AND &H1F8&) \ 32; ":";
  PRINT (FTime% AND &H1F&) * 2
END IF


'**CloseFile**
'Closes a file
'RC = 0 = OK
'     6 = Invalid handle
'
E% = CloseFile%(ThisHandle%)

