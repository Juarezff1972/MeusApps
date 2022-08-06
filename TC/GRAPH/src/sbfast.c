

#include <stdlib.h>
#include <string.h>

#include "sbfast.h"
#include "global.h"

extern int INTERLACE;

#define CLEANUP -1
#define DITHER  -2
#define SBRENDER 1

/* *************************************************************
************************************************************* */
int sbfast::parse(FILE *infile, char *token) {

   switch (token[0]) {

      case 'b':
         if (!strcmp(token, "back")) {
            get_token(infile, token);
            back = atof(token);

            return 1;
         }

         break;

      case 'd':
         if (!strcmp(token, "displacement")) {
            get_token(infile, shadername);

            return 1;
         }

         if (!strcmp(token, "dither")) {
            if (!INTERLACE)
               ditherflag = 1;

            return 1;
         }

         break;

      case 'h':
         if (!token[1]) {
            get_token(infile, token);
            params.H = atof(token);

            return 1;
         }

         break;

      case 'l':
         if (!strcmp(token, "lacunarity")) {
            get_token(infile, token);
            params.lacunarity = atof(token);

            return 1;
         }

         break;

      case 'o':
         if (!strcmp(token, "octaves")) {
            get_token(infile, token);
            params.octaves = atof(token);

            return 1;
         }

         break;

      default:
         break;
   }

   return pc::parse(infile, token);
}


/* *************************************************************
************************************************************* */
void sbfast::preprocess(void *data) {

   listtype *ltr = (listtype *)data;

   pc::preprocess(data);

   displacement = ltr->shade_list->find(shadername);

   if ((mctype > BW && mctype <= PHONG) || mctype > PBW) {

      read_ilum(colorname, &ltr->itr);

      if (mcinfo & CITEXTURE)
         read_tex(texname2, filename, &ltr->tdtr, &ltr->tmap);
      else if (mcinfo & CI3DTEX)
         read_3dtex(texname3, filename, ltr->shade_list, &ltr->shltype);
   }

}


/* *************************************************************
************************************************************* */
int sbfast::read_data(FILE *infile) {

   char  junk[MAXSTRLEN], buffer[MAXSTRLEN];

   fscanf(infile, "%s %f %s %f %s %f %s %f %s %s %s", junk, &back,
        junk, &params.octaves, junk, &params.H, junk, &params.lacunarity, junk, shadername, buffer);

   ditherflag = !(strcmp(buffer, "dither"));

   return 1;
}


/* *************************************************************
************************************************************* */
int sbfast::read_tex(char *filename, char *dfilename, texbase **tptr, texmap **tpptr) {

   FILE *infile;
   int  i, j;
   char token[MAXSTRLEN];
   vector4f pt[4];

   for (tob=*(texsbfast **)tpptr; tob!=(texsbfast *)NULL; tob=(texsbfast *)tob->next)
      if (!strcmp(filename, tob->texname) && !strcmp(dfilename, tob->dataname))
         return 1;

   if (!(infile = fopen(filename, "r"))) {
      printf("Can't find file %s\n", filename);
      mcinfo &= ~CITEXTURE;
      return 0;
   }

   tob = new texsbfast;
   strcpy(tob->texname, filename);
   strcpy(tob->dataname, dfilename);

   tob->next = *tpptr;
   *tpptr = tob;

   get_token(infile, token);
   tob->countobject = atoi(token);

   tob->setup(tob->countobject);

   for (i=0; i<tob->countobject; i++) {
      get_token(infile, token);
      lower_case(token);

      tob->data[i].repeatflag = strcmp(token, "coord");

      get_token(infile, token); // texture name

      for (tob->data[i].ob=*tptr; tob->data[i].ob!=(texbase *)NULL; tob->data[i].ob=tob->data[i].ob->next)
         if (!strcmp(token, tob->data[i].ob->filename))
            break;

      if (tob->data[i].ob == (texbase *)NULL) {
         if (query_video(token)) {
            tob->data[i].ob = new texvid;
            tob->data[i].ob->read_texture(token, tptr);
         }

         else {
            tob->data[i].ob = new texture;
            tob->data[i].ob->read_texture(token);
         }

         tob->data[i].ob->next = *tptr;
         *tptr = tob->data[i].ob;
      }

// start the migration towards 0..1
//      maxx = 1.0/(tob->data[i].ob->query_x()-1.0);
//      maxy = 1.0/(tob->data[i].ob->query_y()-1.0);

      for (j=0; j<4; j++) {
         get_token(infile, token);
         pt[j][0] = atof(token);
         get_token(infile, token);
         pt[j][1] = atof(token);
      }

      tob->data[i].setup(pt);
   }

   fclose(infile);
   return 1;
}


