/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */

/*__________________________________________________________________________
  |     comments        : This routine will map a drive given an operation
  |                       string, a drive string, and a path string.
  |
  |_______________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ndt.h>
#include <nwconsol.h>
#include <nwdir.h>
#include <nwlocal.h>
#include <nwmisc.h>
#include <nwwrkenv.h>

#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))
#define BACK_SLASH 0x5C

#ifndef ANSI
int MapDriveUsingString( mapType, drive, path )
char   *mapType;
char   *drive;
char   *path;
#else
int MapDriveUsingString( char   *mapType,
                         char   *drive,
                         char   *path )
#endif

{
int     ccode;
int     count;
WORD    currConnectionID;
char    currFileServerName[48];
char   *temp;
char   *temp2;
char    driveLetter;
char    fullPath[512];
char    parsedServer[48];
BYTE    searchFlag;        /*-- indicates add, insert, or delete            --*/
int     searchOrder;       /*-- search drive number (0 = not a search drive) -*/



   /*-- determine map operation --*/
    if ( mapType == (char *)NULL )
     {
        searchFlag = DRIVE_ADD;
     }
    else
     {
        if ( *mapType == '\0' )
            searchFlag = DRIVE_ADD;
        else if ( !strcmp( mapType, "ADD" ) )
            searchFlag = DRIVE_ADD;
        else if ( !strcmp( mapType, "INS" ) )
            searchFlag = DRIVE_INSERT;
        else if ( !strcmp( mapType, "DEL" ) )
            searchFlag = DRIVE_DELETE;
        else
         {
            ccode = 0xb3;               /* INVALID_MAP_TYPE */
            goto ERROR0;
         }
     }

    /*-- First find out if we have only a volume in the path --*/
    if ( (temp = strchr( path, ':' )) != '\0' )
    /*-- if there's a colon in the path --*/
     {
       if ( (temp - path) != 1)            /* if not a drive specifier       */
        {                                  /* go get the default file server */
           temp2 = temp;
          *temp2 = '\0';                   /* null out the colon             */
          if ( (temp2 = strchr(path,BACK_SLASH)) == '\0')
           {
              if ( (temp2 = strchr(path,'/')) == '\0' )  /* specified already */
               {
                   *temp = ':';                          /* restore the colon */
                   /*-- get default server --*/
                   GetFileServerName( (WORD)0,fullPath );
                    /*-- insert in path --*/
                   sprintf( fullPath,"%s%c%s",fullPath,BACK_SLASH,path );
                   strcpy( path,fullPath );

                }
               else
                   *temp = ':';
           }
          else
               *temp = ':';
        }
     }
   /*-- determine drive letter and --*/
   /*-- search order               --*/

    if ( drive[ 0 ] == '*' )
        driveLetter = (char)(GetNumberOfLocalDrives() + (int)'@' +
                      atoi(drive + 1));
    else
     {
        driveLetter = upcase( drive[ 0 ] );
        if ( ( drive[ 0 ] < 'A' ) || ( drive[ 0 ] > 'Z' ) )
         {
            ccode = 0xb4;                       /* INVALID_DRIVE_LETTER */
            goto ERROR0;
         }
     }
    searchOrder = 0;
    if ( *drive != '*' )
     {
        if ( driveLetter == 'S' )
           for ( count = 1;
                ( drive[ count ] >= '0' ) && ( drive[ count ] <= '9' );
                  count++ )
                  searchOrder = searchOrder * 10 - '0' + drive[ count ];
        else
            count = 1;
        if ( drive[ count ] != '\0' )
         {
            ccode = 0xb4;           /* INVALID_DRIVE_LETTER */
            goto ERROR0;
         }
        if ( count != 1 )
            driveLetter = '\0';     /* use next available */
     }

    GetDriveInformation( (BYTE)(driveLetter - 'A'),
                         &currConnectionID, (BYTE *)NULL );

    /*--  transform path into full path --*/
    if (searchFlag != DRIVE_DELETE)
     {
        if ( (ccode = RelativeToFullPath(path, fullPath)) != 0 )
         {
             goto ERROR0;
         }
        if ( strlen(fullPath) > 256 )
         {
            ccode = 0x9c;                       /* INVALID_PATH */
            goto ERROR0;
         }

                /*===================
                    check for login
                  ===================*/
        *parsedServer = '\0';
        StripFileServerFromPath( fullPath, parsedServer );
        if ( *parsedServer != '\0' )
         {
            for ( currConnectionID = 1; currConnectionID <= 8;
                  currConnectionID++ )
             {
                GetFileServerName( currConnectionID, currFileServerName );
                if ( strcmp( currFileServerName, parsedServer ) == 0 )
                     break;
             }
            if ( currConnectionID > 8 )
             {
                    ccode = 0xfc;           /* UNKNOWN_FILE_SERVER */
                    goto ERROR0;
             }
         }
     }

    /*-- map the drive --*/
    ccode = MapDrive( currConnectionID, (BYTE)0, fullPath,
                       searchFlag, searchOrder, &driveLetter );

    /*--  exit --*/
ERROR0:
    return( ccode );
}
