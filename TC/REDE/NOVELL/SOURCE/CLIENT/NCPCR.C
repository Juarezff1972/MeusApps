/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <nit.h>
#include <ntt.h>

WORD  _NCPConnRequest
(
WORD      connectionID,
BYTE      NCPFunctionCode,
WORD      requestLength,
VOID      *requestBody,
WORD      replyLength,
VOID      *replyBody
)

{
   WORD      ccode;
   WORD      oldConnID;

   oldConnID   = GetPreferredConnectionID();
   SetPreferredConnectionID(connectionID);
   ccode         = _NCPRequest(NCPFunctionCode, requestLength, requestBody,
                        replyLength, replyBody);
   SetPreferredConnectionID(oldConnID);

   return(ccode);
}                                                     /* end of _NCPRequest */