/* *************************************************************
************************************************************* */
int sbfast::read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr) {

   int i;
   char buffer[MAXSTRLEN];
   FILE *infile;
   int count;

   for (shaderlist=*slptr; shaderlist!=(shaderlisttype *)NULL; shaderlist=shaderlist->next)
      if (!strcmp(filename, shaderlist->shname) && !strcmp(dfilename, shaderlist->dataname))
         return 1;

   if (!(infile = fopen(filename, "r"))) {
      printf("Can't find file %s\n", filename);
      mcinfo &= ~CI3DTEX;
      return 0;
   }

   shaderlist = new shaderlisttype;
   strcpy(shaderlist->shname, filename);
   strcpy(shaderlist->dataname, dfilename);
   shaderlist->init(1);
   shaderlist->next   = *slptr;
   *slptr       = shaderlist;

   fscanf(infile, "%s %f", buffer, &shaderlist->scale);
   lower_case(buffer);

   if (strcmp(buffer, "mono"))                 //ALL
      shaderlist->shade->read_data(infile, sptr);

   fscanf(infile, "%d", &count);

   for (; count>0; count--) {
      fscanf(infile, "%d", &i);

      if (!(i-1))
         shaderlist->shade->read_data(infile, sptr);
   }

   fclose(infile);
   return 1;
}


/* **************************************************
************************************************** */
int sbfast::bound_box(eye *parm) {

//octaves = 5.5 - log2(dist);
//SCALESB*max(rmf2 with pos, h, lac, octaves, 1, 2, 0)-1

// to be done at a later date

//   boundflag = 0;

   return 1;
}


/* **************************************************
 * Finds the intersection between line segment [a,b] and ray;
 * determines surface normal by constructing a plane.
 *
 * Note that this could be simplified by using a line/line intersection
 * test between line segment [a,b] and the ray.
************************************************** */
float sbfast::Intersect_Surface(float distance, float increment, float *cam,
                   float *ray, float *surface, float *normal) {

   vector3f vf,     // vector from p to pf
            vr;     // vector from p to pr
   float    d;      // ax + by + cz + d = 0
   float    t;

   // get two vectors in the surface plane; cross for surface normal

   dis.pt[0] = distance;

   copyarray2(dis.in, old_pos);
   dis.in[1] += increment;   // one pixel-width to front
   displacement->query_data((void *)&dis);
   vf[2] = dis.out[0] - old_pos[2];
   vf[1] = increment;
   vf[0] = 0;

   copyarray2(dis.in, old_pos);
   dis.in[0] += increment;   // one pixel-width to right
   displacement->query_data((void *)&dis);
   vr[2] = dis.out[0] - old_pos[2];
   vr[1] = 0;
   vr[0] = increment;

   xproduct(normal, vr, vf);
   normalize3(normal);

   d = dotproduct3(normal, old_pos);  // this is the "-D" distance from the origin to the plane

   t=dotproduct3(normal, ray);

   // assign the various hit data

   t = ((abs(t) < CORRECT) ? distance :  (( d - dotproduct3(normal, cam) ) / t));

   surface[0] = cam[0] + t*ray[0];
   surface[1] = cam[1] + t*ray[1];
   surface[2] = cam[2] + t*ray[2];

   return t;
}


