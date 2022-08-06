


#if !defined (__ATOM_H)

#define __ATOM_H

#include "anitroll.h"


/* ****************************************************************
   This is the animated object class
**************************************************************** */
class atom : public quark {

   protected:

      int    scale_quark(float s, char *part);
      int    move_quark(char *child, char *parent);
      int    drop_quark(int frameno, char *child, atom **header, FILE *infile);
      int    join_atom(char *part, char *parent, atom **header);
      int    take_quark(char *dest, char *parent, char *part, atom *header);

   public:
      eventtype *event;
      quark     *ob;
      atom      *next;

      atom() {
         event = (eventtype *)NULL;
      }

      ~atom();

      void      whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      void      new_action(FILE *infile, float  timefactor, char *buffer = (char *)NULL);
      void      read_data(char *filename);
      void      new_action(int frameno, atom **header);
      whatamitype whatami() { return ATOM; }
      void      update(quark *parent);
};


#endif
