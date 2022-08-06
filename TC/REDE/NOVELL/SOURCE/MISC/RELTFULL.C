/*	(c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <stdlib.h>
#include <string.h>
#include <nit.h>
#include <ndt.h>
#include <niterror.h>

static int _ProcessDots(char **, char *);

#define upcase(chr) ((((chr)>='a')&&((chr)<='z'))?(chr)-'a'+'A':(chr))

int RelativeToFullPath(relativePath, fullPath)
char   *relativePath;
char   *fullPath;

{
char   *colon;
BYTE    driveStatus;
char   *freePtr             = NULL;
char   *pathOnly;
char   *pathVariable        = NULL;
int     rcode               = SUCCESSFUL;
int     searchOrder;
char    serverName[48];
char    tempChar;
char   *tempPtr1;
char   *tempPtr2;
int     ccode;

                    /*========================
                        check for *<digits>:
                      ========================*/
    if ( ( relativePath[0] == '*' ) &&
         ( relativePath[1] >= '0' ) && ( relativePath[1] <= '9' ) ) {
        tempPtr2 = calloc( 1, strlen( relativePath ) );
        if ( tempPtr2 == (char *)NULL ) {
            rcode = WORKSTATION_OUT_OF_MEMORY;
            goto ERROR0;
        }
        for ( tempPtr1 = relativePath + 1;
              (*tempPtr1 >= '0') && (*tempPtr1 <= '9');
              tempPtr1++ );
        tempChar = *tempPtr1;
        *tempPtr1 = '\0';
        *tempPtr2 = (char)(atoi(relativePath + 1) +
                           GetNumberOfLocalDrives() + 'A' - 1);
        *tempPtr1 = tempChar;
        strcpy( tempPtr2 + 1, tempPtr1 );
        strcpy( relativePath, tempPtr2 );
        free( tempPtr2 );
    }

                    /*==========================
                        check for search drive
                      ==========================*/
    if ( upcase(relativePath[0]) == 'S' ) {
        for ( searchOrder = 0, tempPtr1 = relativePath + 1;
              ( *tempPtr1 >= '0' ) && ( *tempPtr1 <= '9' );
              tempPtr1++ ) {
            searchOrder = searchOrder * 10 - '0' + *tempPtr1;
        }
        if ( ( tempPtr1 != relativePath + 1 ) && ( *tempPtr1 == ':' ) ) {
            ccode = GetPathEnvironmentVariable(&pathVariable);
            tempPtr2 = _FindPathVariableElement( pathVariable, searchOrder );
            if ( tempPtr2 != (char *)NULL ) {
                driveStatus = GetDriveInformation( (BYTE)(upcase(*tempPtr2) - 'A'),
                        (WORD *)NULL, (BYTE *)NULL );
                strcpy( fullPath, tempPtr2 );
                tempPtr2 = strchr( fullPath, ';' );
                if ( tempPtr2 != (char *)NULL )
                    *tempPtr2 = '\0';
                if ( driveStatus & 0x03 ) {
                    tempPtr2 = fullPath + strlen( fullPath ) - 1;
                    if ( *tempPtr2 == '.' )
                        *(tempPtr2++) = '\0';
                    if ( ( *tempPtr2 != '/' ) && ( *tempPtr2 != '\\' ) &&
                         ( *tempPtr2 != ':' ) )
                        strcat( tempPtr2, "/" );
                    strcat( fullPath, tempPtr1 + 1 );
                } else if ( driveStatus == 0 ) {
                    rcode = PATH_ENVIRON_VARIABLE_INVALID;
                    goto ERROR0;
                } else if ( driveStatus != 0x80 ) {
                    rcode = UNEXPECTED_INTERNAL_CONDITION;
                    goto ERROR0;
                }
                freePtr = relativePath = malloc( strlen( fullPath ) + 1 );
                if ( freePtr == (char *)NULL ) {
                    rcode = WORKSTATION_OUT_OF_MEMORY;
                    goto ERROR0;
                }
                strcpy( freePtr, fullPath );
            } else {
                rcode = NO_SUCH_SEARCH_DRIVE;
                goto ERROR0;
            }
        }
    }

                    /*==================================
                        is the relative path complete?
                      ==================================*/
    *serverName = '\0';
    pathOnly = StripFileServerFromPath(relativePath, serverName);
    if (*serverName != '\0') {
        strcpy( fullPath, relativePath );

                /*==========================
                    get the reference path
                  ==========================*/
    } else {
        rcode = GetFullPath( (char)GetDefaultDrive(), fullPath );
        if ( rcode != 0 ) {
            rcode = UNEXPECTED_INTERNAL_CONDITION;
            goto ERROR0;
        }
        StripFileServerFromPath( fullPath, serverName );

                    /*================================
                        check for presence of volume
                      ================================*/
        colon = strchr( pathOnly, ':' );
        if ( colon == pathOnly + 1 ) {
            *fullPath = '\0';
            rcode = GetFullPath( (char)(upcase(*pathOnly) - 'A'), fullPath );
            switch ( rcode ) {
                case 0:
                    break;
                case -1:
                    rcode = DRIVE_IS_NOT_MAPPED;
                default:
                    goto ERROR0;
            }
            if ( *fullPath == '\0' ) {
                rcode = DRIVE_IS_NOT_MAPPED;
                goto ERROR0;
            }
            StripFileServerFromPath( fullPath, serverName );
            pathOnly += 2;
            colon = (char *)NULL;
        }
        if ( colon != (char *)NULL ) {
            strcpy( fullPath + strlen( serverName ) + 1, pathOnly );

                    /*===========================
                        check for leading slash
                      ===========================*/
        } else if ( ( *pathOnly == '\\' ) || ( *pathOnly == '/' ) ) {
            tempPtr1 = strchr( fullPath, ':' );
            if ( tempPtr1 == (char *)NULL ) {
                rcode = UNEXPECTED_INTERNAL_CONDITION;
                goto ERROR0;
            } else {
                strcpy( tempPtr1 + 1, pathOnly + 1 );
            }

                    /*==========================
                        check for leading dots
                      ==========================*/
        } else {
            rcode = _ProcessDots( &pathOnly, fullPath );
            if ( rcode != 0 ) {
                goto ERROR0;
            }
            if ( *pathOnly != '\0' ) {
                tempChar = fullPath[ strlen( fullPath ) - 1 ];
                if ( ( tempChar != '\\' ) && ( tempChar != '/' ) ) {
                    strcat( fullPath, "/" );
                }
                strcat( fullPath, pathOnly );
            }
        }
    }
