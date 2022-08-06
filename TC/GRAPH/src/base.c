

#include <stdlib.h>
#include "datatype.h"
#include "global.h"

#define pre_calc_shade(sparam, i, k, surface, normal, sout, nout)       \
   sparam->set_point(surface, normal);                          \
   sparam->out[0] = sparam->out[1] = sparam->out[2] = 1;        \
   sparam->add[0] = sparam->add[1] = sparam->add[2] = 0;        \
   for (k=0; k<shaderlist->shade[i].scount; k++)                \
      if (shaderlist->shade[i].s[k] != (shader *)NULL && shaderlist->shade[i].stype[k] == SURFACE)      \
         shaderlist->shade[i].s[k]->query_data((void *)sparam); \
   copyarray3(sout, sparam->pt);                                \
   copyarray3(nout, sparam->normal)

#define post_calc_shade(sparam, i, k, acolor, rcolor)           \
   sparam->out[0] = sparam->out[1] = sparam->out[2] = 1;        \
   sparam->add[0] = sparam->add[1] = sparam->add[2] = 0;        \
   for (k=0; k<shaderlist->shade[i].scount; k++)                \
      if (shaderlist->shade[i].s[k] && shaderlist->shade[i].stype[k] == ATMOSPHERE)     \
         shaderlist->shade[i].s[k]->query_data((void *)sparam); \
   multarray3(rcolor, sparam->out);                             \
   acolor[0] = acolor[0]*sparam->out[0] + sparam->add[0]*255.0; \
   acolor[1] = acolor[1]*sparam->out[1] + sparam->add[1]*255.0; \
   acolor[2] = acolor[2]*sparam->out[2] + sparam->add[2]*255.0




int pc::read_data(FILE *infile) {
   return 0;
}


int pc::read_data(char filename[], datatype **dptr) {
   return 0;
}


int pc::read_tex(char *filename, char *dfilename, texbase **tptr, texmap **tpptr) {
   return 0;
}


int pc::read_3dtex(char *filename, char *dfilename, shader *sptr, shaderlisttype **slptr) {
   return 0;
}


pc::pc() {

   base_color = &gshade;
   shaderlist = (shaderlisttype *)NULL;
   mctype = WFBW;
   mcinfo = CINULL;
   center[0] = center[1] = center[2] = 0;
   center[3] = 1;
   size = 1;
   init_mx(rotate);
   boundflag = sflag = 0;
   lob = (shadelist *)NULL;
   renderflag = 0;
   colorname[0] = texname2[0] = texname3[0] = 0;
   shadptr = (pc *)NULL;
   next = (pc *)NULL;
}


int pc::read_ilum(char *filename, shadelist **itr) {

   for (lob=*itr; lob!=(shadelist *)NULL; lob=lob->next)
      if (!strcmp(filename, lob->shadename))
         return 0;

   lob = new shadelist;

   lob->read_data(filename);
   lob->next = *itr;
   *itr = lob;

   return 1;
}


/* *************************************************************
************************************************************* */
void pc::calc_color(float *surface, float *normal, light *lmain,
        eye *parm, texcolortype *texcolor, float *rcolor, float *acolor) {

   rcolor[0] = rcolor[1] = rcolor[2] = 0.0;
   copyarray3(acolor, texcolor->ambient);

   if (lmain)
      lmain->intensity(surface, normal, rcolor, parm, &texcolor->color, this);
}


/* *************************************************************
************************************************************* */
void pc::calc_colorb(float *surface, float *normal, light *lmain, light *spot,
        eye *parm, texcolortype *texcolor, float *rcolor, float *acolor) {

   light *ltr;

   rcolor[0] = rcolor[1] = rcolor[2] = 0.0;
   copyarray3(acolor, texcolor->ambient);

   if (lmain)
      lmain->intensity(surface, normal, rcolor, parm, &texcolor->color, this);

   for (ltr=spot; ltr; ltr=(light *)ltr->next)
      ltr->intensity(surface, normal, acolor, parm, &texcolor->color, this);
}


/* *************************************************************
************************************************************* */
void pc::calc_color2(float *surface, float *normal, light *lmain,
        eye *parm, texcolortype *texcolor, float *tcolor,
        float *rcolor, float *acolor) {

   rcolor[0] = rcolor[1] = rcolor[2] = 0.0;

   if (lmain)
      lmain->intensity(surface, normal, rcolor, parm, &texcolor->color, this);

   multarray3(rcolor, tcolor);

   acolor[0] = texcolor->ambient[0]*tcolor[0];
   acolor[1] = texcolor->ambient[1]*tcolor[1];
   acolor[2] = texcolor->ambient[2]*tcolor[2];
}


