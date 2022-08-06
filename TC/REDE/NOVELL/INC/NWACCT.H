/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWACCT_H
#define _NWACCT_H
/*__________________________________________________________________________
	Prototypes for the Netware API Accounting Services
 _________________________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

#ifdef PROTOTYPE

extern int    AccountingInstalled(WORD);
extern int    GetAccountStatus(WORD, char *, long *, long *, long *);
extern int    GetCurrentAccountStatus(WORD, char *, long *, long *, int, long *, int *);
extern int    SubmitAccountCharge(WORD, char *, WORD, long, long, WORD, char *);
extern int    SubmitAccountHold(WORD, char *, long);
extern int    SubmitAccountNote(WORD, char *, WORD, WORD, int, char *);

#else

extern int    AccountingInstalled();
extern int    GetAccountStatus();
extern int    GetCurrentAccountStatus();
extern int    SubmitAccountCharge();
extern int    SubmitAccountHold();
extern int    SubmitAccountNote();

#endif
#endif
