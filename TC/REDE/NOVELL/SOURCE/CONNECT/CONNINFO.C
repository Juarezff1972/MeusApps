/*      COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwconn.h>
#include <nwlocal.h>
#include <nwmisc.h>


/*__________________ GetConnectionInformation ________________________________
|
| Output:   Returns a 0 if successful or a network error code if unsuccessful.
|
| Comments:
|   This function returns information about the object logged into a specified
|   connection.  The object name, type, bindery ID number, and time the object
|   object logged in are returned.  The loginTime is organized as follows:
|   byte 0 -- Year (0-99 where 80-99 are 1980-1999 and 0-79 are 2000-2079),
|   byte 1 -- Month (1-12),  byte 2 -- Day (1-31),    byte 3 -- Hour (0-23),
|   byte 4 -- Minute (0-59), byte 5 -- Second (0-59), byte 6 -- Day of Week
|   (0-6 where 0 is Sunday and 6 is Saturday).
|____________________________________________________________________________*/

int GetConnectionInformation(connectionNumber, objectName, objectType,
                             objectID, loginTime)
WORD connectionNumber; /* Connection number for which information is returned*/
char *objectName;      /* Receives a null terminated string containing the
                          name of the bindery object that is logged in at the
                          connection number                                  */
WORD *objectType;      /* Receives the type of the bindery object that is
                          logged in at the connection number                 */
long *objectID;        /* Receives the unique ID of the bindery object that
                          is logged in at the connection number              */
BYTE *loginTime;       /* Receives the date and time that the bindery object
                          logged into the connection number                  */
{
    BYTE sendPacket[4], receivePacket[64];
    int ccode;

    sendPacket[2] = 22;
    sendPacket[3] = (BYTE)connectionNumber;
    *((int *)sendPacket) = 2;

    *((int *)receivePacket) = 62;

    ccode = _ShellRequest((BYTE)227, sendPacket, receivePacket);
    if (ccode)
        return(ccode);

    if (objectID != (long *)NULL)
        *objectID = LongSwap(*((long *)(receivePacket + 2)));
    if (objectType != (WORD *)NULL)
        *objectType = IntSwap(*((int *)(receivePacket + 6)));
    if (objectName != (char *)NULL)
        strcpy(objectName, receivePacket + 8);
    if (loginTime != (BYTE *)NULL)
        memmove(loginTime, receivePacket + 56, (WORD)7);

    return(0);
}
