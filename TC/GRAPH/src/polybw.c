/* **************************************************
   this prints 1 scan line for black&white
************************************************** */


{

#ifdef INTZ
      mtempz[0] = ptr->start.point[2]+CORRECT;
      mdeltaz = (qtr->start.point[2] - ptr->start.point[2]) / (qtr->start.point[0] - ptr->start.point[0]);
#endif

   if (ptr->dx > 0)
      mtemp[0] = (round(ptr->end.point[1]) <= scany+1) ? round(ptr->end.point[0]) : round(ptr->start.point[0] + ptr->dx);
   else
      mtemp[0] = (round(ptr->start.point[1]) == scany) ? start : round(ptr->start.point[0] - ptr->dx);

   mtemp[1] = end;

   if (qtr->dx < 0)
      end = (round(qtr->end.point[1]) <= scany+1) ? round(qtr->end.point[0]) : round(qtr->start.point[0] + qtr->dx);
   else
      end = (round(qtr->start.point[1]) == scany) ? mtemp[1] : round(qtr->start.point[0] - qtr->dx);

#ifdef INTZ
      mtempz[1] = qtr->start.point[2] + (end-mtemp[1])*mdeltaz + CORRECT;
#endif

   mtemp[0] += (start == mtemp[0]);
   end -= (end == mtemp[1]);

   if (end < mtemp[0])
      end = mtemp[0] = mtemp[1];

   for (index = scanxy + start; start<mtemp[0]; start++, index++) {
#ifndef INTZ
      engine_dx(ob->normal[i], prenum, prediv, sz);
      sz += CORRECT;
#else
      sz = mtempz[0];
      mtempz[0] += mdeltaz;
#endif

      if (sz>zbuff->zbuff[index].zdata) {
         zbuff->zbuff[index].zdata = sz;
         *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
         *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
      }

   }

#ifndef INTZ
   surface[0] = end-1.0;
   engine(ob->normal[i], surface, parm, &prenum, &prediv);
#endif

   for (index = scanxy + end; end<mtemp[1]; end++, index++) {

#ifndef INTZ
      engine_dx(ob->normal[i], prenum, prediv, sz);
#else
      sz = mtempz[1];
      mtempz[1] += mdeltaz;
#endif

      if (sz>zbuff->zbuff[index].zdata) {
         zbuff->zbuff[index].zdata = sz;
         *(unsigned int *)zbuff->zbuff[index].pixel = 0xffffffff;
         *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
      }

   }

#ifndef INTZ
   surface[0] = mtemp[0]-1.0;
   engine(ob->normal[i], surface, parm, &prenum, &prediv);
#endif

   for (j=mtemp[0], index = scanxy + j; j<end; j++, index++) {
#ifndef INTZ
      engine_dx(ob->normal[i], prenum, prediv, sz);
      sz+= CORRECT;
#else
      sz = mtempz[0];
      mtempz[0] += mdeltaz;
#endif

      if (sz>zbuff->zbuff[index].zdata) {
         zbuff->zbuff[index].zdata = sz;
         *(unsigned int *)zbuff->zbuff[index].pixel = 0;
         *(unsigned int *)zbuff->zbuff[index].ambient = 0;
      }

   }

}
