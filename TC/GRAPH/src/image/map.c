

#include <stdio.h>
#include <string.h>
#include "map.h"

void mapul::flip_bytes() {

   unsigned int i, j;

   j = maxx*maxy;

   for (i=0; i<j; i++)
#ifdef DOS
      data[i] = (data[i]>>8) | ((data[i] & 0xff)<<24);
#else
      data[i] = (data[i]>>24) | ((((data[i]>>16) & 0xff) | (((data[i]>>8) & 0xff) | ((data[i] & 0xff) << 8)) << 8) << 8);
#endif
}


void mapul::init_map(unsigned int x, unsigned int y) {

   unsigned int size;
   unsigned int i, j;

   if (data == (unsigned int *)NULL || x != maxx || y != maxy) {
      maxx = x;
      maxy = y;
      size = x*y;

      if (data != (unsigned int *)NULL) {
         delete [] data;
         delete [] pdata;
      }

      data = new unsigned int[size];
      pdata = new puint[y];

      for (i=j=0; i<y; i++, j+=x)
         pdata[i] = &data[j];
   }

}


void mapul::zero() {

   memset(data, 0, maxx*maxy<<2);       // int - 4 bytes
}


mapul::mapul() {

   data = (unsigned int *)NULL;
   pdata = (puint *)NULL;
}


mapul::~mapul() {

   if (data != (unsigned int *)NULL) {
      delete [] data;
      delete [] pdata;
   }

}


void mapf::init_map(unsigned int x, unsigned int y) {

   unsigned int size;
   unsigned int i, j;

   if (data == (float *)NULL || x != maxx || y != maxy) {
      maxx = x;
      maxy = y;
      size = x*y;

      if (data != (float *)NULL) {
         delete [] data;
         delete [] pdata;
      }

      data = new float[size];
      pdata = new pfloat[y];

      for (i=j=0; i<y; i++, j+=x)
         pdata[i] = &data[j];
   }

}


void mapf::zero() {

   memset(data, 0, maxx*maxy<<2);       // float - 4 bytes
}


mapf::mapf() {

   data = (float *)NULL;
   pdata = (float **)NULL;
}


mapf::~mapf() {

   if (data != (float *)NULL) {
      delete [] data;
      delete [] pdata;
   }

}

void mapuc::init_map(unsigned int x, unsigned int y) {

   unsigned int size;
   unsigned int i, j;

   if (data == (unsigned char *)NULL || x != maxx || y != maxy) {
      maxx = x;
      maxy = y;
      size = x*y;

      if (data != (unsigned char *)NULL) {
         delete [] data;
         delete [] pdata;
      }

      data = new unsigned char[size];
      pdata = new puchar[y];

      for (i=j=0; i<y; i++, j+=x)
         pdata[i] = &data[j];
   }

}


void mapuc::zero() {

   memset(data, 0, maxx*maxy);
}


mapuc::mapuc() {

   data = (unsigned char *)NULL;
   pdata = (unsigned char **)NULL;
}


mapuc::~mapuc() {

   if (data != (unsigned char *)NULL) {
      delete [] data;
      delete [] pdata;
   }

}

void maplexel::init_map(unsigned int x, unsigned int y) {

   unsigned int size;
   unsigned int i, j;

   if (!data || x != maxx || y != maxy) {
      maxx = x;
      maxy = y;
      size = x*y;

      if (data) {
         delete [] data;
         delete [] pdata;
      }

      data = new lexel_type[size];
      pdata = new plexel_type[y];

      for (i=j=0; i<y; i++, j+=x)
         pdata[i] = &data[j];
   }

}


void maplexel::zero() {

   memset(data, 0, maxx*maxy<<3);
}


maplexel::maplexel() {

   data = (lexel_type *)NULL;
   pdata = (lexel_type **)NULL;
}


maplexel::~maplexel() {

   if (data) {
      delete [] data;
      delete [] pdata;
   }

}


