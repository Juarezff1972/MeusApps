/*========================================================
 File name: PlayVOC.c

 DESCRIPTION:
 -----------
 This program illustrates how to play a .VOC file through
 the Sound Blaster card.

 This is a DOS program.

 (C) Copyright Gurewich 1992, 1993 (R) All rights reserved.
 =========================================================*/

/*=======
 #include
 ========*/
#include <bios.h>
#include <conio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===============
 Global variables
 ================*/
char           voc_file_to_play_s[256];
char far *     pointer_to_driver = 0;
unsigned int   status_word_i = 0;
unsigned char  length_of_voc_header_uc;

/*=========
 Prototypes
 ==========*/
int        load_driver  ( void );
char far * load_voc     ( char * filename_s );
void       play_it      ( char far * address_of_voc_buffer );

/*===============
 FUNCTION: main()
 ===============*/
void main (int argc, char **argv )
{
char far * address_of_voc_buffer;

if ( argc != 2 )
   {
   printf("\n To use this program, type:");
   printf("\n    PlayVoc <Path & Name of .VOC file>  {Enter} \n");
   exit(0);
   }

/*---------------------------------------------
Get the .VOC file name from the command line.
----------------------------------------------*/
strcpy ( voc_file_to_play_s, argv[1] );

/*-------------------------------------------
 Load the driver: CT-VOICE.DRV
 The program uses functions from this driver.
 -------------------------------------------*/
if ( load_driver() == -1 )
   {
   printf("\n Failed to load the driver!");
   exit(0);
   } 
/*-------------------
 Load the .VOC file. 
 -------------------*/
address_of_voc_buffer = load_voc(voc_file_to_play_s);

/*-------------------
 Play the .VOC file.
 -------------------*/
printf("\n Press any key to stop the playback...");
play_it ( address_of_voc_buffer );

while (1)
      {
      if ( ( status_word_i != 0) && (kbhit() == 0))
         ;
      else
         break;
      }

if ( kbhit() != 0 )
   {
   /*-----------------
   Stop the playback.
   ------------------*/
   __asm {
       mov bx,8
       call pointer_to_driver
       }
   }

/*-----------------------------------------------
 Free the .VOC buffer, and release the sound card.
 ----------------------------------------------*/
_dos_freemem(FP_SEG(address_of_voc_buffer));
__asm {
       mov  bx,9
       call pointer_to_driver
       }
_dos_freemem(FP_SEG( pointer_to_driver ));

}/* end of file. */
/*============= end of main() ==========*/

/*=======================
 Function: load_driver()
 ======================*/
/*--------------------------------------------------------
 DESCRIPTION:
 This function loads into memory the Sound Blaster driver:
    C:\\SBpro\\DRV\\CT-VOICE.DRV
 --------------------------------------------------------*/ 
int load_driver ( void )
{
int              handler_to_driver_i;
unsigned long    filesize_ul;
unsigned int     blocksize_ui;
unsigned int     segment_ui;
char     huge *  driver_buffer;
unsigned int     bytes_read_ui;
char             temp_s[25];
unsigned int *   p_status_word_i;
unsigned int     seg_address;
unsigned int     ofs_address;
unsigned int     out_result;
unsigned char    result_uc;

/*--------------------
 Open the driver file.
 --------------------*/
if ( _dos_open ("C:\\SBpro\\DRV\\CT-VOICE.DRV",
                 O_RDONLY,
                 &handler_to_driver_i) != 0 )
   {
   printf("\n Can't open the CT-VOICE.DRV!");
   exit (0);
   }
else
   {
   printf("\n Driver was opened successfully!");
   }

/*------------------------------
 Allocate memory for the driver.
 ------------------------------*/
filesize_ul  = filelength ( handler_to_driver_i );
blocksize_ui = (filesize_ul + 15L) / 16;
result_uc = _dos_allocmem ( blocksize_ui, &segment_ui );
if ( result_uc !=0 )
   {
   printf ("\n Can't allocate memory for the driver!");
   return -1;
   }

/*------------------------------
 Loading the driver into memory.
 ------------------------------*/
FP_SEG (pointer_to_driver) = segment_ui;
FP_OFF (pointer_to_driver) = 0;
driver_buffer = (char huge*) pointer_to_driver;
_dos_read ( handler_to_driver_i, driver_buffer, 32000, &bytes_read_ui );
_dos_close ( handler_to_driver_i );

/*-------------------------------------------------------
 Check if the loaded driver has the characters "CT-VOICE"
 starting at the 4th character.
 -------------------------------------------------------*/
temp_s[0] = driver_buffer[3];
temp_s[1] = driver_buffer[4];
temp_s[2] = driver_buffer[5];
temp_s[3] = driver_buffer[6];
temp_s[4] = driver_buffer[7];
temp_s[5] = driver_buffer[8];
temp_s[6] = driver_buffer[9];
temp_s[7] = driver_buffer[10];
temp_s[8] = '\0';
if ( strcmp(temp_s, "CT-VOICE") != 0 )
   {
   printf("\n Can't find CT-VOICE! Not a valid driver!");
   return -1;
   }

 /*---------------------
  Initialize the driver.
  ---------------------*/
 __asm {
     mov       bx,3
     call      pointer_to_driver
     mov       out_result,ax
     }

/*------------------------------------------
 Checking the results of the initialization 
 ------------------------------------------*/
switch ( (int)out_result )
       {
       case 1: printf ("\n No sound card was found!");
               return -1;
       case 2: printf ("\n Incorrect port address!");
               return -1;
       case 3: printf ("\n Incorrect interrupt number!");
               return -1;
       }

/*---------------------------------------
 Calculate the address of the status word.
 ---------------------------------------*/
p_status_word_i = &status_word_i;
seg_address = FP_SEG(p_status_word_i);
ofs_address = FP_OFF(p_status_word_i);

 /*-----------------------------------
  Inform the driver the address of the
  status word.
  -----------------------------------*/
 __asm {
     mov       bx,5
     mov       es,seg_address
     mov       di,ofs_address
     call      pointer_to_driver
     }

return 1;

}/* end of function. */
/*=============== end of function ===============*/

