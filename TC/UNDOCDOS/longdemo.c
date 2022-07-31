#include <dos.h>

// this is an example on viewing long file names in C
// make sure that you have a 'dummy' file named: 'tempfile.tmp'
// in the current directory be for running this program.

// This was created with QC2.5

void main(void)
{
  unsigned char  Buffer[128];
  unsigned char  OrgFileName[13] = "tempfile.tmp\0";
  unsigned char  NewFileName[37] = "this is a temp file with a temp name\0";
  unsigned char  OurError;
  unsigned int   SegBuffer, OffBuffer;
  unsigned int   SegOrgName, OffOrgName;
  unsigned int   SegNewName, OffNewName;


  void far *mptr = Buffer;
  SegBuffer = FP_SEG(mptr);
	OffBuffer = FP_OFF(mptr);

  mptr = OrgFileName;
  SegOrgName = FP_SEG(mptr);
  OffOrgName = FP_OFF(mptr);

  mptr = NewFileName;
  SegNewName = FP_SEG(mptr);
  OffNewName = FP_OFF(mptr);

  // check to see if long file names are supported
  _asm {
 	  mov  ax,71A0h
	  int  21h
	  mov  [OurError],al
  }
  if (OurError == 0) {
    printf("\nLong file names are not supported");
	  exit(-1);
  }

  // get current directory
  OurError = 0x00;      				// clear error flag var.
  _asm {
		push ds
		push si
		push dx
    mov  ax,7147h
	  mov  dl,00h
	  mov  si,[OffBuffer]
	  mov  ds,[SegBuffer]
	  int  21h
		pop  dx
    pop  si
		pop  ds
	  jnc  GetCDone
	  mov  [OurError],al
GetCDone:
  }
  if (OurError == 0) {
    printf("\nThe current Directory is:  %s", Buffer);
  } else {
    printf("\nError getting current directory name");
	  exit(-1);
  }

  // rename a file name
  OurError = 0x00;      				// clear error flag var.
  _asm {
		push ds
    push es
		push di
		push dx
    mov  ax,7156h
		mov  di,[OffNewName]
    mov  es,[SegNewName]
	  mov  dx,[OffOrgName]
	  mov  ds,[SegOrgName]
	  int  21h
		pop  dx
		pop  di
    pop  es
		pop  ds
	  jnc  GetCDone1
	  mov  [OurError],al
GetCDone1:
  }
  if (OurError == 0) {
    printf("\nThe filename, %s\n was correctly changed to:  %s", OrgFileName, NewFileName);
  } else {
    printf("\nError renaming file.");
	  exit(-1);
  }

  exit(0);
}
