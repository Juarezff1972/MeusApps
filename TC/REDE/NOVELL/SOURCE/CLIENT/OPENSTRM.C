/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <string.h>
#include <nat.h>
#include <nit.h>
#include <ntt.h>

typedef struct
{                                           
    WORD     bufferLength;
    BYTE     subFunction;
    BYTE     dataStream;
    BYTE     dirHandle;
    BYTE     fileAttributes;
    BYTE     openRights;
    BYTE     nameLength;
    char     name[NWMAX_NCP_PATH];
} REQUEST;

WORD OpenDataStream
(
WORD       serverConnID,
BYTE       dataStreamType,
BYTE       dirHandle,
BYTE       fileAttributes,
BYTE       openRights,
char       *filePath,
BYTE       *netwareHandle,      /* 6-byte array */
int        *fileHandle
)

{
    WORD          ccode;
    REQUEST       requestBody;
    BYTE          replyBuffer[6];
    LONG          sequence;
    BYTE          tmpHandle;
    char          tmpPath[255];
    NWPHYS_ENTRY  entryInfo;


    memset( tmpPath, 0, 255 );
    ccode = ConvertDirectoryHandle( serverConnID, dirHandle, 
                                    filePath, &tmpHandle, tmpPath );
    if( ccode ) 
      {
         DeallocateDirectoryHandle( tmpHandle );
         return( ccode );
      }
    
    requestBody.bufferLength       = IntSwap(sizeof(requestBody)-2);
    requestBody.subFunction        = 49;
    requestBody.dataStream         = dataStreamType;
    requestBody.dirHandle          = tmpHandle;
    requestBody.fileAttributes     = fileAttributes;
    requestBody.openRights         = openRights;
    requestBody.nameLength         = (BYTE)strlen(tmpPath);
    memcpy(requestBody.name,filePath,requestBody.nameLength);

    *(WORD *)replyBuffer = 4;

    ccode = _NCPConnRequest( serverConnID, 0x16, sizeof(requestBody),
                             &requestBody, sizeof(replyBuffer),
                             &replyBuffer[2] );

    if (!ccode)
     {
         sequence       = (LONG)-1;
         fileAttributes = TF_READ_ONLY | TF_HIDDEN | TF_SYSTEM;
         ccode          = ScanFilePhysical( serverConnID, tmpHandle,
                                            tmpPath, fileAttributes, 
                                            &sequence, &entryInfo );
         if (!ccode)
          {
            if (netwareHandle)
             {
               *((WORD *)replyBuffer) = 0; /* pad two low order bytes       */

               memmove(netwareHandle,replyBuffer,6);
             }

            if (fileHandle)
               ccode = _ConvertHandle( serverConnID, fileAttributes,
                                       replyBuffer,
                                       entryInfo.forkSize[ 2*dataStreamType ],
                                       fileHandle );
          }
     }

    DeallocateDirectoryHandle( tmpHandle );
    return( ccode );
}