/* *************************************************************
************************************************************* */
void pc::calc_color2b(float *surface, float *normal, light *lmain,
        light *spot, eye *parm, texcolortype *texcolor, float *tcolor,
        float *rcolor, float *acolor) {

   light *ltr;

   rcolor[0] = rcolor[1] = rcolor[2] = 0.0;
   copyarray3(acolor, texcolor->ambient);

   if (lmain)
      lmain->intensity(surface, normal, rcolor, parm, &texcolor->color, this);

   for (ltr=spot; ltr; ltr=(light *)ltr->next)
      ltr->intensity(surface, normal, acolor, parm, &texcolor->color, this);

   multarray3(rcolor, tcolor);
   multarray3(acolor, tcolor);
}


/* *************************************************************
************************************************************* */
void pc::calc_color3(shaderparamtype *sparam, int i, float *surface, float *normal,
        light *lmain, eye *parm, texcolortype *texcolor, float *rcolor, float *acolor) {

   vector4f s, n;
   int k;

   pre_calc_shade(sparam, i, k, surface, normal, s, n);

   rcolor[0] = rcolor[1] = rcolor[2] = 0.0;

   if (lmain)
      lmain->intensity(s, n, rcolor, parm, &texcolor->color, this);

   multarray3(rcolor, sparam->out);
   acolor[0] = texcolor->ambient[0]*sparam->out[0] + sparam->add[0]*255.0;
   acolor[1] = texcolor->ambient[1]*sparam->out[1] + sparam->add[1]*255.0;
   acolor[2] = texcolor->ambient[2]*sparam->out[2] + sparam->add[2]*255.0;

   post_calc_shade(sparam, i, k, acolor, rcolor);
}


/* *************************************************************
************************************************************* */
void pc::calc_color3b(shaderparamtype *sparam, int i, float *surface, float *normal,
        light *lmain, light *spot, eye *parm, texcolortype *texcolor, float *rcolor,
        float *acolor) {

   light *ltr;
   vector4f s, n;
   int k;

   pre_calc_shade(sparam, i, k, surface, normal, s, n);

   rcolor[0] = rcolor[1] = rcolor[2] = 0.0;
   copyarray3(acolor, texcolor->ambient);

   if (lmain)
      lmain->intensity(s, n, rcolor, parm, &texcolor->color, this);

   for (ltr=spot; ltr; ltr=(light *)ltr->next)
      ltr->intensity(s, n, acolor, parm, &texcolor->color, this);

   multarray3(rcolor, sparam->out);
   acolor[0] = acolor[0]*sparam->out[0] + sparam->add[0]*255.0;
   acolor[1] = acolor[1]*sparam->out[1] + sparam->add[1]*255.0;
   acolor[2] = acolor[2]*sparam->out[2] + sparam->add[2]*255.0;

   post_calc_shade(sparam, i, k, acolor, rcolor);
}


/* *************************************************************
                // plain shading
************************************************************* */
void pc::calc_colorb(float *surface, float *normal, light *spot, eye *parm,
        texcolortype *texcolor, float *diffuse, float *rcolor, float *acolor) {

   light *ltr;

   copyarray3(rcolor, diffuse);
   copyarray3(acolor, texcolor->ambient);

   for (ltr=spot; ltr!=(light *)NULL; ltr=(light *)ltr->next)
      ltr->intensity(surface, normal, acolor, parm, &texcolor->color, this);
}


/* *************************************************************
        // 2d texture
************************************************************* */
void pc::calc_color2(texcolortype *texcolor, float *bcolor, float *tcolor,
        float *rcolor, float *acolor) {

   rcolor[0] = bcolor[0] * tcolor[0];
   acolor[0] = texcolor->ambient[0] * tcolor[0];
   rcolor[1] = bcolor[1] * tcolor[1];
   acolor[1] = texcolor->ambient[1] * tcolor[1];
   rcolor[2] = bcolor[2] * tcolor[2];
   acolor[2] = texcolor->ambient[2] * tcolor[2];
}

void pc::calc_color2b(float *surface, float *normal, light *spot, eye *parm,
        texcolortype *texcolor, float *bcolor, float *tcolor,
        float *rcolor, float *acolor) {

   light *ltr;

   rcolor[0] = bcolor[0] * tcolor[0];
   rcolor[1] = bcolor[1] * tcolor[1];
   rcolor[2] = bcolor[2] * tcolor[2];

   copyarray3(acolor, texcolor->ambient);

   for (ltr=spot; ltr!=(light *)NULL; ltr=(light *)ltr->next)
         ltr->intensity(surface, normal, acolor, parm, &texcolor->color, this);

   multarray3(acolor, tcolor);
}

