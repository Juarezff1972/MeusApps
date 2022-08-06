



/* **************************************************
   this prints 1 scan line for phong
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

   is = ptr->ds[0] - ptr->ds[1];
   ie = qtr->ds[0] - qtr->ds[1];

   s = SIN(ptr->ds[1]);
   e = SIN(qtr->ds[1]);

   is = SIN(is);
   ie = SIN(ie);

   snormal[0] = ptr->ds[2] * (is*ptr->start.shadedata[0] + s*ptr->end.shadedata[0]);
   snormal[1] = ptr->ds[2] * (is*ptr->start.shadedata[1] + s*ptr->end.shadedata[1]);
   snormal[2] = ptr->ds[2] * (is*ptr->start.shadedata[2] + s*ptr->end.shadedata[2]);

   enormal[0] = qtr->ds[2] * (ie*qtr->start.shadedata[0] + e*qtr->end.shadedata[0]);
   enormal[1] = qtr->ds[2] * (ie*qtr->start.shadedata[1] + e*qtr->end.shadedata[1]);
   enormal[2] = qtr->ds[2] * (ie*qtr->start.shadedata[2] + e*qtr->end.shadedata[2]);

   s = dotproduct3(snormal, enormal);

   if (s > 1-CORRECT) {
      temp[0] = HALFPI;        // theta
      temp[1] = 0.0;           // psi
      temp[2] = 1.0;           // 1.0/sin(theta);
      temp[3] = 0.0;           // dpsi
   }

   else {
      temp[0] = (s < CORRECT-1) ? 0.00447214315061 : ACOS(s);   // theta
      temp[1] = 0.0;                                            // psi
      temp[2] = 1.0/SIN(temp[0])                ;               // 1.0/sin(theta);
      temp[3] = mtemp*temp[0];                                  // dpsi
   }

   for (index = scanxy + start; start<end; start++, index++) {

#ifndef INTZ
         engine_dx(ob->normal[i], prenum, prediv, mtempz);
#endif

      if (mtempz>zbuff->zbuff[index].zdata) {

         is = temp[0] - temp[1];
         s = SIN(temp[1]);
         is = SIN(is);

         normal[0] = temp[2] * (is*snormal[0] + s*enormal[0]);
         normal[1] = temp[2] * (is*snormal[1] + s*enormal[1]);
         normal[2] = temp[2] * (is*snormal[2] + s*enormal[2]);

         surface[0] = -(start*parm->imscale + parm->vrc[0])*mtempz;
         surface[1] = -(scanfy*parm->imscale + parm->vrc[2])*mtempz;
         surface[2] = mtempz;

#ifdef POLYNTRANSBEAM

#ifdef POLY2DTEXTURE
         if (flagt) {
            tempcolor = tob->query_texel(i, mtempt[0], mtempt[1], tcolor, frame, -mtempz*facedata[i].area);
            if (flagb && (tempcolor & 0xffffff00) == inviso)
               goto polypg_inviso;
         }

         calc_color2b(surface, normal, lmain, spot, parm, &texcolor, tcolor, rcolor, acolor);
#endif

#ifdef POLY3DTEXTURE
         calc_color3b(&sparam, i, surface, normal, lmain, spot, parm, &texcolor, rcolor, acolor);
#endif

#ifdef POLYNORMAL
         calc_colorb(surface, normal, lmain,  spot, parm, &texcolor, rcolor, acolor);
#endif

#else

#ifdef POLY2DTEXTURE
         if (flagt) {
            tempcolor = tob->query_texel(i, mtempt[0], mtempt[1], tcolor, frame, -mtempz*facedata[i].area);
            if (flagb && (tempcolor & 0xffffff00) == inviso)
               goto polypg_inviso;
         }

         calc_color2(surface, normal, lmain, parm, &texcolor, tcolor, rcolor, acolor);
#endif

#ifdef POLY3DTEXTURE
         calc_color3(&sparam, i, surface, normal, lmain, parm, &texcolor, rcolor, acolor);
#endif

#ifdef POLYNORMAL
         calc_color(surface, normal, lmain, parm, &texcolor, rcolor, acolor);
#endif

#endif

#ifdef POLYTRANSPARENT
         set_datatr(zbuff->zbuff, index, mtempz, acolor, rcolor, a0, a1, a2, d0, d1, d2);
#else
         set_data(zbuff->zbuff, index, mtempz, acolor, rcolor, a0, a1, a2, d0, d1, d2);
#endif

      }

#ifdef POLY2DTEXTURE
      polypg_inviso:

      addarray2(mtempt, mdeltat);
#endif

#ifdef INTZ
      mtempz += mdeltaz;            // increment variables
#endif

      temp[1] += temp[3];
   }

}