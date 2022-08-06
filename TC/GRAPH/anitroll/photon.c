



#include "photon.h"


/* ********************************************************
   this function reads in camera data
******************************************************** */
int photon::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'a':
         if (!strcmp(token, "ambient")) {
            get_token(infile, token);
            Ia[0] = atof(token);
            get_token(infile, token);
            Ia[1] = atof(token);
            get_token(infile, token);
            Ia[2] = atof(token);

            return 1;
         }

         break;

      case 'd':
         if (strlen(token) < 3)
            break;

         switch (token[2]) {
            case 'f':
               if (!strcmp(token, "diffuse")) {
                  get_token(infile, token);
                  Ip[0] = atof(token);
                  get_token(infile, token);
                  Ip[1] = atof(token);
                  get_token(infile, token);
                  Ip[2] = atof(token);

                  return 1;
               }

               break;

            case 's':
               if (!strcmp(token, "distance2plane")) {
                  get_token(infile, token);
                  distance2plane = atof(token);

                  return 1;
               }

               break;

            case 'r':
               if (!strcmp(token, "direction")) {
                  get_token(infile, token);
                  vpn[0] = atof(token);
                  get_token(infile, token);
                  vpn[1] = atof(token);
                  get_token(infile, token);
                  vpn[2] = atof(token);

                  return 1;
               }

               break;

            default:
               break;
         }

         break;

      case 'f':
         if (!strcmp(token, "fatt")) { 
            get_token(infile, token);
            fatt[0] = atof(token);
            get_token(infile, token);
            fatt[1] = atof(token);
            get_token(infile, token);
            fatt[2] = atof(token);

            return 1;
         }

         break;

      case 'l':
         if (!strcmp(token, "length")) {
            get_token(infile, token);
            length = atof(token);

            return 1;
         }

         break;

      case 't':
         if (!strcmp(token, "type")) {
            get_token(infile, token);
            lower_case(token);

            if (!strcmp(token, "beam_point")) {
               mode = POINT;
               group = SPOT_LIGHT;
               simpleflag = 0;
            }

            else if (!strcmp(token, "beam_far")) {
               mode = FAR;
               group = SPOT_LIGHT;
               simpleflag = 0;
            }

            else if (!strcmp(token, "point")) {
               mode = POINT;
               group = GLOBAL_LIGHT;
               simpleflag = 0;
            }

            else if (!strcmp(token, "far")) {
               mode = FAR;
               group = GLOBAL_LIGHT;
               simpleflag = 0;
            }

            else if (!strcmp(token, "simple_beam_point")) {
               mode = POINT;
               group = SPOT_LIGHT;
               simpleflag = 1;
            }

            else if (!strcmp(token, "simple_beam_far")) {
               mode = FAR;
               group = SPOT_LIGHT;
               simpleflag = 1;
            }

            else if (!strcmp(token, "simple_point")) {
               mode = POINT;
               group = GLOBAL_LIGHT;
               simpleflag = 1;
            }

            else {
               mode = FAR;
               group = GLOBAL_LIGHT;
               simpleflag = 1;
            }

            return 1;
         }

         break;

      case 'u':
         if (!strcmp(token, "up")) {
            get_token(infile, token);
            vup[0] = atof(token);
            get_token(infile, token);
            vup[1] = atof(token);
            get_token(infile, token);
            vup[2] = atof(token);

            return 1;
         }

         break;

      case 'w':
         if (!strcmp(token, "width")) {
            get_token(infile, token);
            width = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return quark::parse(infile, token);
}


/* ********************************************************
   this function calculates the camera position
   and puts data in a list
******************************************************** */
void photon::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;                       // pointer
   bpphoton *bptr;
   vector4f r[4];

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   copymx4x4(xmx, localmx);
   matmatmulto(r, xmx); 

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];

   if (active && wflag) {

      (*obcount)++;

      bptr = new bpphoton;

      bptr->ob = this;
      bptr->frameno = frameno;

      if (group != SPOT_LIGHT && mode != FAR) {
         copyarray3(bptr->location, center);
         bptr->location[3] = 1;
      }

      else {
         copyarray3(bptr->location, vpn);
         matvecmultv(xmx, bptr->location);
      }

      if (group == SPOT_LIGHT) {
         copyarray3(bptr->vpn, vpn);
         matvecmultv(xmx, bptr->vpn);

         copyarray3(bptr->vup, vup);
         matvecmultv(xmx, bptr->vup);
      }

      bptr->next = *mptr;
      *mptr = bptr;
   }

}
