/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifdef _VAP_
#include <nvt.h>
#else
#include <stdlib.h>
#endif
#include <nwmisc.h>
#include <nwconsol.h>


/*___________________________  AllocNITBuffer _________________________________
|
|   Output:    Char pointer to third byte of buffer.
|
|   Comments:  This function allocates a 514 byte buffer and returns a pointer
|	       to the third byte.
|____________________________________________________________________________*/

#ifdef _VAP_

char *AllocNITBuffer()

{
    return( (char *)NULL );
}

#else

char *AllocNITBuffer()

{
char   *tempPointer;

    tempPointer = malloc( MAX_NETWARE_BUFFER_SIZE + sizeof(int) );
    if ( tempPointer ) {
		/* set routine ID to undefined */
	*((int *)tempPointer) = RI_UNDEFINED;
		/* point to start of transfer area */
	tempPointer += sizeof(int)/sizeof(char);
    }
    return( tempPointer );
}

#endif
