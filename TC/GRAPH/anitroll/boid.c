



#include "boid.h"

/* *************************************************************
************************************************************* */
int boid::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'b':
         if (!strcmp(token, "boid_range")) {
            get_token(infile, token);
            nb_min_radius = atof(token);
            get_token(infile, token);
            nb_max_radius = atof(token);
            return 1;
         }

         break;

      case 'c':
         if (!strcmp(token, "collision_loss")) {
            icl = 1 - get_token(infile, token);
            return 1;
         }

         break;

      case 'f':
         if (!strcmp(token, "friction_per_sec")) {
            get_token(infile, token);
            ifps = 1.0-atof(token);
            return 1;
         }

         break;

      case 'g':
         if (!strcmp(token, "gravity")) {
            gflag = 1;
            get_token(infile, token);
            gravity_vel = atof(token);
            get_token(infile, token);
            gravity[0] = atof(token);
            get_token(infile, token);
            gravity[1] = atof(token);
            get_token(infile, token);
            gravity[2] = atof(token);

            normalize3(gravity);
            return 1;
         }

         break;

      case 'l':
         if (!strcmp(token, "leader_range")) {
            get_token(infile, token);
            nl_min_radius = atof(token);
            get_token(infile, token);
            nl_max_radius = atof(token);
            return 1;
         }

         break;

      case 'm':
         if (!strcmp(token, "max_turn_radius")) {
            get_token(infile, token);
            max_turn_rad = deg2rad(atof(token));
            return 1;
         }

         if (!strcmp(token, "max_acceleration")) {
            get_token(infile, token);
            max_accel = atof(token);
            return 1;
         }

         break;

      case 'r':
         if (!strcmp(token, "red_alert")) {
            get_token(infile, token);
            ra_radius = atof(token);
            return 1;
         }

         break;

      case 'v':
         if (!strcmp(token, "velocity")) {
            get_token(infile, token);
            velocity = atof(token);
            return 1;
         }

         if (!strcmp(token, "velocity_range")) {
            get_token(infile, token);
            minspeed = atof(token);
            get_token(infile, token);
            maxspeed = atof(token);
            return 1;
         }

         break;

      case 'y':
         if (!strcmp(token, "yellow_alert")) {
            get_token(infile, token);
            ya_radius = atof(token);

            get_token(infile, token);
            ya_weight = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return wwdigo::parse(infile, token);
}


