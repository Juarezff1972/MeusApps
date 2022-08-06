#if !defined __CONVERT_H
#define __CONVERT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 256

#include "matrix.h"
#include "io.h"

#define MASK_NULL	0
#define MASK_CTEX	1
#define MASK_DTEX	2
#define MASK_STEX	4
#define MASK_RTEX	8
#define MASK_TTEX	16
#define MASK_BTEX	32
			// dunno what LTEX is for... (light texture???)
#define MASK_LTEX	64

typedef char string[MAXSTRLEN];

class texnametype {

   public:
      char texname[MAXSTRLEN];
      int id;

      void init(int mask) {

         id = mask;
         texname[0] = 0;
      }

      texnametype() {
         init(MASK_NULL);
      }

      int read_texname(FILE *infile) {

         char buffer[MAXSTRLEN];
         int  serial;
         int count;

         count = getstring(infile, texname, MAXSTRLEN);

         if (count < 1 || sscanf(texname, "%s %d", buffer, &serial) < 2)
            return count;

         sprintf(texname, "%s.%d", buffer, serial);
         return count;
      }

};


class base_shade {

   public:
      int id;
      string name;
      char flag_dbl_side;
      int count;
      float trans;
      vector3f diffuse, ambient, specular, luminate;
      float specn;

      base_shade *next;

      base_shade() {
         trans = -1;
         count = 0;
         flag_dbl_side = 0;
         name[0] = 0;
         next = (base_shade *)NULL;
         diffuse[0] = diffuse[1] = diffuse[2] = 0.7;
         ambient[0] = ambient[1] = ambient[2] = 1;
         specular[0] = specular[1] = specular[2] = 0.3;
         specn = 1;
         luminate[0] = luminate[1] = luminate[2] = 0.0;
      }

      void write_data(FILE *outfile);
      void write_notes(FILE *outfile);
      virtual void post_read() {}

      base_shade *find (int x) {

         if (x == id)
            return this;

         if (next == (base_shade *)NULL)
            return (base_shade *)NULL;

         return next->find(x);
      }

      base_shade *find(char *x) {
         if (!strcmp(x, name))
            return this;

         if (next == (base_shade *)NULL)
            return (base_shade *)NULL;

         return next->find(x);
      }

};


class lw_shade : public base_shade {

   public:
      float shade[3];
      float lum, diff, spec;

//      float specn;	// 16, 64, 256, 1024;

      texnametype tob;

      lw_shade() {
         shade[0] = shade[1] = shade[2] = 0;
         diff = spec = lum = 0;
      }

      void post_read();

};


class charlist {

   public:
      string x;
      charlist *next;

      charlist() {
         next = (charlist *)NULL;
      }

};


class intlist {

   public:
      int x;
      intlist *next;

      intlist() {
         next = (intlist *)NULL;
      }

};


class float3dlist {

   public:
      vector3f pt;
      float3dlist *next;
};


class list {

   public:
      int id;
      list *back, *next;

      list() { back = next = (list *)NULL; }

      ~list() {

         if (back != (list *)NULL)
            delete back;
         if (next != (list *)NULL)
            delete next;
      }

      virtual void write_data(FILE *outfile) = 0;
      void insert_id(list *x);
      list *find_id(int x);

      list *get_node();
};


class vertexlist : public list {

   public:
      vector3f pt;

      void write_data(FILE *outfile);
      int  insert(float *point, int *vertexcount);
};


class vertexdata  {

   public:
      int index;
      float t[2];
      vertexdata *next;

      vertexdata() {
         index = 0;
         t[0] = t[1] = 0;
         next = (vertexdata *)NULL;
      }

};


class objectlist : public list {

   public:
      int polynum;
      vertexdata  *vptr;
      base_shade *lw;
      char texname[MAXSTRLEN];

      int other_id;
      int rnflag;
      char flag_dbl_side;

      objectlist() {
         vptr = (vertexdata *)NULL;
         lw = (base_shade *)NULL;
         rnflag = 0;
         texname[0] = 0;
         flag_dbl_side = 0;
      }

      ~objectlist() {

         if (vptr != (vertexdata *)NULL)
            delete [] vptr;
      }

      void adjust_totals(int *polycount, int *edgecount);
      void tex_count(int *count);

      void write_ilm(FILE *outfile, int base);
      void write_tex(FILE *outfile);
      void write_data(FILE *outfile);
      void write_dbl_side(FILE *outfile);
      void write_notes(FILE *outfile);
};


class converter {

   public:
      int polycount;
      int vertexcount;
      int edgecount;

      base_shade *surface_list;
      int scount;

      vertexlist *head;                      // vertex list
      objectlist *ob;                        // polygon list

      converter() {
         head = (vertexlist *)NULL;
         ob = (objectlist *)NULL;
         surface_list = (base_shade *)NULL;
      }

      ~converter() {
         if (head != (vertexlist *)NULL)
            delete head;
         if (ob != (objectlist *)NULL)
            delete ob;
         if (surface_list != (base_shade *)NULL)
            delete surface_list;
      }

      int read_ply(char *filename);
      int read_byu(char *filename);

      int read_slide(char *filename);

      int write_spg(char *filename, int sflag);
      int write_ilm(char *filename);
      int write_tex(char *filename);
      int write_notes(char *filename);

      virtual int read_data(char *filename) { return 0; }
};


class lightwave : public converter {

   protected:
      int read_pnts(FILE *infile, int **index, int *bytesize);
      int read_srfs(FILE *infile, int *bytesize);
      int read_pols(FILE *infile, int *index, int *bytesize);
      int read_surf(FILE *infile, int *bytesize);

   public:
      int read_data(char *filename);
};


class wavefront : public converter {

   protected:
      void read_obj_material(char *token, int *countid);

   public:
      int read_data(char *filename);
};


class dive : public converter {

   public:
      int read_data(char *filename);
};


class dxf : public converter {

   protected:
      vector3f vertex[10];
      float floats[10], angles[10];
      int ints[10];
      int color;
      char obname[MAXSTRLEN];

      vertexlist *vstack;
      int        vcount;

   public:
    
      dxf();
      ~dxf();

      int  query_degenerate(int a, int b, int c);
      int  getline(FILE *infile, char *buffer, int *code);
      void store_poly(int a, int b, int c);
      void store_data();
      int  read_data(char *filename);
};


int get_tokena(FILE *infile, char *token);
int get_ca(FILE *infile, char x);


extern int reverse_normal;

#endif
