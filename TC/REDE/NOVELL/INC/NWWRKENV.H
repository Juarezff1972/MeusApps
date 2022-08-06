/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWWRKENV_H
#define _NWWRKENV_H
/*__________________________________________________________________

     Prototypes for the Netware API Workstation Environment Logic
  _________________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#define MAXSERVERS              8

#ifdef PROTOTYPE
   extern void cdecl  EndOfJob(void);
   extern char        *_FindPathVariableElement(char *, int);
   extern int         GetConnectionID(char *, WORD *);
   extern WORD        GetDefaultConnectionID(void);
   extern char        GetDriveLetterFromSearchOrder(char *, int);
   extern void cdecl  GetFileServerName(WORD, char *);
   extern int  cdecl  GetNetWareShellVersion(BYTE *, BYTE *, BYTE *);
   extern int  cdecl  GetNumberOfLocalDrives(void);
   extern int         GetPathEnvironmentVariable(char **);
   extern WORD        GetPreferredConnectionID(void);
   extern WORD        GetPrimaryConnectionID(void);
   extern int         GetSearchOrderFromDriveLetter(char *, char);
   extern void cdecl  GetShellRevision(BYTE *, BYTE *);
   extern void cdecl  _GetWorkstationEnvironment(char *);
   extern void        GetWorkstationEnvironment(char *, char *, char *, char *);
   extern void cdecl  GetWorkstationOS(BYTE *);
   extern int  cdecl  IsConnectionIDInUse(WORD);
   extern int  cdecl  _PrConnRequest(BYTE, BYTE);
   extern BYTE cdecl  SetEndOfJobStatus(BYTE);
   extern BYTE cdecl  SetNetWareErrorMode(BYTE);
   extern void        SetPreferredConnectionID(WORD);
   extern void        SetPrimaryConnectionID(WORD);
   extern int  cdecl _TableReq(BYTE, BYTE, BYTE far ** );
#else
   extern void cdecl  EndOfJob();
   extern char        *_FindPathVariableElement();
   extern int         GetConnectionID();
   extern WORD        GetDefaultConnectionID();
   extern char        GetDriveLetterFromSearchOrder();
   extern void cdecl  GetFileServerName();
   extern int  cdecl  GetNetWareShellVersion();
   extern int  cdecl  GetNumberOfLocalDrives();
   extern int         GetPathEnvironmentVariable();
   extern WORD        GetPreferredConnectionID();
   extern WORD        GetPrimaryConnectionID();
   extern int         GetSearchOrderFromDriveLetter();
   extern void cdecl  GetShellRevision();
   extern void cdecl  _GetWorkstationEnvironment();
   extern void        GetWorkstationEnvironment();
   extern void cdecl  GetWorkstationOS();
   extern int  cdecl  IsConnectionIDInUse();
   extern int  cdecl  _PrConnRequest();
   extern BYTE cdecl  SetEndOfJobStatus();
   extern BYTE cdecl  SetNetWareErrorMode();
   extern void        SetPreferredConnectionID();
   extern void        SetPrimaryConnectionID();
   extern int  cdecl  _TableReq();
#endif
#endif
