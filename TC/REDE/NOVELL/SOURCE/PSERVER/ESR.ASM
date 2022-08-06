; (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.

        _DATA   SEGMENT WORD PUBLIC 'DATA'

        DGROUP  group  _DATA

        _DATA   ends
        INCLUDE c.mac
        NAME    ESR
        DOSExterns

        PSeg

        ProcDef _ReplyReceivedESR, <<farPtr dptr >>
        Push ES
        Push SI
        Push BX

        mov ax, DGROUP
        mov ds, ax
        LoadPtr bx, farPtr, ds
        mov 2[bx], es
        mov 0[bx], si

        Pop BX
        Pop SI
        Pop ES

        Return
        ProcEnd _ReplyReceivedESR

        EndPS

        END