/*=======
 FUNCTION
 ========*/
/*---------------------------------
 DESCRIPTION:
 This function loads the .VOC file.
 ---------------------------------*/
char far * load_voc ( char * filename_s )
{
int            handler_of_file_i;
long           filesize_l;
unsigned int   blocksize_ui;
unsigned int   segment_ui;
char huge *    pointer_to_voc_file;
char far *     address_of_voc_buffer;
unsigned int  bytes_read_ui;

/*------------------
 Open the .VOC file.
 ------------------*/
   if ( _dos_open ( filename_s, O_RDONLY, &handler_of_file_i ) != 0 )
      {
      printf ("\n Can't open the file %s",  filename_s );
      exit (0);
      }
   else
      {
      printf ("\n The .VOC file was opened successfully!");
      }

/*----------------------------------
 Allocating memory for the .VOC file.
 -----------------------------------*/
filesize_l = filelength ( handler_of_file_i );
blocksize_ui = (filesize_l + 15L) /16;
if (  _dos_allocmem ( blocksize_ui ,&segment_ui ) !=0 )
   {
   printf ("\n Can't allocate memory for the .VOC file!");
   exit (0);
   }

/*---------------------
 Loading the .VOC file.
 ---------------------*/
FP_SEG(address_of_voc_buffer) = segment_ui;
FP_OFF(address_of_voc_buffer) = 0;
pointer_to_voc_file = (char huge*)address_of_voc_buffer;


while (1)
      {
       _dos_read ( handler_of_file_i, 
                   pointer_to_voc_file, 
                   32000, 
                   &bytes_read_ui );
      pointer_to_voc_file = pointer_to_voc_file + bytes_read_ui;
      if (  bytes_read_ui != 32000)
         break;
      }
_dos_close ( handler_of_file_i );

/*------------------------------------------------------
 Check if the .VOC file has "Cre" as its first characters.
 ------------------------------------------------------*/
if ( ( address_of_voc_buffer[0] != 'C') || 
     ( address_of_voc_buffer[1] != 'r') ||
     ( address_of_voc_buffer[2] != 'e') 
     )
     {
     printf("\n .VOC should contain Cr as its first characters");
     exit(0);
     }

length_of_voc_header_uc = address_of_voc_buffer[20];

return address_of_voc_buffer;

}/* end of function. */
/*=================== end of function ===============*/

/*==================
 FUNCTION: play_it()
 ==================*/
void play_it ( char far * address_of_voc_buffer )
{
unsigned int   seg_address;
unsigned int   ofs_address;

/*-----------------------
 Enable sound card output.
 ------------------------*/
__asm {
    mov   bx,4
    mov   al,1
    call pointer_to_driver
    }

seg_address = FP_SEG ( address_of_voc_buffer );
ofs_address = FP_OFF ( address_of_voc_buffer ) + length_of_voc_header_uc;

__asm {
    mov  bx, 6
    mov  es, seg_address
    mov  di, ofs_address
    call pointer_to_driver
    }

}/* end of function. */
/*============== end of function =============*/

