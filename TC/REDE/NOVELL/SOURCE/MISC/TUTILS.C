#include <stdio.h>
#include <string.h>
#include <nit.h>
#include <ntt.h>

/*--  Prototypes for functions  --*/
void PrintTrusteeRights( BYTE trusteeRights );
void PrintDate( WORD theDate );
void PrintTime( WORD theTime );
void PrintDateAndTime( long DateAndTime );
void DisplayFileEntry( NWFILE_ENTRY *fileEntry );
void DisplayDirEntry( NWDIR_ENTRY *dirEntry );
void DisplayDirInfo( NWDIR_INFO *dirInfo );
void DisplayScanSalvEntry( NWSALV_ENTRY *salvEntry );
void DisplayPhysFileEntry( NWPHYS_ENTRY *fileEntry );
void PrintTrusteeRightsV2( BYTE trusteeRights );
void PrintTrusteeRightsV3( LONG trusteeRights );


void PrintTrusteeRights( trusteeRights )
BYTE    trusteeRights;

/* Description:  This function simply prints rights descriptor string similar
                 to NOVELL RIGHTS utility for a trustee mask.  */

{
   printf("[%c%c%c%c%c%c%c%c]\n",
         (trusteeRights & TR_SUPERVISOR) ? 'S' : ' ',
         (trusteeRights & TR_READ)       ? 'R' : ' ',
         (trusteeRights & TR_WRITE)      ? 'W' : ' ',
         (trusteeRights & TR_CREATE)     ? 'C' : ' ',
         (trusteeRights & TR_ERASE)      ? 'E' : ' ',
         (trusteeRights & TR_MODIFY)     ? 'M' : ' ',
         (trusteeRights & TR_FILE)       ? 'F' : ' ',
         (trusteeRights & TR_ACCESS)     ? 'A' : ' '
   );

} 

/********************************************************************/

void PrintDate( theDate )
WORD    theDate;                /* DOS format BYTE array */
{
#define  SYEAR      1980
#define  MMASK    0x000F
#define  DMASK    0x001F

   int  year;
   int  month;
   int  day;

   /*-- Compute date and time according to dos format --*/

   year  = (theDate >> 9) + SYEAR;
   month = ((theDate >> 5) & MMASK);
   day   = (theDate & DMASK);

   printf("%d/%d/%d",month, day, year);
}

/********************************************************************/

void PrintTime( theTime )
WORD    theTime;                /* DOS format BYTE array*/

{
#define  NOON                   12
#define  MINMASK                0x003F
#define  SECMASK                0x001F

   int  hour;
   int  minute;
   int  sec;

   /*-- compute date and time according to dos format --*/

   hour   = (theTime >> 11);
   minute = ((theTime >> 5) & MINMASK);
   sec    = ((theTime & SECMASK) * 2);

   /*...print results and pm or am...*/

   printf("%d:%02d.%02d ",(hour > NOON) ? hour - NOON : hour, minute, sec);
   if( hour >= NOON )
     printf("P.M.");
   else
     printf("A.M.");
}



/********************************************************************/

void PrintDateAndTime( DateAndTime )
long    DateAndTime;           /* DOS format BYTE array, date then time */
{
   WORD    theDate;
   WORD    theTime;


   *(((BYTE *)&theDate)+1) = *(((BYTE *)&DateAndTime)+3);
   *((BYTE *)&theDate)     = *(((BYTE *)&DateAndTime)+2);

   *(((BYTE *)&theTime)+1) = *(((BYTE *)&DateAndTime)+1);
   *((BYTE *)&theTime)     = *((BYTE *)&DateAndTime);

   PrintDate( theDate );
   printf("   ");
   PrintTime( theTime );
} 


/********************************************************************/

