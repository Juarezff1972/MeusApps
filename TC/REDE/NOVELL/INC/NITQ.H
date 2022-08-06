/*      (c) COPYRIGHT 1988-1990 by Novell, Inc.  All Rights Reserved.   */
#ifndef _NITQ_H
   #define _NITQ_H
   /*_____________________________________________________________________
      Definitions and structures for the Netware API Queue Services logic
     ____________________________________________________________________*/

   #ifndef _PROLOG_H
      #include <prolog.h>
   #endif

   typedef struct
   {
       BYTE clientStation;
       BYTE clientTaskNumber;
       long clientIDNumber;
       long targetServerIDNumber;
       BYTE targetExecutionTime[6];
       BYTE jobEntryTime[6];
       WORD jobNumber;
       WORD jobType;
       BYTE jobPosition;
       BYTE jobControlFlags;
       BYTE jobFileName[14];
       BYTE jobFileHandle[6];
       BYTE serverStation;
       BYTE serverTaskNumber;
       long serverIDNumber;
       BYTE textJobDescription[50];
       BYTE clientRecordArea[152];
   } JobStruct;

   /* Queue Job Control Flags */

   #define QF_OPERATOR_HOLD    0x80
   #define QF_USER_HOLD        0x40
   #define QF_ENTRY_OPEN       0x20
   #define QF_SERVICE_RESTART  0x10
   #define QF_AUTO_START       0x08

   /* Queue Status Flags */

   #define QS_CANT_ADD_JOBS        0x01
   #define QS_SERVERS_CANT_ATTACH  0x02
   #define QS_CANT_SERVICE_JOBS    0x04

   /* Error return codes */

   #ifndef DIRECTORY_FULL
      #define DIRECTORY_FULL  153
   #endif

   #define Q_ERROR         208
   #define NO_QUEUE        209
   #define NO_Q_SERVER     210
   #define NO_Q_RIGHTS     211
   #define Q_FULL          212
   #define NO_Q_JOB        213
   #define NO_JOB_RIGHTS   214
   #define Q_SERVICING     215
   #define STN_NOT_SERVER  217
   #define Q_NOT_ACTIVE    216
   #define Q_HALTED        218
   #define MAX_Q_SERVERS   219

   #ifdef PROTOTYPE
      extern int AbortServicingQueueJobAndFile(long, WORD, int);
      extern int AttachQueueServerToQueue(long);
      extern int ChangeQueueJobEntry(long, JobStruct *);
      extern int ChangeQueueJobPosition(long, WORD, BYTE);
      extern int ChangeToClientRights(long, WORD);
      extern int CloseFileAndAbortQueueJob(long, WORD, int);
      extern int CloseFileAndStartQueueJob(long, WORD, int);
      extern int CreateQueue(char *, int, char, char *, long *);
      extern int CreateQueueJobAndFile(long, JobStruct *, int *);
      extern int DestroyQueue(long);
      extern int DetachQueueServerFromQueue(long);
      extern int FinishServicingQueueJobAndFile(long, WORD, long, int);
      extern int GetQueueJobEntrysFileSize(WORD, long, WORD, long *);
      extern int GetQueueJobsFileSize(long, WORD, long *);
      extern int GetQueueJobList(long, WORD *, WORD *, WORD);
      extern int ReadQueueJobEntry(long, WORD, JobStruct *);
      extern int ReadQueueCurrentStatus(long, BYTE *, BYTE *, BYTE *, long *, WORD *, WORD);
      extern int ReadQueueServerCurrentStatus(long, long, WORD, char *);
      extern int RemoveJobFromQueue(long, WORD);
      extern int RestoreQueueServerRights(void);
      extern int ServiceQueueJobAndOpenFile(long, WORD, JobStruct *, int *);
      extern int SetQueueCurrentStatus(long, BYTE);
      extern int SetQueueServerCurrentStatus(long, BYTE *);
   #else
      extern int AbortServicingQueueJobAndFile();
      extern int AttachQueueServerToQueue();
      extern int ChangeQueueJobEntry();
      extern int ChangeQueueJobPosition();
      extern int ChangeToClientRights();
      extern int CloseFileAndAbortQueueJob();
      extern int CloseFileAndStartQueueJob();
      extern int CreateQueue();
      extern int CreateQueueJobAndFile();
      extern int DestroyQueue();
      extern int DetachQueueServerFromQueue();
      extern int FinishServicingQueueJobAndFile();
      extern int GetQueueJobEntrysFileSize();
      extern int GetQueueJobsFileSize();
      extern int GetQueueJobList();
      extern int ReadQueueJobEntry();
      extern int ReadQueueCurrentStatus();
      extern int ReadQueueServerCurrentStatus();
      extern int RemoveJobFromQueue();
      extern int RestoreQueueServerRights();
      extern int ServiceQueueJobAndOpenFile();
      extern int SetQueueCurrentStatus();
      extern int SetQueueServerCurrentStatus();
   #endif
#endif
