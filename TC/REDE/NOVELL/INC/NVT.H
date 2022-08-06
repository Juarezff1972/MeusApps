/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NVT_H
  #define _NVT_H
  #ifndef _PROLOG_H
    #include <prolog.h>
  #endif

  /* Vap Console and Process Control Procedures */

  #ifdef PROTOTYPE
    extern int      cdecl AllocateSegment(WORD);
    extern long     cdecl CalculateAbsoluteAddress(WORD,WORD);
    extern void     cdecl ChangeProcess(void);
    extern void     cdecl ChangeSegmentToCode(WORD);
    extern void     cdecl ChangeSegmentToData(WORD);
    extern void     cdecl ClearScreen(void);
    extern void     cdecl ConsoleDisplay(char *);
    extern void     cdecl ConsoleError(char *);
    extern void     cdecl ConsoleMessage(char *);
    extern int      cdecl ConsoleQuery(char *);
    extern int      cdecl ConsoleRead(char *, char *,BYTE);
    extern int      cdecl CreateProcess(WORD, WORD *, WORD, WORD);
    extern int      cdecl DeclareExtendedSegment(WORD,BYTE);
    extern int      cdecl DeclareSegmentAsData(WORD);
    extern void     cdecl DelayProcess(WORD);
    extern void     cdecl DoConsoleError(char *);
    extern void     cdecl GetInterruptVector(BYTE, WORD *, WORD *);
    extern WORD     cdecl GetProcessID(void);
    extern int      cdecl GetScreenMode(void);
    extern WORD     cdecl GetVAPHeader(WORD);
    extern void     cdecl InitializationComplete(void);
    extern int      cdecl InString(BYTE,BYTE,char *,char *,BYTE);
    extern void     cdecl KillProcess(void);
    extern void     cdecl OutString(BYTE,BYTE,char *,BYTE);
    extern void     cdecl PrintString(WORD, WORD, char *, WORD);
    extern int      cdecl ReadKeyboard(char *, WORD);
    extern void far *  cdecl SegmentToPointer(WORD);
    extern void     cdecl SetExternalProcessError( void(*)(), WORD);
    extern BYTE     cdecl SetHardwareInterruptVector(BYTE,WORD,WORD);
    extern void     cdecl SetInterruptVector(BYTE, WORD, WORD);
    extern void     cdecl SetScreenMode(WORD);
    extern void     cdecl ShellPassThroughEnable(BYTE);
    extern void     cdecl SleepProcess(void);
    extern int      cdecl SpawnProcess(void(*)(),WORD,WORD);
    extern int      cdecl VAPAttachToFileServer(char *,WORD *);
    extern int      cdecl VAPGetConnectionID(char *,WORD *);
    extern int      cdecl VAPGetFileServerName(char *);
    extern void     cdecl WakeUpProcess(WORD);
  #else
    extern int      cdecl AllocateSegment();
    extern long     cdecl CalculateAbsoluteAddress();
    extern void     cdecl ChangeProcess();
    extern void     cdecl ChangeSegmentToCode();
    extern void     cdecl ChangeSegmentToData();
    extern void     cdecl ClearScreen();
    extern void     cdecl ConsoleDisplay();
    extern void     cdecl ConsoleError();
    extern void     cdecl ConsoleMessage();
    extern int      cdecl ConsoleQuery();
    extern int      cdecl ConsoleRead();
    extern int      cdecl CreateProcess();
    extern int      cdecl DeclareExtendedSegment();
    extern int      cdecl DeclareSegmentAsData();
    extern void     cdecl DelayProcess();
    extern void     cdecl DoConsoleError();
    extern void     cdecl GetInterruptVector();
    extern WORD     cdecl GetProcessID();
    extern int      cdecl GetScreenMode();
    extern int      cdecl GetVAPHeader();
    extern void     cdecl InitializationComplete();
    extern int      cdecl InString();
    extern void     cdecl KillProcess();
    extern void     cdecl OutString();
    extern void     cdecl PrintString();
    extern int      cdecl ReadKeyboard();
    extern void far *  cdecl SegmentToPointer();
    extern void     cdecl SetExternalProcessError();
    extern BYTE     cdecl SetHardwareInterruptVector();
    extern void     cdecl SetInterruptVector();
    extern void     cdecl SetScreenMode();
    extern void     cdecl ShellPassThroughEnable();
    extern void     cdecl SleepProcess();
    extern int      cdecl SpawnProcess();
    extern int      cdecl VAPAttachToFileServer();
    extern int      cdecl VAPGetConnectionID();
    extern int      cdecl VAPGetFileServerName();
    extern void     cdecl WakeUpProcess();
  #endif
#endif
