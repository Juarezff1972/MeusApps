/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdlib.h>
#include <string.h>
#include <nwacct.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*____________________________GetAccountStatus______________________________
|
|  Output:                   0  --  SUCCESSFUL
|           Network Error Code  --  UNSUCCESSFUL
|
|  Comments:  This function returns the account status of a bindery object.
|___________________________________________________________________________*/

int GetAccountStatus(binderyObjectType,binderyObjectName,balance,limit,holds)
WORD        binderyObjectType;     /* Type of the object to get status of   */
char       *binderyObjectName;     /* Name of the object to get status of   */
long       *balance;               /* Receives the total amount the bindery
                                      object has available to purchase file
                                      server resources                      */
long       *limit;                 /* Receives the lowest level the object's
                                      account balance can reach before the
                                      object can no longer purchase services*/
long       holds[];                /* Receives a list of objects that have
                                      placed the account on hold            */
{
BYTE sendPacket[54];
BYTE receivePacket[258];
int         ccode;
int         nameLen;
long       *recvOffset;
int         i;

    nameLen = strlen(binderyObjectName);
    *((WORD *)sendPacket) = 4 + nameLen;
    sendPacket[2] = 0x96;
    *((WORD *)(sendPacket + 3)) = IntSwap(binderyObjectType);
    sendPacket[5] = nameLen;
    memmove( (char *)(sendPacket + 6), binderyObjectName, nameLen);

    *((WORD *)receivePacket) = 256;

    if ((ccode = _ShellRequest((BYTE)0xE3, sendPacket, receivePacket)) != 0)
        return(ccode);

    *balance = LongSwap(*((long *)(receivePacket + 2)));
    *limit = LongSwap(*((long *)(receivePacket + 6)));
    recvOffset = (long *)(receivePacket + 10);
    for (i = 0; i < 32; i++) {
        holds[i] = LongSwap(*recvOffset);
        recvOffset++;
    }
    return(0);
}
/****************************************************************************/

#define MIN(value1,value2) ((value1)<(value2)?(value1):(value2))

/****  GetCurrentAccountStatus -- returns the account status of a bindery object
*
*   Input:      WORD    binderyObjectType  type of object to get status of
*                   char   *binderyObjectName  name of object to get status of
*                   int     maxAccountHolds maximum number of account holds
*                                           to return
*
*   Output:     long   *accountBalance  total amount bindery object has
*                                           available to purchase file server
*                                           resources
*                   long   *creditLimit     the lowest level the object's
*                                           account balance can reach before
*                                           the object can no longer purchase
*                                           services
*                       long   *accountHolds    a list of objects that have placed
*                                               the account on hold
*                       int    *more            TRUE (1) if more account holds are
*                                               available
*                       Returns a 0 if successful, -1 if out of memory,
*                            or a network error code if unsuccessful.
*****/

static  BYTE *sPacket = NULL;
static  BYTE *rPacket = NULL;
static  BYTE holdOffset = 0;

int GetCurrentAccountStatus(binderyObjectType, binderyObjectName,
            accountBalance, creditLimit, maxAccountHolds, accountHolds, more)
WORD        binderyObjectType;
char       *binderyObjectName;
long       *accountBalance;
long       *creditLimit;
int         maxAccountHolds;
long       *accountHolds;
int        *more;
{
int         ccode;
long       *holdItems;
int         itemNumber;
int         offset;


            /* if any the following three conditions exist - get a new
               packet */
    if ( ( !sPacket ) ||
            /* last packet was completely returned */
         ( !accountHolds ) ||
            /* no reason to send more if accountHolds isnt returned */
         ( *((WORD *)(sPacket + 3)) != binderyObjectType ) ||
            /* new object type was requested */
         ( strcmp( (char *)(sPacket + 6), binderyObjectName ) ) )
            /* new bindery object name was requested */
    {
        if (!sPacket)
        {
            sPacket = (BYTE *)malloc( 54 );
            if (!sPacket)
            {
                ccode = -1;
                goto Return;
            }
        }
        if (!rPacket)
        {
            rPacket = (BYTE *)malloc( 138 );
            if (!rPacket)
            {
                free( sPacket );
                sPacket = NULL;
                ccode = -1;
                goto Return;
            }
        }
    *((WORD *)sPacket) = 4 + ( *((BYTE *)(sPacket + 5)) =
                        (BYTE)strlen( binderyObjectName ) );
    sPacket[2] = 0x96;
    *((WORD *)(sPacket + 3)) = IntSwap(binderyObjectType);
    memmove( (char *)(sPacket + 6), binderyObjectName,
            *((WORD *)(sPacket + 5)) );
    if ( (ccode = _ShellRequest((BYTE)0xE3, sPacket, rPacket))!=0 )
        goto Return;
    holdOffset = 0;
    }
    if (accountBalance)
        *accountBalance = LongSwap(*((long *)(rPacket + 2)));
    if (creditLimit)
        *creditLimit = LongSwap(*((long *)(rPacket + 6)));
    if (accountHolds)
    {
        holdItems = (long *)(rPacket + 10 + sizeof(long) * holdOffset);
        for ( itemNumber = 0;
              ( itemNumber < MIN( maxAccountHolds, 32 - holdOffset ) ) &&
              ( holdItems[ offset = 2 * itemNumber ] != 0 ); itemNumber++ )
        {
            accountHolds[ offset ] = LongSwap(holdItems[ offset ]);
            accountHolds[ offset + 1 ] = LongSwap(holdItems[ offset + 1]);
        }
        *more = ( ( itemNumber < 32 - holdOffset ) &&
                  ( holdItems[ 2 * itemNumber ] != 0 ) );
    }
    else
        *more = FALSE;
    if ( *more )
        holdOffset = (BYTE)itemNumber + holdOffset;
    else
    {
            free( sPacket );
            sPacket = NULL;
            free( rPacket );
            rPacket = NULL;
    }
    ccode = 0;
Return:
    return( ccode );
}
