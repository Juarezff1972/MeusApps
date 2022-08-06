/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <nwlocal.h>
#include <ndt.h>

static WORD maxSize;
static WORD envSeg;
static char *env;       /*-- pointer to the environment           --*/


int GetEnvironmentVariable( name, value )
char *name, *value;
{
   char    *nextEnvironVar;
   char    *environVarPtr;

   env = _ReadEnvironment();
   if( env == (char *)NULL )
      return( -2 );
   for( nextEnvironVar = env; *nextEnvironVar != '\0';
        nextEnvironVar += strlen(nextEnvironVar) + 1 )
      {
      for (environVarPtr = name;; environVarPtr++, nextEnvironVar++)
         {
         if ((*environVarPtr == (char)NULL) && (*nextEnvironVar == '='))
            goto Match;
         if (*environVarPtr != *nextEnvironVar)
            break;
         }
      }
   free( env );
   return( 0 );

Match:
   strcpy( value, ++nextEnvironVar );
   free( env );
   return( 0 );
}



/****************************************************************************/

int GetEnvironmentVariableLength(name)
char *name;
{
char    *nextEnvironVar;
char    *environVarPtr;

    env = _ReadEnvironment();
    if (env == (char *)NULL)
        return (-2);
    for (nextEnvironVar = env; *nextEnvironVar != '\0';
         nextEnvironVar += strlen(nextEnvironVar) + 1)
     {
        for (environVarPtr = name;; environVarPtr++, nextEnvironVar++)
         {
            if ((*environVarPtr == (char)NULL) && (*nextEnvironVar == '='))
                goto Match;
            if (*environVarPtr != *nextEnvironVar)
                break;
         }
     }
    free(env);
    return (-1);

Match:
    free(env);
    return( (int)( strlen( ++nextEnvironVar ) ) );
}




/****************************************************************************/

char *_ReadEnvironment()
{

    envSeg = _GetEnvironmentSeg();
    MemFetch((WORD)(envSeg - 1), 3, (BYTE *)&maxSize, sizeof(maxSize));
    if ((GetDosVersion() & 0xff00) == 0x0200)
        maxSize--;
    maxSize <<= 4;
    env = malloc(maxSize);
    if (env != (char *)NULL)
        MemFetch(envSeg, 0, (BYTE *)env, maxSize);
    return (env);
}

/****************************************************************************/

int PutEnvironmentVariable(name, value)
char *name, *value;
{
char   *namePointer;
char   *currEnvironVar;
char   *environVarPtr;

    env = _ReadEnvironment();
    if (env == (char *)NULL)
        return (-2);
    /*-- Attempt to locate the old environment variable --*/
    for ( currEnvironVar = env; *currEnvironVar != '\0';
          currEnvironVar += strlen(currEnvironVar) + 1 )
     {
        for ( namePointer = name, environVarPtr = currEnvironVar;;
              namePointer++, environVarPtr++ )
         {
            if ( ( *namePointer == '\0' ) && ( *environVarPtr == '=') )
             {
                while ( *environVarPtr != '\0' )
                    environVarPtr += strlen(environVarPtr) + 1;
                namePointer = currEnvironVar + strlen(currEnvironVar) + 1;
                memmove( currEnvironVar, namePointer,
                         (unsigned int)(environVarPtr - namePointer + 1) );
                goto Done;
             }
            if ( *namePointer != *environVarPtr )
                break;
         }
     }
Done:
    if (value != NULL && *value != '\0')
     {
        /*-- Locate the end of the environment --*/
        while (*currEnvironVar != '\0')
            currEnvironVar += strlen(currEnvironVar) + 1;
        if (currEnvironVar - env + strlen(name) + strlen(value) + 3 > maxSize)
         {
            free(env);
            return (-1);
         }
        sprintf(currEnvironVar, "%s=", name);
        strcat(currEnvironVar, value);
        currEnvironVar[strlen(currEnvironVar) + 1] = '\0';
     }
    _WriteEnvironment(env);
    free(env);
    return (0);
}

/****************************************************************************/

void _WriteEnvironment(env)
char *env;
{

    envSeg = _GetEnvironmentSeg();
    MemFetch((WORD)(envSeg - 1), 3, (BYTE *)&maxSize, sizeof(maxSize));
    if ((GetDosVersion() & 0xff00) == 0x0200)
        maxSize--;
    maxSize <<= 4;
    MemStore(envSeg, (WORD)0, (BYTE *)env, maxSize);
}
