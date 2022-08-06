/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*--------------------------------------------------------
   This function can be used to parse the componentList
   that is returned from BeginDiagnostics.

      Input --    Address of componentList
                  componentID to be searched for
      Output --   Offset into the componentList structure.
                  This is used as the componentNumber in
                  most other Diagnostics functions.

 --------------------------------------------------------*/
#include <diag.h>

BYTE  FindComponentOffset( componentList, componentID )
BYTE  *componentList;
BYTE  componentID;
{
   int   numberOfComponents;
   BYTE  *componentPtr;
   BYTE  componentOffset;
   BYTE  numberOfNetworks, i;

   numberOfComponents = *componentList;
   componentPtr = componentList + 1;
   componentOffset = 0;

   while( numberOfComponents > 0 )
      {
      if( *componentPtr == componentID )
         break;
      if( *componentPtr < 5 )
         componentPtr++;
      else
         {
         componentPtr++;
         numberOfNetworks = *componentPtr;
         componentPtr++;
         for( i=0; i<numberOfNetworks; i++ )
            componentPtr += 11;
         }
      componentOffset++;
      numberOfComponents--;
      }

   return( componentOffset );
}
