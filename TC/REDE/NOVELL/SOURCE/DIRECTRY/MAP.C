/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nit.h>
#include <niterror.h>
#ifndef LATTICE
 #include <conio.h>
#endif

#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))

extern int  GetCompletePath( WORD, BYTE, char *, char * );
extern int  MapNonSearchDrive( char *, char *, BYTE );
extern int  MapSearchDrive( char *, char *, BYTE, int, char * );

extern char pathVariableName[];

static void   AddDriveToVector( char, int, char *, char *, int );
static void   DeleteDriveFromVector( int, char * );

static void   DeleteDriveFromVector(searchOrder, pathVariable)
int     searchOrder;
char   *pathVariable;
{
   char   *separator;
   char   *variablePtr;


   if( searchOrder == 0 )
      goto DONE;
   variablePtr = _FindPathVariableElement( pathVariable, searchOrder );
   if( variablePtr != (char *)NULL )
      {
      separator = strchr( variablePtr, ';' );
      if( separator == (char *)NULL )
         {
         if ( variablePtr == pathVariable )
            *variablePtr = '\0';
         else
            *(variablePtr - 1) = '\0';
         }
      else
         {
         strcpy( variablePtr, separator + 1 );
         }
      }
DONE:
   AllignDriveVectorToPath( pathVariable );
}



static void AddDriveToVector( driveLetter, searchOrder, pathVariable,
                              additionalPath, insertReplace )
char    driveLetter;
int     searchOrder;
char   *pathVariable;
char   *additionalPath;
int     insertReplace;
{
   int     len;
   char   *tempPtr;
   char   *tempPtr2;
   char   *variablePtr;
   char   *variablePtr2;
   int     buffer;


   /*-- get path variable for modification --*/
   variablePtr = _FindPathVariableElement( pathVariable, searchOrder );
   variablePtr2 = _FindPathVariableElement( pathVariable, searchOrder + 1 );

   /*-- add in path --*/
   if( variablePtr != (char *)NULL )
      {
      /*-- create buffer to store path under construction --*/
      len = 1;
      if ( (insertReplace != DRIVE_INSERT) && ( variablePtr2 != 0 ) )
         {
         buffer = (int)( variablePtr2 - variablePtr );
         len += strlen( pathVariable ) + 3 - buffer;
         }
      else
         len += strlen( pathVariable ) + 3;

      tempPtr = (char *) calloc( 1, len );

      /*-- copy text before insert point --*/
      len = (int)(variablePtr - pathVariable);
      memmove( tempPtr, pathVariable, len );
      tempPtr2 = tempPtr + len;
      /*-- if insert point is at end, add a separator --*/
      if( *variablePtr == '\0' )
         {
         if ( *(tempPtr2 - 1) != ';' )
            *(tempPtr2++) = ';';
         }

      /*-- add text to insert --*/
      *(tempPtr2++) = upcase( driveLetter );
      *(tempPtr2++) = ':';
      strcpy( tempPtr2, "." );

      if( *variablePtr != '\0' )
         {
         strcat( tempPtr2, ";" );
         if ( insertReplace == DRIVE_INSERT )
            strcat( tempPtr2, variablePtr );
         else
            strcat( tempPtr2, variablePtr2 );
         }
      strcpy( pathVariable, tempPtr );
      free( tempPtr );
      }
   else
      {
      tempPtr = strchr(pathVariable,'\0');
      if( (tempPtr != pathVariable) && ( *(tempPtr-1) != ';') )
         {
         *tempPtr = ';';
         tempPtr++;
         }
      *tempPtr = driveLetter;
      tempPtr++;
      *tempPtr = ':';
      tempPtr++;
      strcpy(tempPtr, ".");
      }

   /*-- update search drive vector --*/
   AllignDriveVectorToPath( pathVariable );
}





int MapDrive( connectionID, baseDriveNumber, directoryPath, searchFlag,
              searchOrder, driveLetter )

