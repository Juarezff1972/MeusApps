/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>



/*__________________ GetWorkstationEnvironment _______________________________
 |
 |  Output:   None.
 |
 |  Comments:
 |    This routine will return 4 null terminated strings which describe the
 |    hardware and operating system type and version that the shell is running
 |    on.  The parameters are optional and a NIL may be passed for the buffer
 |    location if the information in that parameter is not needed.
 |__________________________________________________________________________*/

void GetWorkstationEnvironment(operatingSystemType, operatingSystemVersion,
			       hardwareType, shortHardwareType)

char *operatingSystemType;	/* Receives operating system name, MS-DOS    */
char *operatingSystemVersion;	/* Receives operating system version, V1.10  */
char *hardwareType;		/* Receives hardware type, VICTOR 9000	     */
char *shortHardwareType;	/* Receives short hardware version	     */
{
    char shellVersion[45];
    int index1, index2;

    memset(shellVersion, 0, 45);
    _GetWorkstationEnvironment(shellVersion);

    index1 = 0;

    if (operatingSystemType == (char *)0)
	while (shellVersion[index1++]);
    else
    {
	index2 = 0;
	do
	    operatingSystemType[index2++] = shellVersion[index1];
	while (shellVersion[index1++]);
    }

    if (operatingSystemVersion == (char *)0)
	while (shellVersion[index1++]);
    else
    {
	index2 = 0;
	do
	    operatingSystemVersion[index2++] = shellVersion[index1];
	while (shellVersion[index1++]);
    }

    if (hardwareType == (char *)0)
	while (shellVersion[index1++]);
    else
    {
	index2 = 0;
	do
	    hardwareType[index2++] = shellVersion[index1];
	while (shellVersion[index1++]);
    }

    if (shortHardwareType != (char *)0)
    {
	index2 = 0;
	do
	    shortHardwareType[index2++] = shellVersion[index1];
	while (shellVersion[index1++]);
    }
}
