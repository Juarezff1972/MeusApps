



/* *************************************************************
   This file contains all the global data types for my programs
************************************************************* */

#if !defined (__DATATYPE_H)
#define __DATATYPE_H

#include "base.h"

class polytype : public datatype {              // composite data

   public:
						// datatype
      void init();
      void dest();
						// polytype
      int	countvertex;            // vertex count
      int	countedge;		// edge count
      int	countobject;            // polygon count

      vector4f	*mvertex;
      int	*polynum;		// # of verticies for each polygon
      int	**edgeptr;		// pointer into object table for edges
      int	*object;		// edge data
      float     *iarea;

      vector4f	*normal;		// normals for polygons
      vector4f	*gnormal;		// normals for vertex

      illtype	*shade;			// pointer to color data

      int	borrowflag;
      int	maxpolynum;

      polytype() { init(); }
      ~polytype() { dest(); }

      int build(int cvertex, int cobject, int cedge, int sflag, int bflag);
      int read_data(char *filename);
      void calc_normal();
      void calc_area();
      void calc_vnormal();
      void vtransform(vector4f *mx);
      void ntransform(vector4f *mx);
      void vntransform(vector4f *mx);
      void scale_area(float scale);
      void make_flat(vector2f *extreme, float depth, eye *parm);
};


class spatchtype : public datatype {

   public:
      int		vsize, psize;           // vertex/patch count
      int		prow, pcol;
      int		vrow, vcol;             // x, y count
      int		srow, scol;

      vector4f		*data;
      pvector4f		*mvertex;

      vector4f		*ndata;               // normals for vertex
      pvector4f		*gnormal;

      illtype		*shade;
      illtype		**mshade;

      void init();
      void dest();
      int build(int crow, int ccol, int nflag, int sflag);
      int read_data(char *filename);

      spatchtype() { init(); }

      ~spatchtype() { dest(); }
};


class listtype {                                // main pointer to objects

   public:
      datatype     *dtr;                       // ptr to polygon objects
      shadelist    *itr;
      texbase      *tdtr;
      texmap       *tmap;
      shaderlisttype *shltype;

      camera       **camdtr;
      light        **lightdtr;
      light        **beamdtr;
      pc           **header;
      pc           **raddtr;

      shader       *shade_list;

      listtype() {
         dtr  = (datatype *)NULL;
         itr      = (shadelist *)NULL;
         shltype  = (shaderlisttype *)NULL;
         tdtr     = (texbase *)NULL;
         tmap     = (texmap *)NULL;

         camdtr   = (camera **)NULL;
         lightdtr = (light **)NULL;
         beamdtr  = (light **)NULL;
         header   = (pc **)NULL;
         raddtr   = (pc **)NULL;
         shade_list = (shader *)NULL;
      }

      void init(int x);
};


#endif
