/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <string.h>
#include <nwwrkenv.h>
#include <nwmisc.h>

#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))


/*____________________________________________________________________________
  |
  |  Comments:  This function returns the maximum search order contained in
  |             the path.  If the path is empty the search order is 0, if
  |             there is one entry in the path variable the search order is
  |             1, etc...
  |_________________________________________________________________________*/


int GetMaxSearchOrder(pathVariable)
char   *pathVariable;                        /* The path to be searched     */
{
int     order, ln, i;


        order = 0;
        ln = strlen(pathVariable);
        for (i = 0; i < ln; i++)
           if (*(pathVariable+i) == ';' )
               order++;

        i--;
        if (*(pathVariable+i) != ';')
            order++;

        return(order);
}
