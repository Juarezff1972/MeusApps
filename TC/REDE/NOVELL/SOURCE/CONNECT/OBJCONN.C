/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconn.h>
#include <nwlocal.h>
#include <nwmisc.h>

#define  MIN( a, b )    ( (a) < (b) ) ? (a) : (b)
/*___________________ GetObjectConnectionNumbers __________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This routine returns a list of all connections where an object is logged
|   in.  Connections are returned in a character array, with one connection
|   number per byte.  The number of connection numbers to be returned are
|   put into the variable numberOfConnections.
|____________________________________________________________________________*/

int GetObjectConnectionNumbers(objectName, objectType, numberOfConnections,
                               connectionList, maxConnections)

char *objectName;            /* Name of the bindery object for which
                                connection numbers are returned (maximum of
                                48 characters including the null terminator) */
WORD  objectType;            /* Type of the bindery object for which connection
                                numbers are returned                         */
WORD *numberOfConnections;   /* Receives the number of connection numbers that
                                the bindery object is logged into (0-100)    */
WORD *connectionList;        /* Receives a list of connection numbers of where
                                the bindery object is logged in (max 100 BYTES)*/
WORD  maxConnections;        /* Contains the maximum number of connection
                                numbers to return in connectionList           */
{
   BYTE  sendPacket[54], receivePacket[103];
   int   ccode, offset;

   memset( sendPacket, 0, 54 );
   memset( receivePacket, 0, 103 );
   sendPacket[2] = 21;
   *((WORD *)(sendPacket + 3)) = IntSwap( objectType );
   sendPacket[5] = (BYTE)( MIN( strlen(objectName), 47 ) );
   memmove( sendPacket + 6, objectName, sendPacket[5] );
   *((WORD *)sendPacket) = 4 + sendPacket[5];
   *((WORD *)receivePacket) = 101;

   ccode = _ShellRequest( (BYTE)227, sendPacket, receivePacket );
   if( ccode )
      return( ccode );

   if( (WORD)receivePacket[2] > maxConnections )
      *numberOfConnections = maxConnections;
   else
      *numberOfConnections = (WORD)receivePacket[2];

   if( *numberOfConnections )
      for( offset = -1; ++offset < *numberOfConnections;
           connectionList[offset] = (WORD)(receivePacket[3+offset]) );

   *numberOfConnections = (WORD)(receivePacket[2]);

   return( ccode );
}
