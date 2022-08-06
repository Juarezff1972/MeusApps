



#if !defined __TEXTURE_H
#define __TEXTURE_H

#include "map.h"
#include "io.h"

                // max gif codes + 1
#define GIF_MAX_CODES   4096

typedef enum { TEXTURE, VIDEO } textype;


typedef enum storageenum {

   VERBATIM = 0,
   RLE      = 1
} storagetype;


typedef struct rgbheadstruct {

   volatile unsigned short xsize, ysize, zsize;
            int            bpc;
            storagetype    storage;
            unsigned short dimension;
            unsigned int   pixmin, pixmax;
            char           name[80];
            unsigned int   colormap;
} rgbheadtype;


class rgb : public mapul {

   public:

      volatile rgbheadtype head;
      char                 filename[MAXSTRLEN];

      int  read_header(FILE *infile);
      int  read_data(char *fname, int flip);

      int  scan_header(sfile *file);
      int  scan_data(sfile *file, int flip);

      int  write_ver(char *fname, int flip);
      int  write_rle(char *fname, int flip);

      int read_ilbm(char *filename);
      int read_gif(char *filename);
      int read_jpg(char *filename);
      int read_tiff(char *filename);
      int read_ppm(char *filename);
      int write_ppm(char *filename, int flip);

      void query_coord(float x[][2]);
};


typedef struct gifheadstruct {

   volatile unsigned short xsize, ysize;
            char signature[4];
            char version[4];
            char gctable_flag;
            int  gctable_size;
            char color_resolution;
            char sort_flag;
            char bc_index;
            char pixel_aspect_ratio;
} gifheadtype;


class gifimagetype {

   public:
      unsigned char image_separator;       // = 0x2c
      unsigned short left;
      unsigned short top;
      unsigned short width;
      unsigned short height;

      unsigned char  lctable_flag;
      unsigned char  interlace_flag;
      unsigned char  sort_flag;
      unsigned char  reserved;
      int            lctable_size;

      unsigned char  pass;
      unsigned char  slinc;

      unsigned char *lctable;
      unsigned char **plctable;

      unsigned char transparentflag;
      unsigned char transparentindex;
      unsigned char renderflag;

      gifimagetype() {
         plctable = (unsigned char **)NULL;
         lctable = (unsigned char *)NULL;
         transparentflag = 0;
         renderflag = 0;
      }

      ~gifimagetype() {
         if (plctable != (unsigned char **)NULL) {
            delete [] plctable;
            delete [] lctable;
         }

      }

};


// The following variables are used for seperating out codes
typedef struct gifdecodestruct {

   unsigned char bytes_left;                    /* # bytes left in block */
   unsigned char bits_left;                     /* # bits left in current byte */
   unsigned char buffer[257];                    /* Current block */
   unsigned char *nextcptr;                      /* Pointer to next byte in block */
   unsigned char cptr;                           /* Current byte */
   int           codesize;                      /* The current code size */
} gifdecodetype;


class gif {

   protected:

      unsigned char  stack[GIF_MAX_CODES];         /* Stack for storing pixels */
      unsigned char  suffix[GIF_MAX_CODES];        /* Suffix table */
      unsigned short prefix[GIF_MAX_CODES];        /* Prefix linked list */

      unsigned short clear;                         /* Value for a clear code */
      unsigned short ending;                        /* Value for a ending code */
      unsigned short newcodes;                      /* First available code */
      unsigned short top_slot;                      /* Highest code for current size */
      unsigned short slot;                          /* Last read code */

      unsigned char **pgctable;
      unsigned char *gctable;

      unsigned char **color_table;

      gifimagetype  gimage;
      gifheadtype   head;

      int  read_header(FILE *infile);
      int  get_next_code(FILE *infile, gifdecodetype *stuff);
      int  out_line(unsigned char *buffer, rgb *image, int width, int scany);
      int  decoder(FILE *infile, rgb *image);

   public:

      int  read_data(char *fname, rgb *image);

      gif() {
         pgctable = (unsigned char **)NULL;
         gctable = (unsigned char *)NULL;
      }

      ~gif() {

         if (pgctable != (unsigned char **)NULL) {
            delete [] pgctable;
            delete [] gctable;
         }

      }

};


class ilbm {

   protected:
      int decomprle(FILE *infile, unsigned char *dest, int size);

   public:
      int  read_data(char *fname, rgb *image);
};


class jpeg {

   public:
      int  read_data(char *fname, rgb *image);
};


class tiff {

   public:
      int read_data(char *fname, rgb *image);
};


class ppm {

   protected:
      int xsize, ysize;

   public:
      int  read_data(char *fname, rgb *image);
      int  write_data(char *fname, rgb *image, int flip);
};


class texbase {

   public:
      char    filename[MAXSTRLEN];
      textype whatami;
      int     maxx, maxy;
      texbase *next;

      virtual int read_texture(char *fname, int flip=0) = 0;
      virtual int read_texture(char *fname, texbase **tptr, int flip=0) = 0;
      virtual unsigned int query_texel(float x, float y, float *color, int frame, int mflag, float d);

      textype query_whatami() { return whatami; }
};

typedef texbase *ptexbase;

class mipmaptype {

   public:
      int compression;
      float icompression;
      float idiff;                 // (idifference between this one and previous one)
      mapul tob;
};

class texture : public texbase {

   public:
      rgb tob;
      mipmaptype *mipmap;
      int mapcount;

      texture() {
         whatami = TEXTURE;
         mipmap = (mipmaptype *)NULL;
      }

      ~texture() {
         if (mipmap != (mipmaptype *)NULL)
            delete [] mipmap;
      }

      void build_mipmaps();

      int read_texture(char *fname, int flip=0);
      int read_texture(char *fname, texbase **tptr, int flip=0);
      unsigned int query_texel(float x, float y, float *color, int frame, int mflag, float d);
};


typedef texture *ptexture;

typedef enum {
   VIDONE, VIDREPEAT
} vidplaytype;

/*
                Dont need destructor -> texture's kept in main list.... :)
                assumption : all textures in video are same dimensions
                fact : frame #s are non-negative
*/
class texvid : public texbase {

   protected:
      texbase **tob;
      int count;
      vidplaytype vpflag;

   public:

      texvid() {
         tob = (texbase **)NULL;
         whatami = VIDEO;
      }

      ~texvid() {
         if (tob != (texbase **)NULL)
            delete [] tob;
      }

      int read_texture(char *fname, int flip=0);
      int read_texture(char *fname, texbase **tptr, int flip=0);
      unsigned int query_texel(float x, float y, float *color, int frame, int mflag, float d);
};


int query_video(char *filename);

#endif
