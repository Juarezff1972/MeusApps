/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWSYNC_H
#define _NWSYNC_H
/*______________________________________________________________

      Prototypes for the Netware API Synchronization Logic
  ______________________________________________________________*/


#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#ifdef PROTOTYPE
   extern int           ClearFile(char *);
   extern void          ClearFileSet(void);
   extern int           ClearLogicalRecord(char *);
   extern void          ClearLogicalRecordSet(void);
   extern int           ClearPhysicalRecord(int, long, long);
   extern void          ClearPhysicalRecordSet(void);
   extern int           CloseSemaphore(long);
   extern int    cdecl  ExamineSemaphore(long, int *, WORD *);
   extern int           GetLockMode(void);
   extern int           LockFileSet(WORD);
   extern int           LockLogicalRecordSet(BYTE, WORD);
   extern int           LockPhysicalRecordSet(BYTE, WORD);
   extern int           LogFile(char *, BYTE, WORD);
   extern int           LogLogicalRecord(char *, BYTE, WORD);
   extern int           LogPhysicalRecord(int, long, long, BYTE, WORD);
   extern int    cdecl  OpenSemaphore(char *, int, long *, WORD *);
   extern int           ReleaseFile(char *);
   extern void          ReleaseFileSet(void);
   extern int           ReleaseLogicalRecord(char *);
   extern void          ReleaseLogicalRecordSet(void);
   extern int           ReleasePhysicalRecord(int, long, long);
   extern void          ReleasePhysicalRecordSet(void);
   extern int           SetLockMode(BYTE);
   extern int           SignalSemaphore(long);
   extern int           WaitOnSemaphore(long, WORD);
   extern int    cdecl _clrRelReq(BYTE,char *);
   extern int    cdecl _lockReq(BYTE,BYTE,WORD);
   extern int    cdecl _logReq(BYTE,char *,BYTE,WORD);
   extern int    cdecl _physRecReq(BYTE,WORD,long,long,BYTE,WORD);
   extern int    cdecl  _semaphorReq(BYTE, long, WORD);
   extern void   cdecl _setClearReq(BYTE);
#else
   extern int           ClearFile();
   extern void          ClearFileSet();
   extern int           ClearLogicalRecord();
   extern void          ClearLogicalRecordSet();
   extern int           ClearPhysicalRecord();
   extern void          ClearPhysicalRecordSet();
   extern int           CloseSemaphore();
   extern int    cdecl  ExamineSemaphore();
   extern int           GetLockMode();
   extern int           LockFileSet();
   extern int           LockLogicalRecordSet();
   extern int           LockPhysicalRecordSet();
   extern int           LogFile();
   extern int           LogLogicalRecord();
   extern int           LogPhysicalRecord();
   extern int    cdecl  OpenSemaphore();
   extern int           ReleaseFile();
   extern void          ReleaseFileSet();
   extern int           ReleaseLogicalRecord();
   extern void          ReleaseLogicalRecordSet();
   extern int           ReleasePhysicalRecord();
   extern void          ReleasePhysicalRecordSet();
   extern int           SetLockMode();
   extern int           SignalSemaphore();
   extern int           WaitOnSemaphore();
   extern int    cdecl _clrRelReq();
   extern int    cdecl _lockReq();
   extern int    cdecl _logReq();
   extern int    cdecl _physRecReq();
   extern int    cdecl  _semaphorReq();
   extern void   cdecl _setClearReq();
#endif

#endif
