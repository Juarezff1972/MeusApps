



#include "wwdigo.h"

/* ********************************************************
   This function reads in the data for this child
******************************************************** */
int wwdigo::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'o':

         if (!strcmp(token, "orientation_mx")) {
            get_token(infile, token);
            old_vx[0] = atof(token);
            get_token(infile, token);
            old_vx[1] = atof(token);
            get_token(infile, token);
            old_vx[2] = atof(token);

            get_token(infile, token);
            old_vy[0] = atof(token);
            get_token(infile, token);
            old_vy[1] = atof(token);
            get_token(infile, token);
            old_vy[2] = atof(token);

            get_token(infile, token);
            old_vz[0] = atof(token);
            get_token(infile, token);
            old_vz[1] = atof(token);
            get_token(infile, token);
            old_vz[2] = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return quark::parse(infile, token);
}


void wwdigo::preprocess() {

   normalize3(old_vx);
   normalize3(old_vy);
   normalize3(old_vz);

   quark::preprocess();
}


/* *************************************************************
   place object based on local and parent xforms

1) calc new location
2) vpn = new direction = new loc - old loc
3) if new direction about same as old direction, use old vup, vright
4) else calc new vup/vright based upon plane created by new/old direction vectors
5) 2 possible vups/right -> vup, smallest rotation...

*** need to transpose????
************************************************************* */
void wwdigo::calc_orientation(float *new_dir) {

   vector3f new_vz, new_vy, new_vx, t_u, t_v, t_t;
   float temp, temp2;
   float one = 1 - CORRECT4;
   float a, b;

   if (!firstflag) {
      copyarray3(new_vz, new_dir);
      temp = normalize3(new_vz);
      temp2 = abs(dotproduct3(new_vz, old_vz));

//      if (temp > CORRECT4 && temp2 < one) {
      if (temp > CORRECT4 && temp2 < 0.9998) {
         xproduct(new_vx, new_vz, old_vz);
         normalize3(new_vx);
         xproduct(new_vy, new_vz, new_vx);

         if (dotproduct3(new_vx, old_vx) < 0) {
            new_vx[0] = -new_vx[0];
            new_vx[1] = -new_vx[1];
            new_vx[2] = -new_vx[2];

            new_vy[0] = -new_vy[0];
            new_vy[1] = -new_vy[1];
            new_vy[2] = -new_vy[2];
         }

         if (abs(new_vz[1]) < 1-CORRECT && (new_vy[1] < 0 || abs(new_vx[1]) > CORRECT)) {	// calc rotation back to "normal"

            if (new_vy[1] >= one)
               fraction = 0;		// no rotate - should not get here
            else if (new_vy[1] <= -one) {
               fraction = catchup;
               temp = PI;
            }

            else {
               copyarray3(t_v, new_vx);
               t_v[1] = 0;
               normalize3(t_v);

               xproduct(t_u, new_vz, t_v);	// "y" axis that we want to rotate to...
               normalize3(t_u);

               temp = dotproduct3(t_u, new_vy);	// cos between y axis we have and the one we want

               if (temp < one) {		// at the orientation we want
                  fraction = catchup;
                  temp = ACOS(temp);

                  init_mx(angle);
                  rotate_mx_z(angle, temp);

                  copyarray3(t_t, new_vy);
                  matvecmulto(angle, t_t);

                  init_mx(angle);
                  rotate_mx_z(angle, -temp);

                  copyarray3(t_v, new_vy);
                  matvecmulto(angle, t_v);

                  a = dotproduct3(new_vy, t_t);
                  b = dotproduct3(new_vy, t_v);
                  if (abs(b) < abs(a))
                     temp = -temp;
               }

               else
                  fraction = 0;
            }

            if (fraction) {
               temp *= icatchup;
               init_mx(angle);
               rotate_mx_z(angle, temp);
            }

         }

         else
            fraction = 0;

         copyarray3(old_vx, new_vx);
         smultarray3(old_vx, iscale);
         copyarray3(old_vy, new_vy);
         smultarray3(old_vy, iscale);
         copyarray3(old_vz, new_vz);
         smultarray3(old_vz, iscale);

         return;
      }

      if (fraction && upflag) {		// rotate toward "normal" orientation
         fraction--;

         matvecmultv(angle, old_vx);
         matvecmultv(angle, old_vy);
         matvecmultv(angle, old_vz);
      }

   }

}


void wwdigo::new_action(FILE *infile, float timefactor, char *buffer) {

   char token[MAXSTRLEN];

   if (buffer != (char *)NULL)
      strcpy(token, buffer);
   else {
      get_token(infile, token);
      lower_case(token);
   }

   switch (token[0]) {

      case 'o':

         if (!strcmp(token, "orientation_mx")) {
            get_token(infile, token);
            old_vx[0] = atof(token);
            get_token(infile, token);
            old_vx[1] = atof(token);
            get_token(infile, token);
            old_vx[2] = atof(token);
            normalize3(old_vx);

            get_token(infile, token);
            old_vy[0] = atof(token);
            get_token(infile, token);
            old_vy[1] = atof(token);
            get_token(infile, token);
            old_vy[2] = atof(token);
            normalize3(old_vy);

            get_token(infile, token);
            old_vz[0] = atof(token);
            get_token(infile, token);
            old_vz[1] = atof(token);
            get_token(infile, token);
            old_vz[2] = atof(token);
            return;
         }

         break;

      default:
         break;
   }

   quark::new_action(infile, timefactor, token);
}