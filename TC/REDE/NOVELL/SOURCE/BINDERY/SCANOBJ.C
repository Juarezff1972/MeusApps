/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwbindry.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*_____________________ ScanBinderyObject ____________________________________
|
| Output:   0                        --  SUCCESSFUL
|           bindery error code       --  UNSUCCESSFUL
|
| Comments:
|   This routine scans the bindery for any objects with names that match
|   the search pattern and types that match the search type.  The search
|   pattern and the search type may both contain wild cards.  On the first
|   call the object ID should be set to -1L.  On subsequent calls the
|   object ID should be the object ID number that was returned from the
|   previous call.  The next object that matches is returned.  The
|   objectHasProperties flag will be set if the object has properties
|   associated with it.  The dynamic flag will be set if the object is
|   a temporary object, or clear if the object is static.  The access
|   flags will be set with the read and write security associated with
|   the object.
|___________________________________________________________________________*/

int ScanBinderyObject( searchObjectName, searchObjectType, objectID,
                       objectName, objectType, objectHasProperties,
                       objectFlag, objectSecurity )

char *searchObjectName;     /* Name of the bindery object to be search for  */
WORD  searchObjectType;     /* Type of the bindery object to search for     */
long *objectID;                   /* Contains the objectID from the previous search
                                              and receives the unique bindery object ID for
                                              the matching object                                          */
char *objectName;                 /* Receives a null terminated string containing
                                              the name of the matching bindery object       */
WORD *objectType;                 /* Receives the type of the matching bindery
                                              object (i.e. OT_USER, OT_GROUP)                     */
char *objectHasProperties;  /* Receives a flag indicating if the bindery
                                              object has properties to scan                       */
char *objectFlag;                 /* Receives a flag indicating if the matching
                                              bindery object is dynamic or static              */
char *objectSecurity;       /* Receives a flag indicating the READ/WRITE access
                                              of others to the matching bindery object     */
{
    char sendPacket[57], receivePacket[59];
    int ccode, searchlen;

    sendPacket[2] = 55;
    *((long *)(sendPacket + 3)) = LongSwap(*objectID);
    *((WORD *)(sendPacket + 7)) = IntSwap(searchObjectType);
    searchlen = strlen(searchObjectName);
    sendPacket[9] = searchlen;
    memmove(sendPacket + 10, searchObjectName, searchlen);

    *((int *)sendPacket) = searchlen + 8;
    *((int *)receivePacket) = 57;

    ccode = _ShellRequest((BYTE)227, (BYTE *)sendPacket, (BYTE *)receivePacket);
    if (ccode)
       return(ccode);

    if (objectName != (char *)0)
        strcpy(objectName, receivePacket + 8);
    *objectID = LongSwap(*((long *)(receivePacket + 2)));
    if (objectType != (WORD *)0)
        *objectType = IntSwap(*((WORD *)(receivePacket + 6)));
    if (objectHasProperties != (char *)0)
        *objectHasProperties = receivePacket[58];
    if (objectFlag != (char *)0)
        *objectFlag = receivePacket[56];
    if (objectSecurity != (char *)0)
        *objectSecurity = receivePacket[57];

    return (ccode);
}