/* **************************************************
 * Procedurally ray-trace a terrain given by function Displacement().
 *
 * Algorithm and code by F. Kenton Musgrave, 3/95
 *
 * Brute-force method steps across terrain at pixel-sized steps
 * until an intersection is found.
 *
 * Assumptions:
 *  - bottom-to-top scan
 *  - error inherent in pixel-sized horizontal steps is acceptable
 *  - parameter camera.near_clip is close enough to ray origin
 *  - downward-sloping rays: currently no vertical bound test
 *  - only for eye rays -- shadow rays (for directional light sources)
 *      should use a constant step size: the pixel size at ray origin
 *
 * Possible mods:
 *  - optimize based on prev col calc iff no horizontal jittering
 *  - use derivatives of terrain func to get greater step stride
 *  - add vertical bound test (only for upward-sloping rays)
 *  - modify for shadow rays with constant step size
************************************************** */
int sbfast::Intersect_Terrain(float increment, float *cam, float *ray, float *surface,
                      float *normal) {

   float   t;                 // ray parameter, equal to distance travelled
   float   acceleration, iacceleration;
   float   position[3];       // current position along ray
   float   start;

   copyarray3(position, ray);

   start = t = normalize3(ray);         // get magnitude, then normalize
   acceleration = increment/t;

   if (start_flag) {
      start_flag = 0;
      addarray3(position, cam);
   }

   else {
      t = old_pos[3];

      increment = t*acceleration;

      position[0] = cam[0] + t*ray[0];
      position[1] = cam[1] + t*ray[1];
      position[2] = cam[2] + t*ray[2];
   }

   copyarray2(dis.in, position);
   dis.pt[0] = t;
   displacement->query_data((void *)&dis);

   acceleration += 1;   // (1)
   iacceleration = 1.0/acceleration;

   if (position[2] < dis.out[0]) {
        //  we have "cave/cliff" artifacts

      do {
         increment *= iacceleration;    // (1)
         t -= increment;

         position[0] -= increment*ray[0];
         position[1] -= increment*ray[1];
         position[2] -= increment*ray[2];

         copyarray2(dis.in, position);
         dis.pt[0] = t;
         displacement->query_data((void *)&dis);
     } while (t > start && position[2] < dis.out[0]);   // go back til no intersect...

      copyarray2(old_pos, position);
      old_pos[2] = dis.out[0];
      old_pos[3] = t;

      Intersect_Surface(t, increment, cam, ray, surface, normal);

      return 1;
   }

   while ( t < back  && position[2] > dis.out[0]) { // (add test for top of BB here)
      old_pos[2] = dis.out[0];

      position[0] += increment*ray[0];
      position[1] += increment*ray[1];
      position[2] += increment*ray[2];
      t += increment;           // the all-important ray-creep increment

      increment *= acceleration;        // (1)

      copyarray2(dis.in, position);
      dis.pt[0] = t;
      displacement->query_data((void *)&dis);
   }

   if (position[2] <= dis.out[0]) {       // surface penetrated
      increment *= iacceleration;       // (1)
      t -= increment;

      position[0] -= increment*ray[0];
      position[1] -= increment*ray[1];

      copyarray2(old_pos, position);

      old_pos[3] = t;
      Intersect_Surface(t, increment, cam, ray, surface, normal);

      return 1;
   }

   return -1;       // Exceeded far clip distance; update "prev_dist" appropriately  & exit.
}


/* **************************************************
************************************************** */
void sbfast::transform(eye *parm) {

   copymx4x4(world, rotate);

   isize = 1.0/size;
   back *= isize;

   smultarray3(world[0], size);
   smultarray3(world[1], size);
   smultarray3(world[2], size);

   world[0][3] += center[0];
   world[1][3] += center[1];
   world[2][3] += center[2];

   matmatmulto(parm->transform, world);

   inversemx(world, iworld);
}


