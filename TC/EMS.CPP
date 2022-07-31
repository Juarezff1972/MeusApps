#include <dos.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef char ST3[3];
typedef char ST80[80];
typedef char ST5[5];

const
  EMM_INT                   = 0x67,
  DOS_int                   = 0x21,
  GET_PAGE_FRAME            = 0x41,
  GET_UNALLOCATED_PAGE_COUNT= 0x42,
  ALLOCATE_PAGES            = 0x43,
  MAP_PAGES                 = 0x44,
  DEALLOCATE_PAGES          = 0x45,
  GET_VERSION               = 0x46,
  STATUS_OK                 = 0,
  APPLICATION_PAGE_COUNT    = 60;

union REGS Regs;
unsigned Emm_Handle,
  Page_Frame_Base_Address,
  Pages_Needed,
  Physical_Page,
  Logical_Page,
  Offset,
  Error_Code,
  Pages_EMS_Available,
  Total_EMS_Pages,
  Available_EMS_Pages;
ST3  Version_Number,
  Pages_Number_String;
char  Verify;


char Emm_Installed(void)
{
   struct SREGS Sregs;
   ST80  Emm_Device_Name,Int_67_Device_Name;
   char aux;
   unsigned Position;
//   union REGS Regs;

    for (Position=0;Position<=80;Position++) Int_67_Device_Name[Position]=0;
    strcpy(Emm_Device_Name,"EMMXXXX0");
    Regs.h.ah=0x35;
    Regs.h.al=EMM_INT;
    int86x(DOS_int,&Regs,&Regs,&Sregs);
    for (Position=0;Position<=7;Position++)
    {
        Int_67_Device_Name[Position]=(peekb(Sregs.es,Position+0x0A));
    }
    aux=TRUE;
    if (Int_67_Device_Name!=Emm_Device_Name) aux=FALSE;
    return aux;
}

unsigned EMS_Pages_Available(unsigned *Total_EMS_Pages,unsigned *Pages_Available)
{
      Regs.h.ah=GET_UNALLOCATED_PAGE_COUNT;
      int86(EMM_INT,&Regs,&Regs);
      *Pages_Available=Regs.x.bx;
      *Total_EMS_Pages=Regs.x.dx;
      return (unsigned)Regs.h.ah;
}

unsigned Allocate_Expanded_Memory_Pages(unsigned Pages_Needed,unsigned *Handle)
{
      Regs.h.ah=ALLOCATE_PAGES;
      Regs.x.bx=Pages_Needed;
      int86(EMM_INT,&Regs,&Regs);
      *Handle=Regs.x.dx;
      return (unsigned)Regs.h.ah;
}

unsigned Map_Expanded_Memory_Pages(unsigned Handle,unsigned Logical_Page,unsigned Physical_Page)
{
      Regs.h.ah=MAP_PAGES;
      Regs.h.al=Physical_Page;
      Regs.x.bx=Logical_Page;
      Regs.x.dx=Handle;
      int86(EMM_INT,&Regs,&Regs);
      return (unsigned)Regs.h.ah;
}


unsigned Get_Page_Frame_Base_Address(unsigned *Page_Frame_Address)
{
      Regs.h.ah=GET_PAGE_FRAME;
      int86(EMM_INT,&Regs,&Regs);
      *Page_Frame_Address=Regs.x.bx;
      return (unsigned) Regs.h.ah;
}

unsigned Deallocate_Expanded_Memory_Pages(unsigned Emm_Handle)
{
      Regs.h.ah=DEALLOCATE_PAGES;
      Regs.x.dx=Emm_Handle;
      int86(EMM_INT,&Regs,&Regs);
      return (unsigned) Regs.h.ah;
}

