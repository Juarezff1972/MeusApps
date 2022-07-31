/*----------------------------------------------------------------------
LFNDir.c   
  
Copyright (c) 1998
Ziff-Davis Publishing Company
All rights reserved.
First Published in PC Magazine, US Edition, May 5, 1998
Written by Rick Knoblaugh

LFNDir performs (most of) the equivalent functionality of the Windows 95
DIR command.  It is useful for viewing file listings on volumes which
contain LFNs in environments where LFN support isn't present (such as
when booting from a floppy or restarting the system in MS-DOS mode).

Ver 1.0  -- Initial release

Ported to MS Quick C 2.5 (Small model) by Forever Young Software

----------------------------------------------------------------------*/

#include <dos.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include <bios.h>

#define STRICT
#pragma pack(1)

#include "lfnequ.h"
#include "lfnstruc.h"


// Prototype the functions----------------------------------------------

unsigned  compare_lfn(unsigned char * sec_buf_ptr, struct find_context * find_con_ptr,
                unsigned char * file_name_ptr, unsigned char * lfn_store_ptr);

unsigned detect8_3(unsigned char *);
unsigned detect_subdir(struct find_context *, unsigned char *,
                          unsigned char *, unsigned char, unsigned);

unsigned do_dir(void);
unsigned determine_compressed(unsigned);
unsigned determine_fat_size(unsigned);
unsigned find_in_root(struct find_context * , unsigned char *,
                        unsigned char *, unsigned char);

unsigned find_in_sub(struct find_context * , unsigned char *,
                        unsigned char *, unsigned char);

unsigned find_levels(char *);

unsigned find_match(unsigned char * sec_buf_ptr, struct find_context * find_con_ptr, 
             unsigned char * lfn_store_ptr, unsigned char * file_name_ptr, 
                        unsigned char find_attrib, unsigned spec_flags);

unsigned get_clusters(unsigned long);
unsigned get_default_drive(void);
unsigned get_fat_sec(unsigned long);
unsigned get_root_dir_sec(unsigned long);
unsigned get_sec(unsigned char *, unsigned long, unsigned, unsigned);
unsigned get_sub_dir_sec(unsigned long);
unsigned get_switches(int, char * []);
unsigned get_volume_info(unsigned);
unsigned see_if_star(unsigned char * );
unsigned see_if_wild(unsigned char * );

void disp_bytes_free(void);
void display_header(void);
void display_entry(void);
void display_totals(void);
void do_help(void);
void format_entry(void);
void format_long(unsigned char *, unsigned long);
void get_current_dir(unsigned, unsigned char *);
void get_free_space(unsigned);
void get_number_rows(void);
void inc_row_check(void);
void init_context(void);
void store_dir_of(void);
void lfn_or_formatted83(unsigned char * );
void put_together_spec(unsigned char *,  unsigned char *, unsigned char *);
void retrieve_lfn(unsigned char * sec_buf_ptr, struct find_context * find_con_ptr, 
                            unsigned char * lfn_store_ptr);


// Program variables----------------------------------------------------

struct device_params drive_parms;
struct disk_io drive_io;
struct find_context last_find;          //keep track of where left off in searches
struct find_context hold_context;       
struct media_info media_id;
struct rwblock rwblk;
struct SREGS segregs;
union REGS inregs, outregs, regs;

unsigned char dir_of_buf[MAX_PATH_LEN];
unsigned char entry_format_buf[MAX_PATH_LEN + 80];
unsigned char lfn_buf[MAX_PATH_LEN];
unsigned char fat_sec_buf[512];
unsigned char file_sys_buf[255];
unsigned char hold_prev_buf[512];
unsigned char hold_sec_buf[512];
unsigned char prev_root_buf[512];
unsigned char prev_sub_buf[512];
unsigned char root_dir_buf[512];
unsigned char sub_dir_buf[512];
unsigned char work_dir_path[MAX_PATH_LEN];
unsigned char search_attribute;
unsigned char * dir_path_ptr;
unsigned char last_error;

unsigned bits_per_fat_entry;
unsigned byte_offset;
unsigned dir_drive;
unsigned fat_sec;
unsigned levels_in_path;
unsigned number_of_rows;
unsigned rem_bits;
unsigned rem_byte_offset;
unsigned row_count;

unsigned long bit_offset;
unsigned long free_space;
unsigned long hi_cluster;
unsigned long lo_cluster;
unsigned long start_root_dir;
unsigned long start_of_data;
unsigned long tot_bits;
unsigned long tot_bytes_used;
unsigned long tot_number_dirs;
unsigned long tot_number_files;

const char err_access_msg[]="Error accessing drive\n";
const char err_unknown_msg[]="Unknown drive\n";
const char err_notrdy_msg[]="Drive is not ready\n";
const char err_fat32_msg[]="LFNDir doesn't support FAT32 drives\n";
const char err_compres_msg[]="LFNDir doesn't support compressed drives\n";
const char wild_string[]="*.*";
const char dir_string[]="   <DIR>      ";
const char noname_string[]="           ";
const char has_no_string[]="has no label";
const char no_path_msg[]="Path not found\n";
const char no_file_msg[]="File not found\n";

unsigned char serial_present;


unsigned char bare_flag = FALSE;
unsigned char pause_flag = FALSE;
unsigned char lower_flag = FALSE;
unsigned char help_flag = FALSE;
unsigned char attrib_flag = FALSE;
unsigned char not_attrib_flag=FALSE;


unsigned char attrib_tab[]={'D', SUBDIR_ATTRIB,
                            'H', HIDDEN_ATTRIB,
                            'S', SYSTEM_ATTRIB,
                            'R', READONLY_ATTRIB,
                            'A', ARCHIVE_ATTRIB,
                              0, 0
                            };

struct switch_engine switch_table[]= { 

    {BARE_DISP_SWITCH, 0, 0, NULL, FALSE, NULL, &bare_flag, NULL},
    {LOWER_SWITCH, 0, 0, NULL, FALSE, NULL, &lower_flag, NULL},
    {PAUSE_SWITCH, 0, 0, NULL, FALSE, NULL, &pause_flag, NULL},
    {HELP_SWITCH1, 0, 0, NULL, FALSE, NULL, &help_flag, NULL},
    {HELP_SWITCH2, 0, 0, NULL, FALSE, NULL, &help_flag, NULL},
    {ATTRIB_SWITCH, 5, ':', attrib_tab, TRUE, &not_attrib_flag, &attrib_flag, &search_attribute}, 

                                    };                                      

#define num_switch_entries (sizeof(switch_table) / (sizeof(struct switch_engine)))

struct ext_fcb fcb_data=    {0xff, "\0\0\0\0" ,8,0,"??????????",0,0,0,0,0,0,0,0};


                            
// --------------------------------------------------------------------


main(argc, argv)
int argc;
char *argv[]; 
{
	unsigned char * user_path_ptr;
	unsigned char cur_dir_path[MAX_PATH_LEN];
	unsigned x;

	if (get_switches(argc, argv)) exit(1);      //exit if bad switch
	if (help_flag) {
		do_help();
		exit(0);
	}

	if (pause_flag) get_number_rows();          //for pausing, need # rows

	dir_drive=get_default_drive();

	user_path_ptr=NULL;

	if (argc > 1) {
		//find the parm that's not a switch and use it for the filespec
		for (x=2;x<=argc;x++) {
			if (*argv[x-1] != SWITCH_CHAR) {
				user_path_ptr=argv[x-1];
				if ( *(user_path_ptr + 1) == COLON) {
					dir_drive=(toupper(*user_path_ptr) - 'A') + 1;
					user_path_ptr +=2;      //get past drive letter and ':'
				}
				break;
			}  //if arg doesn't begin with a SWITCH_CHAR
		}  //for all potential parameters
	} //if there were arguments specfied by user

	get_current_dir(dir_drive, cur_dir_path);

	//Go load work_dir_path with specifier.  May simply copy what
	//user entered, or generate one from current path (e.g., if
	//"LFNDIR" (with no parms), generate "\curpath\*.*".

	put_together_spec(user_path_ptr, cur_dir_path, work_dir_path);
	dir_path_ptr=work_dir_path;

	//attempt to use int 25h (this will fail on FAT32 volumes)
	if (get_sec(fat_sec_buf, 1, 1, dir_drive)) {
		printf( (char * ) &err_fat32_msg);
		exit(1);
	}

	if (determine_compressed( dir_drive)) {
		printf( (char * ) &err_compres_msg);
		exit(1);
	}

	if (get_volume_info( dir_drive)) {
		printf( (char * ) &err_access_msg);
		exit(1);
	}

	get_free_space(dir_drive);

	//go determine FAT size
	//if I/O error, or unable to determine FAT size, can't continue
	if (!(bits_per_fat_entry=determine_fat_size(dir_drive) ))
		exit(1);

	//go determine how many levels deep we'll need to look
	levels_in_path=find_levels(work_dir_path);

	init_context();             //zero out history of sectors searched

	//go perform DIR processing and exit if error is returned
	if (do_dir()) exit(1);

	if (last_find.fc_path_found == FALSE) {
		display_header();           //put out volume label, etc.
		printf(no_path_msg);
	}

	if (last_find.fc_file_found == FALSE) {
		display_header();           //put out volume label, etc.
		printf(no_file_msg);
		printf("                   ");
		disp_bytes_free();
	}
}