/* *************************************************************
************************************************************* */
void boid::calc_bound() {

   int i;
   vector3f work, v1, v2;

   switch (bflag) {

      case BPOINT:
         yradius = ya_radius;
         rradius = ra_radius;

         break;

      case BSPHERE:
         yradius = ya_radius + bsphere;
         rradius = ra_radius + bsphere;

         break;

      case BRECT:

         for (i=0; i<8; i++)
            copyarray4(ybdata[i], bdata[i]);

         copyarray3(work, ybdata[0]);
         subarray3(work, ybdata[6]);
         normalize3(work);
         smultarray3(work, ya_radius);
         addarray3(ybdata[0], work);
         subarray3(ybdata[6], work);

         copyarray3(work, ybdata[1]);
         subarray3(work, ybdata[7]);
         normalize3(work);
         smultarray3(work, ya_radius);
         addarray3(ybdata[1], work);
         subarray3(ybdata[7], work);

         copyarray3(work, ybdata[2]);
         subarray3(work, ybdata[4]);
         normalize3(work);
         smultarray3(work, ya_radius);
         addarray3(ybdata[2], work);
         subarray3(ybdata[4], work);

         copyarray3(work, ybdata[3]);
         subarray3(work, ybdata[5]);
         normalize3(work);
         smultarray3(work, ya_radius);
         addarray3(ybdata[3], work);
         subarray3(ybdata[5], work);

         for (i=0; i<8; i++)
            matvecmulto(xmx, ybdata[i]);

         copyarray3(v1, ybdata[1]); // 0 1 2 3
         subarray3(v1, ybdata[0]);
         copyarray3(v2, ybdata[2]);
         subarray3(v2, ybdata[1]);
         xproduct(ybnormal[0], v2, v1);
         normalize3(ybnormal[0]);
         ybnormal[0][3] = -dotproduct3(ybnormal[0], ybdata[0]);

         ybnormal[1][0] = -ybnormal[0][0];
         ybnormal[1][1] = -ybnormal[0][1];
         ybnormal[1][2] = -ybnormal[0][2];
         ybnormal[1][3] = -dotproduct3(ybnormal[1], ybdata[4]);

         copyarray3(v1, ybdata[5]);
         subarray3(v1, ybdata[4]);
         copyarray3(v2, ybdata[1]);
         subarray3(v2, ybdata[5]);
         xproduct(ybnormal[2], v2, v1);
         normalize3(ybnormal[2]);
         ybnormal[2][3] = -dotproduct3(ybnormal[2], ybdata[0]);

         ybnormal[4][0] = -ybnormal[2][0];
         ybnormal[4][1] = -ybnormal[2][1];
         ybnormal[4][2] = -ybnormal[2][2];
         ybnormal[4][3] = -dotproduct3(ybnormal[4], ybdata[2]);

         copyarray3(v1, ybdata[5]);
         subarray3(v1, ybdata[1]);
         copyarray3(v2, ybdata[6]);
         subarray3(v2, ybdata[5]);
         xproduct(ybnormal[3], v2, v1);
         normalize3(ybnormal[3]);
         ybnormal[3][3] = -dotproduct3(ybnormal[3], ybdata[1]);

         ybnormal[5][0] = -ybnormal[3][0];
         ybnormal[5][1] = -ybnormal[3][1];
         ybnormal[5][2] = -ybnormal[3][2];
         ybnormal[5][3] = -dotproduct3(ybnormal[5], ybdata[0]);

         for (i=0; i<8; i++)
            copyarray4(rbdata[i], bdata[i]);

         copyarray3(work, rbdata[0]);
         subarray3(work, rbdata[6]);
         normalize3(work);
         smultarray3(work, ra_radius);
         addarray3(rbdata[0], work);
         subarray3(rbdata[6], work);

         copyarray3(work, rbdata[1]);
         subarray3(work, rbdata[7]);
         normalize3(work);
         smultarray3(work, ra_radius);
         addarray3(rbdata[1], work);
         subarray3(rbdata[7], work);

         copyarray3(work, rbdata[2]);
         subarray3(work, rbdata[4]);
         normalize3(work);
         smultarray3(work, ra_radius);
         addarray3(rbdata[2], work);
         subarray3(rbdata[4], work);

         copyarray3(work, rbdata[3]);
         subarray3(work, rbdata[5]);
         normalize3(work);
         smultarray3(work, ra_radius);
         addarray3(rbdata[3], work);
         subarray3(rbdata[5], work);

         for (i=0; i<8; i++)
            matvecmulto(xmx, rbdata[i]);

         copyarray3(v1, rbdata[1]); // 0 1 2 3
         subarray3(v1, rbdata[0]);
         copyarray3(v2, rbdata[2]);
         subarray3(v2, rbdata[1]);
         xproduct(rbnormal[0], v2, v1);
         normalize3(rbnormal[0]);
         rbnormal[0][3] = -dotproduct3(rbnormal[0], rbdata[0]);

         rbnormal[1][0] = -rbnormal[0][0];
         rbnormal[1][1] = -rbnormal[0][1];
         rbnormal[1][2] = -rbnormal[0][2];
         rbnormal[1][3] = -dotproduct3(rbnormal[1], rbdata[4]);

         copyarray3(v1, rbdata[5]);
         subarray3(v1, rbdata[4]);
         copyarray3(v2, rbdata[1]);
         subarray3(v2, rbdata[5]);
         xproduct(rbnormal[2], v2, v1);
         normalize3(rbnormal[2]);
         rbnormal[2][3] = -dotproduct3(rbnormal[2], rbdata[0]);

         rbnormal[4][0] = -rbnormal[2][0];
         rbnormal[4][1] = -rbnormal[2][1];
         rbnormal[4][2] = -rbnormal[2][2];
         rbnormal[4][3] = -dotproduct3(rbnormal[4], rbdata[2]);

         copyarray3(v1, rbdata[5]);
         subarray3(v1, rbdata[1]);
         copyarray3(v2, rbdata[6]);
         subarray3(v2, rbdata[5]);
         xproduct(rbnormal[3], v2, v1);
         normalize3(rbnormal[3]);
         rbnormal[3][3] = -dotproduct3(rbnormal[3], rbdata[1]);

         rbnormal[5][0] = -rbnormal[3][0];
         rbnormal[5][1] = -rbnormal[3][1];
         rbnormal[5][2] = -rbnormal[3][2];
         rbnormal[5][3] = -dotproduct3(rbnormal[5], rbdata[0]);

         break;

      case BPLANE:

         ybdata[0][0] = bdata[0][0] + yradius*bdata[1][0];
         ybdata[0][1] = bdata[0][1] + yradius*bdata[1][1];
         ybdata[0][2] = bdata[0][2] + yradius*bdata[1][2];
         ybdata[0][3] = 1;
         matvecmulto(xmx, ybdata[0]);

         copyarray3(ybnormal[0], bdata[1]);
         matvecmultv(xmx, ybnormal[0]);
         normalize3(ybnormal[0]);
         ybnormal[0][3] = -dotproduct3(ybnormal[0], ybdata[0]);

         rbdata[0][0] = bdata[0][0] + rradius*bdata[1][0];
         rbdata[0][1] = bdata[0][1] + rradius*bdata[1][1];
         rbdata[0][2] = bdata[0][2] + rradius*bdata[1][2];
         rbdata[0][3] = 1;
         matvecmulto(xmx, rbdata[0]);

         copyarray3(rbnormal[0], bdata[1]);
         matvecmultv(xmx, rbnormal[0]);
         normalize3(rbnormal[0]);
         rbnormal[0][3] = -dotproduct3(rbnormal[0], rbdata[0]);

         break;
   }

}


