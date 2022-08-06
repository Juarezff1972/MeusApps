/*	COPYRIGHT (c) 1989 by Novell, Inc.  All Rights Reserved.   */
#include <nwprint.h>
#include <nwmisc.h>

/*____________________________________________________________________________
|
|  Name:       CAPFLAG.C
|
|  Functions:  GetDefaultCaptureFlags -  Gets capture flags for default LPT
|					                          device
|	            SetDefaultCaptureFlags -  Sets capture flags for default LPT
|					                          device
|	            GetSpecificCaptureFlags - Gets capture flags for a specific
|					                          LPT device
|	            SetSpecificCaptureFlags - Sets capture flags for a specific
|					                          LPT device
|____________________________________________________________________________*/




int GetDefaultCaptureFlags( capFlags )
CAPTURE_FLAGS *capFlags;
{
int  ccode;
#ifdef LC310
int  temp;
#endif


   if ((ccode = _PrnSrvRequest(0,(sizeof(CAPTURE_FLAGS)),(BYTE *)capFlags,0))==0){
     capFlags->flushCaptureTimeoutCount = IntSwap( capFlags->flushCaptureTimeoutCount );
     capFlags->maxLines = IntSwap( capFlags->maxLines );
     capFlags->maxChars = IntSwap( capFlags->maxChars );
#ifndef LC310
     capFlags->printerSetupBuffer = (char far *)LongSwap( (long)(capFlags->printerSetupBuffer) );
     capFlags->printerResetBuffer = (char far *)LongSwap( (long)(capFlags->printerResetBuffer) );
#else
     temp = IntSwap( capFlags->prnSetupBufferOfs );
     capFlags->prnSetupBufferOfs = IntSwap( capFlags->prnSetupBufferSeg );
     capFlags->prnSetupBufferSeg = temp;
     temp = IntSwap( capFlags->prnResetBufferOfs );
     capFlags->prnResetBufferOfs = IntSwap( capFlags->prnResetBufferSeg );
     capFlags->prnResetBufferSeg = temp;
#endif
     capFlags->printQueueID = LongSwap( capFlags->printQueueID );
     capFlags->printJobNumber = IntSwap( capFlags->printJobNumber );
   }
   return(ccode);
}




int SetDefaultCaptureFlags( capFlags )
SET_CAPTURE_FLAGS *capFlags;
{
int  ccode;

   capFlags->flushCaptureTimeoutCount =
                        IntSwap( capFlags->flushCaptureTimeoutCount );
   capFlags->maxLines = IntSwap( capFlags->maxLines );
   capFlags->maxChars = IntSwap( capFlags->maxChars );
   ccode = _PrnSrvRequest( 1, ( sizeof(SET_CAPTURE_FLAGS) ),
				               (BYTE *)capFlags, 0 );
   capFlags->maxLines = IntSwap( capFlags->maxLines );
   capFlags->maxChars = IntSwap( capFlags->maxChars );
   return( ccode );
}




int GetSpecificCaptureFlags(LPTDevice,capFlags)

BYTE	      LPTDevice;
CAPTURE_FLAGS *capFlags;
{
int  ccode;
#ifdef LC310
int  temp;
#endif


   if ((ccode = _PrnSrvRequest(2,(sizeof(CAPTURE_FLAGS)),(BYTE *)capFlags,LPTDevice))==0) {
     capFlags->flushCaptureTimeoutCount = IntSwap( capFlags->flushCaptureTimeoutCount );
     capFlags->maxLines = IntSwap( capFlags->maxLines );
     capFlags->maxChars = IntSwap( capFlags->maxChars );
#ifndef LC310
     capFlags->printerSetupBuffer = (char far *)LongSwap( (long)(capFlags->printerSetupBuffer) );
     capFlags->printerResetBuffer = (char far *)LongSwap( (long)(capFlags->printerResetBuffer) );
#else
     temp = IntSwap( capFlags->prnSetupBufferOfs );
     capFlags->prnSetupBufferOfs = IntSwap( capFlags->prnSetupBufferSeg );
     capFlags->prnSetupBufferSeg = temp;
     temp = IntSwap( capFlags->prnResetBufferOfs );
     capFlags->prnResetBufferOfs = IntSwap( capFlags->prnResetBufferSeg );
     capFlags->prnResetBufferSeg = temp;
#endif
     capFlags->printQueueID = LongSwap( capFlags->printQueueID );
     capFlags->printJobNumber = IntSwap( capFlags->printJobNumber );
   }
   return( ccode );
}





int SetSpecificCaptureFlags( LPTDevice, capFlags )
BYTE	            LPTDevice;
SET_CAPTURE_FLAGS *capFlags;
{
int  ccode;

   capFlags->flushCaptureTimeoutCount =
                        IntSwap( capFlags->flushCaptureTimeoutCount );
   capFlags->maxLines = IntSwap( capFlags->maxLines );
   capFlags->maxChars = IntSwap( capFlags->maxChars );
   ccode = _PrnSrvRequest( 3, ( sizeof(SET_CAPTURE_FLAGS) ),
				               (BYTE *)capFlags, LPTDevice );
   capFlags->maxLines = IntSwap( capFlags->maxLines );
   capFlags->maxChars = IntSwap( capFlags->maxChars );
   return( ccode );
}
