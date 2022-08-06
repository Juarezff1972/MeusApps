
#include <stdlib.h>

#include "base.h"
#include "molecule.h"

/* *************************************************************
   This function prints out data for a neutron to a file
************************************************************* */
void bpneutron::print_data(FILE *outfile, int tflag, float *floor) {

   int i;

   fprintf(outfile, "%d\n", frameno);             // send data to a file

   switch (ob->whatami()) {
      case NEUTRON:
         fprintf(outfile, "POLYGON\n");
         break;

      case BUCKEY:
         fprintf(outfile, "GEOSPHERE\n");
         break;

      case FUNNEL:
         fprintf(outfile, "GEOCONE\n");
         break;

      case TUBE:
         fprintf(outfile, "GEOCYLINDER\n");
         break;

      case SHELL:
         fprintf(outfile, "SPATCH\n");
         break;

      default:
         break;
   }

   if (tflag)
      fprintf(outfile, "WFBW\n");
   else
      fprintf(outfile, "%s\n", ob->obtype);

   fprintf(outfile, "%s\n", ob->obname);

   if (!tflag)
      fprintf(outfile, "%s\n", ob->colorname);

   fprintf(outfile, "%s\n", ob->texname);

   if ((ob->mcinfo & CIBITMAP) && (ob->mcinfo & CITEXTURE) && ob->whatami() == NEUTRON)
      fprintf(outfile, "%d %d %d\n", ((neutron *)ob)->ir, ((neutron *)ob)->ig, ((neutron *)ob)->ib);

   if (ob->shadow)
      fprintf(outfile, "SHADOW %f %f %f %f\n", floor[0], floor[1], floor[2], floor[3]);
   else
      fprintf(outfile, "NOSHADOW\n");

   fprintf(outfile, "0 0 0\n%f\n", ob->scale);

   for (i=0; i<4; i++)                          // local xform mx
      fprintf(outfile, "%f %f %f %f\n", mx[i][0], mx[i][1], mx[i][2], mx[i][3]);

   switch (ob->whatami()) {
      case BUCKEY:
         fprintf(outfile, "%f %f %f\n", ((buckey *)ob)->axis[0], ((buckey *)ob)->axis[1], ((buckey *)ob)->axis[2]);
         break;

      case FUNNEL:
         fprintf(outfile, "%f %f %f\n", ((funnel *)ob)->length, ((funnel *)ob)->radius[0], ((funnel *)ob)->radius[1]);
         break;

      case TUBE:
         fprintf(outfile, "%f %f\n", ((tube *)ob)->length, ((tube *)ob)->radius);
         break;

      default:
         break;
   }

}


/* *************************************************************
   This function prints out data for a photon to a file
************************************************************* */
void bpphoton::print_data(FILE *outfile, int tflag, float  floor[]) {

   fprintf(outfile, "%d\n", frameno);             // send data to a file
   fprintf(outfile, "LIGHT\n");

   if (ob->group == GLOBAL_LIGHT)
      if (ob->mode == POINT)
         if (ob->simpleflag)
            fprintf(outfile, "SIMPLE_POINT\n");
         else
            fprintf(outfile, "POINT\n");
      else
         if (ob->simpleflag)
            fprintf(outfile, "SIMPLE_FAR\n");
         else
            fprintf(outfile, "FAR\n");
   else
      if (ob->mode == POINT)
         if (ob->simpleflag)
            fprintf(outfile, "SIMPLE_BEAM_POINT\n");
         else
            fprintf(outfile, "BEAM_POINT\n");
      else
         if (ob->simpleflag)
            fprintf(outfile, "SIMPLE_BEAM_FAR\n");
         else
            fprintf(outfile, "BEAM_FAR\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   if (ob->group == GLOBAL_LIGHT) {
      fprintf(outfile, "%f %f %f\n", ob->Ia[0], ob->Ia[1], ob->Ia[2]);
      fprintf(outfile, "%f %f %f\n", ob->Ip[0], ob->Ip[1], ob->Ip[2]);
   }

   else {
      fprintf(outfile, "%f %f %f\n", ob->vpn[0], ob->vpn[1], ob->vpn[2]);
      fprintf(outfile, "%f %f %f\n", ob->vup[0], ob->vup[1], ob->vup[2]);
      fprintf(outfile, "%f\n", ob->distance2plane);
      fprintf(outfile, "%f %f\n", ob->width, ob->length);
      fprintf(outfile, "%s\n", ob->texname);
   }

   fprintf(outfile, "%f %f %f\n", ob->fatt[0], ob->fatt[1], ob->fatt[2]);
}


