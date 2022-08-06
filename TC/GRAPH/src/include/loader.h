

#include "superclass.h"
#include "pstring.h"

#if !defined __LOADER_H
#define __LOADER_H

class loader {

   protected:
      char object_name[MAXSTRLEN];

      virtual superclass *make_object() = 0;    

   public:
      loader *next;

      loader() { next = (loader *)NULL; }
      ~loader() { if (next != (loader *)NULL) delete next; }

      void insert(loader *x) {
         x->next = next;
         next = x;
      }

      char *query_name() { return object_name; }

      virtual superclass *parse(FILE *infile, char *token);
};

// *******************************************************************


class camera_loader : public loader {

   protected:
      superclass *make_object();

   public:
      camera_loader() {
         strcpy(object_name, "camera");
      }

};


class light_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      superclass *parse(FILE *infile, char *token);

      light_loader() {
         strcpy(object_name, "light");
      }
};


class far_light_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      far_light_loader() {
         strcpy(object_name, "far_light");
      }
};


class point_light_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      point_light_loader() {
         strcpy(object_name, "point_light");
      }
};


class beam_far_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      beam_far_loader() {
         strcpy(object_name, "beam_far");
      }
};


class beam_point_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      beam_point_loader() {
         strcpy(object_name, "beam_point");
      }
};


class simple_far_light_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      simple_far_light_loader() {
         strcpy(object_name, "simple_far_light");
      }
};


class simple_point_light_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      simple_point_light_loader() {
         strcpy(object_name, "simple_point_light");
      }
};


class simple_beam_far_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      simple_beam_far_loader() {
         strcpy(object_name, "simple_beam_far");
      }
};


class simple_beam_point_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      simple_beam_point_loader() {
         strcpy(object_name, "simple_beam_point");
      }
};


class polygon_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      polygon_loader() {
         strcpy(object_name, "polygon");
      }
};


class spatch_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      spatch_loader() {
         strcpy(object_name, "spatch");
      }
};


class sphere_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      sphere_loader() {
         strcpy(object_name, "geosphere");
      }
};


class cone_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      cone_loader() {
         strcpy(object_name, "geocone");
      }
};


class cylinder_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      cylinder_loader() {
         strcpy(object_name, "geocylinder");
      }
};


class line_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      line_loader() {
         strcpy(object_name, "line");
      }
};


class point_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      point_loader() {
         strcpy(object_name, "particle");
      }
};


class radiant_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      radiant_loader() {
         strcpy(object_name, "radiant");
      }
};


class sbfast_loader : public loader {

   protected:
      superclass *make_object();    

   public:
      sbfast_loader() {
         strcpy(object_name, "sbfast");
      }
};

extern loader *gloader;
void init_loader();
#endif