/* *************************************************************
************************************************************* */
int boid::query_collision(quark *drunk) {

   if (bflag == BNONE || drunk->bflag == BNONE)
      return NO_COLLISION;

   switch (bflag) {

      case BPOINT:
      case BSPHERE:
         switch (drunk->bflag) {

            case BPOINT:
               if (!query_point_sphere_intersect(drunk->oldcenter, oldcenter, yradius))
                  return NO_COLLISION;
               if (query_point_sphere_intersect(drunk->oldcenter, oldcenter, rradius))
                  return RED_ALERT;

               return YELLOW_ALERT;

            case BSPHERE:
               if (!query_sphere_sphere_intersect(drunk->oldcenter, drunk->bsphere, oldcenter, yradius))
                  return NO_COLLISION;
               if (query_sphere_sphere_intersect(drunk->oldcenter, drunk->bsphere, oldcenter, rradius))
                  return RED_ALERT;

               return YELLOW_ALERT;

            case BRECT:
               if (!query_sphere_rect_intersect(oldcenter, yradius, drunk->bold_normal))
                  return NO_COLLISION;
               if (query_sphere_rect_intersect(oldcenter, rradius, drunk->bold_normal))
                  return RED_ALERT;

               return YELLOW_ALERT;

            default:		// case BPLANE:
               if (!query_sphere_plane_intersect(oldcenter, yradius, drunk->bold_normal[0]))
                  return NO_COLLISION;
               if (query_sphere_plane_intersect(oldcenter, rradius, drunk->bold_normal[0]))
{
// printf("red %s %f %f %f %f\n", drunk->name, drunk->bold_normal[0][0], drunk->bold_normal[0][1], drunk->bold_normal[0][2], drunk->bold_normal[0][3]);
                  return RED_ALERT;
}

               return YELLOW_ALERT;
         }

      case BRECT:

         switch (drunk->bflag) {

            case BPOINT:
               if (!query_point_rect_intersect(drunk->oldcenter, ybnormal))
                  return NO_COLLISION;
               if (query_point_rect_intersect(drunk->oldcenter, rbnormal))
                  return RED_ALERT;

               return YELLOW_ALERT;

            case BSPHERE:
               if (!query_sphere_rect_intersect(drunk->oldcenter, drunk->bsphere, ybnormal))
                  return NO_COLLISION;
               if (query_sphere_rect_intersect(drunk->oldcenter, drunk->bsphere, rbnormal))
                  return RED_ALERT;

               return YELLOW_ALERT;

            case BRECT:
               if (!query_rect_rect_intersect(ybnormal, ybdata, drunk->bold_normal, drunk->bold_data))
                  return NO_COLLISION;
               if (query_rect_rect_intersect(rbnormal, rbdata, drunk->bold_normal, drunk->bold_data))
                  return RED_ALERT;

               return YELLOW_ALERT;

            default:		// case BPLANE:
               if (!query_plane_rect_intersect(drunk->bold_normal[0], ybdata))
                  return NO_COLLISION;
               if (query_plane_rect_intersect(drunk->bold_normal[0], rbdata))
                  return RED_ALERT;

               return YELLOW_ALERT;
         }


      default:		// BPLANE

         switch (drunk->bflag) {

            case BPOINT:
               if (!query_point_plane_intersect(drunk->oldcenter, ybnormal[0]))
                  return NO_COLLISION;
               if (query_point_plane_intersect(drunk->oldcenter, rbnormal[0]))
                  return RED_ALERT;

               return YELLOW_ALERT;

            case BSPHERE:
               if (!query_sphere_plane_intersect(drunk->oldcenter, drunk->bsphere, ybnormal[0]))
                  return NO_COLLISION;
               if (query_sphere_plane_intersect(drunk->oldcenter, drunk->bsphere, rbnormal[0]))
                  return RED_ALERT;

               return YELLOW_ALERT;

            case BRECT:
               if (!query_plane_rect_intersect(ybnormal[0], drunk->bold_data))
                  return NO_COLLISION;
               if (query_plane_rect_intersect(rbnormal[0], drunk->bold_data))
                  return RED_ALERT;

               return YELLOW_ALERT;

            default:		// case BPLANE:
               if (!query_plane_plane_intersect(ybnormal[0], drunk->bold_normal[0]))
                  return NO_COLLISION;
               if (query_plane_plane_intersect(rbnormal[0], drunk->bold_normal[0]))
                  return RED_ALERT;

               return YELLOW_ALERT;
         }

   }

}


