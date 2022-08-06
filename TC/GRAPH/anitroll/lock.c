



#include "lock.h"


void lock::new_action(FILE *infile, float timefactor, char *buffer) {


   char token[MAXSTRLEN];

   if (buffer != (char *)NULL)
      strcpy(token, buffer);
   else {
      get_token(infile, token);
      lower_case(token);
   }

   switch (token[0]) {

      case 'l':			// target
         if (!strcmp(token, "lock")) {
            get_token(infile, actor_name);
            get_token(infile, piece);
            target = (quark *)NULL;

            return;
         }

         break;

      case 'o':			// offset

         if (!strcmp(token, "offset")) {

            get_token(infile, token);
            offset[0] = atof(token);
            get_token(infile, token);
            offset[1] = atof(token);
            get_token(infile, token);
            offset[2] = atof(token);

            return;
         }

         break;

      case 'u':

         if (!strcmp(token, "unlock")) {
            actor_name[0] = piece[0] = 0;
            target = (quark *)NULL;
            offset[0] = offset[1] = offset[2] = 0;

            return;
         }

         break;

      default:
         break;
   }

   wwdigo::new_action(infile, timefactor, token);
}


/* *************************************************************

************************************************************* */
void lock::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   atom *atr;
   linktype *ptr;                         // pointer
   vector3f new_vz;
   vector4f temp;
   vector4f r[4];

   init_mx(xmx);
   calc_initmx(xmx, r);

   if (!firstflag) {

      if (target == (quark *)NULL && actor_name[0]) {
         for (atr=(atom *)actors; atr != (atom *)NULL && strcmp(atr->name, actor_name); atr = atr->next);

         if (atr != (atom *)NULL)
            target = atr->ob->find((quark *)NULL, piece);
      }

      if (target != (quark *)NULL) {
         copyarray4(temp, target->oldcenter);		// old location
         matvecmulto(old_imx, temp);

         new_vz[0] = temp[0] - r[0][3] + offset[0];
         new_vz[1] = temp[1] - r[1][3] + offset[1];
         new_vz[2] = temp[2] - r[2][3] + offset[2];

         calc_orientation(new_vz);
      }

   }

   copyarray3(xmx[0], old_vx);
   copyarray3(xmx[1], old_vy);
   copyarray3(xmx[2], old_vz);
   xmx[3][0] = r[0][3];
   xmx[3][1] = r[1][3];
   xmx[3][2] = r[2][3];
   transpose(xmx);
   matmatmulto(mx, xmx);

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];
   center[3] = 1;

   inversemx(mx, old_imx);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, xmx);
}