/*-----------------------------------------------------------
| do_dir - Main processing for carrying out DIR.            |
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|   dir_path_ptr = specifier on which to perform the DIR.   |  
|                                                           |  
|                  Note:  this has already been "massaged"; |  
|                  drive letter has been removed;  any      |  
|                  periods indicating use current dir or    |  
|                  parent have been replaced with           |  
|                  appropriate directory.  In these cases   |  
|                  when it could be determined that we're   |  
|                  dealing with a directory, "*.*" has      |  
|                  been placed at the end of the string.    |  
|                                                           |  
| levels_in_path = number of levels to traverse.            |  
|                                                           |  
|        Exit:                                              |  
|              return TRUE if error accessing disk          |  
------------------------------------------------------------*/
unsigned do_dir(void) {
	unsigned list_all=FALSE;
	unsigned char * work_ptr;
	unsigned x;
	unsigned match_flag;
	unsigned match_subdir;
	unsigned char attrib_to_use;

	++dir_path_ptr;    //get past "\" (something is guaranteed to be there)

	work_ptr=dir_path_ptr;


	//determine if there are wildcards
	list_all=see_if_wild(dir_path_ptr);

	match_subdir=match_flag=FALSE;

	//Use normal attributes until we're at the last path element. Otherwise,
	//if user has specified an attribute like "hidden" like this:
	//lfndir \foo1\foo2\*.* /AH
	//We don't want to apply the hidden attribute criterion until the first
	//path elements have been matched.

	attrib_to_use=NORMAL_ATTRIB;

	if (levels_in_path == 1) {
		attrib_to_use=search_attribute; //last one, apply user attribute conditions
		if (!(list_all)) {    //no wildcards specified
			//At the last element of the path, and there were no wild
			//cards specified. Besides looking for what could be just
			//a plain ol' file, there are also two other scenarios to
			//be handled:  (1) the item could be file with no extension (this
			//can also be an LFN with no extension --e.g.,
			//foo123456789 in  which case you would want to find it
			//as well as: foo1223456789.xxx.morestuff),
			//(2) the item could be a subdirectory in which case you wouldn't
			//want to try to find any other files regardless of a common
			//main portion with various other extensions.

			//There's no other way around it, before we attempt to look for
			//files that may match up (and have other extensions), it must
			//first be determined if this is an actual directory. The call
			//below checks for this.
           
			match_subdir=detect_subdir(&last_find, lfn_buf, dir_path_ptr, attrib_to_use, 0);

			//For the case where no extension has been supplied, tell low-level
			//if it can't find a match, to generate a ".*" to the filespec. This
			//saves us a ton of work to let that level deal with it. It in
			//turn will indicate that it's done this by setting the
			//FM_SET_WILD_EXTENSION bit in fc_spec_flags.

		} //if no wildcards

		//This not only takes care of getting foo.exe when user
		//enters: lfndir foo    but it also takes care of giving
		//all files when the user enters: lfndir *

		last_find.fc_spec_flags = GEN_WILD_EXTENSION ;

//if only in the root (levels_in_path == 1)
	} else
		 last_find.fc_spec_flags |= NO_ATTRIB_IF_SUB;

	if (!match_subdir)
		match_flag=find_in_root(&last_find, lfn_buf, dir_path_ptr, attrib_to_use);

	if (match_flag || match_subdir) {
		//can't go further if disk inaccessible
		if (last_find.fc_error) return(TRUE);

		last_find.fc_path_found=TRUE;  //so far, path is found
		last_find.fc_file_found=TRUE;  //so far, files found

		//if only 1 level in path and no wildcards, we're done (if it's
		//not a subdirectory --in which case, we need to go get all the
		//files in there)
		if (levels_in_path == 1 && !(list_all) ) {
			if ((last_find.dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)==SUBDIR_ATTRIB) {
				//basically turn "\subdir" into "\subdir\*.*"
				++levels_in_path;
				work_ptr=dir_path_ptr + strlen(dir_path_ptr);
				*work_ptr++=BACK_SLASH;
				memmove(work_ptr, wild_string, sizeof(wild_string) + 1);
			//not a subdir, we've found the only file
			} else {
				//If low level match engine (in response to our setting
				//GEN_WILD_EXTENSION) detected that the last item searched for
				//had no extension, and was able to find a match by tacking on a ".*"
				//We'll want to fall through and loop through all directory entries
				//just as if user had dynamically specified:   foo.*

				if (last_find.fc_spec_flags & FM_SET_WILD_EXTENSION)
					list_all=TRUE;
				else {
					display_header();           //put out volume label, etc.
					display_entry();
					display_totals();
					return(FALSE);
				}
			}  //else --it's not a subdirectory
		} //if one level and no wildcards

    //Dealing with more than 1 level (e.g., lfndir \foo1\foo2)  --must
    //do a sanity check to ensure that the entry matched on is 
    //actually a subdirectory.

		if (levels_in_path > 1) {
			if ((last_find.dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)!=SUBDIR_ATTRIB) {
				last_find.fc_path_found=FALSE; //path not found
				return(FALSE);
			}
		}
	//if entry was found
	} else {
		if (levels_in_path == 1)
			last_find.fc_file_found=FALSE;
		else
			last_find.fc_path_found=FALSE;
		return(FALSE);     //entry not found
	}

	//if listing all files that appear only in the root (dealing with wildcards)
	if (levels_in_path == 1) {
		display_header();           //put out volume label, etc.
		do {
			if (last_find.fc_error) return(TRUE);
			display_entry();
		} while(find_in_root(&last_find, lfn_buf, dir_path_ptr, attrib_to_use));
		display_totals();

		return(FALSE);      //done
	}

	//if get here, we've found the first subdirectory in the root
	//now, using the cluster specified in directory entry to locate
	//the start of the subdirectory, we'll go search it

	list_all=FALSE;
	last_find.number_dirs=0;    //only count dirs in final dir
	attrib_to_use=NORMAL_ATTRIB;
	last_find.fc_spec_flags = 0;
    
	for (x=1;x<levels_in_path;x++) {
		while(*dir_path_ptr++ != BACK_SLASH);  //advance to next element
		match_subdir=match_flag=FALSE;

		last_find.fc_cluster=last_find.dir_entry_ptr->starting_cluster;
		last_find.fc_cluster_offset=0;
		last_find.fc_entry_num=0;
		last_find.prev_sec_ptr=NULL;
		//store the directory for "directory of" display
		store_dir_of();

		if (x==levels_in_path -1 ) {
			list_all=see_if_wild(dir_path_ptr);
			last_find.number_dirs=0;    //only count dirs in final dir
			attrib_to_use=search_attribute; //last one, apply user attribute conditions
			if (!list_all)
				match_subdir=detect_subdir(&last_find, lfn_buf,dir_path_ptr, attrib_to_use, 1);
			last_find.fc_spec_flags =  GEN_WILD_EXTENSION;
		} else
			last_find.fc_spec_flags |= NO_ATTRIB_IF_SUB;

		if (!match_subdir)
			match_flag=find_in_sub(&last_find, lfn_buf, dir_path_ptr,attrib_to_use);

		match_flag |= match_subdir;

		//can't go further if disk inaccessible
		if (last_find.fc_error) return(TRUE);

		//if not a match and not at end of specified, path wasn't found
		//if at end and not found, it's file not found.

		if (!(match_flag)) {
			if (x !=levels_in_path -1)
				last_find.fc_path_found=FALSE;  //path not found
			else
				last_find.fc_file_found=FALSE;  //file not found
			return(FALSE);    //not found
		}

		//if not at last element of path
		if (x != levels_in_path -1 ) {
			if ((last_find.dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)!=SUBDIR_ATTRIB) {
				last_find.fc_path_found=FALSE; //path not found
				return(FALSE);
			}
		} else {
			//Reached the last one.  Check to see if it's a directory (also
			//check to see if this is only 1 level beyond root --if so,
			//we've already done this)
			if (((last_find.dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)==SUBDIR_ATTRIB) && !(list_all)) {
				++levels_in_path;
				work_ptr=dir_path_ptr + strlen(dir_path_ptr);
				*work_ptr++=BACK_SLASH;
				memmove(work_ptr, wild_string, sizeof(wild_string) + 1);
			}

			//If low level match engine (in response to our setting
			//GEN_WILD_EXTENSION) detected that the last item searched for
			//had no extension, and was able to find a match by tacking on a ".*"
			//We'll want to fall through and loop through all directory entries
			//just as if user had dynamically specified foo.*

			if (last_find.fc_spec_flags & FM_SET_WILD_EXTENSION) list_all=TRUE;
		}
	}   //for x - each element of the path

	display_header();           //put out volume label, etc.

	if (!list_all) {
		display_entry();        //display the file
		display_totals();
		return(FALSE);
	}

	do {
		if (last_find.fc_error) return(TRUE);
		display_entry();
	} while(find_in_sub(&last_find, lfn_buf, dir_path_ptr, attrib_to_use));

	display_totals();
	return(FALSE);
}

void store_dir_of() {
	unsigned dir_of_len;

	//store lfn for "directory of" display
	//Go place the lfn (if it exists --otherwise place the formatted 8.3)
	//into the "directory of" hold buffer

	dir_of_buf[last_find.last_level_off++]=BACK_SLASH;
	lfn_or_formatted83(&dir_of_buf[last_find.last_level_off]);
	dir_of_len=strlen(&dir_of_buf[last_find.last_level_off]);

	//note location for placing next portion
	last_find.last_level_off += dir_of_len;
	dir_of_buf[last_find.last_level_off]=0;
}    


void lfn_or_formatted83(unsigned char * disp_buf_ptr) {
	unsigned x;

	if (lfn_buf[0] == '\0') {    //if no lfn
		for (x=0;x<sizeof(last_find.dir_entry_ptr->filename);x++) {
			if (last_find.dir_entry_ptr->filename[x] == ' ') break;
			*disp_buf_ptr++=last_find.dir_entry_ptr->filename[x];
		}
		//don't add the dot when listing the "." and ".." <dir> entries
		if (!(x < 3 && last_find.dir_entry_ptr->filename[x - 1] == DOT ))
			if (last_find.dir_entry_ptr->fileext[0] != ' ')
				*disp_buf_ptr++=DOT;

		for (x=0;x<sizeof(last_find.dir_entry_ptr->fileext);x++) {
			if (last_find.dir_entry_ptr->fileext[x] == ' ') break;
			*disp_buf_ptr++=last_find.dir_entry_ptr->fileext[x];
		}

		*disp_buf_ptr++=0;
	} else
		 sprintf(disp_buf_ptr, "%s", lfn_buf);
}

