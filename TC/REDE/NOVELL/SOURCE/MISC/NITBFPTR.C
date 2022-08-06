/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdio.h>
#include <ctype.h>
#include <nwmisc.h>
#include <nwconsol.h>

char *NITBufferPtr = NITBuffer + (sizeof(int)/sizeof(char));
char NITBuffer[ MAX_NETWARE_BUFFER_SIZE + 2*(sizeof(int)/sizeof(char)) ];
dummy() {}		 /*  Must be done to satisfy LATTICE 3.10  */




/*___________________________________________________________________________
  |
  |  Comments:	This function compares two buffers of data (case insensitive).
  |		Lattice C is the only compiler that does not have a memicmp
  |		function call.	This routine will be used for the Lattice C
  |		compilers only.
  |_________________________________________________________________________*/

#ifdef LATTICE

int memicmp(buf1, buf2, count )
char		*buf1;
char		*buf2;
WORD   	 count;
{
int		i;

	for ( i = 0; i < count; ++i )
	 {
	     if ( tolower(*buf1++) != tolower(*buf2++) )
		  return( tolower(*(--buf1)) - tolower(*(--buf2)) );
	 }

	return( 0 );
}

#endif

