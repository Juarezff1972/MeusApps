/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>

typedef struct                                        /* NCP request format */
{
   WORD   bufferLength;
   BYTE   subFunction;
   BYTE   dirHandle;
   LONG   entryID;
} requestFormat;

typedef struct                                          /* NCP reply format */
{
   LONG   entryID;
   BYTE   reserved1[4];
   LONG   attributes;
   BYTE   reserved2[2];
   BYTE   nameSpaceType;
   BYTE   nameLength;
   BYTE   name[12];
   BYTE   reserved3[8];
   LONG   lastArchiveDateAndTime;
   LONG   lastArchiverID;
   LONG   modifyDateAndTime;
   LONG   lastModifierID;
   LONG   dataForkSize;
   BYTE   reserved4[46];
   WORD   lastAccessedDate;
   LONG   deletedFileTime;
   LONG   deletedDateAndTime;
   LONG   deletorID;
   BYTE   reserved5[16];
} replyFormat;



WORD ScanSalvagableFiles
(
WORD         serverConnID,
BYTE         dirHandle,
LONG         *entryID,
NWSALV_ENTRY *salvEntry
)
{
   WORD          ccode;
   requestFormat requestBody;
   replyFormat   replyBody;

                                          /* set up the request body buffer */
   requestBody.bufferLength     = sizeof(requestBody) - sizeof(WORD);
   requestBody.subFunction      = 27;
   requestBody.dirHandle        = dirHandle;
   requestBody.entryID          = *entryID;

                           /* fill in buffer length upon entry, upon return */
                             /* from NCPConnRequest offset 0 is overwritten */
   *(WORD *)&replyBody          = sizeof(replyBody) - sizeof(WORD);
       /* 22 is the NCP command code for "file services" directory services */
   ccode = _NCPConnRequest(serverConnID, 22, sizeof(requestBody),
                           &requestBody, sizeof(replyBody), &replyBody);
   if (!ccode)
      {                    /* transfer return values to NWSALV_ENTRY format */
      *entryID = replyBody.entryID;
      salvEntry->entryID                    =
                                   replyBody.entryID;
      salvEntry->attributes                 =
                                   replyBody.attributes;
      salvEntry->nameSpaceType              =
                                   replyBody.nameSpaceType;
      salvEntry->nameLength                 =
                                   replyBody.nameLength;
      memcpy(salvEntry->name, replyBody.name, replyBody.nameLength);
             /* null terminate the name, salvEntry.name is 32 bytes long so */
          /* we're ok if we come across names that are a full 12 bytes long */
      salvEntry->name[replyBody.nameLength] = '\0';
      salvEntry->lastArchiveDateAndTime     =
                                   replyBody.lastArchiveDateAndTime;
      salvEntry->lastArchiverID             =
                                   LongSwap(replyBody.lastArchiverID);
      salvEntry->modifyDateAndTime          =
                                   replyBody.modifyDateAndTime;
      salvEntry->lastModifierID             =
                                   LongSwap(replyBody.lastModifierID);
      salvEntry->dataForkSize               =
                                   replyBody.dataForkSize;
      salvEntry->lastAccessedDate           =
                                   replyBody.lastAccessedDate;
      salvEntry->deletedFileTime            =
                                   replyBody.deletedFileTime;
      salvEntry->deletedDateAndTime         =
                                   replyBody.deletedDateAndTime;
      salvEntry->deletorID                  =
                                   LongSwap(replyBody.deletorID);
      }
   return (ccode);
}
