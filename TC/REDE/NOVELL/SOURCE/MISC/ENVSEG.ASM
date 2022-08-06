;	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME    ENVSEG
        INCLUDE c.mac

        DOSExterns

        PSeg

        ProcDef _GetEnvironmentSeg
        PushES
        PushSI
        PushDI
        mov     ah, 186
        EnterDOS
        RetVal  word, reg, dx
        PopDI
        PopSI
        PopES
        Return
        ProcEnd _GetEnvironmentSeg

        EndPS

        END

