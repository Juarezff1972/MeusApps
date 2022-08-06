


#if !defined (__MOLECULE_H)

#define __MOLECULE_H

#include "anitroll.h"

/* ****************************************************************
   This is the base class of the list of objects to be printed to a file
**************************************************************** */
class molecule {

   public:
      int      frameno;
      vector4f location;

      molecule *next;

      virtual void print_data(FILE *outfile, int tflag, float  floor[]) = 0;
};


/* ****************************************************************
   This is a list of 3D objects to be printed to a file
**************************************************************** */
class bpneutron : public molecule {

   public:
      float   mx[4][4];

      neutron *ob;

      void print_data(FILE *outfile, int tflag, float  floor[]);
};


/* ****************************************************************
   This is a list of light objects to be printed to a file
**************************************************************** */
class bpphoton : public molecule {

   public:
      float  vpn[4], vup[4];

      photon *ob;

      void print_data(FILE *outfile, int tflag, float  floor[]);
};


/* ****************************************************************
   This is a list of radiant objects to be printed to a file
**************************************************************** */
class bpelectron : public molecule {

   public:
      float  dop[4];
      float  vup[4];

      electron *ob;

      void print_data(FILE *outfile, int tflag, float  floor[]);
};


/* ****************************************************************
   This is a list of radiant objects to be printed to a file
**************************************************************** */
class bpion : public molecule {

   public:
      float  inrad, outrad;
      ion *ob;

      void print_data(FILE *outfile, int tflag, float  floor[]);
};


/* ****************************************************************
   This is a list of tachyon objects to be printed to a file
**************************************************************** */
class bptachyon : public molecule {

   public:
      float    intensity;
      tachyon  *ob;

      void print_data(FILE *outfile, int tflag, float *floor);
};


/* ****************************************************************
**************************************************************** */
class bpchain : public molecule {

   public:
      vector4f endpt;
      float    intensity;
      chain    *ob;

      void print_data(FILE *outfile, int tflag, float *floor);
};


/* ****************************************************************
   This is a list of tachyon objects to be printed to a file
**************************************************************** */
class bphelix : public molecule {

   public:
      float  mx[4][4];

      helix *ob;

      void print_data(FILE *outfile, int tflag, float  floor[]);
};


#endif

