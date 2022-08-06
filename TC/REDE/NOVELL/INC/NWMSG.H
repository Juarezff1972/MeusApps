/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWMSG_H
#define _NWMSG_H
/*______________________________________________________________

     Prototypes for the Netware API Message Handling Services
  ______________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#ifdef PROTOTYPE
   extern int	 BroadcastToConsole(char *);
   extern int	 CheckPipeStatus( WORD *, BYTE *, WORD );
   extern int	 CloseMessagePipe(WORD *, BYTE *, WORD);
   extern int	 DisableBroadcasts(void);
   extern int	 EnableBroadcasts(void);
   extern int	 GetBroadcastMessage(char *);
   extern BYTE cdecl  GetBroadcastMode(void);
   extern int	 GetPersonalMessage(char *, WORD *);
   extern int	 GetPipeStatus(BYTE *, BYTE *, BYTE);
   extern int	 LogNetworkMessage(char *);
   extern int	 OpenMessagePipe(WORD *, BYTE *, WORD);
   extern int	 SendBroadcastMessage(char *, WORD *, BYTE *, WORD);
   extern int	 SendPersonalMessage(char *, WORD *, BYTE *, WORD);
   extern int	 SetBroadcastMode(BYTE);
#else
   extern int	 BroadcastToConsole();
   extern int	 CheckPipeStatus();
   extern int	 CloseMessagePipe();
   extern int	 DisableBroadcasts();
   extern int	 EnableBroadcasts();
   extern int	 GetBroadcastMessage();
   extern BYTE cdecl  GetBroadcastMode();
   extern int	 GetPersonalMessage();
   extern int	 GetPipeStatus();
   extern int	 LogNetworkMessage();
   extern int	 OpenMessagePipe();
   extern int	 SendBroadcastMessage();
   extern int	 SendPersonalMessage();
   extern int	 SetBroadcastMode();
#endif

#endif
