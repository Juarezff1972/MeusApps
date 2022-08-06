#include <stdio.h>


void main() {

   FILE *o;

   o = fopen("x.lst", "w");

   int i;

   fprintf(o, "list\nframe_count 60\nobject_count 240\n\n");

   for (i=1; i<61; i++) {

   fprintf(o, "camera {\n");
   fprintf (o, "\tlocation %f %f %f\n", (0.415*i)/60.0, 
//					(1.04*i)/60.0,
					(1.04*i)/60.1,
					5 - (3.03*i)/60.0);
   fprintf (o, "\tcoa %f %f 0\n", (0.415*i)/60.0, (1.04*i)/60.0);
   fprintf(o, "\tframe %d\n}\n\n", i);

   fprintf(o, "spatch {\n\t2d_texture us.tes\n\tshade pphong us.ilm\n\tfile x.%05d.sp\n", i-1);
   fprintf(o, "\traycast\n\ttr_matrix %f 0 0 0\n\t          0 %f 0 0\n\t          0 0 1 0\n\t          0 0 0 1\n", 
					1 + (1.7*i)/60.0, 1 + (1.7*i)/60.0);
   fprintf(o, "\tscale %f\n", 1.01 - i/6000.0);
// 0.7 + i*0.005);
   fprintf(o, "\tframe %d\n}\n\n", i);

   fprintf(o, "polygon {\n\tframe %d\n\tshade constant stars.ilm\n\tluminate\n\ttransparent\n\traycast\n\t2d_texture stars.tex\n\ttranslate 0 0 -400\n\tfile backgrnd.spg\n}\n\n", i);

   fprintf(o, "polygon {\n\tframe %d\n\tfile globe.spg\n\tshade phong globe.ilm\n\tscale 0.1\n\trotate_y 102\n\trotate_x  25\n", i);
   fprintf(o, "\traycast\n\ttr_matrix %f 0 0 0\n\t          0 %f 0 0\n\t          0 0 1 %f\n\t          0 0 0 1\n", 
//					1 + (1.7*i)/60.0, 1 + (1.7*i)/60.0, -i*0.02);
					1 + (4*i)/60.0, 1 + (4*i)/60.0, -i*0.02);
   fprintf(o, "}\n\n", i);


//   fprintf(o, "spatch {\n\tframe %d\n\tfile shell2.sp\n\tshade pphong us.ilm\n\traycast\n\t2d_texture us.tes\n\tscale 1.01\n}\n\n", i);

   }

   fclose (o);
}
