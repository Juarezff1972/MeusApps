/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*____________________ _MemberBinderyCall ____________________________________
|
| Output:    0                   --  SUCCESSFUL
|            bindery error code  --  UNSUCCESSFUL
|
| Comments:
|   This routine sends the IPX packet to the file server bindery for the
|   AddMember, DeleteMember and IsMember bindery functions.  The calling
|   routine specifies which bindery function is to be performed AddMember (65),
|   DeleteMember (66), or IsMember (67).
|_____________________________________________________________________________*/

int _MemberBinderyCall( binderyFunctionNumber, objectName, objectType,
                        propertyName, memberName, memberType )

char *objectName;             /* Bindery object name                         */
char *propertyName;           /* Bindery property name                       */
char *memberName;             /* Name of member which will be added, deleted,
                                 or checked for existance.                   */
int   binderyFunctionNumber;  /* IPX function number of the bindery call to
                                 to be made                                  */
WORD  objectType;             /* Bindery object type (OT_USERS, OT_GROUP,...)*/
WORD  memberType;             /* Type of the member to be worked on (i.e.
                                 OT_USERS, OT_GROUP, OT_PRINT_SERVER)        */
{
    BYTE sendPacket[119], receivePacket[2];
    int ccode, objLength, propLength, memLength, offset;

    sendPacket[2] = binderyFunctionNumber;
    *((int *)(sendPacket + 3)) = IntSwap(objectType);

    objLength =  strlen(objectName);
    propLength = strlen(propertyName);
    memLength =  strlen(memberName);

    sendPacket[5] = objLength;
    memmove(sendPacket + 6, objectName, objLength);
    offset = objLength + 6;

    sendPacket[offset++] = propLength;
    memmove(sendPacket + offset, propertyName, propLength);
    offset =  objLength + propLength + 7;

    *((int *)(sendPacket + offset)) = IntSwap(memberType);
    offset += 2;
    sendPacket[offset++] = memLength;
    memmove(sendPacket + offset, memberName, memLength);

    *((int *)sendPacket) = objLength + propLength + memLength + 8;
    *((int *)receivePacket) = 0;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    return(ccode);
}
