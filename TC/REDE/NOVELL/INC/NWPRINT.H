/*      COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWPRINT_H
#define _NWPRINT_H

/*______________________________________________________________

       Prototypes for the Netware API Print Services logic
  ______________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

typedef struct
 {
    BYTE    status;
    BYTE    flags;
    BYTE    tabSize;
    BYTE    serverPrinter;
    BYTE    numberOfCopies;
    BYTE    formType;
    BYTE    reserved;
    char    bannerText[13];
    BYTE    reserve;
    BYTE    localLPTDevice;
    int     flushCaptureTimeoutCount;
    BYTE    flushCaptureOnDeviceClose;
    int     maxLines;
    int     maxChars;
    char    formName[13];
    BYTE    LPTCaptureFlag;
    BYTE    fileCaptureFlag;
    BYTE    timingOutFlag;
#ifdef LC310
    WORD    prnSetupBufferOfs;
    WORD    prnSetupBufferSeg;
    WORD    prnResetBufferOfs;
    WORD    prnResetBufferSeg;
#else
    char far *printerSetupBuffer;
    char far *printerResetBuffer;
#endif
    BYTE    connectionIDQueuePrintJob;
    BYTE    captureInProgress;
    BYTE    printQueueFlag;
    BYTE    printJobValid;
    long    printQueueID;
    WORD    printJobNumber;
 } CAPTURE_FLAGS;

typedef struct
 {
    BYTE    status;
    BYTE    flags;
    BYTE    tabSize;
    BYTE    serverPrinter;
    BYTE    numberOfCopies;
    BYTE    formType;
    BYTE    reserved;
    char    bannerText[13];
    BYTE    reserve;
    BYTE    localLPTDevice;
    int     flushCaptureTimeoutCount;
    BYTE    flushCaptureOnDeviceClose;
    int     maxLines;
    int     maxChars;
    char    formName[13];
    BYTE    PAD[21];
} SET_CAPTURE_FLAGS;

/****************************************************************************/

#ifdef PROTOTYPE
   extern int         AppendToCaptureFile( WORD, char * );
   extern int         CancelCaptureFile( void );
   extern int         CancelLPTCapture( void );
   extern int         CancelSpecificLPTCapture( BYTE );
   extern int         DeletePrintQueueEntry( int, BYTE );
   extern int         DeletePrintQueueEntries( int, int, BYTE * );
   extern int  cdecl  DisablePrinterDefaults( void );
   extern int  cdecl  EnablePrinterDefaults( void );
   extern int         EndLPTCapture( void );
   extern int         EndSpecificLPTCapture( BYTE );
   extern int         FlushLPTCapture( void );
   extern int         FlushSpecificLPTCapture( BYTE );
   extern int         GetBannerUserName( char * );
   extern int  cdecl  GetCaptureInfo( BYTE *, BYTE *, WORD *, WORD *, WORD *, 
                                      WORD *, BYTE *, WORD *, long * );
   extern int         GetDefaultCaptureFlags( CAPTURE_FLAGS * );
   extern int  cdecl  GetDefaultLocalPrinter( void );
   extern int         GetLPTCaptureStatus( WORD * );
   extern int  cdecl  GetPrinterDefaults( int *, int *, int *, int *, int *, 
                                          int *, char *, int *, int *, int * );
   extern int         GetPrinterQueue( BYTE, long * );
   extern int         GetPrintQueueEntry( int, BYTE *, char *, BYTE *, int *,
                                          int *, int *, char *, WORD *, 
                                          long *, BYTE * );
   extern int         GetPrinterStatus( int, BYTE *, BYTE *, int *, int * );
   extern int         GetSpecificCaptureFlags( BYTE, CAPTURE_FLAGS * );
   extern int         QueueExistingFile( BYTE, char * );
   extern int         SetBannerUserName( char * );
   extern int  cdecl  SetCapturePrintJob( BYTE, WORD, BYTE * );
   extern int  cdecl  SetCapturePrintQueue( BYTE, BYTE, long );
   extern int         SetDefaultCaptureFlags( SET_CAPTURE_FLAGS * );
   extern int         SetDefaultLocalPrinter( BYTE );
   extern int  cdecl  SetPrinterDefaults( WORD, WORD, WORD, WORD, WORD, 
                                          char *, WORD, WORD, WORD );
   extern int         SetSpecificCaptureFlags( BYTE, SET_CAPTURE_FLAGS * );
   extern int         SetSpoolFlags( char, int, int, int, int, char * ); 
   extern int         SpecifyCaptureFile( BYTE, char * );
   extern int         StartLPTCapture( void );
   extern int         StartSpecificLPTCapture( BYTE );
   extern int  cdecl _LPTCapReq( BYTE, BYTE );
   extern int  cdecl _PrCapRequest( BYTE, BYTE );
   extern int  cdecl _PrnSrvRequest( BYTE, WORD, char *, BYTE );
#else
   extern int         AppendToCaptureFile();
   extern int         CancelCaptureFile();
   extern int         CancelLPTCapture();
   extern int         CancelSpecificLPTCapture();
   extern int         DeletePrintQueueEntry();
   extern int         DeletePrintQueueEntries();
   extern int  cdecl  DisablePrinterDefaults();
   extern int  cdecl  EnablePrinterDefaults();
   extern int         EndLPTCapture();
   extern int         EndSpecificLPTCapture();
   extern int         FlushLPTCapture();
   extern int         FlushSpecificLPTCapture();
   extern int         GetBannerUserName();
   extern int  cdecl  GetCaptureInfo();
   extern int         GetDefaultCaptureFlags();
   extern int  cdecl  GetDefaultLocalPrinter();
   extern int         GetLPTCaptureStatus();
   extern int  cdecl  GetPrinterDefaults();
   extern int         GetPrinterQueue();
   extern int         GetPrintQueueEntry();
   extern int         GetPrinterStatus();
   extern int         GetSpecificCaptureFlags();
   extern int         QueueExistingFile();
   extern int         SetBannerUserName();
   extern int  cdecl  SetCapturePrintJob();
   extern int  cdecl  SetCapturePrintQueue();
   extern int         SetDefaultCaptureFlags();
   extern int         SetDefaultLocalPrinter();
   extern int  cdecl  SetPrinterDefaults();
   extern int         SetSpecificCaptureFlags();
   extern int         SetSpoolFlags(); 
   extern int         SpecifyCaptureFile();
   extern int         StartLPTCapture();
   extern int         StartSpecificLPTCapture();
   extern int cdecl  _LPTCapReq();
   extern int cdecl  _PrCapRequest();
   extern int cdecl  _PrnSrvRequest();
#endif


#endif
