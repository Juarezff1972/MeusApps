; (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.

        INCLUDE c.mac

        NAME    NCPR

;*****************************************************************************
;
;                     ----  _NCPRequest ----
;
; Input:  BYTE NCPFunctionCode      -  The NCP function number to be executed.
;         WORD requestLength        -  The length of the request body
;         BYTE *requestBody         -  The address of the body portion of
;                                      request to be sent to a server
;         WORD replyLength          -  The length of the reply body
;         BYTE *replybody           -  The address where the returned IPX packet
;                                      should be stored
;
; Output:   Returns NCP completion code
;
; Comments:
;   This routine is the interface between the Novell 'C' Library routines
;   and SHELL function 0xF2 that allows sending and receiving NCP queries
;   directly.  The 'C' routines set up the request body and the reply body
;   before calling _NCPRequest.
;
;   The request body is the portion of the NCP request after the
;   NCP header field Function Code.  The reply body is the portion of the
;   reply after the NCP reply header field Completion Status.
;*******************************************************************************

        DOSExterns

        PSeg

        ProcDef _NCPRequest, <<NCPFunctionCode,byte>,<requestLength,word>,<requestBody,dptr>,<replyLength,word>,<replyBody,dptr>>
                                ; MASM require are arguments on one line
        PushDS
        PushSI
        PushDI
        mov     ah, 0F2h                ; SHELL raw NCP interface
        mov     al, NCPFunctionCode
        mov     cx, requestLength
        LoadPtr si, requestBody, ds
        mov     dx, replyLength
        LoadPtr di, replyBody, es
        EnterDOS                        ;Execute the shell function
        RetVal  byte, reg, al
        PopDI
        PopSI
        PopDS
        Return
        ProcEnd _NCPRequest

        EndPS

        END