/* *************************************************************
        3d texture
************************************************************* */
void pc::calc_color3(shaderparamtype *sparam, int i, float *surface, float *normal,
        texcolortype *texcolor, float *bcolor, float *rcolor, float *acolor) {

   vector4f s, n;
   int k;

   pre_calc_shade(sparam, i, k, surface, normal, s, n);

   rcolor[0] = bcolor[0]*sparam->out[0];
   rcolor[1] = bcolor[1]*sparam->out[1];
   rcolor[2] = bcolor[2]*sparam->out[2];

   acolor[0] = texcolor->ambient[0]*sparam->out[0] + sparam->add[0]*255.0;
   acolor[1] = texcolor->ambient[1]*sparam->out[1] + sparam->add[1]*255.0;
   acolor[2] = texcolor->ambient[2]*sparam->out[2] + sparam->add[2]*255.0;

   post_calc_shade(sparam, i, k, acolor, rcolor);
}


void pc::calc_color3b(shaderparamtype *sparam, int i, float *surface, float *normal,
        light *spot, eye *parm, texcolortype *texcolor, float *bcolor, float *rcolor,
        float *acolor) {

   light *ltr;
   vector4f s, n;
   int k;

   pre_calc_shade(sparam, i, k, surface, normal, s, n);

   copyarray3(acolor, texcolor->ambient);

   for (ltr=spot; ltr!=(light *)NULL; ltr=(light *)ltr->next)
      ltr->intensity(s, n, acolor, parm, &texcolor->color, this);

   rcolor[0] = bcolor[0]*sparam->out[0];
   rcolor[1] = bcolor[1]*sparam->out[1];
   rcolor[2] = bcolor[2]*sparam->out[2];
   acolor[0] = acolor[0]*sparam->out[0] + sparam->add[0]*255.0;
   acolor[1] = acolor[1]*sparam->out[1] + sparam->add[1]*255.0;
   acolor[2] = acolor[2]*sparam->out[2] + sparam->add[2]*255.0;

   post_calc_shade(sparam, i, k, acolor, rcolor);
}