void DisplayFileEntry( fileEntry )
NWFILE_ENTRY    *fileEntry;
{
   char  name[13];


   printf("attributes             : %08lX\n", fileEntry->attributes );

   printf("creationDate           : ");
   PrintDate( fileEntry->creationDate );
   printf("\n");

   printf("creationTime           : ");
   PrintTime( fileEntry->creationTime );
   printf("\n");

   printf("ownerID                : %08lX\n", fileEntry->ownerID );

   printf("lastArchiveDate        : ");
   PrintDate( fileEntry->lastArchivedDate );
   printf("\n");

   printf("lastArchiveTime        : ");
   PrintTime( fileEntry->lastArchivedTime );
   printf("\n");

   printf("lastArchiverID         : %08lX\n", fileEntry->lastArchiverID );

   printf("modifyDate             : ");
   PrintDate( fileEntry->modifyDate );
   printf("\n");

   printf("modifyTime             : ");
   PrintTime( fileEntry->modifyTime );
   printf("\n");

   printf("lastModifierID         : %08lX\n", fileEntry->lastModifierID );

   printf("dataSize               : %08lX\n", fileEntry->dataForkSize );

   printf("inheritedRightsMask    : ");
   PrintTrusteeRights( (BYTE)fileEntry->inheritedRightsMask );

   printf("lastAccessDate         : ");
   PrintDate( fileEntry->lastAccessDate );
   printf("\n");

   printf("NSType                 : %d\n", fileEntry->NSType );

   printf("nameLength             : %d\n", fileEntry->nameLength );

   memmove( name, fileEntry->name, fileEntry->nameLength );
   name[fileEntry->nameLength] = 0;
   printf("name                   : %s ", name );
   printf("\n\n");
}

/********************************************************************/

void DisplayDirEntry(dirEntry)
NWDIR_ENTRY     *dirEntry;
{
   char   name[13];


   printf("attributes             : %08lX\n", dirEntry->attributes );

   printf("NSType                 : %d\n", dirEntry->NSType );

   printf("nameLength             : %d\n", dirEntry->nameLength );

   memmove( name, dirEntry->name, dirEntry->nameLength );
   name[dirEntry->nameLength] = 0;
   printf("name                   : %s ", name );
   printf("\n");

   printf("creationDate           : ");
   PrintDate( dirEntry->creationDate );
   printf("\n");
   printf("creationTime           : ");
   PrintTime( dirEntry->creationTime );
   printf("\n");

   printf("ownerID                : %08lX\n", dirEntry->ownerID );

   printf("lastArchivedDate       : ");
   PrintDate( dirEntry->lastArchivedDate );
   printf("\n");

   printf("lastArchivedTime       : ");
   PrintTime( dirEntry->lastArchivedTime );
   printf("\n");

   printf("lastArchiverID         : %08lX\n", dirEntry->lastArchiverID );

   printf("lastModifiedDate       : ");
   PrintDate( dirEntry->modifyDate );
   printf("\n");

   printf("lastModifiedTime       : ");
   PrintTime( dirEntry->modifyTime );
   printf("\n");

   printf("inheritedRightsMask    : ");
   PrintTrusteeRights( (BYTE)dirEntry->inheritedRightsMask );

   printf("\n\n");
}

/********************************************************************/

void DisplayDirInfo( dirInfo )
NWDIR_INFO *dirInfo;
{
   printf("Total blocks           : %lu\n", dirInfo->totalBlocks );
   printf("Free blocks            : %lu\n", dirInfo->freeBlocks );
   printf("Total Dir Entries      : %lu\n", dirInfo->totalDirEntries );
   printf("Available Dir Entries  : %lu\n", dirInfo->availDirEntries );
   printf("Sectors per block      : %d\n",  dirInfo->sectorsPerBlock );
   printf("Volume Name            : %s\n",  dirInfo->volumeName );
}



/********************************************************************/

void DisplayScanSalvEntry( salvEntry )
NWSALV_ENTRY *salvEntry;
{
   char   name[13];

   printf("attributes             : %08lX\n", salvEntry->attributes );
   printf("Name Space type        : %d\n", salvEntry->nameSpaceType );
   printf("Name length            : %d\n", salvEntry->nameLength );
   memmove( name, salvEntry->name, salvEntry->nameLength );
   name[salvEntry->nameLength] = 0;
   printf("name                   : %s ", name );
   printf("\n");

   printf("lastArchiveDateAndTime: ");
   PrintDateAndTime( salvEntry->lastArchiveDateAndTime );

   printf("lastArchiverID         : %08lX\n", salvEntry->lastArchiverID );
   printf("lastModifiedDateAndTime: ");
   PrintDateAndTime( salvEntry->modifyDateAndTime );
   printf("lastModifierID         : %08lX\n", salvEntry->lastModifierID );
   printf("dataForkSize           : %ld\n", salvEntry->dataForkSize );
   printf("lastAccessDate         : ");
   PrintDate( salvEntry->lastAccessedDate );
   printf("DeletedFileTime        : ");
   PrintTime( (WORD)salvEntry->deletedFileTime );
   printf("DeletedDateAndTime     : ");
   PrintDateAndTime( salvEntry->deletedDateAndTime );
   printf("DeletorID             : salvEntry->deletorID" );
}

