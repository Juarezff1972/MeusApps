
#include <stdlib.h>
#include "pixcon.h"
#include "particle.h"
#include "spatch.h"
#include "cylinder.h"
#include "cone.h"
#include "sbfast.h"
#include "line.h"
#include "sphere.h"
#include "loader.h"


/* *******************************************************************
******************************************************************* */
superclass *loader::parse(FILE *infile, char *token) {

   superclass *ptr;

   if (strcmp(token, query_name()))
      return (next != (loader *)NULL ? next->parse(infile, token) : (superclass *)NULL);

   ptr = make_object();

   do {
      if (!get_token(infile, token) || token[0] == '}')
         break;

      lower_case(token);

      if (!ptr->parse(infile, token)) {
         printf("ERROR: Invalid data format... Last read token %s... Aborting...\n", token);
         fclose(infile);
         exit(1);
      }

   } while (1);

   return ptr;
}


/* *******************************************************************
******************************************************************* */
superclass *light_loader::parse(FILE *infile, char *token) {

   superclass *ptr, *mtr;

   if (strcmp(token, query_name()))
      return (next != (loader *)NULL ? next->parse(infile, token) : (superclass *)NULL);

   ptr = make_object();

   do {
      if (!get_token(infile, token) || token[0] == '}')
         break;

      lower_case(token);

      if (!strcmp(token, "type")) {
         if (!get_token(infile, token) || token[0] == '}')
            break;

         mtr = ptr;

         if (!strcmp(token, "point"))
            ptr = new point_light;
         else if (!strcmp(token, "beam_far"))
            ptr = new beam_far;
         else if (!strcmp(token, "beam_point"))
            ptr = new beam_point;
         else if (!strcmp(token, "far"))
            ptr = new far_light;
         else if (!strcmp(token, "simple_point"))
            ptr = new simple_point_light;
         else if (!strcmp(token, "simple_beam_far"))
            ptr = new simple_beam_far;
         else if (!strcmp(token, "simple_beam_point"))
            ptr = new simple_beam_point;
         else if (!strcmp(token, "simple_far"))
            ptr = new simple_far_light;
         else {
            printf ("Error: bad light type : %s\n", token);
            fclose(infile);
            exit(1);
         }

         *ptr = *mtr;
         delete mtr;
      }

      else if (!ptr->parse(infile, token)) {
         printf("ERROR: Invalid data format... Last read token %s... Aborting...\n", token);
         fclose(infile);
         exit(1);
      }

   } while (1);

   return ptr;
}

superclass *light_loader::make_object() { return new far_light; }
superclass *camera_loader::make_object() { return new camera(WINX, WINY); }
superclass *far_light_loader::make_object() { return new far_light; }
superclass *point_light_loader::make_object() { return new point_light; }
superclass *beam_far_loader::make_object() { return new beam_far; }
superclass *beam_point_loader::make_object() { return new beam_point; }
superclass *simple_far_light_loader::make_object() { return new simple_far_light; }
superclass *simple_point_light_loader::make_object() { return new simple_point_light; }
superclass *simple_beam_far_loader::make_object() { return new simple_beam_far; }
superclass *simple_beam_point_loader::make_object() { return new simple_beam_point; }
superclass *polygon_loader::make_object() { return new polygon; }
superclass *spatch_loader::make_object() { return new spatch; }
superclass *sphere_loader::make_object() { return new sphere; }
superclass *cone_loader::make_object() { return new cone; }
superclass *cylinder_loader::make_object() { return new cylinder; }
superclass *line_loader::make_object() { return new line; }
superclass *point_loader::make_object() { return new particle; }
superclass *radiant_loader::make_object() { return new radiant; }
superclass *sbfast_loader::make_object() { return new sbfast; }

loader *gloader;

void init_loader() {

   gloader = new camera_loader;
   gloader->insert(new light_loader);
   gloader->insert(new far_light_loader);
   gloader->insert(new point_light_loader);
   gloader->insert(new beam_far_loader);
   gloader->insert(new beam_point_loader);
   gloader->insert(new simple_far_light_loader);
   gloader->insert(new simple_point_light_loader);
   gloader->insert(new simple_beam_far_loader);
   gloader->insert(new simple_beam_point_loader);
   gloader->insert(new polygon_loader);
   gloader->insert(new spatch_loader);
   gloader->insert(new line_loader);
   gloader->insert(new point_loader);
   gloader->insert(new sphere_loader);
   gloader->insert(new cone_loader);
   gloader->insert(new cylinder_loader);
   gloader->insert(new radiant_loader);
   gloader->insert(new sbfast_loader);
}