/*-----------------------------------------------------------
| find_in_root - search root directory for a specified      |
|                entry.                                     |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|   fc_ptr->fc_root_sec = sector of root dir to start       |
|                         search.                           |  
|                                                           |  
| fc_ptr->fc_root_entry = entry within current dir sector   |
|                         to begin this search.             |  
|                                                           |  
|              path_ptr = ptr to item being searched for.   |
|                                                           |  
|              lfnb_ptr = ptr to buffer in which to place   |  
|                         long filename counterpart to      |  
|                         this entry.                       |  
|                                                           |  
|           file_attrib = attribute to also be matched.     |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|              return TRUE if found                         |  
|                                                           |  
|              fc_error in find_context struct holds any    |  
|              error code from the disk I/O                 |  
|                                                           |  
------------------------------------------------------------*/
unsigned find_in_root(struct find_context * fc_ptr , unsigned char * lfnb_ptr,
												unsigned char * path_ptr, unsigned char file_attrib) {
	unsigned x;
	for (x= fc_ptr->fc_root_sec;x< (drive_parms.dp_bpb_de / ENTRIES_PER_SEC); x++) {
		if (fc_ptr->fc_root_entry == 0) { //if starting at first entry in sector
			//if not at the first sector, store previous
			if (x != 0) fc_ptr->prev_sec_ptr=prev_root_buf;

			//go get the next root dir sector --exit if error
			if (get_root_dir_sec(x)) return(FALSE);
		}

		//Tell find_match which dir entry at which to start search.
		//(Separate hold values are maintained for root and for
		//currently being searched entry.)
		fc_ptr->fc_entry_num=fc_ptr->fc_root_entry;
		fc_ptr->fc_root_sec=x;

		if (find_match(root_dir_buf, fc_ptr, lfnb_ptr,path_ptr, file_attrib, fc_ptr->fc_spec_flags )) {
			//note where found
			fc_ptr->fc_root_entry=fc_ptr->fc_entry_num + 1;

			if ( fc_ptr->fc_root_entry == ENTRIES_PER_SEC) {
				fc_ptr->fc_root_entry = 0;
				++fc_ptr->fc_root_sec;
			}
			return(TRUE);
		}
		fc_ptr->fc_root_entry = 0;

		//can't go further if disk inaccessible
		if (fc_ptr->fc_error) break;
	}  //for x - all root dir sectors
	return(FALSE);
}

/*-----------------------------------------------------------
| find_in_sub - search subdirectory for a specified entry.  |
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|        fc_ptr->fc_cluster = cluster to start search.      |
|                                                           |  
| fc_ptr->fc_cluster_offset = sector offset within cluster  |
|                             to begin this search.         |  
|                                                           |  
|            fc_ptr->fc_sec = sector to start search (note  |
|                             if entry offset == 0 and      |  
|                             cluster offset == 0, we       |  
|                             are just starting search of   |  
|                             this cluster, so this sector  |  
|                             value is calculated).         |  
|                                                           |  
|      fc_ptr->fc_entry_num = directory entry within sector |
|                             to start search.              |  
|                                                           |  
|          (other parms same as root dir search)            |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|              return TRUE if found                         |  
|                                                           |  
|              fc_error in find_context struct holds any    |  
|              error code from the disk I/O                 |  
|                                                           |  
------------------------------------------------------------*/
unsigned find_in_sub(struct find_context * fc_ptr , unsigned char * lfnb_ptr,
												unsigned char * path_ptr, unsigned char file_attrib) {

	unsigned clust_off, match_flag;

	while(TRUE) {
		//First, check to see if last search reached end of cluster. If it
		//did, go read FAT and get next entry.

		if (fc_ptr->fc_cluster_offset == drive_parms.dp_bpb_au) {
			//get next in chain, return 0 if end of chain
			if ((fc_ptr->fc_cluster=get_clusters(fc_ptr->fc_cluster)) == 0) break;
			fc_ptr->fc_cluster_offset=0;
			fc_ptr->fc_entry_num=0;
		}

		for (clust_off=fc_ptr->fc_cluster_offset; clust_off < drive_parms.dp_bpb_au; ++clust_off) {
			fc_ptr->fc_cluster_offset = clust_off;
			if (clust_off == 0 && fc_ptr->fc_entry_num == 0)
				fc_ptr->fc_sec=( ((fc_ptr->fc_cluster - 2) * drive_parms.dp_bpb_au)+ start_of_data) -1;
			if (!(fc_ptr->fc_entry_num)) { //if no offset into a sector
				if (clust_off !=0)
					fc_ptr->prev_sec_ptr=prev_sub_buf;
				//go get the next dir sector --exit if error
				if (get_sub_dir_sec(fc_ptr->fc_sec)) return(FALSE);
			}

			match_flag=find_match(sub_dir_buf, fc_ptr, lfnb_ptr,path_ptr, file_attrib, fc_ptr->fc_spec_flags );
			++fc_ptr->fc_entry_num;

			if (fc_ptr->fc_entry_num == ENTRIES_PER_SEC ) {
				fc_ptr->fc_entry_num=0;
				++fc_ptr->fc_sec;
				fc_ptr->fc_cluster_offset=clust_off + 1;
			}

			if (match_flag) return(TRUE);

			//can't go further if disk inaccessible
			if (fc_ptr->fc_error) return(FALSE);
		}  //for clust_off --all sectors within current cluster
	}  //while --stay until end of cluster chain reached
	return(FALSE);
}

/*-----------------------------------------------------------
| detect_subdir - search root or other directory to         |
|                 determine whether the specfied entry      |  
|                 is a directory. If found, contexts are    |  
|                 updated, as callers will want to pick     |  
|                 up from the found directory. If a         |  
|                 directory isn't found, contexts are       |  
|                 preserved.                                |  
|                                                           |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|   Parmameters are the same as the find_in_root type       |
|   functions, with one additional parameter:               |  
|                                                           |  
|        dir_for_search = 0 indicates root, 1 indicates     |
|                         current directory.                |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|              return TRUE if found                         |  
|                                                           |  
------------------------------------------------------------*/
unsigned detect_subdir(struct find_context * fc_ptr , unsigned char * lfnb_ptr,
                        unsigned char * path_ptr, unsigned char file_attrib,
												unsigned dir_for_search) {
	unsigned match_flag;

	//save contents of previous sector buffer (this is used for retrieving LFNs
	memmove(hold_prev_buf, fc_ptr->prev_sec_ptr, sizeof(hold_prev_buf));

	//save entire context structure. If subdirectory isn't found, we'll
	//restore everything so that other searches can start from this location
	memmove(&hold_context, fc_ptr, sizeof (struct find_context));

	fc_ptr->fc_spec_flags = NO_ATTRIB_IF_SUB;

	if (dir_for_search == 0) {
		memmove(hold_sec_buf, root_dir_buf, sizeof(hold_sec_buf));
		if (!(match_flag=find_in_root(fc_ptr, lfn_buf, path_ptr, file_attrib)))
			memmove(root_dir_buf, hold_sec_buf, sizeof(root_dir_buf));
	} else {
		memmove(hold_sec_buf, sub_dir_buf, sizeof(sub_dir_buf));
		if (!(match_flag=find_in_sub(fc_ptr, lfn_buf, path_ptr, file_attrib)))
			memmove(sub_dir_buf, hold_sec_buf, sizeof(sub_dir_buf));
	}

	if (match_flag)
		if ((fc_ptr->dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)== SUBDIR_ATTRIB) return(TRUE);

	memmove(fc_ptr, &hold_context, sizeof (struct find_context));
	memmove(fc_ptr->prev_sec_ptr, hold_prev_buf, sizeof(hold_prev_buf));
    
	return(FALSE);
}

/*-----------------------------------------------------------
| get_clusters - Given a cluster number, go to the FAT and  |
|                find the next cluster in the chain.        |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
| bits_per_fat_entry = number of bits per entry (either     |  
|                      12 or 16 --FAT 32 not supported)     |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|              return next cluster (or zero if at end)      |  
|                                                           |  
------------------------------------------------------------*/
unsigned get_clusters(unsigned long cluster) {

	tot_bits=(unsigned long) (cluster * (unsigned long) bits_per_fat_entry);
	fat_sec=(tot_bits/BITS_PER_SEC);
	rem_bits=(tot_bits % BITS_PER_SEC);

	bit_offset = rem_bits;

	//return 0 if I/O error
	if (get_fat_sec(fat_sec)) return(0);

	byte_offset=bit_offset/8;
	rem_byte_offset=bit_offset % 8;
	lo_cluster=(unsigned long) ( (unsigned char) fat_sec_buf[byte_offset]);
	if (rem_byte_offset)
		lo_cluster = lo_cluster >> 4;
	++byte_offset;
	if ((BITS_PER_SEC - tot_bits) < bits_per_fat_entry) {
		++fat_sec;
		if (get_fat_sec(fat_sec)) return(0);
		byte_offset=0;
	}
	hi_cluster=(unsigned long) ( (unsigned char) fat_sec_buf[byte_offset]);

	if (rem_byte_offset == 0) {
		if (bits_per_fat_entry == 12) {
			hi_cluster = hi_cluster & 0x0f;
			hi_cluster = hi_cluster << 8;
		} else
			hi_cluster = hi_cluster << 8;
	} else
		if (bits_per_fat_entry == 12)
			hi_cluster = hi_cluster << 4;

	lo_cluster = lo_cluster | hi_cluster;

	//if at end of chain, return 0
	if ( (bits_per_fat_entry == 12 && lo_cluster == 0x0fff) ||
			(bits_per_fat_entry == 16 && lo_cluster == 0x0ffff)) return(0);
	return( (unsigned) lo_cluster);
}

//Note: These functions which access sectors for the root directory, sub-
//      directory, and FAT, could possibly be enhanced to use a 
//      caching scheme.
unsigned get_root_dir_sec(unsigned long root_sec) {

	//store previous buffer (may need for retrieving LFNs)
	memmove(prev_root_buf, root_dir_buf, sizeof(prev_root_buf));
	return(get_sec(root_dir_buf, root_sec + start_root_dir, 1, dir_drive));
}

unsigned get_sub_dir_sec(unsigned long dir_sec) {

	//store previous buffer (may need for retrieving LFNs)
	memmove(prev_sub_buf, sub_dir_buf, sizeof(prev_sub_buf));
	return(get_sec(sub_dir_buf, dir_sec, 1, dir_drive));
}

