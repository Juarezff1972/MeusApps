/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWCONN_H
#define _NWCONN_H

/*______________________________________________________________
    Definitions for the Netware API Connection Services Logic
  ______________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#ifdef PROTOTYPE

extern int    AttachToFileServer(char *, WORD *);
extern int    AttachToFileServerWithAddress( char *, WORD *, BYTE * );
extern void   DetachFromFileServer(WORD);
extern int    EnterLoginArea(char *, int);
extern int    GetConnectionInformation(WORD, char *, WORD *, long *, BYTE *);
extern WORD   GetConnectionNumber(void);
extern int    GetInternetAddress(WORD, char *, char *, WORD *);
extern int    GetObjectConnectionNumbers(char *, WORD, WORD *, WORD *, WORD);
extern void cdecl  GetStationAddress(BYTE *);
extern int    LoginToFileServer(char *, WORD, char *);
extern void   Logout(void);
extern void   LogoutFromFileServer(WORD);
extern int  cdecl  _ServerReq(BYTE, BYTE, BYTE);

#else

extern int    AttachToFileServer();
extern int    AttachToFileServerWithAddress();
extern void   DetachFromFileServer();
extern int    EnterLoginArea();
extern int    GetConnectionInformation();
extern WORD   GetConnectionNumber();
extern int    GetInternetAddress();
extern void cdecl  GetStationAddress();
extern int    LoginToFileServer();
extern void   Logout();
extern void   LogoutFromFileServer();
extern int  cdecl  _ServerReq();

#endif

#endif
