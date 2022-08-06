



#if !defined (__SBFAST_H)
#define __SBFAST_H

#include "texobj.h"

/* *************************************************************
        sbuffer -
                        0:      not done
                        1:      done and hit
                        2:      done and no hit
                        4:      dither
************************************************************* */
class sbfast : public pc {

   protected:
                                        // pc
      int bound_box(eye *parm);
      void transform(eye *parm);
      int clip(eye *parm, int maxx, int maxy) { return 0; }

                                        // sbfast
      float   back;                     // max distance from camera
      fractal_params params;

      shaderparamtype dis;

      vector4f old_pos;
      int   start_flag;

      char shadername[MAXSTRLEN];
      shader  *displacement;

      float   isize;
      char    ditherflag;

      texsbfast *tob;
      mapuc *sbbuffer;

      float Intersect_Surface(float distance, float increment,
                      float *cam, float *ray, float *surface, float *normal);
      int Intersect_Terrain(float increment, float *cam, float *ray,
                      float *surface, float *normal);

      void sbfastpg_vertical(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int dflag);
      void sbfastpg_horizontal(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int dflag);

      void sbfastpg_llurh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy);
      void sbfastpg_llurv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx);

      void sbfastpg_urllh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy);
      void sbfastpg_urllv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx);

      void sbfastpg_ullrv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx);
      void sbfastpg_ullrh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy);

      void sbfastpg_lrulv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx);
      void sbfastpg_lrulh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy);

      void sbfastpg_verticald(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int midy);
      void sbfastpg_horizontald(eye *parm, light *lmain, light *spot, zbuffer *zbuff, int midx);

      void sbfastpg_posv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx, float *up, float *horizon);
      void sbfastpg_posh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy, float *up, float *horizon);

      void sbfastpg_negv(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dx, float *up, float *horizon);
      void sbfastpg_negh(eye *parm, light *lmain, light *spot, zbuffer *zbuff, float dy, float *up, float *horizon);

//      void sbfastpg_postscan(zbuffer *zbuff);
      void dither(zbuffer *zbuff);

      void sbfastbm_vertical(spotlight *spot, int dflag);
      void sbfastbm_horizontal(spotlight *spot, int dflag);

      void sbfastbm_llurh(spotlight *spot, float dy);
      void sbfastbm_llurv(spotlight *spot, float dx);

      void sbfastbm_urllh(spotlight *spot, float dy);
      void sbfastbm_urllv(spotlight *spot, float dx);

      void sbfastbm_ullrv(spotlight *spot, float dx);
      void sbfastbm_ullrh(spotlight *spot, float dy);

      void sbfastbm_lrulv(spotlight *spot, float dx);
      void sbfastbm_lrulh(spotlight *spot, float dy);

      void sbfastbm_verticald(spotlight *spot, int midy);
      void sbfastbm_horizontald(spotlight *spot, int midx);

      void sbfastbm_posv(spotlight *spot, float dx, float *up, float *horizon);
      void sbfastbm_posh(spotlight *spot, float dy, float *up, float *horizon);

      void sbfastbm_negv(spotlight *spot, float dx, float *up, float *horizon);
      void sbfastbm_negh(spotlight *spot, float dy, float *up, float *horizon);

      void sbfastbm_postscan(spotlight *spot);

   public:
                                                        // superclass
      object_type query_whatami() { return SBFAST; }
      int  parse(FILE *infile, char *token);
      void preprocess(void *data);
                                                        // pc
      void begin_scan() {

         params.freq = (float *)NULL;
         init_freq(&params);

         if (displacement == (shader *)NULL)
            displacement = shades->find("default_alt");

         sbbuffer = new mapuc;
      }

      void end_scan() {

         if (params.freq) {
            delete [] params.freq;
            params.freq = NULL;
         }

         if (sbbuffer != (mapuc *)NULL) {
            delete sbbuffer;
            sbbuffer = (mapuc *)NULL;
         }

      }

      void datacopy() {
         if (mctype == PALEX)
            base_color = &lob->base;
      }

      int read_data(FILE *infile);
      int  read_tex(char *filename, char *dfilename, texbase **tptr, texmap **tpptr);
      int  read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr);

      int beamscan(spotlight *spot);
      int scan(camera *cparm, light *lmain);

      void beamrender(spotlight *spot);
      void render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff);
      void prender();
                                                        // sbfast
      sbfast() {
         back = 1000;
         params.octaves = 100;
         params.H = 0.9;
         params.lacunarity = 2.1;
         displacement = (shader *)NULL;
//         SCALESB = 157.241;
         ditherflag = 0;
         strcpy(shadername, "default_alt");
      }

      float query_octaves() { return params.octaves; }
};

#endif
