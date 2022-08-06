/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWTTS_H
#define _NWTTS_H
/*__________________________________________________________________________

     Prototypes for the Netware API Transaction Tracking Services (TTS)
  _________________________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#ifdef PROTOTYPE
   extern int  cdecl  TTSAbortTransaction(void);
   extern int  cdecl  TTSBeginTransaction(void);
   extern int  cdecl  TTSEndTransaction(long *);
   extern int  cdecl  TTSGetApplicationThresholds(BYTE *, BYTE *);
   extern int         TTSGetStats( WORD, int *, int, TTS_STATS * );
   extern int  cdecl  TTSGetWorkstationThresholds(BYTE *, BYTE *);
   extern int  cdecl  TTSIsAvailable(void);
   extern int  cdecl  TTSSetApplicationThresholds(BYTE, BYTE);
   extern int  cdecl  TTSSetWorkstationThresholds(BYTE, BYTE);
   extern int  cdecl  TTSTransactionStatus(long);
   extern int         _TTSGetStats( WORD );
#else
   extern int  cdecl  TTSAbortTransaction();
   extern int  cdecl  TTSBeginTransaction();
   extern int  cdecl  TTSEndTransaction();
   extern int  cdecl  TTSGetApplicationThresholds();
   extern int         TTSGetStats();
   extern int  cdecl  TTSGetWorkstationThresholds();
   extern int  cdecl  TTSIsAvailable();
   extern int  cdecl  TTSSetApplicationThresholds();
   extern int  cdecl  TTSSetWorkstationThresholds();
   extern int  cdecl  TTSTransactionStatus();
   extern int         _TTSGetStats();
#endif
#endif