unsigned Get_Version_Number(ST3 *Version_String)
{
    char Word_Part,Fractional_Part;
    char aux[4];
    Regs.h.ah=GET_VERSION;
    int86(EMM_INT,&Regs,&Regs);
    if (Regs.h.ah==STATUS_OK)
    {
        Word_Part=((Regs.h.al >> 4) + 48);
        Fractional_Part=((Regs.h.al & 0xF) +48);
        aux[0]=Word_Part;
        aux[1]='.';
        aux[2]=Fractional_Part;
        aux[3]=0;
        strcpy(*Version_String,aux);
    }
    return (unsigned) Regs.h.ah;
}

void Error(ST80 Error_Message,unsigned Error_Number)
{
    printf("\n %d",Error_Message);
    printf("\n  Error_Number = %d",Error_Number);
    printf("\n EMS test program aborting.");
    exit(1);
}

void main(void)
{
  clrscr();
  window(5,2,77,22);
  if (!Emm_Installed)
  {
    printf("\n The LIM Expanded Memory Manager is not installed.");
    exit(1);
  }
  Error_Code=Get_Version_Number(&Version_Number);
  if (Error_Code!=STATUS_OK)
    Error("Error trying to get the EMS version number ",Error_Code);
  else
    printf("\n LIM Expanded Memory Manager, version %s, is ready for use.",Version_Number);
  Pages_Needed=APPLICATION_PAGE_COUNT;
  Error_Code=EMS_Pages_Available(&Total_EMS_Pages,&Available_EMS_Pages);
  if (Error_Code!=STATUS_OK)
    Error("Error trying to determine the number of EMS pages available.",Error_Code);
  printf("\n There are a total of %d expanded memory pages present in this system.",Total_EMS_Pages);
  printf("\n %d of those pages are available for your usage.",Available_EMS_Pages);
  Error_Code=Allocate_Expanded_Memory_Pages(Pages_Needed,&Emm_Handle);

  itoa(Pages_Needed,Pages_Number_String,10);
  if (Error_Code!=STATUS_OK)
    printf("\n S H I T    H A P P E N S ! ! !");
  Logical_Page=0;
  Physical_Page=0;
  Error_Code=Map_Expanded_Memory_Pages(Emm_Handle,Logical_Page,Physical_Page);
  if (Error_Code!=STATUS_OK)
  printf("\n M O R E    S H I T    H A P P E N S !!!!!!!!!!!!!!!!!!");
//    Error('EMS test program failed trying to map '+
//          'logical pages onto physical pages.',Error_Code);

  printf("\n Logical Page %d successfully mapped onto Physical Page %d",Logical_Page,Physical_Page);
  Error_Code=Get_Page_Frame_Base_Address(&Page_Frame_Base_Address);
  if(Error_Code!=STATUS_OK)
  printf("\n M O R E    S H I T    H A P P E N S    A G A I N !!!!!!!!!!!!!!!!!!");
//    Error('EMS test program unable to get the base Page'+
//          ' Frame Address.',Error_Code);
  printf("\n The base address of the EMS page frame is - %X",Page_Frame_Base_Address);
  for (Offset=0;Offset<=16382;Offset++)
     pokeb(Page_Frame_Base_Address,Offset,Offset % 256);
  printf("\n Testing EMS memory.");
  Offset=1;
  Verify=TRUE;
  while ((Offset<=16382) && (Verify==TRUE))
  {
    if (peekb(Page_Frame_Base_Address,Offset)!=Offset % 256)
      Verify=FALSE;
    Offset--;
  }
  if (!Verify) printf("\n That means S H I T   H A P P E N S...");
//    Error('What was written to EMS memory was not found during '+
//          'memory verification  test.',0);
  printf("\n EMS memory test successful.");

  Error_Code=Deallocate_Expanded_Memory_Pages(Emm_Handle);
//  if (Error_Code!=STATUS_OK)
//    Error('EMS test program was unable to deallocate '+
//          'the EMS pages in use.',Error_Code);
  printf("\n %d page(s) deallocated.",APPLICATION_PAGE_COUNT);
  printf("\n EMS test program completed.");
}
