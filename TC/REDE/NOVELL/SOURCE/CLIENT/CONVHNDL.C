/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <nit.h>
#include <niterror.h>
#include <ntt.h>

#define TRUE    1
#define FALSE   0

WORD GetOriginalLetter( BYTE, WORD, char * );
void ConvertDots( char *, char * );
BYTE ValidateDot( char * );

/*----------------------------------------------------------------------------
    This function can be used to convert a directory handle and a path into
    the correct format needed by the operation system.  This can then be
    passed into functions such as ScanDirEntry, ScanFileEntry...
    NOTE:  This function maps a temporary directory handle to the newDirectory
           Path that will be passed back to the user.
  ---------------------------------------------------------------------------*/

WORD  ConvertDirectoryHandle(  WORD  connectionID,
                               BYTE  oldHandle,
                               char *oldDirPath,
                               BYTE *newHandle,
                               char *newDirPath )
{
WORD   fullPath, checkLetter, endOfString;
char  *charPtr, *volPtr, *startPtr, *endPtr;
char   tempPath[255], letterPath[255], driveLetter;
BYTE   effectiveRights;
WORD   oldConnectionID;
WORD   ccode;


    memset( tempPath, 0, 255 );
    memset( letterPath, 0, 255 );
    checkLetter = FALSE;

    oldConnectionID = GetPreferredConnectionID();
    SetPreferredConnectionID( connectionID );

    /*-- If directory handle != 0, then we need to use this handle to
         find what the full directory specification is for that handle.
         If directory handle = 0, then we are assuming that the path
         in oldDirectoryPath is a full path specification.               --*/

    startPtr = oldDirPath;
    volPtr = strchr( oldDirPath, ':' );
    if( volPtr != NULL )
     {
      if( (volPtr - startPtr) > 1 )
         {
         fullPath = TRUE;
         ConvertDots( oldDirPath, tempPath );
         }
      else
         {
         checkLetter = TRUE;
         fullPath = FALSE;
         }
     }
    else
       fullPath = FALSE;

    if( (fullPath == FALSE) && oldHandle )
     {
       if( checkLetter == FALSE )
         {
         ccode = GetOriginalLetter( oldHandle, connectionID, &driveLetter );
         if( ccode )
            return( ccode );
         letterPath[0] = driveLetter;
         strcat( letterPath, ":" );
         strcat( letterPath, oldDirPath );
         }
       else
         strcpy( letterPath, oldDirPath );

       ccode = ConvertNameToFullPath( letterPath, tempPath );
       charPtr = tempPath;
       do
        {
          if( *charPtr == '/' )
             *charPtr = '\\';
          charPtr++;
        }while( *charPtr != '\0' );

       fullPath = TRUE;
     }

    if( fullPath == TRUE )
     {
       charPtr = tempPath;
       do
        {
          if( *charPtr == '/' )
             *charPtr = '\\';
          charPtr++;
        }while( *charPtr != '\0' );

       endOfString = strlen( tempPath );
       endOfString--;
       if( *(tempPath + endOfString) == '\\' )
         *(tempPath + endOfString) = '\0';
       charPtr = strrchr( tempPath, '\\' );
       if( charPtr == NULL )
        {
          volPtr = strchr( tempPath, ':' );
          endPtr = strchr( tempPath, '\0' );
          endPtr--;
          if( *volPtr == *endPtr )
           {
             /*-- If oldDirPath is the root directory, then the newDirPath
                  should be set to \0 and then a new handle should be
                  allocated                                              --*/

             strcpy( newDirPath, '\0' );
           }
          else
           {
             strcpy( newDirPath, volPtr+1 );
             *(volPtr + 1) = 0;
           }
        }
       else
        {
          strcpy( newDirPath, charPtr + 1 );
          *charPtr = 0;
        }

       ccode = AllocTemporaryDirectoryHandle( 0, tempPath, '[',
                                              newHandle, &effectiveRights );

       SetPreferredConnectionID( oldConnectionID );
       return( ccode );
     }
}

WORD GetOriginalLetter( BYTE oldHandle, WORD connID, char *driveLetter )
{
   WORD  compareID;
   BYTE  far *tablePtr;
   BYTE  driveNumber = 0;
   BYTE  tmpDirHandle;
   int   ccode;

   ccode = _TableReq( 0xEF, 0x00, &tablePtr );
   if( !ccode )
      {
      do
         {
         if( *tablePtr == oldHandle )
            {
            /*-- This is done in case there are multiple server with
                 the same directory handles. --*/
            GetDriveInformation( driveNumber, &compareID, &tmpDirHandle );
            if( compareID == connID )
               break;
            }
         tablePtr++;
         driveNumber++;
         } while( driveNumber < 32 );
         if( driveNumber == 32 )
            return( INVALID_DIRECTORY_HANDLE );
         *driveLetter = (char)(driveNumber + 'A');
      }
   return( 0 );
}



void  ConvertDots( char *oldPath, char *finalPath )
{
char   scrap1[255], scrap2[255];
char  *ptr1, *varPtr, *scrapPtr2, charHold;
WORD   valid, dotCount, moreDots;
int    i;

    memset( scrap1, 0, 255 );
    memset( scrap2, 0, 255 );
    strcpy( scrap1, oldPath );
    ptr1 = scrap1;
    dotCount = 0;

    do
     {
        varPtr = strchr( ptr1, '.' );
        if( varPtr == NULL )
           moreDots = FALSE;
        else
           moreDots = TRUE;

        if( moreDots )
         {
            do
             {
                valid = ValidateDot( varPtr );
                if( valid )
                 {
                    dotCount++;
                    varPtr++;
                    ptr1 = varPtr;
                 }
             }while( valid );

            if( ( !valid ) && ( dotCount == 0 ) )
               ptr1 = varPtr + 1;
            else
               if( !valid )
                {
                   charHold = *ptr1;
                   *ptr1 = 0;
                   strcat( scrap2, scrap1 );
                   for( i=dotCount-1; i>0; i-- )
                    {
                       scrapPtr2 = strrchr( scrap2, '\\' );
                       *scrapPtr2 = 0;
                    }
                   *ptr1 = charHold;
                   strcpy( scrap1, ptr1 );
                   ptr1 = scrap1;
                   dotCount = 0;
                }
         } /*  end if moreDots  */
     }while( moreDots );
    if( !moreDots )
     {
        strcat( scrap2, scrap1 );
        strcpy( finalPath, scrap2 );
     }
}


BYTE ValidateDot( char *varPtr )
{

    if( ( ( *(varPtr-1) == '.' )  ||  ( *(varPtr-1) == '\\' ) ) &&
        ( ( *(varPtr+1) == '.' )  ||  ( *(varPtr+1) == '\\' ) )   )
        return( TRUE );
    else
        return( FALSE );
}