unsigned get_fat_sec(unsigned long fat_sec) {
	return(get_sec(fat_sec_buf, fat_sec + 1, 1, dir_drive));
}

unsigned  get_sec(unsigned char * sec_buf_ptr, unsigned long sec_num,
												 unsigned scount, unsigned gsdrive) {
	struct disk_io far * rw_ptr;

	rw_ptr=&drive_io;
	rw_ptr->dio_startsec=sec_num;
	rw_ptr->dio_numsecs=scount;     //number of sectors to read
	rw_ptr->dio_buf_ptr=sec_buf_ptr;

	inregs.h.al=gsdrive - 1;        //0=a:, 1=b:, etc.
	inregs.x.cx=0xffff;             //indicate to get info from struct
	segregs.ds=FP_SEG( (void far * ) rw_ptr);
	inregs.x.bx=FP_OFF( (void far *) rw_ptr);

	int86x(0x25, &inregs, &outregs, &segregs);
	last_error=outregs.h.al;
	return (outregs.x.cflag);
}

unsigned see_if_star(unsigned char * work_ptr) {

	while(*work_ptr != '\0' && *work_ptr != BACK_SLASH) {
		if (*work_ptr == '*') return(TRUE);
		++work_ptr;
	}
	return(FALSE);
}

unsigned see_if_wild(unsigned char * work_ptr) {

	while(*work_ptr != '\0' && *work_ptr != BACK_SLASH) {
		if (*work_ptr == '?' || *work_ptr == '*') return(TRUE);
		++work_ptr;
	}
	return(FALSE);
}

/*-----------------------------------------------------------
| find_match - Attempt to find matching directory entry.    |
|                                                           |  
|       Enter:                                              |  
|         sec_buf_ptr = ptr to start of directory sector    |
|                                                           |  
|        find_con_ptr = ptr to context of find operations   |  
|                                                           |  
|       lfn_store_ptr = ptr to area where LFN should be     |  
|                       placed.                             |  
|                                                           |  
|       file_name_ptr = ptr to element we're looking for.   |  
|                       This may be a subdirectory or a     |  
|                       file.  Drive letter and or "\"      |  
|                       have been removed from first part   |  
|                       of string.  If may contain wildcard |  
|                       chars (? or *).  It may be an LFN   |  
|                       or 8.3 string.  We search until     |  
|                       hit terminating "\" or NULL.        |  
|                                                           |  
|                                                           |  
|         find_attrib = only match on dir entries that      |  
|                       have this attribute.                |  
|                                                           |  
|                                                           |  
|          spec_flags = caller can place values in here to  |  
|                       handle special situations.  These   |  
|                       take care of things that would      |  
|                       really be a hassle otherwise.       |  
|                                                           |  
|                       If specfied (non zero) values can   |  
|                       be:                                 |  
|                                                           |  
|                       Value    Meaning                    |  
|                       -----    --------                   |  
|                                                           |  
|            NO_ATTRIB_IF_SUB    Don't check attribute if   |  
|                                item turns out to be a     |  
|                                directory.                 |  
|                                                           |  
|                                                           |  
|          GEN_WILD_EXTENSION    If there's no extension    |  
|                                on a file being searched   |  
|                                for, match it as if        |  
|                                there's a ".*" at end.     |  
|                                                           |  
|                                                           |  
|        Exit:                                              |  
|          Return TRUE if match found and store LFN         |  
|          Find context updated                             |  
|                                                           |  
|                                                           |  
|                                                           |  
------------------------------------------------------------*/
unsigned find_match(unsigned char * sec_buf_ptr, struct find_context * find_con_ptr, 
					unsigned char * lfn_store_ptr, unsigned char * file_name_ptr,
					unsigned char find_attrib, unsigned spec_flags) {
	unsigned isit_8_3=FALSE;
	unsigned no_extension;
	unsigned match_flag;
	unsigned entry_to_start;
	struct dir_layout * dir_entry_ptr;
	unsigned x, y;
	unsigned char * start_file_name;
	unsigned char work_attrib;

	start_file_name=file_name_ptr;

	isit_8_3=detect8_3(file_name_ptr);
    
	entry_to_start=find_con_ptr->fc_entry_num;

	dir_entry_ptr=(struct dir_layout *) (sec_buf_ptr + (LEN_DIR_ENTRY * find_con_ptr->fc_entry_num));
	for (x=entry_to_start; x< ENTRIES_PER_SEC; match_flag=FALSE,x++,++dir_entry_ptr) {
		find_con_ptr->fc_entry_num=x; //note the entry last searched
		find_con_ptr->dir_entry_ptr=dir_entry_ptr;  //and the ptr to it's data
		file_name_ptr=start_file_name;  //beginning of filename for searching
		//Locate only the alias first --skip LFN entries
		if ( (dir_entry_ptr->file_attrib & LFN_ATTRIB) ==  LFN_ATTRIB)  continue;
		//not interested in volume labels
		if ( (dir_entry_ptr->file_attrib & VOLUME_ATTRIB) ==  VOLUME_ATTRIB)  continue;
		//The following handles special request processing by caller.  If
		//the special handling code of NO_ATTRIB_IF_SUB is specified, don't
		//apply attribute criterion to the item. Callers usually set this
		//prior to searching for the last path element.
		//If the item is a file, then the attribute criterion should
		//be applied. If it's a sub directory, caller will want to match on
		//this, then generate another search of "*.*"

		if (!((find_con_ptr->fc_spec_flags & NO_ATTRIB_IF_SUB)  &&
						(dir_entry_ptr->file_attrib & SUBDIR_ATTRIB))) {
			if (((find_attrib == NORMAL_ATTRIB) && (dir_entry_ptr->file_attrib & 0x0f) > READONLY_ATTRIB))
				continue;

			//Get only only entries that match desired attribute
			//check for NOT first (it will be the NOT of attributes, so upper
			//bits being on indicate '-' was specified)
        
			if (find_attrib & 0x80) {
				work_attrib = ~find_attrib;
				if (  (dir_entry_ptr->file_attrib & work_attrib) == work_attrib) continue;
			} else
				if ((dir_entry_ptr->file_attrib & find_attrib) != find_attrib)
					continue;
		}  //if it's not a subdirectory (when NO_ATTRIB_IF_SUB option being used)

		if (dir_entry_ptr->filename[0] == 0 || dir_entry_ptr->filename[0] == ERASE_FLAG) continue;

		retrieve_lfn(sec_buf_ptr, find_con_ptr, lfn_store_ptr);
		match_flag=TRUE;    //default to having found a match
		no_extension=FALSE;

		//if item being searched for is an LFN, go use LFN compare logic
		if (!(isit_8_3)) {
			if(compare_lfn(sec_buf_ptr, find_con_ptr,start_file_name, lfn_store_ptr))
				return(TRUE);
			else
				continue;
		}

		for (y=0;y<8;y++,++file_name_ptr) {      //main portion of file name
			if (*file_name_ptr == '?') continue;  //if wild card char

			//Note that normal DOS only really deals with wildcards for
			//file names. For example:
			//          dir subnam*       will find the subdir
			//
			//but, if you do this:  dir subnam*\*.*   --it won't find the
			//subdirectory
			//lfndir allows wildcards to be specified as part of subdirectories --
			//in such cases, it will find the first match.
			if (*file_name_ptr == '*') {
				++file_name_ptr;
				break;
			}

			if ( (*file_name_ptr == BACK_SLASH || *file_name_ptr == '\0')
					 && dir_entry_ptr->filename[y] == SPACE ) break;

			//determine if hit '.' and main portion dir name at end
			if (y > 0 && *file_name_ptr == DOT && dir_entry_ptr->filename[y] == SPACE)
				 break;
			if ((toupper(*file_name_ptr)) != (toupper(dir_entry_ptr->filename[y]))) {
				 match_flag=FALSE;           //no match
				 break;
			}

		} //for Y - each char in main portion of file name

		//if no match, check the 8.3 item being seached for to see if
		//wildcard was specifed. It's possible that even though it
		//looks like an 8.3, it will only match on the LFN. For example,
		//
		//if the filename is: this123456789
		//the alias will be: this12~1
		//
		//if we're looking for this123*
		//
		//it looks like an 8.3, but will only match on the LFN
		if (match_flag == FALSE) {
			if (see_if_star(start_file_name))
				if (compare_lfn(sec_buf_ptr, find_con_ptr,start_file_name, lfn_store_ptr))
					 return(TRUE);
				else
					 continue;

			continue; //go to next dir entry and try again
		}

		//if element ends with '\\', we're looking for a subdirectory
		if (*file_name_ptr == BACK_SLASH && (dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)
			 !=SUBDIR_ATTRIB) continue;

		if (*file_name_ptr == BACK_SLASH || *file_name_ptr == '\0')
			no_extension=TRUE;

		/* do some checking to handle the extension situation
            LFNDIR *        Gives you everything (as does LFNDIR *.*  --
             caller takes care of this one by setting GEN_WILD_EXTENSION )
						LFNDIR *.       Gives you only files with no extensions
		 also   LFNDIR FOO.     Gives you only "FOO" as main with no extensions
		*/

		if (no_extension == FALSE) {
			if (*file_name_ptr == DOT) {
				++file_name_ptr; //advance to extension

				//if file spec ends with a dot, don't want to get
				//matching files that contain extensions
				if (*file_name_ptr == BACK_SLASH || *file_name_ptr == '\0')
					find_con_ptr->fc_spec_flags &= ~GEN_WILD_EXTENSION;
			}
			if (*file_name_ptr == BACK_SLASH || *file_name_ptr == '\0')
				no_extension=TRUE;
		}

		for (y=0;y<3;y++) {       //extension portion of name
			//if there must be no extension to qualify as a match
			if (no_extension) {
				if (dir_entry_ptr->fileext[y] != SPACE) {
					if (find_con_ptr->fc_spec_flags & GEN_WILD_EXTENSION ) {
						//treat as if filespec had ".*" at the end and
						//indicate to caller that this has been done
						find_con_ptr->fc_spec_flags |= FM_SET_WILD_EXTENSION;
						break;
					}
					match_flag=FALSE;           //no match
					break;
				}
				continue;
			}  //if there's no extension

			if (*file_name_ptr == '*') {
				++file_name_ptr;
				break;
			}

			//if hit end, the remaining part of the directory
			//entry must be blank.
			if (*file_name_ptr == BACK_SLASH || *file_name_ptr == '\0') {
				if (dir_entry_ptr->fileext[y] == SPACE) break;
					match_flag=FALSE;
				break;
			}

			//if not single char wild card --these will simply fall through
			if (*file_name_ptr != '?')
				if ((toupper(*file_name_ptr)) != (toupper(dir_entry_ptr->fileext[y]))) {
					match_flag=FALSE;           //no match
					break;
				}

			++file_name_ptr;
		}  // for y - each character in the extension portion of dir entry

		if (match_flag == FALSE) continue;

		//if element ends with '\\', we're looking for a subdirectory
		if (*file_name_ptr == BACK_SLASH && (dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)
			!=SUBDIR_ATTRIB) continue;
		else
			break;

	} // for X - each directory entry in the sector being inspected

	if (match_flag)
		//If option for specified to match on files regardless of
		//extension. Indicate that at least one match was found
		//even though there was no extension at all --this will
		//in turn cause upper level to make multiple calls to
		//look for further matches with varying extensions.
		if ((find_con_ptr->fc_spec_flags & GEN_WILD_EXTENSION) && no_extension )
			find_con_ptr->fc_spec_flags |= FM_SET_WILD_EXTENSION;

	//add to the total number of directories or files found (and number
	//of total bytes occupied by the files)
	if (match_flag) {
		if ((dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)==SUBDIR_ATTRIB)
			++find_con_ptr->number_dirs;
		else {
			++find_con_ptr->number_files;
			find_con_ptr->bytes_used += dir_entry_ptr->file_size;
		}
	}
	return (match_flag);
}

