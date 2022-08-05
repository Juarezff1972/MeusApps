'  Format a disk
'RC = ???
'Paramters:
'   FDrive% = 1 = a:, 2 = b:, can't format hard drive
'   FSize% = 0 = 360k, 1 = 1.2m, 2 = 720k, 3 = 1.44m
'   Verb% = not(0) Doesn't print anything, 0 prints status, etc.
'Note:  Make sure to free some memory for this function with SETMEM

DECLARE FUNCTION FrmtDisk% (FDrive%, FSize%, Verb%)

CleanUp& = SETMEM(-50000)   'a bit much, but the more the better
FDrive% = 1
FSize% = 3
Verb% = 0
V% = FrmtDisk%(FDrive%, FSize%, Verb%)