/* **************************************************
************************************************** */
void sbfast::beamrender(spotlight *lparm) {

   vector4f pt = {0,0,1,0};             // vertical column
   float one = 1.0-CORRECT;
   vector4f pzero = {0,0,0,1};          // camera origin
   vector4f vnormal = {0,0,1,1};       // vpn
   vector4f vector;                     // vertical vector
   vector4f temp;
   float dx, dy;
   vector4f vertex[4];

   if (mctype != PALEX  && mctype != ALEX)
      return;

   matvecmultv(world, pt);            // up vector
   copyarray4(vector, pt);
   pt[2] /= magnitude3(pt);
   normalize2(pt);

   vertex[0][0] = lparm->vrc[0];       // corners of viewplane "polygon"
   vertex[0][1] = lparm->vrc[2];
   vertex[0][2] = -1;
   vertex[0][3] = 1;

   vertex[1][0] = lparm->vrc[0];
   vertex[1][1] = lparm->vrc[3];
   vertex[1][2] = -1;
   vertex[1][3] = 1;

   vertex[2][0] = lparm->vrc[1];
   vertex[2][1] = lparm->vrc[3];
   vertex[2][2] = -1;
   vertex[2][3] = 1;

   vertex[3][0] = lparm->vrc[1];
   vertex[3][1] = lparm->vrc[2];
   vertex[3][2] = -1;
   vertex[3][3] = 1;

   if (line_cpoly_intersect( pzero, vector, vnormal, vertex, 4, temp)) {

      lparm->map2screen(temp);

      // if here then we are in the "special case"
      if (abs(pt[2]) > one || abs(pt[1]) > one) // perpendicular to viewplane || vertical
         sbfastbm_verticald(lparm, (int)temp[1]);
      else
         if (abs(pt[0]) > one)
            sbfastbm_horizontald(lparm, (int)temp[0]);
         else {       // if here then we can do diagonal :)

            dx = pt[0]/pt[1];
            dy = pt[1]/pt[0];

            vector[1] = pt[0];
            vector[0] = -pt[1];
            vector[2] = -(vector[0]*temp[0] + vector[1]*temp[1]);    // column

            pt[2] = -(pt[0]*temp[0] + pt[1]*temp[1]);        // perpendicular line

            if (dx > 0) {
               sbfastbm_posh(lparm, dy, vector, pt);
               sbfastbm_posv(lparm, dx, vector, pt);
            }

            else {
               sbfastbm_negh(lparm, dy, vector, pt);
               sbfastbm_negv(lparm, dx, vector, pt);
            }

         }

   }

   else
      if (abs(pt[1]) > one)
         sbfastbm_vertical(lparm, pt[1] >= 0);
      else
         if (abs(pt[0]) > one)
            sbfastbm_horizontal(lparm, pt[0] >= 0);
         else        // if here then we can do diagonal :)
            if (pt[0] > 0)
               if (pt[1] > 0) { // ll to ur
                  sbfastbm_llurh(lparm, pt[1]/pt[0]);
                  sbfastbm_llurv(lparm, pt[0]/pt[1]);
               }

               else { // ul to lr
                  sbfastbm_ullrh(lparm, pt[1]/pt[0]);
                  sbfastbm_ullrv(lparm, pt[0]/pt[1]);
               }

            else
               if (pt[1] > 0) { // lr to ul
                  sbfastbm_lrulh(lparm, pt[1]/pt[0]);
                  sbfastbm_lrulv(lparm, pt[0]/pt[1]);
               }

               else { // urll
                  sbfastbm_urllh(lparm, pt[1]/pt[0]);
                  sbfastbm_urllv(lparm, pt[0]/pt[1]);
               }

//   sbfastbm_postscan(lparm);
}


