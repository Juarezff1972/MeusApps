/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*___________________ GetBinderyObjectName ________________________________
|
| Output:    0	                 --  SUCCESSFUL
|	          bindery error code --  UNSUCCESSFUL
|
| Comments:
|   This function returns the name and type of a bindery object.
|___________________________________________________________________________*/

int GetBinderyObjectName(objectID, objectName, objectType)

long  objectID;     /* Unique bindery object ID 			                   */
char *objectName;   /* Receives a null terminated string containing the name
                       of the bindery object (max 48 chars INCLUDING null   */
WORD *objectType;   /* Receives the type of the bindery object	      	    */
{
BYTE  sendPacket[7], receivePacket[56];
int   ccode;


    sendPacket[2] = 54;
    *((long *)(sendPacket + 3)) = (DWORD)(LongSwap((DWORD)objectID));
    *((int *)sendPacket) = 5;
    *((int *)receivePacket) = 54;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);

    if (ccode)
       return(ccode);
    if (objectType != (WORD *)0)
	*objectType = IntSwap(*((int *)(receivePacket + 6)));
    if (objectName != (char *)0)
	strcpy(objectName, receivePacket + 8);
    return (ccode);
}