/*-----------------------------------------------------------
| compare_lfn - helper function to find_match. Do all the   |
|               same stuff, except compare specifically on  |  
|               the LFN.                                    |  
|       Enter:                                              |  
|                                                           |  
|         sec_buf_ptr = ptr to start of directory sector    |
|                                                           |  
|        find_con_ptr = ptr to context of find operations   |  
|                                                           |  
|       lfn_store_ptr = ptr to area where LFN has been      |  
|                       placed.                             |  
|                                                           |  
|       file_name_ptr = ptr to element we're looking for.   |  
|                       This should be an LFN.              |  
|                       have this attribute.                |  
|                                                           |  
|          spec_flags = caller can place values in here to  |  
|                       handle special situations.  These   |  
|                       take care of things that would      |  
|                       really be a hassle otherwise.       |  
|                                                           |  
|                       If specfied (non zero) values can   |  
|                       be:                                 |  
|                                                           |  
|                       Value    Meaning                    |  
|                       -----    --------                   |  
|                                                           |  
|            NO_ATTRIB_IF_SUB    Don't check attribute if   |  
|                                item turns out to be a     |  
|                                directory.                 |  
|                                                           |  
|                                                           |  
|          GEN_WILD_EXTENSION    If there's no extension    |  
|                                on a file being searched   |  
|                                for, match it as if        |  
|                                there's a ".*" at end.     |  
|                                                           |  
|                                                           |  
|        Exit:                                              |  
|          Return TRUE if match found                       |  
|          Find context updated                             |  
|                                                           |  
------------------------------------------------------------*/
unsigned  compare_lfn(unsigned char * sec_buf_ptr, struct find_context * find_con_ptr,
		 unsigned char * file_name_ptr, unsigned char * lfn_store_ptr) {

	unsigned match_flag=TRUE;

	//no compare if LFN doesn't exist
	if (*lfn_store_ptr == 0) return(FALSE);

	for(;;++file_name_ptr, ++lfn_store_ptr) {
		if((*file_name_ptr=='\0' || *file_name_ptr==BACK_SLASH) && *lfn_store_ptr=='\0')
			break;
		//above, if hit the end of both, it was a match

		//if caller has specified in flags that we should treat
		//the file as if it had ".*", see if this will
		//enable it to match and tell caller this occurred.
		if (find_con_ptr->fc_spec_flags & GEN_WILD_EXTENSION )
			if (*file_name_ptr == '\0' && *lfn_store_ptr == DOT) {
				//treat as if filespec had ".*" at the end and
				//indicate to caller that this has been done
				find_con_ptr->fc_spec_flags |= FM_SET_WILD_EXTENSION;
				break;
			}

		//if have: 123456789.xxx.yyy and enter: 123456789.
		//don't want files that contain anything beyond the period

		if (*file_name_ptr == DOT && *(file_name_ptr + 1) == '\0')
			if (*lfn_store_ptr == '\0') break;

		//if have: 123456789 and enter: 123456789.*
		//also want to match

		if (*file_name_ptr == DOT && *(file_name_ptr + 1) == '*')
			if (*lfn_store_ptr == '\0') break;

		if (*file_name_ptr == '*') break;
		if (*file_name_ptr == '?' && *lfn_store_ptr != '\0') continue;
		//below, if hit the end of either, it's no match
		if ((*file_name_ptr=='\0' || *file_name_ptr==BACK_SLASH)|| *lfn_store_ptr=='\0') {
			 match_flag=FALSE;
			 break;
		}
        
		if ((toupper(*file_name_ptr)) != (toupper(*lfn_store_ptr))) {
			match_flag=FALSE;           //no match
			break;
		}
	}  //while checking lfn

	if (match_flag) {
		if ((find_con_ptr->dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)==SUBDIR_ATTRIB)
			++find_con_ptr->number_dirs;
		else {
			++find_con_ptr->number_files;
			find_con_ptr->bytes_used += find_con_ptr->dir_entry_ptr->file_size;
		}

		//matched on LFN. We may or may not have made the
		//match happen by making the extension wild. LFNs
		//can contain so many period characters...just set
		//flag so that upper level will try additional finds
		//using the wild card extension feature.

		if (find_con_ptr->fc_spec_flags & GEN_WILD_EXTENSION)
			find_con_ptr->fc_spec_flags |= FM_SET_WILD_EXTENSION;
	}  //if match

	return(match_flag);
}

/*-----------------------------------------------------------
| retrieve_lfn - Found an alias.  Now attempt to retrieve   |
|                the associated LFN and store it.  LFNs     |  
|                must directly precede the alias (this      |  
|                can be in previous directory sector if     |  
|                necessary).                                |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|         sec_buf_ptr = ptr to start of directory sector    |
|                                                           |  
|        find_con_ptr = ptr to context of find operations   |  
|                       important info in structure:        |  
|                                                           |  
|                       prev_sec_ptr (ptr to previous dir   |  
|                       sector contents)                    |  
|                                                           |  
|                       fc_entry_num (the last searched     |  
|                       entry in the current dir sector)    |  
|                                                           |  
|                                                           |  
|       lfn_store_ptr = ptr to area where LFN should be     |  
|                       placed.                             |  
|                                                           |  
|                                                           |  
|        Exit:                                              |  
|          Store the LFN.  If not found, store NULL         |  
|                                                           |  
------------------------------------------------------------*/
void retrieve_lfn(unsigned char * sec_buf_ptr, struct find_context * find_con_ptr, 
								 unsigned char * lfn_store_ptr) {

	unsigned x, z;
	struct lfn_dir_layout * lfn_dir_ptr;
	unsigned char entry_start_search;
	unsigned char * start_lfn_ptr;
	unsigned safety_count=0;

	start_lfn_ptr=lfn_store_ptr;

	*lfn_store_ptr='\0';            //default to not found
	if (find_con_ptr->fc_entry_num > 0) {
		entry_start_search=find_con_ptr->fc_entry_num - 1;
		lfn_dir_ptr = (struct lfn_dir_layout * )
			(sec_buf_ptr + (entry_start_search * LEN_DIR_ENTRY));
	} else {
		if (find_con_ptr->prev_sec_ptr == NULL) return;
		lfn_dir_ptr = (struct lfn_dir_layout * )
			(find_con_ptr->prev_sec_ptr + ((ENTRIES_PER_SEC - 1) * LEN_DIR_ENTRY));
		entry_start_search=ENTRIES_PER_SEC - 1;
	}

	while(1) {
		//Note: LFNs are stored in Unicode. It would be nice to have
		//a function such as the Win 95 IFS manager UniToBCS
		//service to easily convert it to the base characer set. Since it's
		//not that simple in vanilla DOS, just pull out the first byte
		//of each Unicode character.

		for (x=0;x<=entry_start_search;x++) {
			//just keep copying until a null is reached (or until an
			//entry that isn't an lfn is reached --if the lfn fits
			//exactly in an entry, there's no null).

			++safety_count;

			if (( (lfn_dir_ptr->lfn_dir_atrrib & LFN_ATTRIB)  != LFN_ATTRIB)
					 || safety_count > MAX_ENTRIES_FOR_LFN) {
				*lfn_store_ptr='\0';    //terminate it
				return;
			}

			for (z=0;z<sizeof(lfn_dir_ptr->lfn_name_area1); z++, z++) {
				*lfn_store_ptr++=lfn_dir_ptr->lfn_name_area1[z];
				if (!lfn_dir_ptr->lfn_name_area1[z]) return;
			}

			for (z=0;z<sizeof(lfn_dir_ptr->lfn_name_area2); z++, z++) {
				 *lfn_store_ptr++=lfn_dir_ptr->lfn_name_area2[z];
				 if (!lfn_dir_ptr->lfn_name_area2[z]) return;
			}

			for (z=0;z<sizeof(lfn_dir_ptr->lfn_name_area3); z++, z++) {
				 *lfn_store_ptr++=lfn_dir_ptr->lfn_name_area3[z];
				 if (!lfn_dir_ptr->lfn_name_area3[z]) return;
			}

			--lfn_dir_ptr;  //move back in sector buffer to preceding entry
		} //for x -- entries in the directory sector

		//if still haven't found all of the LFN, we need to need to
		//retrieve the rest from the previous directory sector (if it exists)
		if (*find_con_ptr->prev_sec_ptr == '\0') {
			*start_lfn_ptr='\0';    //disregard any previous stuff
			return;
		} else {
			lfn_dir_ptr = (struct lfn_dir_layout * )
				(find_con_ptr->prev_sec_ptr + ((ENTRIES_PER_SEC - 1) * LEN_DIR_ENTRY));
			entry_start_search=ENTRIES_PER_SEC - 1;
		}
	} //while haven't retrieved all of LFN (or hit sanity checks --if
		//for some reason, things are corrupted)
}

