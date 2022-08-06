/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nit.h>

/*-- This API is the same as the AttachToFileServer API only it requires
     another parameter, the NET_ADDRESS of the file server you wish to 
     attach to.  If you lose all of your attachments to the server you
     can reattach, because it doesn't make any calls to the bindery.  --*/

/*---------------------------------------------------------------------------
   Get the network address of the server in question by calling
   ReadPropertyValue with NET_ADDRESS as the property name.
   Check errors from ReadPropertyValue.
   Then make same calls as GetConnectionID
   to see if we are already attached, and to get the table address.
   If already connected, return ConnectionID and 0xf8.
   If not already connected, if no space in connection table, return 0xfa.
   Else, connect:
        Put 0xff in first byte, order number into 2nd byte of connection id tab.
        Copy new address into connection id table.
        Get the address of the server name table with call EF 04.
        Copy the server name into the indexed place in the
        server name table.
        Attach to server with call F1 00.
        Return connection ID.
 ---------------------------------------------------------------------------*/

int AttachToFileServerWithAddress( fileServerName, connectionID, netAddress )
char *fileServerName;           /* Name of file server to attach to.       */
WORD *connectionID;             /* Receives the connectionID of the file
                                   server after being attached.            */
BYTE *netAddress;               /* Net Address of the file server to be 
                                   attached to                             */
{
int       ccode;
BYTE far *connectionTableptr;   /* pointer to table in workstation's 
                                   memory of connection id 8 x 32 array    */
BYTE      emptySlot;            /* 0-based index of first empty slot into 
                                   connectionTable                         */
int       offset;
BYTE far *serverNameTableptr;
int       i, j, k, found;

   
   /*-- get the table address --*/
   ccode = _TableReq(0xef, 0x03, &connectionTableptr);  
   if( ccode != 0 )
      return( ccode );

   /*-- check 8x32 table --*/
   emptySlot = 0xFF;
   for( i=0, found = 0; i<8; i++ )
      {
      if( *(connectionTableptr + (i * 32)) != 0 )    /* check inUse flag */
         {
         j = (i*32) + 2;                             /* address is offset 2 */
         for (k = 0; k<10; k++,j++)
            {
            if (*(connectionTableptr+j) != *(netAddress+k) )
               break;
            }
         if (k > 9)
            found = 1;
         }
         else                                 /*  not inUse           */
            if( emptySlot == 0xFF )           /*  if first empty slot */
               emptySlot = (BYTE)i;           /*  get index           */
         if( found == 1 )
            break;
      }
   /*-- if already attached to server --*/
   if( found != 0 )
      {
      *connectionID = (WORD)(i + 1);
       return (0xf8);  
      }

   /*-- if there are no more server slots in table --*/
   if( emptySlot > 7 )
      return( 0xfa );          

   /*-- fill in 8 x 32 table --*/
   offset = emptySlot * 32;      
   *(connectionTableptr + offset) = 0xFF;
   *(connectionTableptr + offset + 1) = emptySlot + 1;
   offset = offset + 2;
   for( i=0; i<12; i++ )
      *(connectionTableptr + i + offset) = *(netAddress+i);

   /*-- get address of name table --*/
   ccode = _TableReq( 0xEF, 4, &serverNameTableptr );
   if (ccode == 0)
      {
      offset = emptySlot * 48;           /* fill in the file server name table */
      j = strlen( fileServerName );
      for( i=0; i<j; i++ )
         *(serverNameTableptr + offset + i) = *(fileServerName + i);
      for( ; i < 48; i++ )
         *(serverNameTableptr + offset + i) = '\0';

      /*-- call attach to fileserver --*/
      ccode = _ServerReq( (BYTE)(emptySlot+1), 0xF1, 0 );
      *connectionID = (WORD)emptySlot + 1;
      }

   return( ccode );
}
