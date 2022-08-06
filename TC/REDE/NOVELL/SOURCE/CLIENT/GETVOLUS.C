/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <ntt.h>
#include <nwmisc.h>
#include <prolog.h>
#include <niterror.h>

/* constant definitions (define statements) */
#define V30_BLOCKSIZE     4000      /* server block size */
#define SECTOR_SIZE_386OS 512
#define REQUEST_BODY      4

/* macro definitions */
#ifndef min
#define min(a,b)   ((a < b) ? a : b)
#endif

/* NCP formats using NCP documentation names */
typedef struct
{
   BYTE   systemIntervalMarker[4];
   BYTE   volumeNumber;
   BYTE   logicalDriveNumber;
   BYTE   blockSize[2];
   BYTE   startingBlock[4];
   BYTE   totalBlocks[2];
   BYTE   freeBlocks[2];
   BYTE   totalDirEntries[2];
   BYTE   freeDirEntries[2];
   BYTE   actualMaxUsedDirEntries[2];
   BYTE   volHashed;
   BYTE   volCached;
   BYTE   volRemovable;
   BYTE   volMounted;
   BYTE   volName[16];
} VOL_INFO_V2X;

typedef struct
{
   LONG   totalBlocks;
   LONG   freeBlocks;
   LONG   purgableBlocks;
   LONG   notYetPurgableBlocks;
   LONG   totalDirEntries;
   LONG   availDirEntries;
   LONG   flags;               /* currently unused */
   BYTE   sectorsPerBlock;
   BYTE   volNameLength;
   char   volName[NWMAX_VOL_NAME];
} VOL_INFO_V30;


/* Descrip:
         This function will return the volume usage information for
         a V2X OS or 386 OS v3.x.
*/
WORD  GetVolUsage
(
WORD        serverConnID,
WORD        volNumber,
NWVOL_USAGE *volUsage
)
{
   WORD           ccode;
   BYTE           requestBody[REQUEST_BODY];
   VOL_INFO_V2X   volInfoV2X;
   VOL_INFO_V30   volInfoV30;
   /*-------------------------------------------------- function body */
/*         requestBody[0] = REQUEST_BODY / 256;
         requestBody[1] = REQUEST_BODY % 256;  DOES IT NEED TO BE SWAPPED? */
         *(WORD *)requestBody = REQUEST_BODY;
         requestBody[3] = volNumber;

      if (IsV3Supported(serverConnID) == FALSE)
      {
         requestBody[2]   = 233;               /* subfunction code */

         ccode   = _NCPConnRequest(serverConnID, 23, REQUEST_BODY, requestBody,
                                   sizeof(volInfoV2X), &volInfoV2X);
         if (ccode == 0)
         {
            if (volInfoV2X.volMounted == FALSE)
                        /* This is what 386 OS does when volume not mounted */
               return(VOLUME_DOES_NOT_EXIST);

            volUsage->totalBlocks       = (LONG)IntSwap(
                                 *(WORD *)volInfoV2X.totalBlocks);
            volUsage->freeBlocks        = (LONG)IntSwap(
                                 *(WORD *)volInfoV2X.freeBlocks);
            volUsage->purgableBlocks    = 0;
            volUsage->totalDirEntries   = (LONG)IntSwap(
                                 *(WORD *)volInfoV2X.totalDirEntries);
            volUsage->availDirEntries   = (LONG)IntSwap(
                                 *(WORD *)volInfoV2X.freeDirEntries);
            volUsage->maxDirEntriesUsed = (LONG)IntSwap(
                                 *(WORD *)volInfoV2X.actualMaxUsedDirEntries);
            volUsage->blockSize         = IntSwap((int) volInfoV2X.blockSize);
            volUsage->isHashing         = IntSwap(volInfoV2X.volHashed);
            volUsage->isRemovable       = IntSwap(volInfoV2X.volRemovable);
            strncpy(volUsage->volName,volInfoV2X.volName,16);
  /* notYetPurgableBlocks, and flags return fields undefined non-v3 servers */
         }
      }
      else
      {
         requestBody[2]   = 44;               /* subfunction code */

         ccode   = _NCPConnRequest(serverConnID, 22, REQUEST_BODY, requestBody,
                                    sizeof(volInfoV30), &volInfoV30);
         if (ccode == 0)
         {
#if 1                   /* a little tricky, but more efficient, could cause */
             /* bug if structure changes.  I'm leaving the if 1 here incase */
           /* in the future in becomes necessary to use the assignment form */

            memcpy(&volUsage->totalBlocks, &volInfoV30.totalBlocks,
                   sizeof(LONG) * 7);
#else
            volUsage->totalBlocks          = volInfoV30.totalBlocks;
            volUsage->freeBlocks           = volInfoV30.freeBlocks;
            volUsage->purgableBlocks       = volInfov30.purgableBlocks;
            volUsage->notYetPurgableBlocks = volInfov30.notYetPurgableBlocks;
            volUsage->availDirEntries      = volInfoV30.freeDirEntries;
            volUsage->maxDirEntriesUsed    = volInfoV30.maxDirEmtriesUsed;
#endif
            volUsage->blockSize            =
                  volInfoV30.sectorsPerBlock * SECTOR_SIZE_386OS;
            volUsage->isHashing            = TRUE;        /* All 386 hashed */
            volUsage->isRemovable          = FALSE;    /* not option on 386 */
              /* try to catch long volume names incase they ever get longer */
            strncpy(volUsage->volName, volInfoV30.volName,
                               min(NWMAX_VOL_NAME, volInfoV30.volNameLength));
            volUsage->
                 volName[min(NWMAX_VOL_NAME,volInfoV30.volNameLength)] = '\0';
                              /* maxDirEntriesUsed undefined for 386 server */
         }
      }
   return(ccode);
}
