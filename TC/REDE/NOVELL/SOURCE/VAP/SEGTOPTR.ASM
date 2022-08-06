;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
;-------------------------------------------------------------------------------
;       PROGRAM NAME: SegmentToPointer
;
;       FILENAME:     SEGTOPTR.ASM
;       FUNCTION:     Convert Segment to Far Pointer
;       USAGE:        farptr = SegmentToPointer(seg);
;       VERSION:        01.00
;_______________________________________________________________________________

        INCLUDE c.mac

        PSeg

        ProcDef SegmentToPointer,<<mseg,word>>
        PushDI
        PushDS
        PushES
        PushSI

        mov     dx, mseg
        mov     ax, 0

        PopSI
        PopES
        PopDS
        PopDI

        RetVal  long,reg,ax,dx
        Return

        ProcEnd SegmentToPointer

        EndPS

        END
