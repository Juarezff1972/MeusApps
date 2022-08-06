/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWLOCAL_H
#define _NWLOCAL_H
/*______________________________________________________________

       Definitions for the Netware API File Services logic
  ______________________________________________________________*/


#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#ifdef PROTOTYPE
   extern int  cdecl  _FileServiceRequest(BYTE, BYTE *, WORD, BYTE *, WORD);
   extern WORD cdecl  _GetEnvironmentSeg(void);
   extern int	 GetDefaultDrive(void);
   extern int	 GetEnvironmentVariable(char *, char *);
   extern int	 GetEnvironmentVariableLength(char *);
   extern int	 GetNetworkSerialNumber(long *, int *);
   extern int	 RenameFile(char, char *, char, char, char *);
   extern int	 PutEnvironmentVariable(char *, char *);
   extern char	*_ReadEnvironment(void);
   extern int	 cdecl	_ShellRequest(BYTE, BYTE *, BYTE *);
   extern int	 VerifyNetworkSerialNumber(long, int *);
   extern void	_WriteEnvironment(char *);
#else
   extern int  cdecl  _FileServiceRequest();
   extern WORD cdecl  _GetEnvironmentSeg();
   extern int	 GetDefaultDrive();
   extern int	 GetEnvironmentVariable();
   extern int	 GetEnvironmentVariableLength();
   extern int	 GetNetworkSerialNumber();
   extern int	 PutEnvironmentVariable();
   extern char *_ReadEnvironment();
   extern int	 cdecl	_ShellRequest();
   extern int	 VerifyNetworkSerialNumber();
   extern void	_WriteEnvironment();
#endif


#endif
