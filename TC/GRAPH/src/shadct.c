/* **************************************************
   this prints 1 scan line for constant
************************************************** */


{
#ifdef INTZ
   mtempz  = ptr->start.point[2];
   mdeltaz = (qtr->start.point[2] - mtempz) / (qtr->start.point[0] - ptr->start.point[0]);
#endif

   for (index = scanxy + start, end += scanxy; index<end; index++) {

#ifndef INTZ
      engine_dx(ob->normal[i], prenum, prediv, mtempz);
#endif

      if (mtempz>zbuff->zbuff[index].zdata) {
         zbuff->zbuff[index].zdata = mtempz;
         *(unsigned int*)zbuff->zbuff[index].pixel = *(unsigned int*)zbuff->zbuff[index].ambient;
      }

#ifdef INTZ
         mtempz += mdeltaz;
#endif
   }

}