/*************************************************************************/

void DisplayPhysFileEntry(fileEntry)
NWPHYS_ENTRY    *fileEntry;
{
   char  name[13];


   printf("attributes             : %08lX\n", fileEntry->attributes );

   printf("creationDate           : ");
   PrintDate( fileEntry->creationDate );
   printf("\n");

   printf("creationTime           : ");
   PrintTime( fileEntry->creationTime );
   printf("\n");

   printf("ownerID                : %08lX\n", fileEntry->ownerID );

   printf("lastArchiveDate        : ");
   PrintDate( fileEntry->lastArchivedDate );
   printf("\n");

   printf("lastArchiveTime        : ");
   PrintTime( fileEntry->lastArchivedTime );
   printf("\n");

   printf("lastArchiverID         : %08lX\n", fileEntry->lastArchiverID );

   printf("modifyDate             : ");
   PrintDate( fileEntry->modifyDate );
   printf("\n");

   printf("modifyTime             : ");
   PrintTime( fileEntry->modifyTime );
   printf("\n");

   printf("lastModifierID         : %08lX\n", fileEntry->lastModifierID );

   printf("inheritedRightsMask    : ");
   PrintTrusteeRights( (BYTE)fileEntry->inheritedRightsMask);

   printf("lastAccessDate         : ");
   PrintDate( fileEntry->lastAccessDate );
   printf("\n");

   printf("NSType                 : %d\n", fileEntry->NSType );

   printf("nameLength             : %d\n", fileEntry->nameLength );

   memmove( name, fileEntry->name, fileEntry->nameLength );
   name[fileEntry->nameLength] = 0;
   printf("name                   : %s ", name );
   printf("\n\n");
}

/********************************************************************/

void PrintTrusteeRightsV2( trusteeRights )
BYTE   trusteeRights;
/*-- 
      This function simply prints rights descriptor string similar
      to NOVELL RIGHTS utility for a trustee mask in v2.x.  --*/
   
{ 
  
   printf( "rights : [%c%c%c%c%c%c%c%c]\n",
               (trusteeRights & TA_READ)        ? 'R' : ' ',
               (trusteeRights & TA_WRITE)       ? 'W' : ' ',
               (trusteeRights & TA_OPEN)        ? 'O' : ' ',
               (trusteeRights & TA_CREATE)      ? 'C' : ' ',
               (trusteeRights & TA_DELETE)      ? 'D' : ' ',
               (trusteeRights & TA_OWNERSHIP)   ? 'P' : ' ',
               (trusteeRights & TA_SEARCH)      ? 'S' : ' ',
               (trusteeRights & TA_MODIFY)      ? 'M' : ' '
         );
   
} 

/********************************************************************/

void PrintTrusteeRightsV3( trusteeRights )
LONG   trusteeRights;
/*--
      This function simply prints rights descriptor string similar
      to NOVELL RIGHTS utility for a trustee mask in v3.x. --*/
   
{
   printf("rights : [%c%c%c%c%c%c%c%c]\n",
               (trusteeRights & TR_SUPERVISOR)   ? 'S' : ' ',
               (trusteeRights & TR_READ)         ? 'R' : ' ',
               (trusteeRights & TR_WRITE)        ? 'W' : ' ',
               (trusteeRights & TR_CREATE)       ? 'C' : ' ',
               (trusteeRights & TR_ERASE)        ? 'E' : ' ',
               (trusteeRights & TR_MODIFY)       ? 'M' : ' ',
               (trusteeRights & TR_FILE)         ? 'F' : ' ',
               (trusteeRights & TR_ACCESS)       ? 'A' : ' '
   );
} 