/* *************************************************************
   This function prints out data for an electron to a file
************************************************************* */
void bpelectron::print_data(FILE *outfile, int tflag, float  floor[]) {

   fprintf(outfile, "%d\n", frameno);             // send data to a file
   fprintf(outfile, "CAMERA\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);
   fprintf(outfile, "%f %f %f\n", dop[0], dop[1], dop[2]);
   fprintf(outfile, "%f %f %f\n", vup[0], vup[1], vup[2]);
}


/* *************************************************************
   This function prints out data for an ion to a file
************************************************************* */
void bpion::print_data(FILE *outfile, int tflag, float *floor) {

   fprintf(outfile, "%d\n", frameno);             // send data to a file

   if (tflag)
      fprintf(outfile, "RADIANT\n%WFBW\n");
   else
      fprintf(outfile, "RADIANT\n%s\n%s\n", ob->obtype, ob->colorname);

   fprintf(outfile, "CENTER %f %f %f\n", location[0], location[1], location[2]);
   fprintf(outfile, "RADIUS %f\nINNERRAD %f\n", outrad, inrad);
}


/* *************************************************************
   This function prints out data for an particle/tachyon
************************************************************* */
void bptachyon::print_data(FILE *outfile, int tflag, float  floor[]) {

   fprintf(outfile, "%d\n", frameno);             // send data to a file

   fprintf(outfile, "PARTICLE\n");

   if (tflag)
      fprintf(outfile, "WFBW\n");
   else
      fprintf(outfile, "%s\n", ob->obtype);

   if (ob->shadow)
      fprintf(outfile, "SHADOW %f %f %f %f\n", floor[0], floor[1], floor[2], floor[3]);
   else
      fprintf(outfile, "NOSHADOW\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   if (tflag || ob->obtype[0] != 'G')
      return;

   fprintf(outfile, "%f %f %f\n", ob->color[0]*intensity, ob->color[1]*intensity, ob->color[2]*intensity);
}


/* *************************************************************
************************************************************* */
void bpchain::print_data(FILE *outfile, int tflag, float  floor[]) {

   fprintf(outfile, "%d\n", frameno);             // send data to a file

   fprintf(outfile, "LINE\n");

   if (tflag)
      fprintf(outfile, "WFBW\n");
   else
      fprintf(outfile, "%s\n", ob->obtype);

   if (ob->shadow)
      fprintf(outfile, "SHADOW %f %f %f %f\n", floor[0], floor[1], floor[2], floor[3]);
   else
      fprintf(outfile, "NOSHADOW\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   if (!tflag && ob->obtype[0] == 'G')
      fprintf(outfile, "%f %f %f\n", ob->color[0][0]*intensity, ob->color[0][1]*intensity, ob->color[0][2]*intensity);

   fprintf(outfile, "%f %f %f\n", endpt[0], endpt[1], endpt[2]);

   if (tflag || ob->obtype[0] != 'G')
      return;

   fprintf(outfile, "%f %f %f\n", ob->color[1][0]*intensity, ob->color[1][1]*intensity, ob->color[1][2]*intensity);
}


/* *************************************************************
   This function prints out data for an sbfast/helix
************************************************************* */
void bphelix::print_data(FILE *outfile, int tflag, float  floor[]) {

   int i;

   fprintf(outfile, "%d\n", frameno);             // send data to a file

   fprintf(outfile, "SBFAST\n");

   fprintf(outfile, "%s\n", ob->obtype);

   fprintf(outfile, "%s\n", ob->colorname);

   fprintf(outfile, "%s\n", ob->texname);

   fprintf(outfile, "0 0 0\n%f\n", ob->scale);

   for (i=0; i<4; i++)                          // local xform mx
      fprintf(outfile, "%f %f %f %f\n", mx[i][0], mx[i][1], mx[i][2], mx[i][3]);

   if (ob->dither)
      fprintf(outfile, "back %f octaves %f h %f lacunarity %f displacement %s dither\n",
			ob->back, ob->octaves, ob->h, ob->lac, ob->disname);
   else
      fprintf(outfile, "back %f octaves %f h %f lacunarity %f displacement %s nodither\n",
			ob->back, ob->octaves, ob->h, ob->lac, ob->disname);
}

