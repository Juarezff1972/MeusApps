/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
/*________________________________________________________________________
 |                                                                        |
 |  Comments:   This function returns TRUE if the character passed in is  |
 |              a  white space character (space, tab, carriage return, or |
 |              a line feed).                                             |
 |________________________________________________________________________*/

#include <nwmisc.h>

int IsWhiteSpace( testChar )
char    testChar;

{
int rcode;

    switch( testChar ) {
        case ' ':
        case '\t':
        case '\n':
            rcode = TRUE;
            break;
        default:
            rcode = FALSE;
            break;
    }
    return( rcode );
}
