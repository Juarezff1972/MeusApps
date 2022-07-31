/*----------------------------------------------------------------------
LFNsdtruc.h
  
Copyright (c) 1998
Ziff-Davis Publishing Company
All rights reserved.
First Published in PC Magazine, US Edition, May 5, 1998
Written by Rick Knoblaugh

Structures for LFNDIR program.

Ver 1.0  -- Initial release

Ported to MS Quick C 2.5 (Small model) by Forever Young Software

----------------------------------------------------------------------*/

struct  rwblock {
  unsigned char   rwspecfunc;
  unsigned        rwhead;
  unsigned        rwcyl;
  unsigned        rwfirstsec;
  unsigned        rwnumsecs;
  unsigned char far * rwbuf_ptr;
};


struct  device_params {
  unsigned char   dp_spec_func;       //function code: 1 indicates current volume    
  unsigned char   dp_dev_type;        //device type: 5 indicates hard disk
  unsigned        dp_dev_attrib;      //device attributes: bit 0 indicates if removable
  unsigned        dp_num_cyls;        //number of cylinders
  unsigned char   dp_media_type;      //for floppies, tells media density supported
  unsigned        dp_bpb_ss;          //sector size
  unsigned char   dp_bpb_au;          //cluster size in sectors
  unsigned        dp_bpb_rs;          //number of reserved sectors
  unsigned char   dp_bpb_nf;          //number of FATs
  unsigned        dp_bpb_de;          //number of root directory entries
  unsigned        dp_bpb_ts;          //number of sectors
  unsigned char   dp_bpb_md;          //media descriptor
  unsigned        dp_bpb_fs;          //number of sectors in each FAT
  unsigned        dp_bpb_spt;         //sectors per track
  unsigned        dp_bpb_head;        //heads
  unsigned long   dp_bpb_hidsec;      //number of hidden sectors
  unsigned long   dp_bpb_huge_sec;    //number of sectors if (dp_bpb_ts == 0)
};

  struct  find_context {
  unsigned char * prev_sec_ptr;       //ptr to previous dir entry sector buffer searched
  struct dir_layout * dir_entry_ptr;  //ptr to last dir entry found
  unsigned long   fc_root_sec;        //next root directory sector to search
  unsigned        fc_root_entry;      //next root dir entry to search
  unsigned long   fc_sec;             //last directory sector searched
  unsigned        fc_entry_num;       //the dir entry last searched within last sector
  unsigned long   fc_cluster;         //cluster of last subdirectory searched
  unsigned        fc_cluster_offset;  //sector offset within the cluster
  unsigned        last_level_off;     //last byte of search path
  unsigned long   number_dirs;        //number directories encountered
  unsigned long   number_files;       //number of files encountered
  unsigned long   bytes_used;         //space used by all files encountered
  unsigned        fc_error;           //TRUE if error accessing disk
  unsigned        fc_path_found;      //TRUE if path found
  unsigned        fc_file_found;      //TRUE if file found
  unsigned        fc_spec_flags;      //handles special matching situations
};

struct file_time_layout {
  unsigned        ft_secs : 5;
  unsigned        ft_mins : 6;
  unsigned        ft_hrs  : 5;
};

struct file_date_layout {
  unsigned        fd_day  : 5;
  unsigned        fd_mo   : 4;
  unsigned        fd_yr   : 7;
};

struct access_date_layout {
  unsigned        ad_day  : 5;
  unsigned        ad_mo   : 4;
  unsigned        ad_yr   : 7;
};

struct dir_layout {
  unsigned char   filename[8];
  unsigned char   fileext[3];
  unsigned char   file_attrib;
  unsigned char   lo_reserved[6];
  struct          access_date_layout;
  unsigned        ea_handle;
  struct          file_time_layout;
  struct          file_date_layout;
  unsigned        starting_cluster;
  unsigned long   file_size;
};          

struct lfn_dir_layout {
  unsigned char   lfn_seq_num; 
  unsigned char   lfn_name_area1[10];
  unsigned char   lfn_dir_atrrib;
  unsigned char   lfn_type;
  unsigned char   lfn_cksum;
  unsigned char   lfn_name_area2[12];
  unsigned        lfn_zero;
  unsigned char   lfn_name_area3[4];
};          

struct disk_io   {
  unsigned long   dio_startsec;       
  unsigned        dio_numsecs;
  unsigned char far * dio_buf_ptr;
};

struct media_info {
  unsigned        info_level;
  unsigned        serial_numhi;
  unsigned        serial_numlo;
  unsigned char   vol_label[11];
  unsigned char   file_sys_type[8];
};

struct switch_engine {
  char            swit_letter;            //letter after switch char
  unsigned char   swit_req;               //number of things allowed after letter
  char            swit_separate;          //char used to separate letter & option
  unsigned char * feature_ptr;            //ptr to allowable option letters/hex values
  unsigned char   neg_supported;          //TRUE if '-' is allowed
  unsigned char * set_if_not;             //if '-' set flag at this address
  unsigned char * set_if_switch;          //TRUE if this switch specified
  unsigned char * set_to_feature;         //place option in here
};

struct ext_fcb {
  unsigned char   ext_sig;                //signature
  unsigned char   ext_res1[5];
  unsigned char   ext_attrib;
  unsigned char   ext_driveid;
  unsigned char   ext_filename[11];
  unsigned        ext_blockno;
  unsigned        ext_recsize;
  unsigned long   ext_filesize;
  unsigned        ext_filedate;
  unsigned        ext_filetime;
  unsigned char   ext_res2[8];
  unsigned char   ext_currec;
  unsigned char   ext_randrec[4];
};

struct psp  {
  unsigned char   idontcare[128];
  unsigned char   psp_defdta[128];
};
