


#if !defined (__BOID_H)

#define __BOID_H

#include "anitroll.h"


#define NO_COLLISION 0
#define RED_ALERT    1
#define YELLOW_ALERT 2

/* *************************************************************
   This is a boid class.  Since I can't see this as anything but
   it's own atom, with no children but other boids, I'm coding
   it as such

   Orientation - nose points down -Z, up points +Y

   so that boids are upright most of the time.  For turning,
   we want nose to be new direction, up "in" the turn - cross product
   old vector w/ new vector, use result w/ cross product of new vector.
   If no change in direction, use old vup. (Simulates bird flaps wings
   to change direction

************************************************************* */
class boid : public wwdigo {

   protected:
      int gflag;			// is there a gravity/force vector
      vector3f gravity;			// gravity vector
      float gravity_vel;		// gravity velocity

      float max_accel, minspeed, maxspeed, velocity;
      float ra_radius, ya_radius;
      float ya_weight;			// yellow alert weight
      float ifps;			// inverse friction per sec....
      float icl;			// inverse collision loss
      float nb_min_radius, nb_max_radius;       // nearest bird radii
      float nl_min_radius, nl_max_radius;       // nearest leader radii
      float max_turn_rad;

      vector3f direction, vup;

      vector4f ybnormal[6];
      vector4f ybdata[8];
      vector4f rbnormal[6];
      vector4f rbdata[8];
      float yradius, rradius;

      vector4f old_imx[4];
      vector4f old_local_center;

      void calc_bound();
      void calc_reflect(quark *drunk, float *dir, float *ref);
      void calc_avoid_leader_boid(float *avoid, quark *qtr, int *aflag, quark **nl,
                      float *nld, quark **nb, float *nbd, quark *parent);
      int  query_collision(quark *drunk);

   public:

      boid() {
         gflag = 0;
         gravity[0] = gravity[2] = 0; gravity[1] = -1;
         gravity_vel = 1;
         max_accel = maxspeed = minspeed = velocity = 1;
         ra_radius = ya_radius = 1;
         ya_weight = 0.5;
         ifps = 1;
         icl = 0.5;
         nb_min_radius = nb_max_radius = nl_min_radius = nl_max_radius = 1;
         max_turn_rad = PI;
      }

      int parse(FILE *infile, char *token);
      void whereami(int frameno, molecule **mptr, int *obcount, int wflag, quark *actors,
			quark *parent = (quark *)NULL, vector4f *mx = (vector4f *)NULL);
      whatamitype whatami() { return BOID; }
};


#endif