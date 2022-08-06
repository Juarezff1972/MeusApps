;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
INCLUDE c.mac

NAME   SERVREQ

DOSExterns
PSeg

ProcDef _ServerRequest, <<serverNumber, byte>,<functionNumber, byte>,<subFunctionNumber, byte>>
mov    dl, serverNumber
mov    ah, functionNumber
mov    al, subFunctionNumber
EnterDOS
Return
ProcEnd _ServerRequest

EndPS

END