/*-----------------------------------------------------------
| detect8_3 - called to determine if a path element is      |
|             is 8.3 style or an LFN.                       |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|            name_ptr = ptr to path element to inspect      |
|                                                           |  
|        Exit:                                              |  
|          Return TRUE if this is an 8.3 type file or       |  
|          subdirectory name.  Return FALSE if it's an LFN. |  
|                                                           |  
------------------------------------------------------------*/
unsigned detect8_3(unsigned char * name_ptr ) {

	unsigned x;

	//LFNs can actually have periods anywhere in the filename --even
	//as the first character.

	if (*name_ptr == DOT) return (FALSE);

	for (x=0;x<8;x++,++name_ptr) {
		//spaces only allowed in LFNs
		if (*name_ptr == SPACE) return (FALSE);
		if (*name_ptr == '\0' || *name_ptr == BACK_SLASH || *name_ptr == '*') return (TRUE);
			if (*name_ptr == DOT) break;
	}

	if (x==8) {
		if(*name_ptr != DOT && *name_ptr != '\0' && *name_ptr != BACK_SLASH )
			return (FALSE);
		if (*name_ptr == '\0' || *name_ptr == BACK_SLASH) return(TRUE);
	}
   
	++name_ptr;     //get past the period
	for (x=0;x<4;x++,++name_ptr) {
		//spaces only allowed in LFNs
		if (*name_ptr == SPACE) return (FALSE);
		if (*name_ptr == '\0' || *name_ptr == BACK_SLASH || *name_ptr == '*') return (TRUE);
		if (*name_ptr == DOT) return (FALSE);
	}

	return(FALSE);
}

/*-----------------------------------------------------------
| get_volume_info - Use Get Device Parameters IOCTL to      |
|                   retrieve characteristics of drive.      |  
|                   Also, given the information, calculate  |  
|                   and store away the sector numbers where |  
|                   the root directory and data areas begin.|
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|            drive_no = number of drive to check            |
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|      start_root_dir = first sector for root directory     |  
|       start_of_data = first sector of data area           |  
|                                                           |  
|          Return TRUE if error                             |  
|                                                           |  
|          If successful, drive_parms structure is filled   |  
|          in with the drive information.                   |  
|                                                           |  
|                                                           |  
------------------------------------------------------------*/
unsigned get_volume_info(unsigned drive_no) {

	struct device_parms far * dev_ptr;

	dev_ptr = (struct device_parms far * ) &drive_parms;
	drive_parms.dp_spec_func = 1;   //retrieve for current drive

	inregs.x.ax=0x440d;
	inregs.x.bx=drive_no;
	inregs.h.ch=MAJ_CODE;               //must be 08
	inregs.h.cl=GET_DEVICE_PARMS;       //get device parms function
	segregs.ds=FP_SEG( (void far * ) dev_ptr);
	inregs.x.dx=FP_OFF( (void far *) dev_ptr);

	int86x(0x21, &inregs, &outregs, &segregs);

	start_root_dir=(drive_parms.dp_bpb_fs * drive_parms.dp_bpb_nf) + 1;
	start_of_data=start_root_dir + (drive_parms.dp_bpb_de  / ENTRIES_PER_SEC) + 1;

	return (outregs.x.cflag);
}

unsigned get_default_drive(void) {

	inregs.h.ah=GET_DEFAULT_DRIVE;
	int86x(0x21, &inregs, &outregs, &segregs);
	++outregs.h.al;     //want a: = 1 not zero, etc.
	return ( (unsigned) outregs.h.al);
}

/*-----------------------------------------------------------
| get_switches - find any switches user may have supplied.  |
|                Set corresponding flags. If an invalid     |  
|                switch is detected, display a message and  |  
|                and return TRUE.                           |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
| switch_table = array of switch_engine entries. These 
|                control parsing and assigning of switch    |  
|                values.                                    |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|          Return TRUE if invalid switch is detected.       |  
|                                                           |  
|                                                           |  
------------------------------------------------------------*/
unsigned get_switches(int argc, char * argv[]) {

	unsigned x, z, y;
	char * parm_ptr;
	char * option_ptr;

	search_attribute=NORMAL_ATTRIB;

	for (x=2;x<=argc;x++) {
		parm_ptr=argv[x-1];

		if (*parm_ptr != SWITCH_CHAR) continue; //skip if not a switch
		++parm_ptr;
		for (z=0;z<num_switch_entries ;z++) {
			if (toupper(*parm_ptr) != switch_table[z].swit_letter) continue;
			*(switch_table[z].set_if_switch)=TRUE; //indicate switch found

			//found the switch, now check for further parms
			if (switch_table[z].swit_req == 0) break;
			++parm_ptr;

			//check to see if it allowable to have a separator char
			//if so (not zero), check for the char
			//for example, /A:H
			//               ^
			//               |
			//                ---------------------

			if (switch_table[z].swit_separate)
				if (*parm_ptr == switch_table[z].swit_separate)
					++parm_ptr;

			if (switch_table[z].neg_supported)
				if (*parm_ptr == '-') { //not prefix?
					*(switch_table[z].set_if_not) = TRUE;
					++parm_ptr;
				}

			//each switch can have several options specified. For example:
			//    /AHR   indicating files with attributes of hidden and read-only
			//loop through inspecting for each allowable option

			for (y=0;y< switch_table[z].swit_req; y++) {
				//point to table of allowable options
				//each item in table has two entries (first is the feature letter
				//and the 2nd is an associated hex value --if any)
				//for example: /AH  1st entry would be 'H' and 2nd would be
				//0x02 for hidden
				option_ptr=switch_table[z].feature_ptr;
				while(1) {
					if (*option_ptr == 0) {
						printf("Invalid option specified with switch %s\n", argv[x-1]);
						return(TRUE);
					}
					if (toupper(*parm_ptr) == *option_ptr) break;
					++option_ptr;   //past associated hex value
					++option_ptr;   //to the next letter
				} //while looking up switch options

				//found in table, pull out corresponding values
				if (*(option_ptr + 1) == NO_CODE)
					*(switch_table[z].set_to_feature) |=toupper(*parm_ptr);
				else
					*(switch_table[z].set_to_feature) |=*(option_ptr + 1);

				++parm_ptr; //advance to next byte of parm
				if (*parm_ptr == 0) break;
			} //y --all allowable switch options

			break;
		} //z - for all entries in switch table

		//if never found the switch in table of allowable switches
		if (z == num_switch_entries ) {
			printf("Invalid switch - %s\n", argv[x-1]);
			return(TRUE);
		}

		if (*(switch_table[z].set_if_not))
			*(switch_table[z].set_to_feature) = ~(*(switch_table[z].set_to_feature));
	} //x - for all command line parms
	return(FALSE);
}

void get_current_dir(unsigned drive_num, unsigned char * work_dir_ptr) {

	unsigned char far * work_dir_fptr;

	//must first prime it with '\' since this call doesn't
	//precede the path with a '\' for root
	*work_dir_ptr++=BACK_SLASH;
	work_dir_fptr=(unsigned char far *) work_dir_ptr;

	inregs.h.dl=drive_num;
	inregs.h.ah=GET_CUR_DIR;
	segregs.ds=FP_SEG( (void far * ) work_dir_fptr );
	inregs.x.si=FP_OFF( (void far *) work_dir_fptr);
	int86x(0x21, &inregs, &outregs, &segregs);

	work_dir_ptr+= strlen(work_dir_ptr);
}
                                    
