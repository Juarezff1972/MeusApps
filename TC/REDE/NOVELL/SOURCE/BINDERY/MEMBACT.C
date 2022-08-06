/*	COPYRIGHT (c) 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwbindry.h>

/*__________________________________________________________________________
       INPUT:

  char *objectName;    Bindery object name (max 48 characters INCLUDING null
  WORD  objectType;    Type of the bindery object (i.e. OT_USER, OT_GROUP...)
  char *propertyName;  Name of the property that the object will be added
  char *memberName;    Name of the bindery object to be added to the property
                       (maximum of 48 characters INCLUDING the null terminator)
  WORD *memberType;    Type of the bindery object to be added to the property
                       (i.e. OT_USER, OT_GROUP,...)

       OUTPUT:

   0   (0x00)          --  SUCCESSFUL
   bindery error code  --  UNSUCCESSFUL

 Comments:
   These routines add, delete, and determine whether an object is in a set.
   The objectName, objectType, and propertyName must uniquely identify the
   property and may not contain wildcard specifiers.
____________________________________________________________________________*/

int AddBinderyObjectToSet(objectName, objectType, propertyName,
                          memberName, memberType)

char *objectName, *propertyName, *memberName;
WORD  objectType;
WORD  memberType;
{
    return (_MemberBinderyCall(65, objectName, objectType, propertyName,
            memberName, memberType));
}


/****************************************************************************/


int DeleteBinderyObjectFromSet(objectName, objectType, propertyName,
                               memberName, memberType)

char *objectName, *propertyName, *memberName;
WORD  objectType;
WORD  memberType;
{
    return (_MemberBinderyCall(66, objectName, objectType, propertyName,
            memberName, memberType));
}


/****************************************************************************/


int IsBinderyObjectInSet(objectName, objectType, propertyName,
                         memberName, memberType)

char *objectName, *propertyName, *memberName;
WORD  objectType;
WORD  memberType;
{
    return (_MemberBinderyCall(67, objectName, objectType, propertyName,
            memberName, memberType));
}
