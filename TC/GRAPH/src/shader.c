
#include <string.h>
#include "shader.h"
#include "shaders.h"


shader *shades = (shader *)NULL;

/* *************************************************************************
************************************************************************* */
void shaderparamtype::setup(vector4f *xf, vector4f *ixf, int f, eye *c, pc *o, float fscale) {

   float ifscale;

   copymx4x4(ixform, ixf);
   smultarray4(ixform[0], fscale);
   smultarray4(ixform[1], fscale);
   smultarray4(ixform[2], fscale);
   smultarray3(ixform[3], fscale);

   ifscale = 1.0/fscale;
   copymx4x4(xform, xf);
   smultarray4(xform[0], ifscale);
   smultarray4(xform[1], ifscale);
   smultarray4(xform[2], ifscale);
   smultarray3(xform[3], ifscale);

   frame = f;

   cam = c;
   ob = o;
}


/* *************************************************************************
************************************************************************* */
void shaderparamtype::set_point(float *local, float *n) {

   copyarray4(normal, n);
   in[0] = pt[0] = local[0];
   in[0] = pt[0] = local[0];
   in[1] = pt[1] = local[1];
   in[2] = pt[2] = local[2];
   matvecmulto(ixform, in);
}


/* *************************************************************************
************************************************************************* */
shadertype &shadertype::operator = (shadertype &x) {

   scount = x.scount;
   if (s != (shader **)NULL)
      delete [] s;
   if (stype != (shader_type *)NULL)
      delete [] stype;

   s = new pshader[scount];
   stype = new shader_type[scount];

   memcpy(stype, x.stype, sizeof(shader_type *)*scount);
   memcpy(s, x.s, sizeof(pshader)*scount);
   return x;
}


/* *************************************************************************
************************************************************************* */
void shadertype::read_data(FILE *infile, shader *sptr) {

   char tmpstr[MAXSTRLEN], tmpstr2[MAXSTRLEN];
   int j;

   fscanf(infile, "%d", &scount);

   if (s != (pshader *)NULL)
      delete [] s;

   if (stype != (shader_type *)NULL)
      delete [] stype;

   s = new pshader[scount];
   stype = new shader_type[scount];

   for (j=0; j<scount; j++) {
      fscanf(infile, "%s %s", tmpstr, tmpstr2);

      if (tmpstr2[0] == 'A')
         stype[j] = ATMOSPHERE;
      else
         stype[j] = SURFACE;

      if ((s[j] = sptr->find(tmpstr)) == (shader *)NULL)
         printf("Can't find shader %s\n", tmpstr);
      else
         s[j]->init();
   }

}


/* *************************************************************************
************************************************************************* */

#ifdef LINUX
int shader::set(char *tmpstr, int func_call(void *), void init_call()) {
#else
int shader::set(char *tmpstr, void *func_call, void *init_call) {
#endif

   strcpy(name, tmpstr);
   shade_fcn = func_call;
   init_fcn = init_call;

   return 1;
}


/***************************************************************************
****************************************************************************/
shader *shader::find(char *target) {

   int x;

   if (!(x = strcmp(target, name)))
      return this;

   if (x < 0)
      if (left != (shader *)NULL)
         return left->find(target);
      else
         return (shader *)NULL;

   if (right != (shader *)NULL)
      return right->find(target);

   return (shader *)NULL;
}


/***************************************************************************
****************************************************************************/
void shader::insert(shader *x) {

   if (strcmp(x->name, name) < 0)
      if (left == (shader *)NULL)
         left = x;
      else
         left->insert(x);
   else
      if (right == (shader *)NULL)
         right = x;
      else
         right->insert(x);
}


/***************************************************************************
****************************************************************************/
void insert_shader(shader *head, char *str, int fcn(void *), void icn() = NULL) {

   shader *ptr;

   ptr = new shader;
   ptr->set(str, fcn, icn);
   head->insert(ptr);
}


/* **************************************************
************************************************** */
void init_shader() {

   init_noise();

   shades = new shader;
   shades->set("marble", marble);                                               // surface: marble texture
   insert_shader(shades, "wood", wood);                                         // surface: plywood texture
   insert_shader(shades, "wave", wave);                                         // surface: normal manipulator
   insert_shader(shades, "checker", checker);                                   // surface: 3D checker pattern
   insert_shader(shades, "wood_panel", wood_panel);                             // surface: plywood panels

   insert_shader(shades, "moon", moon, moon_init);                              // surface: fractal moon
   insert_shader(shades, "planet", planet, planet_init);                        // surface: fractal planet - variation of moon
   insert_shader(shades, "cloud", cloud, cloud_init);                           // surface: cloud texture
   insert_shader(shades, "stars", stars, stars_init);                           // surface: star texture
   insert_shader(shades, "warp", warp, warp_init);                              // surface: displaces xyz for color calculations
   insert_shader(shades, "terrain", terrain, terrain_init);                     // surface: color base on altitude
   insert_shader(shades, "planet2", planet2, planet2_init);                     // surface: another planet
   insert_shader(shades, "cloud2", cloud2, cloud2_init);                        // surface: another cloud texture
   insert_shader(shades, "terrain2", terrain2, terrain2_init);                  // surface: another terrain texture
   insert_shader(shades, "terrain3", terrain3, terrain3_init);                  // surface: yet another terrain
   insert_shader(shades, "cave_ceiling", cave_ceiling, cave_ceiling_init);      // surface: (stalagmite?) shader
   insert_shader(shades, "cave_floor", cave_floor, cave_floor_init);            // surface: (stalagtite?) shader

   insert_shader(shades, "fog_mus", fog_mus);                                   // atmosphere: musgrave's fog shader
   insert_shader(shades, "fog", fog);                                           // atmosphere: another fog shader
   insert_shader(shades, "fog2", fog2);                                         // atmosphere: yet another fog shader
   insert_shader(shades, "energy_beam", energy_beam);                           // atmosphere: "Vandagraph" electron bolts

   insert_shader(shades, "planetclouds", planetclouds);                         // surface: better than cloud2()
   insert_shader(shades, "contour", contour);                                   // surface: a weird contour map thingy
   insert_shader(shades, "carpet", carpet);                                     // surface: carpet texture

   insert_shader(shades, "default_alt", default_alt);                           // sbfast: a height field
   insert_shader(shades, "x_alt", x_alt);                                       // sbfast: default_alt w/o clamping
   insert_shader(shades, "mnb_alt", mnb_alt);                                   // sbfast: another height field
   insert_shader(shades, "barfland_alt", barfland_alt);                         // sbfast: another height field
}