/* *************************************************************
************************************************************* */
void boid::calc_reflect(quark *drunk, float *dir, float *ref) {

   vector3f n;

   switch (bflag) {

      case BPOINT:
      case BSPHERE:

         switch (drunk->bflag) {              // calc reflect normal

            case BPOINT:
            case BSPHERE:
               if (!sphere_sphere_reflectn3vf(drunk->oldcenter, oldcenter, n)) {
                  n[0] = -dir[0];
                  n[1] = -dir[1];
                  n[2] = -dir[2];
                  normalize3(n);
               }

               break;

            case BPLANE:
               sphere_plane_reflectn3vf(drunk->bold_normal[0], n);
               break;

            default:		//case BRECT:
               sphere_rect_reflectn3vf(oldcenter, drunk->bold_data, drunk->bold_normal, n);
               break;
         }

         break;

      case BRECT:
         switch (drunk->bflag) {
            case BPOINT:
            case BSPHERE:
               sphere_rect_reflectn3vf(drunk->oldcenter, bold_data, bold_normal, n);
               n[0] = -n[0];
               n[1] = -n[1];
               n[2] = -n[2];
               break;

            case BPLANE:
               sphere_plane_reflectn3vf(drunk->bold_normal[0], n);
               break;

            default:		//case BRECT:
               rect_rect_reflectn3vf(bold_data, drunk->bold_data, drunk->bold_normal, n);
               break;
         }

         break;

      case BPLANE:
         switch (drunk->bflag) {
            case BPOINT:
            case BSPHERE:
               sphere_plane_reflectn3vf(drunk->bold_normal[0], n);
               n[0] = -n[0];
               n[1] = -n[1];
               n[2] = -n[2];
               break;

            case BPLANE:
               sphere_plane_reflectn3vf(drunk->bold_normal[0], n);
               break;

            default:		//case BRECT:
               plane_rect_reflectn3vf(bold_normal[0], drunk->bold_data, drunk->bold_normal, n);
               break;
         }

         break;
   }

   if (dotproduct3(n, dir) < -CORRECT) {	// if above surface, reflect
      ref[0] = -dir[0];
      ref[1] = -dir[1];
      ref[2] = -dir[2];
      reflect(n,ref);
   }

   else						// else travel through
      copyarray3(ref, dir);

//printf("n %f %f %f | %f %f %f\n", n[0], n[1], n[2], dir[0], dir[1], dir[2] );
//printf("ref %f %f %f\n", ref[0], ref[1], ref[2]);
}


