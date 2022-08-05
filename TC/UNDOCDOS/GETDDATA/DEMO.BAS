' Get disk free space
' Forever Young Software
' Benjamin David Lunt
' (c)opyright 1997
' All rights reserved
' 08/13/97
'
' to assemble GETDDATA.ASM:
'   masm getddata
' to create the quick lib:
'   link /q getddata,getddata,,bqlb45;
' to run the program in the editor
'   qb /l getddata demo
'
'
'
'Returns the Disk data of a specified disk
'RC = 0 = OK
'    -1 = Error
'Parameters:
' Input
'   Drv% =  Drive to be read, 0 = current default drive, 1 = A:, 2 = B:, etc
' RETURN
'   SPC& =  Number of sectors per cluster
'   NAC& =  Number of available clusters
'   BPS& =  Number of bytes per sector
'   CPD& =  Total number of clusters (whether used or not)
'Note:  Check drive status before using.  This function has no error
'          checking for disk in drive


DECLARE FUNCTION DiskData% (Drv%, SPaC&, NAC&, BPS&, CPD&)

PRINT "Getting the data of disk drive:  C:"
IF DiskData%(3, SPaC&, NAC&, BPS&, CPD&) = 0 THEN
  PRINT "sctrs/clus:   "; USING "#,###,###,###"; SPaC&
  PRINT "Avail/clus:   "; USING "#,###,###,###"; NAC&
  PRINT "Bytes/sctr:   "; USING "#,###,###,###"; BPS&
  PRINT "Bytes/clus:   "; USING "#,###,###,###"; SPaC& * BPS&
  PRINT "Total clus:   "; USING "#,###,###,###"; CPD&
  PRINT "Total Sctrs:  "; USING "#,###,###,###"; SPaC& * CPD&
  PRINT "Bytes free:   "; USING "#,###,###,###"; SPaC& * BPS& * NAC&
  PRINT "Total bytes:  "; USING "#,###,###,###"; SPaC& * BPS& * CPD&
  PRINT "Precent free: "; USING "    ###.##%"; (NAC& * 100) / CPD&
ELSE
  PRINT "Error"
END IF