/*-----------------------------------------------------------
| put_together_spec - User has possibly specified the files |
|                     to be displayed.  The information     |  
|                     may not be present (user_ptr == null),|  
|                     or the specifier may not be complete  |  
|                     (i.e., it's assumed to be off of the  |  
|                     current directory, etc.)  Based       |  
|                     on what the user entered and the      |  
|                     current subdirectory information,     |  
|                     build the complete specifier and      |  
|                     save it.                              |  
|                                                           |  
|       Enter:                                              |  
|           user_ptr = ptr to text entered by user          |
|        cur_dir_ptr = ptr to current directory             |
|       work_dir_ptr = ptr to buffer in which to place the  |  
|                      complete spec.                       |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|       area at work_dir_ptr filled in.                     |
|                                                           |  
|                                                           |  
------------------------------------------------------------*/
void  put_together_spec(unsigned char * user_ptr, unsigned char * cur_dir_ptr,
									 unsigned char *  work_dir_ptr) {

	unsigned cur_dir_len;
	unsigned use_cur;
	unsigned parent_dir_len;
	unsigned char * work_cur_ptr;
	unsigned char * bs_spot_ptr;

	cur_dir_len=strlen(cur_dir_ptr);
	//handle  lfndir   or lfndir .  or lfndir .\
	use_cur=FALSE;
	if (user_ptr == NULL) use_cur=TRUE;
	else {
    if (*user_ptr == DOT && *(user_ptr + 1) == '\0') use_cur = TRUE;
    if (*user_ptr == DOT && *(user_ptr + 1) == BACK_SLASH &&
                *(user_ptr + 2) == '\0' ) use_cur = TRUE;
	}

	if (use_cur) {
		memmove(work_dir_ptr, cur_dir_ptr, cur_dir_len);
		work_dir_ptr += cur_dir_len;
		//if not just root, need a '\\'
		if (cur_dir_len > 1) *work_dir_ptr++=BACK_SLASH;
		memmove(work_dir_ptr, wild_string, sizeof(wild_string) + 1);
		return;
	}

	//next, if user entered root dir and nothing else, or if user entered
	//nothing (and current directory is root), put in wildcards
	//i.e., if LFNDIR \   make it LFNDIR \*.*
    
	if ((*user_ptr == BACK_SLASH  && *(user_ptr + 1) == '\0') ||
		((*cur_dir_ptr == BACK_SLASH  && *(cur_dir_ptr + 1) == '\0')
		&& *user_ptr == '\0')) {

		*work_dir_ptr++=BACK_SLASH;
		memmove(work_dir_ptr, wild_string, sizeof(wild_string) + 1);
		return;
	}
   
	//handle lfndir .. or lfndir ..\   --must use parent directory
	if (*user_ptr==DOT && *(user_ptr + 1) == DOT && *(user_ptr + 2) == '\0') {
		user_ptr += 2;      //advance to end of string
		use_cur = TRUE;
	}
            
	if ((*user_ptr == DOT) && (*(user_ptr + 1) == DOT) && (*(user_ptr + 2) == BACK_SLASH) ) {
		user_ptr+=3;        //advance to end of string or possible more
		use_cur=TRUE;
	}

	if (use_cur) {           //if specified off of parent
		work_cur_ptr=cur_dir_ptr + strlen(cur_dir_ptr); //position at end
		//search back for parent
		bs_spot_ptr=cur_dir_ptr;
		while(work_cur_ptr != cur_dir_ptr) {
			--work_cur_ptr;
			//make note of first backslash encountered moving backwards
			if (*work_cur_ptr == BACK_SLASH) {
				bs_spot_ptr=work_cur_ptr;
				break;
			}
		} //while not back at start of string

		//get length of parent portion of path (and the last back slash)
		cur_dir_len=(bs_spot_ptr - cur_dir_ptr) + 1;
		memmove(work_dir_ptr, cur_dir_ptr, cur_dir_len);
		work_dir_ptr += cur_dir_len;

		//if user entered lfndir ..\xxxxx will be positioned at x
		//we'll fall through and copy the rest of user's string
		//otherwise it was just "lfndir .." or "lfndir ..\" and
		//we'll put in the wildcards and leave.

		if (*user_ptr == '\0') {
			//if not simply off root, need \
			if (cur_dir_len > 1) *work_dir_ptr++=BACK_SLASH;
        
			memmove(work_dir_ptr, wild_string, sizeof(wild_string)+ 1 );
			return;
		}
	}  //if ..\ used in specifier and need parent info

	//handle .\xxxx
	if (*user_ptr == DOT && *(user_ptr + 1) == BACK_SLASH &&
		 *(user_ptr + 2) != '\0' )
		user_ptr += 2;      //advance past .\

	//if user entered: lfndir foo  versus lfndir \foo, need to qualify it
	if (*user_ptr != BACK_SLASH && use_cur == FALSE) {
		memmove(work_dir_ptr, cur_dir_ptr, cur_dir_len);
		work_dir_ptr += cur_dir_len;

		//if only adding the root to front, don't need another \
		//otherwise, add one (e.g. \dir1\dir2 + '\\' + foo)
		//only if user has entered something that will be added afterwards
		if (*(work_dir_ptr - 1) != BACK_SLASH && *user_ptr != '\0'  )
			*work_dir_ptr++=BACK_SLASH;
	}
	memmove(work_dir_ptr, user_ptr, strlen(user_ptr));
}

/*-----------------------------------------------------------
| get_free_space - issue Get Disk Free Space DOS function   |
|                  for specified drive.  Calculate bytes    |  
|                  free from returned data and store this.  |  
|                                                           |  
|       Enter:                                              |  
|          dir_drive = drive to get info on                 |
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|         free_space = total bytes free                     |
|                                                           |  
------------------------------------------------------------*/
void get_free_space(unsigned dir_drive) {

	inregs.h.dl=dir_drive;
	inregs.h.ah=GET_DISK_FREE_SPACE;
	int86x(0x21, &inregs, &outregs, &segregs);

	if (outregs.x.ax != 0xffff) { //if valid drive
		//number of available clusters * number of sectors per cluster
		free_space=(unsigned long) outregs.x.bx * (unsigned long)outregs.x.ax;

		//times number of bytes per sector
		free_space *= (unsigned long) outregs.x.cx;
	} else
		free_space=0;
}

unsigned determine_compressed(unsigned drive_no) {

	char volume_string[]="A:\\";
	void far * pass_ptr;

	volume_string[0]=(char) (drive_no + 'A' - 1);

	pass_ptr= (void far *) volume_string;

	inregs.x.ax=GET_VOLUME_INFO;
	inregs.x.cx=sizeof(file_sys_buf);       //max length of buffer

	segregs.ds=FP_SEG( pass_ptr);
	inregs.x.dx=FP_OFF( pass_ptr);

	pass_ptr= (void far *) file_sys_buf;

	segregs.es=FP_SEG( (void far * ) pass_ptr);
	inregs.x.di=FP_OFF( (void far *) pass_ptr);

	int86x(0x21, &inregs, &outregs, &segregs);

	//This call may not be supported (could actually be running on
	//pure DOS)
	if  (outregs.x.cflag) return(FALSE);

	//bx=0x8000 if volume is compressed
	return (outregs.x.bx & 0x8000);
}

/*-----------------------------------------------------------
| determine_fat_size - Find file system type (12 or 16 bit).|
|                      Use Get Media ID function to get     |  
|                      this information. This also retrieves| 
|                      the volume label and serial number.  |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|    drive_no = drive number (a=1, b=2, etc.)               |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|    Number of bits (12 or 16) returned.  Zero returned     |  
|    if it's not a valid file system type.                  |  
|                                                           |  
|    Serial number and volume label stored in media_id      |  
|    structure.                                             |  
|                                                           |  
|    serial_present = TRUE if call processed ok                               |  
|                                                           |  
------------------------------------------------------------*/
unsigned determine_fat_size(unsigned drive_no) {

	struct media_info far * media_ptr;
	struct ext_fcb far * fcb_ptr;
	struct psp far * psp_ptr;
	unsigned long num_sectors;
	unsigned fat_size;
	unsigned x;

	serial_present=TRUE;    //default to serial number found

	media_ptr = (struct media_info far * ) &media_id;
	fcb_ptr = (struct ext_fcb far * ) &fcb_data;

	inregs.x.ax=0x440d;
	inregs.x.bx=drive_no;
	inregs.h.ch=MAJ_CODE;               //must be 08
	inregs.h.cl=GET_MEDIA_ID;
	segregs.ds=FP_SEG( (void far * ) media_ptr);
	inregs.x.dx=FP_OFF( (void far *) media_ptr);
	int86x(0x21, &inregs, &outregs, &segregs);

	//This call shouldn't fail. However, on older floppies where there is
	//not volume label, serial number, or indication of the file system,
	//it will fail. Thus, attempt alternate method of retrieving
	//the volume label and determine and return the FAT size.

	if  (outregs.x.cflag) {
		serial_present=FALSE;        //no serial number present

		fcb_data.ext_filename[10]='?';
		fcb_data.ext_driveid=drive_no;

		inregs.h.ah=FIND_FIRST_FILE;
		segregs.ds=FP_SEG( (void far * ) fcb_ptr);
		inregs.x.dx=FP_OFF( (void far *) fcb_ptr);
		int86x(0x21, &inregs, &outregs, &segregs);
		if  (outregs.x.cflag) return(0);
		FP_SEG(psp_ptr)=_psp;
		FP_OFF(psp_ptr)=0;
		fcb_ptr= (struct ext_fcb far * ) &psp_ptr->psp_defdta;

		for (x=0;x<sizeof(fcb_ptr->ext_filename);x++)
			media_id.vol_label[x]=fcb_ptr->ext_filename[x];

		num_sectors=(unsigned long) drive_parms.dp_bpb_ts;
		if (drive_parms.dp_bpb_ts == 0) num_sectors=drive_parms.dp_bpb_huge_sec;
			fat_size=12;
		if ( (num_sectors /drive_parms.dp_bpb_au) > 4094) fat_size=16;
			return(fat_size);
	}

	//looking for "FAT12" or "FAT16"
	if (media_id.file_sys_type[0] != 'F' || media_id.file_sys_type[3] !='1')
		return(0);
	if (media_id.file_sys_type[4] != '2' && media_id.file_sys_type[4] !='6')
		return(0);
	return ( (media_id.file_sys_type[4] - '0') + 10 );
}

/*-----------------------------------------------------------
| display_header - display:  volume label, serial number,   |
|                  and "directory of" info.                 |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|      media_id = structure with serial number and volume   |  
|                label.                                     |  
|                                                           |  
|     dir_drive = drive number.                             |  
|                                                           |  
| work_dir_path = holds path being listed.                  |  
|                                                           |  
|    dir_of_buf = holds path for display in                 |  
|                 "directory of" message.                   |  
|                                                           |  
|serial_present = TRUE if serial number exists              |  
|        Exit:                                              |  
|                                                           |  
|     row_count = number of rows on screen taken up by      |  
|                 heading lines.                            |  
|                                                           |  
|                                                           |  
------------------------------------------------------------*/
void display_header(void) {

	char form_buf[10];

	if (bare_flag) {
		row_count = 1;
		return;
	}

	printf("\n Volume in drive %c ", (char) (dir_drive + 0x40));

	if (!(memcmp("NO NAME    ", media_id.vol_label, sizeof(media_id.vol_label))))
		printf("has no label\n");
	else
		printf("is %.11s\n", media_id.vol_label );

	sprintf(form_buf, "%04X-", media_id.serial_numlo);
	sprintf(&form_buf[5], "%04X", media_id.serial_numhi);

	if (serial_present)
		printf(" Volume Serial Number is %9s\n", form_buf);

	if (last_find.fc_path_found == TRUE) {
		printf(" Directory of %c:", (dir_drive + 0x40));
		memmove(entry_format_buf, dir_of_buf, strlen(dir_of_buf) + 1 );
		printf("%s\n\n", entry_format_buf);
	}

	row_count += 4;
}

