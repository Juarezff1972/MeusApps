/*  (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved. */

#include <nit.h>
#include <ntt.h>

long _NW286DateAdjust
(
int    time,
int    date
)
{
     long    temp;
     int     *curPtr;

     date          =IntSwap(date);
     time          =IntSwap(time);
     temp          = time;
     curPtr        = (int *)&temp;
     *(curPtr + 1) = date;

     return(temp);
}
