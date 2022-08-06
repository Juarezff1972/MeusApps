



#include "flight.h"



/* *************************************************************
   place object based on local and parent xforms

1) calc new location
2) vpn = new direction = new loc - old loc
3) if new direction about same as old direction, use old vup, vright
4) else calc new vup/vright based upon plane created by new/old direction vectors
5) 2 possible vups/right -> vup, smallest rotation...


want to rotate around "z" until y[1] is positive, and x[1] = 0, as long as
z != { 0, +-1, 0 }
1) figure out positive or neg rotation around z, and the rotation
2) 1) A = old matrix, B = rotate mx
3) new matrix C = A*B


************************************************************* */
void flight::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   linktype *ptr;                         // pointer
   vector3f new_vz;
   vector4f r[4];

   init_mx(xmx);
   calc_initmx(xmx, r);

   if (!firstflag) {
      new_vz[0] = r[0][3] - old_local_center[0];
      new_vz[1] = r[1][3] - old_local_center[1];
      new_vz[2] = r[2][3] - old_local_center[2];

      calc_orientation(new_vz);
   }

   copyarray3(xmx[0], old_vx);
   copyarray3(xmx[1], old_vy);
   copyarray3(xmx[2], old_vz);
   old_local_center[0] = xmx[3][0] = r[0][3];
   old_local_center[1] = xmx[3][1] = r[1][3];
   old_local_center[2] = xmx[3][2] = r[2][3];
   transpose(xmx);
   matmatmulto(mx, xmx);

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];
   center[3] = 1;

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, xmx);
}