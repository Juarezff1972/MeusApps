/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */
#include <string.h>
#include <ntt.h>
#include <nwmisc.h>

#define  MIN( a, b )       ( ( a ) < ( b ) ) ? ( a ) : ( b )

WORD  ScanDirRestrictions( WORD  serverConnID,
                           BYTE  dirHandle,
                           BYTE *buffer,
                           BYTE  numberOfStructuresToReturn )
{
   WORD  ccode;
   BYTE  requestBuff[4], replyBuff[NCP_MAX_BUFFER];
   int   bytesToCopy;


   *(WORD *)&requestBuff[0] = IntSwap( 2 );
   requestBuff[2]  = 35;   
   requestBuff[3]  = dirHandle;
   *(WORD *)&replyBuff[0] = NCP_MAX_BUFFER - 2;

   ccode  = _NCPConnRequest(  serverConnID, 22, 4, &requestBuff[0],
                              NCP_MAX_BUFFER-2, &replyBuff[2] );
   if( ccode != 0 )
      return(ccode);

   bytesToCopy = (numberOfStructuresToReturn * 9) + 1;
   bytesToCopy = MIN( bytesToCopy, NCP_MAX_BUFFER - 2 );
   memmove( buffer, &replyBuff[2], bytesToCopy );

   return( ccode );
}
