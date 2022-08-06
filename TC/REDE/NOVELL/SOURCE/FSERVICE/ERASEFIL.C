/*	(c) COPYRIGHT 1988 - 1990 Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <ctype.h>
#include <nwfile.h>
#include <nwlocal.h>


/*__________________________ EraseFiles ____________________________________
 |
 | Output:			0  --  SUCCESSFUL
 |	       Network Error Code  --  UNSUCCESSFUL
 |
 | Comments:   This function erases specified files from a directory.  Netware
 |	       wild cards are allowed in the file path.
 |__________________________________________________________________________*/

int EraseFiles(directoryHandle, filePath, searchAttributes)
BYTE  directoryHandle;	  /* File path is relative to this directory	      */
char *filePath; 	        /* Path relative to the file server that the drive
			                    is currently mapped to. An absolute path starting
			                    with the volume may be specified. 	         */
BYTE  searchAttributes;   /* Contains the attributes of the files to be
			                    deleted					                        */
{
int  ccode, i;
char sendPacket[258];

    sendPacket[0] = directoryHandle;
    sendPacket[1] = searchAttributes;
    sendPacket[2] = (char)((BYTE)strlen(filePath));
    for( i=0; i<sendPacket[2]; i++ )
      {
      *(filePath + i) = toupper( *(filePath + i) );
      }
    strcpy(sendPacket + 3, filePath);
    ccode = _FileServiceRequest((BYTE)68, (BYTE *)sendPacket,
				(WORD)(3 + sendPacket[2]), (BYTE *)0, (WORD)0);
    return(ccode);
}
