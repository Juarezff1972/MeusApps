/* **************************************************
   this prints 1 scan line for black&white
************************************************** */


{
   mtempz = ptr->end.point[0]-ptr->start.point[0];

   if (abs(mtempz) > CORRECT)
      mdeltaz = (ptr->end.point[2] - ptr->start.point[2]) / mtempz;

   mtempz  = ptr->start.point[2];

   if (round(ptr->end.point[1]) > scany+1)
      end = round(ptr->start.point[0] + ptr->dx);
   else
      end = round(ptr->end.point[0]);

   if (start == end) {
      index = scanxy + start;
      if (mtempz > zbuff->zbuff[index].zdata) {
         zbuff->zbuff[index].zdata = mtempz;
         *(unsigned int *)zbuff->zbuff[index].pixel = 0xffffffff;
         *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
      }

   }

   else {
      if (end < start) {
         j = start;
         start = end;
         end = j;
         mtempz += mdeltaz*(start-end);
         mdeltaz = -mdeltaz;
      }

      index = scanxy + start;
      for (; start<end; start++, index++) {
         if (mtempz > zbuff->zbuff[index].zdata) {
            zbuff->zbuff[index].zdata = mtempz;
            *(unsigned int *)zbuff->zbuff[index].pixel   = 0xffffffff;
            *(unsigned int *)zbuff->zbuff[index].ambient = 0xffffffff;
         }

         mtempz += mdeltaz;
      }

   }

}