WORD    connectionID;
BYTE    baseDriveNumber;
char   *directoryPath;
BYTE    searchFlag;
int     searchOrder;
char   *driveLetter;
{
   int         ccode               = 0;    /*-- default is no error --*/
   BYTE        driveStatus;
   char        lastLocalDriveLetter;
   WORD        oldConnectionID;
   WORD        pathConnectionID;
   BYTE        pathDirectoryHandle;
   char       *pathVariable;
   int         nextAvailableSearchOrder;
   char        fullPath[255];
   BYTE        searchHandle;
   WORD        tempID;
   int         placeMarker;


   if( (searchFlag != DRIVE_INSERT) && (searchFlag != DRIVE_DELETE) )
      searchFlag = DRIVE_ADD;

   ccode = GetPathEnvironmentVariable( &pathVariable );
   if( ccode )
      return( ccode );
   lastLocalDriveLetter = (char)( GetNumberOfLocalDrives() + 'A' - 1 );
   ccode = IsConnectionIDInUse( connectionID );
   if( !ccode )
      connectionID = GetDefaultConnectionID();

   oldConnectionID = GetPreferredConnectionID();
   SetPreferredConnectionID( connectionID );

   if( ( *driveLetter == '\0' ) && ( searchFlag != DRIVE_DELETE ) )
      {
      for ( *driveLetter = 'Z';
            *driveLetter > lastLocalDriveLetter;
            (*driveLetter)--  )
         {
         driveStatus = GetDriveInformation( (BYTE)(*driveLetter - 'A'),
                                            &pathConnectionID,
                                            &pathDirectoryHandle );
         if( driveStatus == 0x00 )
            break;
         }
      if( *driveLetter <= lastLocalDriveLetter )
         return( NO_DRIVE_AVAILABLE );
      }

   GetCompletePath( connectionID, baseDriveNumber, directoryPath, fullPath );

   placeMarker = GetSearchOrderFromDriveLetter( pathVariable, *driveLetter );
   if( placeMarker && searchFlag != DRIVE_DELETE )
      {
      driveStatus = GetDriveInformation( (BYTE)(*driveLetter - 'A'),
                                       &tempID, &searchHandle );
      if ( driveStatus != 0x80 )
         {
         DeleteDriveFromVector( placeMarker, pathVariable );
         }
      }

   if( searchOrder == 0 )
      {
      ccode = MapNonSearchDrive( fullPath, driveLetter, searchFlag );
      if( ccode )
         return( ccode );
      }
   else
      {
      nextAvailableSearchOrder = GetMaxSearchOrder( pathVariable ) + 1;
      if( searchOrder > nextAvailableSearchOrder )
         searchOrder = nextAvailableSearchOrder;
                                                                                                /* than the last assigned # */
      if( searchOrder > (nextAvailableSearchOrder - 1) )
         {                                                                               /* if S# is > than last assigned # */
         if( searchFlag == DRIVE_INSERT )
            searchFlag = DRIVE_ADD;
         if( searchFlag == DRIVE_DELETE )
            return( NO_SUCH_SEARCH_DRIVE );
         }
      ccode = MapSearchDrive( fullPath, driveLetter, searchFlag,
                              searchOrder, pathVariable );
      if( ccode )
         return( ccode );
      }

   /*-- UPDATE THE DOS PATH VARIABLE --*/
   PutEnvironmentVariable( pathVariableName, pathVariable );

   SetPreferredConnectionID( oldConnectionID );
   return( ccode );
}



int  GetCompletePath( connectionID, baseDriveNumber, partialPath, fullPath )
WORD   connectionID;
BYTE   baseDriveNumber;
char  *partialPath;
char  *fullPath;
{
   char      serverName[48];
   char      tmpPath[255];
   int       ccode;
   char     *ptr;


   if ( (ptr = strchr( partialPath, ':' )) == 0 )
      {
      ccode = GetFullPath( baseDriveNumber, fullPath );
      if( ccode )
         return( ccode );
      if( partialPath[1] != ':' )
         {
         if ( partialPath[0] != '\\' )
            strcat( fullPath, "\\" );
         strcat( fullPath, partialPath );
         }
      }
   else
      {
      memset( tmpPath, 0, 255 );
      ccode = ConvertNameToFullPath( partialPath, tmpPath );
      if( ccode )
         return( ccode );
      else
         {
         GetFileServerName( connectionID, serverName );
         strcpy( fullPath, serverName );
         strcat( fullPath, "/" );
         strcat( fullPath, tmpPath );
         }
      }
   return( ccode );
}





int MapNonSearchDrive( fullPath, driveLetter, searchFlag )
char  *fullPath;
char  *driveLetter;
BYTE   searchFlag;
{
   WORD   connectionID;
   BYTE   pathDirectoryHandle;
   BYTE   driveStatus;
   int    ccode = 0;


   if ( searchFlag == DRIVE_DELETE )
      {
      driveStatus = GetDriveInformation( (BYTE)(*driveLetter - 'A'),
                                          &connectionID,
                                          &pathDirectoryHandle );
      if ( ( driveStatus == 0x80 ) || ( driveStatus == 0x00 ) )
         return( DRIVE_IS_NOT_MAPPED );
      ccode = DeallocateDirectoryHandle( pathDirectoryHandle );
      return( ccode );
      }
   else
      {
      ccode = SetDrivePath( (BYTE)(*driveLetter - 'A'),
                             NO_BASE_DRIVE, fullPath, (BYTE)TRUE );
      if( ccode )
         return( ccode );
      }
      return( ccode );
}




int MapSearchDrive( fullPath, driveLetter, searchFlag, searchOrder,
                    pathVariable )
char  *fullPath;
char  *driveLetter;
BYTE   searchFlag;
int    searchOrder;
char  *pathVariable;
{
   WORD   connectionID;
   BYTE   pathDirectoryHandle;
   BYTE   driveStatus;
   int    ccode = 0;
   char  *directoryPath;


   if( searchFlag == DRIVE_DELETE )
      {
      *driveLetter = GetDriveLetterFromSearchOrder( pathVariable, searchOrder );
      driveStatus = GetDriveInformation( (BYTE)(*driveLetter - 'A'),
                                          &connectionID,
                                          &pathDirectoryHandle );
      if( ( driveStatus == 0x80 ) || ( driveStatus == 0x00 ) )
         return( DRIVE_IS_NOT_MAPPED );
      if( driveStatus != 0x80 )
         {
         ccode = DeallocateDirectoryHandle( pathDirectoryHandle );
         if( ccode )
            return( ccode );
         }
      DeleteDriveFromVector( searchOrder, pathVariable );
      }

   if( ( searchFlag == DRIVE_INSERT ) || ( searchFlag == DRIVE_ADD ) )
      {
      ccode = SetDrivePath( (BYTE)(*driveLetter - 'A'), NO_BASE_DRIVE,
                             fullPath, (BYTE)TRUE );
      if( ccode )
         return( ccode );
      directoryPath = strchr( fullPath, ':' );
      directoryPath++;
      AddDriveToVector( *driveLetter, searchOrder,
                           pathVariable, directoryPath, searchFlag );
      }
   return( ccode );
}