/*-----------------------------------------------------------
| get_number_rows - determine number of text rows supported |
|                   in current video mode. Store this for   |  
|                   use in dealing with /P option.          |  
|                                                           |  
|       Enter:                                              |  
|             nothing displayed by program yet.             |  
|        Exit:                                              |  
|                                                           |  
|        number_of_rows = rows                              |
|             row_count = 0 (number displayed so far)       |  
------------------------------------------------------------*/
void get_number_rows(void) {

	unsigned char far * row_ptr;

	FP_SEG( row_ptr)=0x40;     //BIOS data area ptr to number of rows
	FP_OFF( row_ptr)=0x84;

	if (*row_ptr)
		number_of_rows=*row_ptr + 1;
	else
		number_of_rows=25;

	row_count=0;
}

void inc_row_check(void) {

	++row_count;
	if (row_count > number_of_rows - 3) {
		printf("Press any key to continue . . .\n");
		getch();
		printf("\n(continuing %c:%s)\n", (dir_drive + 0x40), dir_of_buf);
    row_count=1;
	}
}

/*-----------------------------------------------------------
| display_entry - display the directory entry.              |
|                                                           |  
|       Enter:                                              |  
|                                                           |  
| last_find.dir_entry_ptr = ptr to directory structure      |
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|     row_count updated                                     |  
|                                                           |  
------------------------------------------------------------*/
void display_entry(void) {

	format_entry();
	if (bare_flag)    //with /b switch, don't display the first . and ..
		if ((entry_format_buf[0] == '.' && entry_format_buf[1] == '\0') ||
			(entry_format_buf[0] == '.' && entry_format_buf[1] == '.' &&
			entry_format_buf[2] == '\0')) return;

	printf("%s\n", entry_format_buf);
	inc_row_check();
}

/*-----------------------------------------------------------
| format_entry - format a directory display line just like  |
|                DIR does it.                               |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
| last_find.dir_entry_ptr = ptr to directory structure      |
|                                                           |  
|        Exit:                                              |  
|                                                           |  
|        entry_format_buf contains the fomratted entry      |
------------------------------------------------------------*/
void format_entry(void) {

	unsigned char * disp_buf_ptr;
	unsigned len_moved;
	unsigned num_len;
	char day_or_night;
	unsigned char num_text_buf[20];
	unsigned char * lower_work_ptr;

	disp_buf_ptr=entry_format_buf;
	if (!bare_flag) {
		memmove(disp_buf_ptr, last_find.dir_entry_ptr->filename,
			sizeof(last_find.dir_entry_ptr->filename));
		disp_buf_ptr+= sizeof(last_find.dir_entry_ptr->filename);
		*disp_buf_ptr++=' ';
		memmove(disp_buf_ptr, last_find.dir_entry_ptr->fileext,
			sizeof(last_find.dir_entry_ptr->fileext));
		disp_buf_ptr+= sizeof(last_find.dir_entry_ptr->fileext);

		//if user wants aliases displayed in lowercase
		if (lower_flag) {
			lower_work_ptr=entry_format_buf;
			while(lower_work_ptr < disp_buf_ptr) {
				*lower_work_ptr=tolower(*lower_work_ptr);
				++lower_work_ptr;
			}
		}

		if ((last_find.dir_entry_ptr->file_attrib & SUBDIR_ATTRIB)==SUBDIR_ATTRIB) {
			memmove(disp_buf_ptr, dir_string, sizeof(dir_string));
			disp_buf_ptr += (sizeof(dir_string) -1);
		} else {
			format_long(num_text_buf, last_find.dir_entry_ptr->file_size);
			num_len=14-strlen(num_text_buf);
			memset(disp_buf_ptr, ' ', num_len);
			disp_buf_ptr+=num_len;
			memmove(disp_buf_ptr, num_text_buf, strlen(num_text_buf));
			disp_buf_ptr += strlen(num_text_buf);
	 }

	 if (last_find.dir_entry_ptr->fd_yr >= 20)
		 last_find.dir_entry_ptr->fd_yr -= 20;
	 else
		 last_find.dir_entry_ptr->fd_yr += 80;

	 len_moved=sprintf(disp_buf_ptr, "  %02u-%02u-%02u", last_find.dir_entry_ptr->fd_mo,
		 last_find.dir_entry_ptr->fd_day, last_find.dir_entry_ptr->fd_yr);
	 disp_buf_ptr += len_moved;
	 day_or_night='a';       //'a' for 'am'

	 if (last_find.dir_entry_ptr->ft_hrs >= 12)
		 day_or_night='p';       //'p' for 'pm'
	 if (last_find.dir_entry_ptr->ft_hrs > 12)
		 last_find.dir_entry_ptr->ft_hrs -= 12;
	 if (last_find.dir_entry_ptr->ft_hrs == 0)   //if midnight
		 last_find.dir_entry_ptr->ft_hrs = 12;
	 len_moved=sprintf(disp_buf_ptr, " %2u:%02u%c", last_find.dir_entry_ptr->ft_hrs,
		 last_find.dir_entry_ptr->ft_mins, day_or_night);
	 disp_buf_ptr += len_moved;
	 *(disp_buf_ptr++)=' ';
 } //if not bare display (lists only the lfn)

 //go place the lfn or the formatted 8.3 filename (if no lfn exists)
 lfn_or_formatted83(disp_buf_ptr);
}

/*-----------------------------------------------------------
| do_help - display the help screen.                        |
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
------------------------------------------------------------*/
void do_help(void) {

	printf("\n\nLFNDir 1.0 Copyright (c) 1998\n");
	printf("Ziff-Davis Publishing Company. All rights reserved.\n");
	printf("First Published in PC Magazine US Edition, May 5, 1998\n");
	printf("Written by Rick Knoblaugh\n");
	printf("Ported to MS Quick C 2.5 by Forever Young Software\n\n");

	printf("LFNDir [drive:][path][filename] [/P] [/A[[:]attributes]] [/B] [/L]\n\n");
	printf("where:\n\n");
	printf("[drive:][path][filename]\n");
	printf("       Specifies drive, directory, and/or files to list.\n\n");
	printf("/P     Pauses after each screenful of information.\n\n");
	printf("/A     Displays files with specified attributes:\n");
	printf("  D  Directories    R  Read-only files\n");
	printf("  H  Hidden files   A  Files ready for archiving\n");
	printf("  S  System files   -  Prefix meaning not\n\n");
	printf("/B       Uses bare format (no heading information or summary.)\n\n");
	printf("/L       Uses lowercase for displaying aliases.\n\n");
}

/*-----------------------------------------------------------
| display_totals - display the total number of directories  |
|                  and files found, and display the         |  
|                  total number of bytes occupied by the    |  
|                  files.                                   |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
| last_find.number_dirs = total number of dirs found        |  
|l\st_find.number_files = total number of files found       |
|  last_find.bytes_used = total of all file sizes           |  
|            free_space = number of bytes free on volume    |  
|                                                           |  
|        Exit:                                              |  
|                                                           |  
------------------------------------------------------------*/
void display_totals(void) {

	unsigned char * disp_buf_ptr;
	unsigned char num_text_buf[20];

	if (bare_flag) return;
	disp_buf_ptr=entry_format_buf;
	format_long(num_text_buf, last_find.number_files);
	if (strlen(num_text_buf) < 11)
		printf("%10s file(s) ", num_text_buf);
	else
		printf("%s file(s)", num_text_buf);

	format_long(num_text_buf, last_find.bytes_used);
	printf("%14s bytes\n", num_text_buf);
	format_long(num_text_buf, last_find.number_dirs);
	if (strlen(num_text_buf) < 11)
		printf("%10s dir(s)  ", num_text_buf);
	else
		printf("%s dir(s)", num_text_buf);
	disp_bytes_free();
}

void disp_bytes_free(void) {

	unsigned char num_text_buf[20];

	format_long(num_text_buf, free_space);
	printf("%14s bytes free\n", num_text_buf);
}

/*-----------------------------------------------------------
| format_long - want to format an unsigned long so that     |
|               there are commas:  xx,xxx,xxx,xxx           |  
|                                                           |  
|       Enter:                                              |  
|                                                           |  
| form_buf_ptr = ptr the buffer to hold the formatted       |  
|                number.                                    |  
|                                                           |  
|     form_num = number to format                           |  
|                                                           |  
|        Exit:                                              |  
|              buffer is filled                             |  
------------------------------------------------------------*/
void format_long(unsigned char * form_buf_ptr, unsigned long form_num) {

	unsigned char work_form_buf[20];
	int x;
	unsigned digit_count;
	unsigned len_needed;
	unsigned len_buf;

	sprintf(work_form_buf, "%lu\0", form_num);
	len_buf=strlen(work_form_buf);
	len_needed = len_buf;
	len_needed += ((len_needed-1) / 3);  //space needed for commas
	form_buf_ptr += len_needed;
	//zero terminator at end
	*form_buf_ptr-- = 0;
	digit_count=1;
	//start building at end and work back
	for (x=len_buf-1;x>=0;x--,++digit_count) {
		*form_buf_ptr--=work_form_buf[x];
		if (x-1 >=0)
			if ( digit_count == ( (digit_count/3) * 3))
				*form_buf_ptr--= ',';       //add a comma

	}
}

/*-----------------------------------------------------------
| levels_in_path - determine how many levels to traverse.   |
|                                                           |  
|       Enter:                                              |  
|                                                           |  
|        dir_path_ptr = ptr to path string                  |  
|                                                           |  
|        Exit:                                              |  
|              return the number                            |  
------------------------------------------------------------*/
unsigned find_levels(char * dir_path_ptr) {

	unsigned level_count=0;

	do {
		if (*dir_path_ptr == BACK_SLASH) ++level_count;
	} while (*dir_path_ptr++);
	return(level_count);
}

void init_context() {
	last_find.prev_sec_ptr=NULL;
	last_find.fc_root_sec=0;
	last_find.fc_root_entry=0;
	last_find.fc_sec=0;
	last_find.fc_entry_num=0;
	last_find.fc_cluster=0;
	last_find.fc_cluster_offset=0;
	last_find.number_dirs=0;
	last_find.number_files=0;
	last_find.bytes_used=0;
	last_find.fc_error=FALSE;
	last_find.fc_path_found=TRUE;
	last_find.fc_file_found=TRUE;
	last_find.last_level_off=0;
	last_find.fc_spec_flags=0;

	//init "directory of" buffer to root
	dir_of_buf[0]=BACK_SLASH;
}
