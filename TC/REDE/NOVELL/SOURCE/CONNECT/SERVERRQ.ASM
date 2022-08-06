;	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.
NAME	SERVERRQ
INCLUDE c.mac

DOSExterns
PSeg

ProcDef _ServerReq, <<serverNumber, byte>,<functionNumber, byte>,<subFunctionNumber, byte>>
mov    dl, serverNumber
mov    ah, functionNumber
mov    al, subFunctionNumber
EnterDOS
RetVal byte, reg, al
Return
ProcEnd _ServerReq

EndPS

END
