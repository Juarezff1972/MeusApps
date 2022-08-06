



/* *************************************************************
************************************************************* */

#include <string.h>
#include <stdlib.h>

#include "base.h"
#include "memman.h"


/* *************************************************************
************************************************************* */
void simple_far_light::lighting(float *surface, float *gnormal, float *rcolor,
        shadetype *shade, float *mIp, float *vlight, float dist2surface) {

   simple_light;
}


/* *************************************************************
************************************************************* */
void simple_point_light::lighting(float *surface, float *gnormal, float *rcolor,
        shadetype *shade, float *mIp, float *vlight, float dist2surface) {

   simple_light;
}


/* *************************************************************
************************************************************* */
void simple_beam_far::lighting(float *surface, float *gnormal, float *rcolor,
        shadetype *shade, float *mIp, float *vlight, float dist2surface) {

   simple_light;
}


/* *************************************************************
************************************************************* */
void simple_beam_point::lighting(float *surface, float *gnormal, float *rcolor,
        shadetype *shade, float *mIp, float *vlight, float dist2surface) {

   simple_light;
}


/* *************************************************************
************************************************************* */
void light::set_ambient(float *ka, float *ambient, float *lum) {

   ambient[0] = Ia[0]*ka[0] + lum[0];
   ambient[1] = Ia[1]*ka[1] + lum[1];
   ambient[2] = Ia[2]*ka[2] + lum[2];
}


/* *************************************************************
   This procedure calculates the intensity of light at a certain point
   * this function also takes into account the position of the light
     source, and shadows the appropriate sides :)

************************************************************* */
void light::lighting(float *surface, float *gnormal, float *rcolor,
                      shadetype *shade, float *mIp, float *vlight, float dist2surface) {

   int i;                                                  /* looping var  */
   float NL;                                               /* normal.light */
   float fatten;                                           /* light atten  */
   float R[4];                                             /* R vector     */
   vector3f V;

   NL = dotproduct3(gnormal, vlight);   // normal . light

   if (NL <= 0)         // backface
      return;

   V[0] = -surface[0];
   V[1] = -surface[1];
   V[2] = -surface[2];

   fatten = fnormalize3(V) + dist2surface;
   fatten = fatt[0] + (fatt[1] + fatt[2]*fatten)*fatten;    // calculate fatt

   for (i=0; i<3; i++) {                              // calculate R vector
      R[i] = NL * gnormal[i];
      R[i] += R[i] - vlight[i];
   }

   R[0] = dotproduct3(R, V);

   if (R[0] > CORRECT) {
//      R[3] = (shade->specn > CORRECT) ? pow(R[0], shade->specn) : 1.0;
      i = (int)(shade->specn);
      R[3] = (i) ? POW(R[0], i) : 1.0;
   }

   else
      R[3] = 0;


   if (fatten > 1) {
      R[3] /= fatten;
      NL   /= fatten;
   }
                                                      // assign pixel color
   rcolor[0] += mIp[0]*(shade->kp[0]*NL + shade->ks[0]*R[3]);
   rcolor[1] += mIp[1]*(shade->kp[1]*NL + shade->ks[1]*R[3]);
   rcolor[2] += mIp[2]*(shade->kp[2]*NL + shade->ks[2]*R[3]);
}


/* *************************************************************
************************************************************* */
int light::parse(FILE *infile, char *token) {

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

      default:
         break;
   }

   return eye::parse(infile, token);
}


/* *************************************************************
        parallel project... (no div by z)
************************************************************* */
void far_light::map2screen(float *v) {

   v[0]  = mscale * (v[0] - vrc[0]);
   v[1]  = mscale * (v[1] - vrc[2]);
}


/* *************************************************************
   Scale image from the window
************************************************************* */
void far_light::screen2map(float *v) {

   v[0]     = (v[0]*imscale + vrc[0]);
   v[1]     = (v[1]*imscale + vrc[2]);
}


/* *************************************************************
************************************************************* */
int far_light::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'd':
         if (!strcmp(token, "direction")) {
            get_token(infile, token);
            vpn[0] = -atof(token);
            get_token(infile, token);
            vpn[1] = -atof(token);
            get_token(infile, token);
            vpn[2] = -atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return light::parse(infile, token);
}


