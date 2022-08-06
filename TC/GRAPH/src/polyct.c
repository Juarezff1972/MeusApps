



/* **************************************************
   this prints 1 scan line for constant
************************************************** */


{

   mtemp = 1.0/(qtr->start.point[0] - ptr->start.point[0]);

#ifdef INTZ
   mtempz  = ptr->start.point[2];
   mdeltaz = (qtr->start.point[2] - mtempz) * mtemp;
#endif

#ifdef POLY2DTEXTURE
   copyarray2(mtempt, ptr->start.uv);
   mdeltat[0] = (qtr->start.uv[0] - mtempt[0]) * mtemp;
   mdeltat[1] = (qtr->start.uv[1] - mtempt[1]) * mtemp;
#endif

   for (index = scanxy + start; start<end; start++, index++) {

#ifndef INTZ
         engine_dx(ob->normal[i], prenum, prediv, mtempz);
#endif

      if (mtempz>zbuff->zbuff[index].zdata) {

#ifdef POLYNTRANSBEAM
         surface[0] = -(start*parm->imscale + parm->vrc[0])*mtempz;
         surface[1] = -(scanfy*parm->imscale + parm->vrc[2])*mtempz;
         surface[2] = mtempz;

#ifdef POLY2DTEXTURE
         if (flagt) {
            tempcolor = tob->query_texel(i, mtempt[0], mtempt[1], tcolor, frame, -mtempz*facedata[i].area);
            if (flagb && (tempcolor & 0xffffff00) == inviso)
               goto polyct_inviso;
         }

         calc_color2b(surface, ob->normal[i], spot, parm, &texcolor, facedata[i].constcolor, tcolor, rcolor, acolor);
#endif

#ifdef POLY3DTEXTURE
         calc_color3b(&sparam, i, surface, ob->normal[i], spot, parm, &texcolor, facedata[i].constcolor, rcolor, acolor);
#endif

#ifdef POLYNORMAL
         calc_colorb(surface, ob->normal[i], spot, parm, &texcolor, facedata[i].constcolor, rcolor, acolor);
#endif


#else


#ifdef POLY2DTEXTURE
         if (flagt) {
            tempcolor = tob->query_texel(i, mtempt[0], mtempt[1], tcolor, frame, -mtempz*facedata[i].area);
            if (flagb && (tempcolor & 0xffffff00) == inviso)
               goto polyct_inviso;
         }

         calc_color2(&texcolor, facedata[i].constcolor, tcolor, rcolor, acolor);
#endif

#ifdef POLY3DTEXTURE
         surface[0] = -(start*parm->imscale + parm->vrc[0])*mtempz;
         surface[1] = -(scanfy*parm->imscale + parm->vrc[2])*mtempz;
         surface[2] = mtempz;
         calc_color3(&sparam, i, surface, ob->normal[i], &texcolor, facedata[i].constcolor, rcolor, acolor);
#endif

#ifdef POLYNORMAL

#ifdef POLYTRANSPARENT
         calc_color(texcolor.ambient, facedata[i].constcolor, rcolor, acolor);
#else
         zbuff->zbuff[index].zdata = mtempz;
         *(unsigned int *)zbuff->zbuff[index].ambient = *(unsigned int *)flatacolor;
         *(unsigned int *)zbuff->zbuff[index].pixel   = *(unsigned int *)flatdcolor;
#endif

#endif

#endif

#ifdef POLYTRANSPARENT
         set_datatr(zbuff->zbuff, index, mtempz, acolor, rcolor, a0, a1, a2, d0, d1, d2);
#else

#ifdef POLYNORMAL
#ifdef POLYBEAM
         set_data(zbuff->zbuff, index, mtempz, acolor, rcolor, a0, a1, a2, d0, d1, d2);
#else
        // taken care of above
#endif

#else
         set_data(zbuff->zbuff, index, mtempz, acolor, rcolor, a0, a1, a2, d0, d1, d2);
#endif

#endif

      }

#ifdef POLY2DTEXTURE
      polyct_inviso:

      addarray2(mtempt, mdeltat);
#endif

#ifdef INTZ
      mtempz += mdeltaz;
#endif

   }

}