DONE:
    ConvertToUpperCase( fullPath );
ERROR0:
    if ( freePtr != (char *)NULL )
        free( freePtr );
    if ( pathVariable != (char *)NULL )
        free( pathVariable );
    return( rcode );
}


static int _ProcessDots(pathOnly, fullPath)
char **pathOnly;
char *fullPath;

{
char   *colon;
int     dotCount;
int     loopCount;
int     rcode               = 0;
char   *tempPtr1;
char   *tempPtr2;

    while ( ( (*pathOnly)[0] == '.' ) && ( (*pathOnly)[1] == '.' ) ) {
        for ( dotCount = 0; (*pathOnly)[ dotCount ] == '.'; dotCount++ );
        colon = strchr( fullPath, ':' );
        for ( loopCount = dotCount - 1; loopCount >0; loopCount-- ) {
            tempPtr1 = strrchr( fullPath, '/' );
            tempPtr2 = strrchr( fullPath, '\\' );
            if ( tempPtr1 < tempPtr2 ) {
                tempPtr1 = tempPtr2;
            }
            if ( tempPtr1 <= colon ) {
                tempPtr1 = colon;
                if ( ( tempPtr1 == (char *)NULL ) ||
                     ( tempPtr1 == fullPath + strlen( fullPath ) - 1 ) ) {
                    rcode = INVALID_PATH;
                    goto ERROR0;
                }
                tempPtr1++;
            }
            *tempPtr1 = '\0';
        }
        (*pathOnly) += dotCount;
        if ( ( (*pathOnly)[0] == '/' ) || ( (*pathOnly)[0] == '\\' ) )
            (*pathOnly)++;
        while ( *(*pathOnly) == ' ' )
            (*pathOnly)++;
    }
ERROR0:
    return( rcode );
}