/* *************************************************************
************************************************************* */
void boid::calc_avoid_leader_boid(float *avoid, quark *qtr, int *aflag, quark **nl,
                      float *nld, quark **nb, float *nbd, quark *parent) {

   linktype *ptr;
   float    reflect[3];
   float    temp;

   if (qtr != parent && qtr != this) {
      switch (query_collision(qtr)) {
         case NO_COLLISION:
            break;

         case YELLOW_ALERT:
            if (*aflag == RED_ALERT)
               break;

            if (*aflag == NO_COLLISION)
               *aflag = YELLOW_ALERT;

            calc_reflect(qtr, direction, reflect);
            addarray3(avoid, reflect);

            break;

         default:	//case RED_ALERT:
            calc_reflect(qtr, direction, reflect);

            if (*aflag == YELLOW_ALERT) {
               copyarray3(avoid, reflect);
            }

            else
               addarray3(avoid, reflect);

            *aflag = RED_ALERT;

            break;
      }

      if (*aflag != RED_ALERT)
         if (qtr->whatami() == BOIDBAIT) {
            if ((temp = distance3(qtr->oldcenter, oldcenter)) < *nbd || *nbd < 0) {
               *nb = qtr;
               *nbd = temp;
            }

            if (temp < *nld || *nld < 0) {
               *nl = qtr;
               *nld = temp;
            }

         }

         else if (qtr->whatami() == BOID)
            if ((temp = distance3(qtr->oldcenter, oldcenter)) < *nbd || *nbd < 0) {
               *nb = qtr;
               *nbd = temp;
            }

   }

   for (ptr = qtr->edge; ptr != (linktype *)NULL && ptr->link != parent; ptr=ptr->next)
      calc_avoid_leader_boid(avoid, ptr->link, aflag, nl, nld, nb, nbd, qtr);
}


