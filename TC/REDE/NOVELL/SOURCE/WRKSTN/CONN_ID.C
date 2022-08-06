/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#include <nwwrkenv.h>


/*******************************************************************************

    PROGRAM NAME:            CONN_ID
    ------------

    FUNCTIONS:               SetPrimaryConnectionID
    ---------                SetPreferredConnectionID
                             GetPreferredConnectionID
                             GetPrimaryConnectionID
                             GetDefaultConnectionID

    COMMENTS:                The default file server is the preferred file
    ---------                server.  If no preferred file server is set, the
                             default file server is the file server that
                             services the current default drive.  If the current
                             default drive is mapped to a local drive, then the
                             default file server is the primary file server.

                             Example of Primary, Preferred, and Default
                             connection IDs is given below:


       Example:
  _________________________
  | Login to SERVER_1     |
  | map f:=SERVER_1/VOL1  | then ---> primary = preferred = default connectionID
  | f:                    |
  |-----------------------|
  | Attach SERVER_2       |
  | map g:=SERVER_2/VOL2  | then ---> preferred = default connectionID
  | g:                    |
  |-----------------------|
  | a: (local drive)      | then ---> No preferred, default = SERVER_1 = primary
  |_______________________|


*******************************************************************************/



/*________________________________________________________________________

    Output:    None.

    Comments:  This function sets the connection ID to be that of the
               primary file server.  The primary file server is the first
               file server that a user logs in, unless it has been changed
               by this function, or unless the workstation has detached from
               its primary file server.
 _______________________________________________________________________*/

void SetPrimaryConnectionID(connectionID)
WORD   connectionID;
{
   _PrConnRequest((BYTE)0x04, (BYTE)connectionID);
}






/*________________________________________________________________________

    Output:  None.

    Comments:  This function sets the connection ID to be that of the
               preferred file server.  If a user has a current default drive
               and that drive is a network drive, then the preferred server
               is the server of that drive.
 _______________________________________________________________________*/

void SetPreferredConnectionID(connectionID)
WORD   connectionID;
{
   _PrConnRequest((BYTE)0x00, (BYTE)connectionID);
}





/*________________________________________________________________________

    Output:    ConnectionID

    Comments:  This function gets the connectionID  of the preferred file
               server.
 _______________________________________________________________________*/

WORD  GetPreferredConnectionID()
{
WORD   connectionID;
WORD   ID;

   connectionID = 0;
   ID = _PrConnRequest((BYTE)0x01, (BYTE)connectionID);
   return(ID);
}





/*________________________________________________________________________

    Output:    ConnectionID

    Comments:  This function gets the connectionID  of the primary file
               server.
 _______________________________________________________________________*/

WORD  GetPrimaryConnectionID()
{
WORD   connectionID;
WORD   ID;

   connectionID = 0;
   ID = _PrConnRequest((BYTE)0x05, (BYTE)connectionID);
   return(ID);
}




/*________________________________________________________________________

    Output:    ConnectionID

    Comments:  This function gets the connectionID  of the default file
               server.
 _______________________________________________________________________*/

WORD  GetDefaultConnectionID()
{
WORD   connectionID;
WORD   ID;

   connectionID = 0;
   ID = _PrConnRequest((BYTE)0x02, (BYTE)connectionID);
   return(ID);
}
