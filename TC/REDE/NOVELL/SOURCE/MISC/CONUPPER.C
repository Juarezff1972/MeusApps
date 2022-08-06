/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <ctype.h>
#include <nwmisc.h>


/*_________________________ ConvertToUpperCase ______________________________
|
| Output:    Returns the length of the string.
|
| Comments:  This function converts a string to upper case.
|___________________________________________________________________________*/

int ConvertToUpperCase(string)
char  *string;     /* The string that this points to is returned as all
                      upper case characters                                 */
{
int position;

    position = -1;
    while (string[++position] != '\0')
        string[position] = toupper(string[position]);

    return (position);
}
