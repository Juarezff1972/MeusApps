/* **************************************************
   this prints 1 scan line for constant
************************************************** */


{
#ifdef INTZ
      mdeltaz = (qtr->start.point[2] - (mtempz=ptr->start.point[2])) / (qtr->start.point[0] - ptr->start.point[0]);
#endif

   for (index = scanxy + start; start<end; start++, index++) {

#ifndef INTZ
         if (parm->query_whatami() == FAR) {
            fengine_dx(prenum, prediv, mtempz);
         }

         else {
            engine_dx(ob->normal[i], prenum, prediv, mtempz);
         }
#endif

      if (mtempz>parm->lbuff->lbuff.data[index].zdata) {
         parm->lbuff->lbuff.data[index].zdata = mtempz;
         parm->lbuff->lbuff.data[index].idbuff = id;
      }

#ifdef INTZ
      mtempz+=mdeltaz;
#endif
   }

}
