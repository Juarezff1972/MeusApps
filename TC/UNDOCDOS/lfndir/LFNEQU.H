/*----------------------------------------------------------------------
LFNequ.h  
  
Copyright (c) 1998
Ziff-Davis Publishing Company
All rights reserved.
First Published in PC Magazine, US Edition, Nay 5, 1998
Written by Rick Knoblaugh

Defines for LFNDIR program.

Ver 1.0  -- Initial release

Ported to MS Quick C 2.5 (Small model) by Forever Young Software

----------------------------------------------------------------------*/

#define TRUE 1
#define FALSE 0
#define BITS_PER_SEC 4096
#define LEN_DIR_ENTRY 32
#define LFN_CHARS_PER_ENTRY 26
#define MAX_PATH_LEN 260
#define MAX_ENTRIES_FOR_LFN (MAX_PATH_LEN / LFN_CHARS_PER_ENTRY)
#define SECTOR_SIZE 512
#define ENTRIES_PER_SEC (SECTOR_SIZE / LEN_DIR_ENTRY)

#define BACK_SLASH '\\'
#define DOT '.'
#define SPACE ' '
#define TILDE '~'
#define COLON ':'
#define SWITCH_CHAR '/'

#define LAST_LFN_ENTRY 0x40
#define NORMAL_ATTRIB 0
#define READONLY_ATTRIB 0x01
#define HIDDEN_ATTRIB 0x02
#define SYSTEM_ATTRIB 0x04
#define VOLUME_ATTRIB 0x08

//impossible combination of read only, hidden, system and volume label
#define LFN_ATTRIB (READONLY_ATTRIB + HIDDEN_ATTRIB + SYSTEM_ATTRIB + VOLUME_ATTRIB)

#define SUBDIR_ATTRIB 0x10
#define ARCHIVE_ATTRIB 0x20

#define MAJ_CODE 8 
#define GET_DEVICE_PARMS 0x60
#define GET_DEFAULT_DRIVE 0x19
#define GET_CUR_DIR 0x47 
#define GET_DISK_FREE_SPACE 0x36 
#define GET_MEDIA_ID 0x66
#define GET_VOLUME_INFO 0x71a0;
#define FIND_FIRST_FILE 0x11;    

#define ERASE_FLAG 0xe5 
#define LOWER_SWITCH 'L'
#define PAUSE_SWITCH 'P'
#define ATTRIB_SWITCH 'A'
#define ALL_SUBS_SWITCH 'S'
#define BARE_DISP_SWITCH 'B'
#define HELP_SWITCH1 'H'
#define HELP_SWITCH2 '?'
#define NO_CODE 0xff
#define NO_ATTRIB_IF_SUB 1
#define GEN_WILD_EXTENSION 2
#define FM_SET_WILD_EXTENSION 0x10
#define DRIVE_NOT_READY 2
#define UNKNOWN_DRIVE 1
