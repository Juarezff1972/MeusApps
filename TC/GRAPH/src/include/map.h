


#if !defined __MAP_H_
#define __MAP_H_


class map {

   public:
      unsigned int maxx, maxy;
      virtual void init_map(unsigned int x, unsigned int y) = 0; 
      virtual void zero() = 0;
};


class mapul : public map {

   public:
      unsigned int *data;
      unsigned int **pdata;

      void init_map(unsigned int x, unsigned int y);
      void zero();
      void flip_bytes();

      int query_x() {
         return maxx;
      }

      int query_y() {
         return maxy;
      }

      mapul();
      ~mapul();
};

class mapf : public map {

   public:
      float *data;
      float **pdata;

      void init_map(unsigned int x, unsigned int y);
      void zero();

      mapf();
      ~mapf();
};

class mapuc : public map {

   public:
      unsigned char *data;
      unsigned char **pdata;

      void init_map(unsigned int x, unsigned int y);
      void zero();

      mapuc();
      ~mapuc();
};

typedef struct {
   float zdata;
   unsigned long idbuff;
} lexel_type;

class maplexel : public map {

   public:
      lexel_type *data;
      lexel_type **pdata;

      void init_map(unsigned int x, unsigned int y);
      void zero();

      maplexel();
      ~maplexel();
};


typedef float * pfloat;
typedef float ** ppfloat;
typedef int * pint;
typedef unsigned int * puint;
typedef unsigned int ** ppuint;
typedef unsigned char * puchar;
typedef lexel_type *plexel_type;

#endif