/* *************************************************************
    NOTE: boid class is independent on external xforms....
          and this object is not actually drawn - its an
          animation device, not a 3d object (its child is though :) )

// FUTURE:
//6) note: for animals - if going down, glide
//                        if going up, flap

************************************************************* */
void boid::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;
   atom     *atr;
   int      aflag = NO_COLLISION;			// collision type;

   vector3f want = {0,0,0};
   vector3f work, work2, want2;
   float mag_work, mag_want;
   vector4f r[4], q[4];

   quark    *nearest_leader, *nearest_boid;
   float    nl_dist, nb_dist;

   float    weight;
   vector3f dir;
   float    temp;

   if (!firstflag) {
      calc_bound();				// calc bound sphere/rect/plane

      nl_dist = nb_dist = -1.0;
      nearest_leader = nearest_boid = (quark *)NULL;

      for (atr = (atom *)actors; atr != (atom *)NULL; atr = atr->next)
         if (atr->ob != (quark *)NULL)
            calc_avoid_leader_boid(want, atr->ob, &aflag, &nearest_leader,
                                   &nl_dist, &nearest_boid, &nb_dist, (quark *)NULL);

      if (aflag != NO_COLLISION)
         normalize3(want);

      if (aflag == RED_ALERT) {			// collision
         smultarray3(want, maxspeed);
         velocity *= icl;
         weight = 0;
      }

      else {
         weight = 1;

         if (aflag == YELLOW_ALERT) {                      // near collision
            temp = lerp(ya_weight, minspeed, maxspeed);	// future - figure percentage based on how close
            smultarray3(want, temp);
            weight -= ya_weight;
         }

         else
            want[0] = want[1] = want[2] = 0;

         if (weight > CORRECT) {
            if (nearest_leader != (quark *)NULL)           // nearest flock leader
               if (nl_dist > nl_max_radius) {
//printf("g leader\n");
                  copyarray3(work, nearest_leader->oldcenter);

                  subarray3(work, oldcenter);
                  temp = (normalize3(work) - nl_max_radius)/nl_max_radius;

                  if (temp > weight)
                     temp = weight;

                  weight -= temp;
                  temp = lerp(temp, minspeed, maxspeed);

                  want[0] += work[0] * temp;
                  want[1] += work[1] * temp;
                  want[2] += work[2] * temp;
               }

               else if (nl_dist < nl_min_radius) {
//printf("m leader\n");
                  copyarray3(work, oldcenter);
                  subarray3(work, nearest_leader->oldcenter);
                  temp = (nl_min_radius - normalize3(work))/nl_min_radius;

                  if (temp > weight)
                     temp = weight;

                  weight -= temp;
                  temp = lerp(temp, minspeed, maxspeed);

                  want[0] += work[0] * temp;
                  want[1] += work[1] * temp;
                  want[2] += work[2] * temp;
               }
//else printf("leader | nl_dist %f | %f %f\n", nl_dist, nl_min_radius, nl_max_radius);

            if (weight > CORRECT) {
               if (nearest_boid != (quark *)NULL)          // nearest boid
                  if (nb_dist > nb_max_radius) {
                     copyarray3(work, nearest_boid->oldcenter);
                     subarray3(work, oldcenter);
                     temp = (normalize3(work) - nb_max_radius)/nb_max_radius;

                     if (temp > weight)
                        temp = weight;

                     weight -= temp;
                     temp = lerp(temp, minspeed, maxspeed);

                     want[0] += work[0] * temp;
                     want[1] += work[1] * temp;
                     want[2] += work[2] * temp;
                  }

                  else if (nb_dist < nb_min_radius) {
                     copyarray3(work, oldcenter);
                     subarray3(work, nearest_boid->oldcenter);
                     temp = (nb_min_radius - normalize3(work))/nb_min_radius;

                     if (temp > weight)
                        temp = weight;

                     weight -= temp;
                     temp = lerp(temp, minspeed, maxspeed);

                     want[0] += work[0] * temp;
                     want[1] += work[1] * temp;
                     want[2] += work[2] * temp;
                  }

               if (weight > CORRECT) {                // part of previous path at average speed
                  copyarray3(work, direction);
//                  temp = velocity * weight;
                  temp = weight * 0.5;
                  temp = lerp(temp, minspeed, maxspeed);
                  weight = 0;

                  want[0] += work[0] * temp;
                  want[1] += work[1] * temp;
                  want[2] += work[2] * temp;
               }

            }

         }

      }

	// at this stage, want == direction of where want to be in global frame

      matvecmultv(old_imx, want);	// want = dir in local frame

//printf("want %f %f %f\n", want[0], want[1], want[2]);

      copyarray3(dir, direction);	// backup old dir

      temp = velocity*ifps;		// slow down
      work[0] = dir[0] *temp;
      work[1] = dir[1] *temp;
      work[2] = dir[2] *temp;

      if (gflag) {
         work[0] += gravity[0] * gravity_vel;
         work[1] += gravity[1] * gravity_vel;
         work[2] += gravity[2] * gravity_vel;
      }

      // at this stage, work == direction of where going in global frame

//printf("work %f %f %f\n", work[0], work[1], work[2]);
      mag_want = normalize3(want);
      mag_work = normalize3(work);

      temp = ACOS(dotproduct3(want, work));

      if (temp >  max_turn_rad) {

         if (dotproduct3(work, want) < -1+CORRECT) { // 180 deg turn...
            init_mx(r);
            rotate_mx_z(r, rnd(TWOPI));		// calc "Y" in local space

            if (abs(work[1]) > CORRECT) {	// calc rot on meridian(x)
               init_mx(q);
               q[2][0] = 0;
               q[2][1] = work[1];

               if (abs(work[1]) < 1-CORRECT)
                  q[2][2] = sqrt(1-SQR(work[1]));
               else
                  q[2][2] = 0;

               normalize3(q[2]);
               q[0][0] = 1; q[0][1] = 0; q[0][2] = 0;
               xproduct(q[1], q[2], q[0]);
               transpose(q);
               matmatmulto(q, r);
            }
						// calc rot on equator (y)
            if (abs(work[0]) > CORRECT || abs(work[2]) > CORRECT) {
               init_mx(q);
               q[2][0] = work[0];
               q[2][1] = 0;
               q[2][2] = work[2];
               normalize3(q[2]);
               q[1][0] = 0; q[1][1] = 1; q[1][2] = 0;
               xproduct(q[0], q[1], q[2]);
               transpose(q);
               matmatmulto(q, r);
            }

            work2[0] = 0; work2[1] = 1; work2[2] = 0;
            matvecmulto(r, work2);
         }

         else
            xproduct(work2, work, want);

         rotate_vector(work, work2, max_turn_rad, work);
      }

      else
         copyarray3(work, want);

//printf("worknew %f %f %f | %f\n", work[0], work[1], work[2], mag_work);

	// turned as much as possible

      want2[0] = want[0] * mag_want;
      want2[1] = want[1] * mag_want;
      want2[2] = want[2] * mag_want;

      smultarray3(work, mag_work);

      work2[0] = want2[0] - work[0];
      work2[1] = want2[1] - work[1];
      work2[2] = want2[2] - work[2];

      if ((temp=normalize3(work2)) <= max_accel) {
         copyarray3(direction, want2);
      }

      else {
         direction[0] = work[0] + work2[0]*max_accel;
         direction[1] = work[1] + work2[1]*max_accel;
         direction[2] = work[2] + work2[2]*max_accel;
      }

      velocity = normalize3(direction);

      calc_orientation(direction);

      center[0] = old_local_center[0] + direction[0] * velocity;
      center[1] = old_local_center[1] + direction[1] * velocity;
      center[2] = old_local_center[2] + direction[2] * velocity;
   }

   else {
      copyarray3(center, origin);
      copyarray3(direction, old_vz);
   }

//printf("direction %f %f %f | %f\n", direction[0], direction[1], direction[2], velocity);

   inversemx(mx, old_imx);				// backup global inverse mx
   copyarray3(old_local_center, center);

   copyarray3(xmx[0], old_vx);
   copyarray3(xmx[1], old_vy);
   copyarray3(xmx[2], old_vz);
   copyarray3(xmx[3], center);
   transpose(xmx);
   matmatmulto(mx, xmx);

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];

//printf("center %f %f %f\n", center[0], center[1], center[2]);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, xmx);
}