/* *************************************************************
************************************************************* */
int pc::parse(FILE *infile, char *token) {

   vector4f tmx[4];

   switch (token[0]) {

      case '2':
         if (!strcmp(token, "2d_texture")) {
            get_token(infile, texname2);
            mcinfo |= CITEXTURE;
            return 1;
         }

         break;

      case '3':
         if (!strcmp(token, "3d_texture")) {
            get_token(infile, texname3);
            mcinfo |= CI3DTEX;

            return 1;
         }

         break;

      case 'b':
         if (!strcmp(token, "biplane")) {
            mcinfo |= CIBITMAP;
            return 1;
         }

         break;

      case 'l':
         if (!strcmp(token, "luminate")) {              // obsolete
            return 1;
         }

         break;

      case 'r':

         if (!strcmp(token, "raycast")) {
            mcinfo |= CIRAYCAST;
            return 1;
         }

         if (strlen(token) < 8)
            break;

         switch(token[7]) {

            case 'x':
               if (!strcmp(token, "rotate_x")) {
                  get_token(infile, token);
                  rotate_mx_x(rotate, deg2rad(atof(token)));
                  return 1;
               }

               break;

            case 'y':
               if (!strcmp(token, "rotate_y")) {
                  get_token(infile, token);
                  rotate_mx_y(rotate, deg2rad(atof(token)));
                  return 1;
               }

               break;

            case 'z':
               if (!strcmp(token, "rotate_z")) {
                  get_token(infile, token);
                  rotate_mx_z(rotate, deg2rad(atof(token)));
                  return 1;
               }

               break;

            default:
               break;
         }

         break;

      case 's':
         if (!strcmp(token, "scale")) {
            get_token(infile, token);
            size = atof(token);
            return 1;
         }

         if (!strcmp(token, "shadow")) {
            sflag = 1;
            get_token(infile, token);
            splane[0] = atof(token);
            get_token(infile, token);
            splane[1] = atof(token);
            get_token(infile, token);
            splane[2] = atof(token);
            get_token(infile, token);
            splane[3] = atof(token);

            mcinfo &= ~CITRANSPARENT;
            return 1;
         }

         if (!strcmp(token, "shade")) {
            get_token(infile, token);
            lower_case(token);

            switch (token[0]) {
               case 'a':
                  if (!strcmp(token, "alex")) {
                     get_token(infile, colorname);
                     mctype = ALEX;
                     return 1;
                  }

                  break;

               case 'b':
                  if (!strcmp(token, "bw")) {
                     mctype = BW;
                     return 1;
                  }

                  break;

               case 'c':
                  if (!strcmp(token, "constant")) {
                     get_token(infile, colorname);
                     mctype = CONSTANT;
                     return 1;
                  }

                  break;

               case 'd':
                  if (!strcmp(token, "dot")) {
                     mctype = DOT;
                     return 1;
                  }

                  break;

               case 'g':                        // line/particle called before this
                  if (!strcmp(token, "gouraud")) {
                     get_token(infile, colorname);
                     mctype = GOURAUD;
                     return 1;
                  }

                  break;

               case 'i':
                  if (!strcmp(token, "irradiant")) {
                     get_token(infile, colorname);
                     mctype = IRRADIANT;
                     return 1;
                  }

                  break;

               case 'p':
                  if (!strcmp(token, "phong")) {
                     get_token(infile, colorname);
                     mctype = PHONG;
                     return 1;
                  }

                  if (strlen(token) < 2)
                     break;

                  switch (token[1]) {

                     case 'a':
                        if (!strcmp(token, "palex")) {
                           get_token(infile, colorname);
                           mctype = PALEX;
                           return 1;
                        }

                        break;

                     case 'b':
                        if (!strcmp(token, "pbw")) {
                           mctype = PBW;
                           return 1;
                        }

                        break;

                     case 'c':
                        if (!strcmp(token, "pconstant")) {
                           get_token(infile, colorname);
                           mctype = PCONSTANT;
                           return 1;
                        }

                        break;

                     case 'd':
                        if (!strcmp(token, "pdot")) {
                           mctype = PDOT;
                           return 1;
                        }

                        break;

                     case 'g':
                        if (!strcmp(token, "pgouraud")) {
                           get_token(infile, colorname);
                           mctype = PGOURAUD;
                           return 1;
                        }

                        break;

                     case 'p':
                        if (!strcmp(token, "pphong")) {
                           get_token(infile, colorname);
                           mctype = PPHONG;
                           return 1;
                        }

                        break;

                     case 'w':
                        if (!strcmp(token, "pwfbw")) {
                           mctype = PWFBW;
                           return 1;
                        }

                        break;

                     default:
                        break;
                  }

                  break;

               case 'r':
                  if (!strcmp(token, "rsolid")) {
                     get_token(infile, colorname);
                     mctype = RSOLID;
                     return 1;
                  }

                  break;

               case 'w':
                  if (!strcmp(token, "wfbw")) {
                     mctype = WFBW;
                     return 1;
                  }

                  break;

               default:
                  break;
            }

            break;
         }

         break;

      case 't':
         if (strlen(token) < 6)
            break;

         switch (token[5]) {

            case 't':
               if (!strcmp(token, "tr_matrix")) {
                  get_token(infile, token);
                  tmx[0][0] = atof(token);
                  get_token(infile, token);
                  tmx[0][1] = atof(token);
                  get_token(infile, token);
                  tmx[0][2] = atof(token);
                  get_token(infile, token);
                  tmx[0][3] = atof(token);

                  get_token(infile, token);
                  tmx[1][0] = atof(token);
                  get_token(infile, token);
                  tmx[1][1] = atof(token);
                  get_token(infile, token);
                  tmx[1][2] = atof(token);
                  get_token(infile, token);
                  tmx[1][3] = atof(token);

                  get_token(infile, token);
                  tmx[2][0] = atof(token);
                  get_token(infile, token);
                  tmx[2][1] = atof(token);
                  get_token(infile, token);
                  tmx[2][2] = atof(token);
                  get_token(infile, token);
                  tmx[2][3] = atof(token);

                  get_token(infile, token);
                  tmx[3][0] = atof(token);
                  get_token(infile, token);
                  tmx[3][1] = atof(token);
                  get_token(infile, token);
                  tmx[3][2] = atof(token);
                  get_token(infile, token);
                  tmx[3][3] = atof(token);

                  matmatmulto(tmx, rotate);

                  return 1;
               }

               break;

            case 'l':
               if (!strcmp(token, "translate")) {
                  get_token(infile, token);
                  rotate[0][3] += atof(token);
                  get_token(infile, token);
                  rotate[1][3] += atof(token);
                  get_token(infile, token);
                  rotate[2][3] += atof(token);

                  return 1;
               }

               break;

            case 'p':
               if (!strcmp(token, "transparent")) {
                  mcinfo |= CITRANSPARENT;
                  sflag = 0;

                  return 1;
               }

               break;

            default:
               break;
         }

         break;

      default:
         break;
   }

   return superclass::parse(infile, token);
}


void pc::preprocess(void *data) {

   superclass::preprocess(data);

   if (PENCILTEST) {
      mctype = WFBW;
      mcinfo = CINULL;
      colorname[0] = 0;
      return;
   }

   if (colorname[0])
      read_ilum(colorname, &(((listtype *)data)->itr));
}


int  basenull::bound_box(eye *parm) { return 0; }
void basenull::transform(eye *parm) {}
int  basenull::clip(eye *parm, int maxx, int maxy) { return 0; }
int  basenull::beamscan(spotlight *spot) { return 0; }
int  basenull::scan(camera *cparm, light *lmain) { return 0; }
void basenull::beamrender(spotlight *spot) {}
void basenull::render(camera *cparm, light *lmain, light *spot, zbuffer *zbuff) {}
void basenull::prender() {}