/* *************************************************************
   This procedure calculates the intensity of light at a certain point
   * this function also takes into account the position of the light
     source, and shadows the appropriate sides :)
************************************************************* */
void far_light::intensity(float* surface, float *gnormal, float *rcolor,
                      eye *parm, shadetype *shade, superclass *source) {

   lighting(surface, gnormal, rcolor, shade, Ip, transvec, 0);
}


/* *************************************************************
************************************************************* */
void far_light::xform(eye *parm) {

   copyarray3(transvec, vpn);
   transvec[3] = 0;
   matvecmultv(parm->transform, transvec);
}


/* *************************************************************
************************************************************* */
int far_light::read_data(FILE *infile, texbase **tptr) {

   fscanf(infile, "%f %f %f", &vpn[0], &vpn[1], &vpn[2]);
   fscanf(infile, "%f %f %f", &Ia[0], &Ia[1], &Ia[2]);
   fscanf(infile, "%f %f %f", &Ip[0], &Ip[1], &Ip[2]);
   smultarray3(vpn, -1);

   fscanf(infile, "%f %f %f", &fatt[0], &fatt[1], &fatt[2]);
   preprocess((void *)tptr);

   return 1;
}


/* *************************************************************
************************************************************* */
void far_light::write_lst(FILE *outfile, int frameno) {

   fprintf(outfile, "%d\nLIGHT\n", frameno);
   fprintf(outfile, "BEAM_FAR\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   fprintf(outfile, "%f %f %f\n", Ia[0], Ia[1], Ia[2]);
   fprintf(outfile, "%f %f %f\n", Ip[0], Ip[1], Ip[2]);

   fprintf(outfile, "%f %f %f\n", fatt[0], fatt[1], fatt[2]);
}

/* *************************************************************
   Scale image to the window
************************************************************* */
void point_light::map2screen(float *v) {

   v[0]  = -mscale * (v[0]/v[2] + vrc[0]);
   v[1]  = -mscale * (v[1]/v[2] + vrc[2]);
}


/* *************************************************************
   Scale image from the window
************************************************************* */
void point_light::screen2map(float *v) {

   v[1]     = -(v[1]*imscale + vrc[2]) * v[2];
   v[0]     = -(v[0]*imscale + vrc[0]) * v[2];
}


/* *************************************************************
   This procedure calculates the intensity of light at a certain point
   * this function also takes into account the position of the light
     source, and shadows the appropriate sides :)
************************************************************* */
void point_light::intensity(float* surface, float *gnormal, float *rcolor,
                      eye *parm, shadetype *shade, superclass *source) {

   vector3f vlight;
   float    dist2surface;

   subeqarray3(vlight, transloc, surface);
   dist2surface = fnormalize3(vlight);

   lighting(surface, gnormal, rcolor, shade, Ip, vlight, dist2surface);
}


/* *************************************************************
************************************************************* */
void point_light::xform(eye *parm) {

   copyarray3(transloc, location);
   transloc[3] = 1;
   matvecmulto(parm->transform, transloc);
}


/* *************************************************************
************************************************************* */
int point_light::read_data(FILE *infile, texbase **tptr) {

   fscanf(infile, "%f %f %f", &location[0], &location[1], &location[2]);
   fscanf(infile, "%f %f %f", &Ia[0], &Ia[1], &Ia[2]);
   fscanf(infile, "%f %f %f", &Ip[0], &Ip[1], &Ip[2]);
   fscanf(infile, "%f %f %f", &fatt[0], &fatt[1], &fatt[2]);

   preprocess((void *)tptr);

   return 1;
}


/* *************************************************************
************************************************************* */
void point_light::write_lst(FILE *outfile, int frameno) {

   fprintf(outfile, "%d\nLIGHT\n", frameno);
   fprintf(outfile, "BEAM_POINT\n");
   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   fprintf(outfile, "%f %f %f\n", Ia[0], Ia[1], Ia[2]);
   fprintf(outfile, "%f %f %f\n", Ip[0], Ip[1], Ip[2]);

   fprintf(outfile, "%f %f %f\n", fatt[0], fatt[1], fatt[2]);
}


/* *************************************************************
************************************************************* */
void spotlight::preprocess(void *data) {

   float temp[2];
   float i, j;
   texbase **tptr = (texbase **)data;

   light::preprocess(data);

   width /= distance2plane;
   length /= distance2plane;

   for (tob=*tptr; tob!=(texbase *)NULL; tob=tob->next)
      if (!strcmp(texname, tob->filename))
         break;

   if (tob == (texbase *)NULL) {

      if (query_video(texname)) {
         tob = new texvid;
         tob->read_texture(texname, tptr);
      }

      else {
         tob = new texture;
         tob->read_texture(texname);
      }

      tob->next = *tptr;
      *tptr = tob;
   }

   i = tob->maxx;
   j = tob->maxy;

   temp[0] = length/width;
   temp[1] = i/j;

   if (temp[0] > temp[1])            // reshape viewport to texture ratios;
      length = temp[1]*width;
   else
      width = length/temp[1];

   temp[0] = 0.5*length;
   temp[1] = 0.5*width;

   vrc[0] = -temp[0];
   vrc[1] =  temp[0];
   vrc[2] = -temp[1];
   vrc[3] =  temp[1];

   mscale = (width > length) ? MAXLDIM/width : MAXLDIM/length;
   imscale = 1.0/mscale;

   maxx = (int)(length * mscale);
   maxy = (int)(width * mscale);

   ztscale = (i>j) ? i/(float)MAXLDIM : j/(float)MAXLDIM;      // scale from lbuff to tmap

   nper();
}


/* *************************************************************
************************************************************* */
int spotlight::parse(FILE *infile, char *token) {

   switch (token[0]) {
      case '2':
         if (!strcmp(token, "2d_texture")) {
            get_token(infile, texname);

            return 1;
         }

         break;

      case 'd':
         if (!strcmp(token, "distance2plane")) {
            get_token(infile, token);
            distance2plane = atof(token);

            return 1;
         }

         if (!strcmp(token, "direction")) {
            get_token(infile, token);
            vpn[0] = -atof(token);
            get_token(infile, token);
            vpn[1] = -atof(token);
            get_token(infile, token);
            vpn[2] = -atof(token);

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

   return light::parse(infile, token);
}


/* *************************************************************
************************************************************* */
int spotlight::init_buff() {

   unsigned int i;
   unsigned int zero = 0;
   float b = back;

   if (lbuff == (lightbufftype *)NULL)
      lbuff = (lightbufftype *)control.pop(LIGHTBUFF);

   if (!lbuff->lbuff.data)
      lbuff->lbuff.init_map(MAXLDIM, MAXLDIM);

   i = SQR(MAXLDIM);

   do {
      lbuff->lbuff.data[i-1].zdata = b;
      lbuff->lbuff.data[i-1].idbuff = zero;
      lbuff->lbuff.data[i-2].zdata = b;
      lbuff->lbuff.data[i-2].idbuff = zero;
      lbuff->lbuff.data[i-3].zdata = b;
      lbuff->lbuff.data[i-3].idbuff = zero;
      lbuff->lbuff.data[i-4].zdata = b;
      lbuff->lbuff.data[i-4].idbuff = zero;
      i -= 4;
   } while (i);

   return 1;
}


/* *************************************************************
************************************************************* */
void spotlight::xform(eye *parm) {

   float temp[4][4];

   copyarray3(transvec, vpn);
   transvec[3] = 0;
   matvecmultv(parm->transform, transvec);

   copyarray3(transloc, location);
   transloc[3] = 1;
   matvecmulto(parm->transform, transloc);

   copymx4x4(temp, transform);
   copymx4x4(transform, parm->Tinverse);
   matmatmulto(temp, transform);

   matmatmulto(parm->transform, Tinverse);

   area = 1.0/(mscale*parm->query_scale());
}


/* *************************************************************
************************************************************* */
int spotlight::read_data(FILE *infile, texbase **tptr) {

   fscanf(infile, "%f %f %f", &location[0], &location[1], &location[2]);
   fscanf(infile, "%f %f %f", &vpn[0], &vpn[1], &vpn[2]);
   fscanf(infile, "%f %f %f", &vup[0], &vup[1], &vup[2]);
   fscanf(infile, "%f", &distance2plane);
   fscanf(infile, "%f %f", &width, &length);
   fscanf(infile, "%s", texname);               // texture name
   smultarray3(vpn, -1);
   fscanf(infile, "%f %f %f", &fatt[0], &fatt[1], &fatt[2]);

   preprocess((void *)tptr);

   return 1;
}


/* *************************************************************
        parallel project... (no div by z)
************************************************************* */
void beam_far::map2screen(float *v) {

   v[0] = mscale * (v[0] - vrc[0]);
   v[1] = mscale * (v[1] - vrc[2]);
}


/* *************************************************************
   Scale image from the window
************************************************************* */
void beam_far::screen2map(float *v) {

   v[0] = (v[0]*imscale + vrc[0]);
   v[1] = (v[1]*imscale + vrc[2]);
}


/* *************************************************************
id - 0          always (transparent)
id - "+"        id of object
id - "-"        ignore (should never call... ergo never happen)
************************************************************* */
void beam_far::intensity(float *surface, float *gnormal, float *rcolor, eye *parm,
                        shadetype *shade, superclass *source) {

   vector4f pt;
   int  i, j, k, l;
   int  frac;
   int  pt2[2][2];
   float temp;
   float csquared;
   vector3f tempv;
   float light_cam_dist;

   vector3f mIp;
   float dist2surface;

   if (!((pc *)source)->id)
      return;

   copyarray3(pt, surface);
   pt[3] = 1;

   matvecmulto(transform, pt);          // from camera space to light space

   dist2surface = -pt[2];

   if (dist2surface < CORRECT)
      return;

   map2screen(pt);

   if (pt[0] < 0 || pt[0] >= maxx || pt[1] < 0 || pt[1] >= maxy)
      return;

   csquared = dotproduct3(gnormal, transvec);

   if (csquared < CORRECT)
      return;

   csquared = SQRT(csquared);

   j = (int)pt[0];
   i = (int)pt[1];

   subeqarray3(tempv, surface, parm->location);
   light_cam_dist = fmagnitude3(tempv);

   if (((pc *)source)->id == lbuff->lbuff.pdata[i][j].idbuff) {

      j = (int)(tempv[0] = pt[0]*ztscale);
      i = (int)(tempv[1] = pt[1]*ztscale);

      temp = (light_cam_dist*area)/csquared;

      tob->query_texel(tempv[0], tempv[1], mIp, frame,1,temp);

      lighting(surface, gnormal, rcolor, shade, mIp, transvec, dist2surface);

      return;
   }

   pt2[0][0] = j-1;
   pt2[1][0] = j+1;
   pt2[0][1] = i-1;
   pt2[1][1] = i+1;

   j = (int)(tempv[0] = pt[0]*ztscale);
   i = (int)(tempv[1] = pt[1]*ztscale);

   if (pt2[0][0] < 0)
      pt2[0][0] = 0;
   if (pt2[1][0] >= maxx)
      pt2[1][0] = maxx-1;
   if (pt2[0][1] < 0)
      pt2[0][1] = 0;
   if (pt2[1][1] >= maxy)
      pt2[1][1] = maxy-1;

   frac = 0;

   for (l=pt2[0][1]; l<pt2[1][1]; l++)
      for (k=pt2[0][0]; k<pt2[1][0]; k++)
         if (((pc *)source)->id == lbuff->lbuff.pdata[l][k].idbuff)
            frac += 25;

   if (!frac)
      return;

   temp = (light_cam_dist*area)/csquared;

   tob->query_texel(tempv[0], tempv[1], mIp, frame,1,temp);

   temp = (frac + 50) * 0.002;
   smultarray3(mIp, temp);

   lighting(surface, gnormal, rcolor, shade, mIp, transvec, dist2surface);
}


/* *************************************************************
************************************************************* */
void beam_far::write_lst(FILE *outfile, int frameno) {

   fprintf(outfile, "%d\nLIGHT\n", frameno);

   fprintf(outfile, "BEAM_FAR\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   fprintf(outfile, "%f %f %f\n", -vpn[0], -vpn[1], -vpn[2]);
   fprintf(outfile, "%f %f %f\n", vup[0], vup[1], vup[2]);
   fprintf(outfile, "1\n");
   fprintf(outfile, "%f %f\n", vrc[3]+vrc[3], vrc[1]+vrc[1]);
   fprintf(outfile, "%s\n", tob->filename);               // texture name

   fprintf(outfile, "%f %f %f\n", fatt[0], fatt[1], fatt[2]);
}


/* *************************************************************
   Scale image to the window
************************************************************* */
void beam_point::map2screen(float *v) {

   v[0]  = -mscale * (v[0]/v[2] + vrc[0]);
   v[1]  = -mscale * (v[1]/v[2] + vrc[2]);
}


/* *************************************************************
   Scale image from the window
************************************************************* */
void beam_point::screen2map(float *v) {

   v[1]     = -(v[1]*imscale + vrc[2]) * v[2];
   v[0]     = -(v[0]*imscale + vrc[0]) * v[2];
}


/* *************************************************************
id - 0          always (transparent)
id - "+"        id of object
id - "-"        ignore (should never call... ergo never happen)
************************************************************* */
void beam_point::intensity(float *surface, float *gnormal, float *rcolor, eye *parm,
                        shadetype *shade, superclass *source) {

   vector4f pt;
   int  i, j, k, l;
   int  frac;
   int  pt2[2][2];
   float temp;
   float csquared;
   vector3f tempv;
   float light_cam_dist;

   vector3f mIp;
   float dist2surface;
   vector3f vlight;

   if (!((pc *)source)->id)
      return;

   copyarray3(pt, surface);
   pt[3] = 1;

   matvecmulto(transform, pt);          // from camera space to light space

   dist2surface = fmagnitude3(pt);

   if (dist2surface < CORRECT)
      return;

   map2screen(pt);

   if (pt[0] < 0 || pt[0] >= maxx || pt[1] < 0 || pt[1] >= maxy)
      return;

   subeqarray3(vlight, transloc, surface);      // calculate surface to light
   fnormalize3(vlight);

   csquared = dotproduct3(gnormal, tempv);

   if (csquared < CORRECT)
      return;

   csquared = SQRT(csquared);

   j = (int)pt[0];
   i = (int)pt[1];

   subeqarray3(tempv, surface, parm->location);
   light_cam_dist = fmagnitude3(tempv);

   if (((pc *)source)->id == lbuff->lbuff.pdata[i][j].idbuff) {

      j = (int)(tempv[0] = pt[0]*ztscale);
      i = (int)(tempv[1] = pt[1]*ztscale);

      temp = (light_cam_dist*area)/csquared;

      tob->query_texel(tempv[0], tempv[1], mIp, frame,1,temp*light_cam_dist);           // alt - could use zbuff for z values instead of calc distance....

      lighting(surface, gnormal, rcolor, shade, mIp, vlight, dist2surface);

      return;
   }

   pt2[0][0] = j-1;
   pt2[1][0] = j+1;
   pt2[0][1] = i-1;
   pt2[1][1] = i+1;

   j = (int)(tempv[0] = pt[0]*ztscale);
   i = (int)(tempv[1] = pt[1]*ztscale);

   if (pt2[0][0] < 0)
      pt2[0][0] = 0;
   if (pt2[1][0] >= maxx)
      pt2[1][0] = maxx-1;
   if (pt2[0][1] < 0)
      pt2[0][1] = 0;
   if (pt2[1][1] >= maxy)
      pt2[1][1] = maxy-1;

   frac = 0;

   for (l=pt2[0][1]; l<pt2[1][1]; l++)
      for (k=pt2[0][0]; k<pt2[1][0]; k++)
         if (((pc *)source)->id == lbuff->lbuff.pdata[l][k].idbuff)
            frac += 25;

   if (!frac)
      return;

   temp = (light_cam_dist*area)/csquared;

   tob->query_texel(tempv[0], tempv[1], mIp, frame,1,temp*light_cam_dist);              // alt - could use zbuff for z values instead of calc distance....

   temp = (frac + 50) * 0.002;
   smultarray3(mIp, temp);

   lighting(surface, gnormal, rcolor, shade, mIp, vlight, dist2surface);
}


/* *************************************************************
************************************************************* */
void beam_point::write_lst(FILE *outfile, int frameno) {

   fprintf(outfile, "%d\nLIGHT\n", frameno);

   fprintf(outfile, "BEAM_POINT\n");

   fprintf(outfile, "%f %f %f\n", location[0], location[1], location[2]);

   fprintf(outfile, "%f %f %f\n", -vpn[0], -vpn[1], -vpn[2]);
   fprintf(outfile, "%f %f %f\n", vup[0], vup[1], vup[2]);
   fprintf(outfile, "1\n");
   fprintf(outfile, "%f %f\n", vrc[3]+vrc[3], vrc[1]+vrc[1]);
   fprintf(outfile, "%s\n", tob->filename);               // texture name

   fprintf(outfile, "%f %f %f\n", fatt[0], fatt[1], fatt[2]);
}

