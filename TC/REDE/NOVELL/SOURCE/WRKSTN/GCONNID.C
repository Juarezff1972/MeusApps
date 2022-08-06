/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nit.h>
#include <niterror.h>

/* Get the network address of the server in question by calling
   ReadPropertyValue with NET_ADDRESS as the property name.
   Then call EF03 to get the connection table, and compare the
   addresses until you find a match (or return not attached)
   Add 1 to connection table index to return a 1 based connectionID
*/

int GetConnectionID (fileServerName, connectionID)
char * fileServerName;
WORD * connectionID;
{
int     ccode;
BYTE    propertyValue[128];
BYTE    moreSegments;
BYTE    propertyFlags;
BYTE far * connectionTableptr;   /* pointer to table in workstation's */
                                 /* memory of connection id 8 x 32 array */
int     i,j,k;
int     found;
char    propName[16];

  strcpy(propName, "NET_ADDRESS");
  ccode = ReadPropertyValue(fileServerName, 4, propName,
                            1, propertyValue, &moreSegments, &propertyFlags);
  if (ccode != 0)
    return (ccode);
  else
    {
      ccode = _TableReq(0xef, 0x03, &connectionTableptr);
      if (ccode != 0)
        return (ccode);
      else
        {                                /* check 8x32 table */
          for (i=0, found = 0; i<8; i++)
            {
              if (*(connectionTableptr + (i * 32)) != 0) /* check inUse flag */
                {
                 j = (i*32) + 2;               /* address is offset 2 */
                 for (k = 0; k<10; k++,j++)
                   {
                     if (*(connectionTableptr+j) != propertyValue[k])
                       break;
                   }
                 if (k > 9)
                   found = 1;
                }
              if (found == 1)
                break;
            }
          if (found == 0)
            return (NOT_ATTACHED_TO_SERVER);
          else
            {
              *connectionID = (WORD)(i + 1);
              return (0);
            }
        }
    }
}
