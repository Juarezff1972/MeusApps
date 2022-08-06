/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NWBINDRY_H
#define _NWBINDRY_H
/*_______________________________________________________________________

      Definitions and structures for the Netware API Bindery Logic
  _____________________________________________________________________*/

#ifndef _PROLOG_H
   #include <prolog.h>
#endif

/**** Bindery security definitions ****/

#define BS_ANY_READ        ((BYTE)0x00) /* Readable by anyone                */
#define BS_LOGGED_READ     ((BYTE)0x01) /* Must be logged in to read         */
#define BS_OBJECT_READ     ((BYTE)0x02) /* Readable by same object or super  */
#define BS_SUPER_READ      ((BYTE)0x03) /* Readable by supervisor only       */
#define BS_BINDERY_READ    ((BYTE)0x04) /* Readable only by the bindery      */
#define BS_ANY_WRITE       ((BYTE)0x00) /* Writeable by anyone               */
#define BS_LOGGED_WRITE    ((BYTE)0x10) /* Must be logged in to write        */
#define BS_OBJECT_WRITE    ((BYTE)0x20) /* Writeable by same object or super */
#define BS_SUPER_WRITE     ((BYTE)0x30) /* Writeable only by the supervisor  */
#define BS_BINDERY_WRITE   ((BYTE)0x40) /* Writeable by the bindery only     */

/* Bindery object type definitions */
#define OT_WILD                         (-1)  /* Matches any type            */
#define OT_UNKNOWN                      0x00  /* Unknown object type         */
#define OT_USER                         0x01  /* The object is a "user"      */
#define OT_USER_GROUP                   0x02  /* A group of users            */
#define OT_PRINT_QUEUE                  0x03  /* Services print queues       */
#define OT_FILE_SERVER                  0x04  /* The object serves files     */
#define OT_JOB_SERVER                   0x05
#define OT_GATEWAY                      0x06
#define OT_PRINT_SERVER                 0x07
#define OT_ARCHIVE_QUEUE                0x08
#define OT_ARCHIVE_SERVER               0x09  /* Services backup jobs        */
#define OT_JOB_QUEUE                    0x0A
#define OT_ADMINISTRATION               0x0B
#define OT_NAS_SNA_GATEWAY              0x21
#define OT_REMOTE_BRIDGE_SERVER         0x24
#define OT_TIME_SYNCHRONIZATION_SERVER  0x2D
#define OT_ARCHIVE_SERVER_DYNAMIC_SAP   0x2E
#define OT_ADVERTISING_PRINT_SERVER     0x47
#define OT_BTRIEVE_VAP                  0x4B  /*       prior to 2.15, 50     */
#define OT_PRINT_QUEUE_USER             0x53

/*#define OT_REMOTE_BRIDGE_SERVER         0x26 **38 CONFLICT EXISTS!!        */
/*#define OT_TCPIP_GATEWAY                0x27 **39 CONFLICT EXISTS!!        */

/* Trustee Access Rights in a network directory */

#ifndef TA_NONE
   #define TA_NONE             ((BYTE)0x00)
   #define TA_READ             ((BYTE)0x01)
   #define TA_WRITE            ((BYTE)0x02)
   #define TA_OPEN             ((BYTE)0x04)
   #define TA_CREATE           ((BYTE)0x08)
   #define TA_DELETE           ((BYTE)0x10)
   #define TA_OWNERSHIP        ((BYTE)0x20)
   #define TA_SEARCH           ((BYTE)0x40)
   #define TA_MODIFY           ((BYTE)0x80)
   #define TA_ALL              ((BYTE)0xFF)
#endif

/* Attributes of objects and properties in the bindery */

#define BF_STATIC       ((BYTE)0x00)
#define BF_DYNAMIC      ((BYTE)0x01)
#define BF_ITEM         ((BYTE)0x00)
#define BF_SET          ((BYTE)0x02)

#ifdef PROTOTYPE

extern int    AddBinderyObjectToSet(char *, WORD, char *, char *, WORD);
extern int    ChangeBinderyObjectPassword(char *, WORD, char *, char *);
extern int    ChangeBinderyObjectSecurity(char *, WORD, BYTE);
extern int    ChangePropertySecurity(char *, WORD, char *, BYTE);
extern int    CloseBindery(void);
extern int    CreateBinderyObject(char *, WORD, BYTE, BYTE);
extern int    CreateProperty(char *, WORD, char *, BYTE, BYTE);
extern int    DeleteBinderyObject(char *, WORD);
extern int    DeleteBinderyObjectFromSet(char *, WORD, char *, char *, WORD);
extern int    DeleteProperty(char *, WORD, char *);
extern int    GetBinderyAccessLevel(BYTE *, long *);
extern int    GetBinderyObjectID(char *, WORD, long *);
extern int    GetBinderyObjectName(long, char *, WORD *);
extern int    IsBinderyObjectInSet(char *, WORD, char *, char *, WORD);
extern int   _MemberBinderyCall(int, char *, WORD, char *, char *, WORD);
extern int    OpenBindery(void);
extern int    ReadPropertyValue(char *, WORD, char *, int, BYTE *, BYTE *, BYTE *);
extern int    RenameBinderyObject(char *, char *, WORD);
extern int    ScanBinderyObject(char *, WORD, long *, char *, WORD *, char *, char *, char *);
extern int    ScanBinderyObjectTrusteePaths(long, BYTE, int *, char *, char *);
extern WORD   ScanForEquivalence(WORD, DWORD, WORD, BYTE *, WORD *, DWORD *);
extern int    ScanProperty(char *, WORD, char *, long *, char *, char *, char *, char *, char *);
extern int    VerifyBinderyObjectPassword(char *, WORD, char *);
extern int    WritePropertyValue(char *, WORD, char *, int, BYTE *, BYTE);

#else

extern int    AddBinderyObjectToSet();
extern int    ChangeBinderyObjectPassword();
extern int    ChangeBinderyObjectSecurity();
extern int    ChangePropertySecurity();
extern int    CloseBindery();
extern int    CreateBinderyObject();
extern int    CreateProperty();
extern int    DeleteBinderyObject();
extern int    DeleteBinderyObjectFromSet();
extern int    DeleteProperty();
extern int    GetBinderyAccessLevel();
extern int    GetBinderyObjectID();
extern int    GetBinderyObjectName();
extern int    IsBinderyObjectInSet();
extern int   _MemberBinderyCall();
extern int    OpenBindery();
extern int    ReadPropertyValue();
extern int    RenameBinderyObject();
extern int    ScanBinderyObject();
extern int    ScanBinderyObjectTrusteePaths();
extern WORD   ScanForEquivalence();
extern int    ScanProperty();
extern int    VerifyBinderyObjectPassword();
extern int    WritePropertyValue();

#endif

#endif
