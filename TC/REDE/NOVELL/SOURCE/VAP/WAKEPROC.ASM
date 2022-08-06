;	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.
PAGE	  54,132
TITLE	  WakeUp Process
SUBTTL	  Novell 'C' Network Value-Added Library (NVT.LIB)
NAME	  WakeUpProcess
.LALL

;-------------------------------------------------------------------------------
;	PROGRAM NAME: WakeUpProcess
;		      Novell 'C' Network Value-Added Library (NVT.LIB)
;
;	FILENAME:     WAKEPROC.ASM
;	FUNCTION:     Places THe Specified Process Back On The Run Queue
;	USAGE:	      WakeUpProcess(ProcID);
;	VERSION:	01.00
;_______________________________________________________________________________

	INCLUDE c.mac
	VExtern ProcessControlServices,dword

	PSeg

	ProcDef WakeUpProcess,<<processid,word>>
	PushDI
	PushDS
	PushES
	PushSI

	mov	bx, processid
	mov	di, 0Dh
	mov	cx, seg ProcessControlServices
	mov	bp, offset ProcessControlServices
	mov	es, cx
	call	dword ptr es:[bp]

	PopSI
	PopES
	PopDS
	PopDI

	Return

	ProcEnd WakeUpProcess

	EndPS

	END
