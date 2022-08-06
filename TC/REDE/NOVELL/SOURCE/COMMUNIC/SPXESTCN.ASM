;     (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.
PAGE      54,132
TITLE     SPX ESTABLISH CONNECTION
SUBTTL    Novell 'C' Network Interface Library (NIT.LIB)
NAME      SPXESTCN
.LALL

;*******************************************************************************
;       PROGRAM NAME: SPX Establish Connection
;                     Novell 'C' Network Interface Library (NIT.LIB)
;
;       FILENAME:     SPXESTCN.ASM
;       FUNCTION:     Establish a connection for SPX
;       USAGE:        ccode = SPXEstablishConnection(retrycount,watchdogflag,connectionidnumber,eventcontrolblock)
;       INPUTS:       RetryCount = int 0-255.
;                     WatchDogFlag = int zero enables, non-zero disables.
;                     *ConnectionIDNumber = handle used for further SPX calls on connection
;                     ECB = *EventControlBlock.
;
;       PROCESSES:    Prepare for IPX call, SPX function 11h.
;                     Make the call.
;                     Return to C.
;
;
;       OUTPUTS:      None
;*******************************************************************************
;       DATE:           23 MAR 87
;       VERSION:        01.01
;       REMARKS:        This function is used to establish a connection through
;                       SPX to a waiting partner.  An initial message is sent to the
;                       indacted internetwork address, attempting to create an
;                       SPX connection.  If the target responds affirmatively,
;                       then a two way connection is established.
;
;*******************************************************************************

        INCLUDE c.mac
        IPXExterns

        PSeg

        ProcDef SPXEstablishConnection,<<retrycount,byte>,<watchdog,byte>,<connectionidnumber,dptr>,<eventcontrolblock,dptr>>
        PushDI
        PushES
        PushSI
        mov     al,retrycount
        mov     ah,watchdog
        LoadPtr si, eventcontrolblock, es
        push    bp
        mov     bx,11h
        EnterIPX                                ;IPX Interrupt
        pop     bp
        PopSI
        PopES
        PopDI
        LoadPtr bx, connectionidnumber, es
        mov     es:[bx],dx
        RetVal  byte,reg,al
        Return
        ProcEnd SPXEstablishConnection

        EndPS

        END