/* **************************************************
************************************************** */
void sbfast::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {

   vector4f pt = {0,0,1,0};             // vertical column
   float one = 1.0-CORRECT;
   vector4f pzero = {0,0,0,1};          // camera origin
   vector4f vnormal = {0,0,1,1};       // vpn
   vector4f vector;                     // vertical vector
   vector4f temp;
   float dx, dy;
   vector4f vertex[4];

   if (mctype != PALEX  && mctype != ALEX)
      return;

   matvecmultv(world, pt);            // up vector
   copyarray4(vector, pt);
   pt[2] /= magnitude3(pt);
   normalize2(pt);

   vertex[0][0] = cparm->vrc[0];       // corners of viewplane "polygon"
   vertex[0][1] = cparm->vrc[2];
   vertex[0][2] = -1;
   vertex[0][3] = 1;

   vertex[1][0] = cparm->vrc[0];
   vertex[1][1] = cparm->vrc[3];
   vertex[1][2] = -1;
   vertex[1][3] = 1;

   vertex[2][0] = cparm->vrc[1];
   vertex[2][1] = cparm->vrc[3];
   vertex[2][2] = -1;
   vertex[2][3] = 1;

   vertex[3][0] = cparm->vrc[1];
   vertex[3][1] = cparm->vrc[2];
   vertex[3][2] = -1;
   vertex[3][3] = 1;

   if (line_cpoly_intersect( pzero, vector, vnormal, vertex, 4, temp)) {

      cparm->map2screen(temp);

      // if here then we are in the "special case"
      if (abs(pt[2]) > one || abs(pt[1]) > one) // perpendicular to viewplane || vertical
         sbfastpg_verticald(cparm, lmain, spot, zbuff, (int)temp[1]);
      else if (abs(pt[0]) > one)
         sbfastpg_horizontald(cparm, lmain, spot, zbuff, (int)temp[0]);
      else {       // if here then we can do diagonal :)
         dx = pt[0]/pt[1];
         dy = pt[1]/pt[0];

         vector[1] = pt[0];
         vector[0] = -pt[1];
         vector[2] = -(vector[0]*temp[0] + vector[1]*temp[1]);    // column

         pt[2] = -(pt[0]*temp[0] + pt[1]*temp[1]);        // perpendicular line

         if (dx > 0) {
            sbfastpg_posh(cparm, lmain, spot, zbuff, dy, vector, pt);
            sbfastpg_posv(cparm, lmain, spot, zbuff, dx, vector, pt);
         }

         else {
            sbfastpg_negh(cparm, lmain, spot, zbuff, dy, vector, pt);
            sbfastpg_negv(cparm, lmain, spot, zbuff, dx, vector, pt);
         }

      }

   }

   else if (abs(pt[1]) > one)
      sbfastpg_vertical(cparm, lmain, spot, zbuff, pt[1] >= 0);
   else if (abs(pt[0]) > one)
      sbfastpg_horizontal(cparm, lmain, spot, zbuff, pt[0] >= 0);
   else if (pt[0] > 0)       // if here then we can do diagonal :)
      if (pt[1] > 0) { // ll to ur
         sbfastpg_llurh(cparm, lmain, spot, zbuff, pt[1]/pt[0]);
         sbfastpg_llurv(cparm, lmain, spot, zbuff, pt[0]/pt[1]);
      }

      else { // ul to lr
         sbfastpg_ullrh(cparm, lmain, spot, zbuff, pt[1]/pt[0]);
         sbfastpg_ullrv(cparm, lmain, spot, zbuff, pt[0]/pt[1]);
      }

   else if (pt[1] > 0) { // lr to ul
      sbfastpg_lrulh(cparm, lmain, spot, zbuff, pt[1]/pt[0]);
      sbfastpg_lrulv(cparm, lmain, spot, zbuff, pt[0]/pt[1]);
   }

   else { // urll
      sbfastpg_urllh(cparm, lmain, spot, zbuff, pt[1]/pt[0]);
      sbfastpg_urllv(cparm, lmain, spot, zbuff, pt[0]/pt[1]);
   }

//   sbfastpg_postscan(zbuff);

   if (ditherflag)
      dither(zbuff);
}


/* **************************************************
************************************************** */
void sbfast::prender() {

}


/* **************************************************
************************************************** */
int sbfast::scan(camera *cparm, light *lparm) {

   float deg;

   deg = 1.0/sqrt(1 + SQR(cparm->vrc[1]));
   deg = ACOS(deg);    // fov wrt x axis
   deg += deg;
   deg = LOG2(tan(deg/WINX)) + 3.92;          // usually around ~ -5, relates Noise() to Nyquist limit
// LOG2(tan(HALFPI/ WINX)) + 3.5

   transform(cparm);

   sbbuffer->init_map(WINX, WINY);
   memset(sbbuffer->data, 0, WINX*WINY);

   dis.params.octaves = deg;
   dis.params.H = params.H;
   dis.params.lacunarity = params.lacunarity;
   dis.params.freq = params.freq;
   dis.frame = frame;
   dis.ob = this;

   renderflag = SBRENDER;
   return 1;
}


/* **************************************************
************************************************** */
int sbfast::beamscan(spotlight *lparm) {

   float deg;

   deg = 1.0/sqrt(1 + SQR(lparm->vrc[1]));
   deg = ACOS(deg);    // fov wrt x axis
   deg += deg;
   deg = LOG2(tan(deg/WINX)) + 3.92;          // usually around ~ -5, relates Noise() to Nyquist limit
// LOG2(tan(HALFPI/ WINX)) + 3.5

   transform(lparm);

   sbbuffer->init_map(lparm->maxx, lparm->maxy);
   memset(sbbuffer->data, 0, lparm->maxx * lparm->maxy);

   dis.params.octaves = deg;
   dis.params.H = params.H;
   dis.params.lacunarity = params.lacunarity;
   dis.params.freq = params.freq;
   dis.frame = frame;
   dis.ob = this;

   renderflag = SBRENDER;
   return 1;
}
