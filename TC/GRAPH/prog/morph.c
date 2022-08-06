

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"


#define MAXSTRLEN 256
#define interpolate(start, finish, delta) ((start) + ((finish)-(start))*(delta))


class grid {

   public:
	int x, y;
	vector3f *data;

	grid() {
	   data = (vector3f *)NULL;
	}

	~grid() {
	   if (data != (vector3f *)NULL)
	      delete [] data;
	}

        void read_grid(char *filename) {

           FILE *infile;
           int i;

           if (!(infile = fopen(filename, "r"))) {
              printf("Error: Could not access %s... Aborting...\n", filename);
              exit(1);
           }

           fscanf(infile, "%d %d", &y, &x);
           if (data != (vector3f *)NULL)
              delete [] data;

           data = new vector3f[x*y];

           for (i=0; i<x*y; i++)
              fscanf(infile, "%f %f %f", &data[i][0], &data[i][1], &data[i][2]);

           fclose(infile);
	}

        void write_grid(char *filename) {

           FILE *outfile;
           int i, j, k;

           if (!x || !y || data == (vector3f *)NULL) {
              printf("Error: No data to write... Aborting...\n");
              exit(1);
           }

           if (!(outfile = fopen(filename, "w"))) {
              printf("Error: Could not access %s... Aborting...\n", filename);
              exit(1);
           }

           fprintf(outfile, "%d %d\n", y, x);

           for (k=i=0; i<y; i++) {
              for (j=0; j<x; j++, k++)
                 fprintf(outfile, "%f %f %f ", data[k][0], data[k][1], data[k][2]);
              fprintf(outfile, "\n");
           }

           fclose(outfile);
	}

};


void generate(grid *start, grid *end, int count, char *filename) {

   char buffer[MAXSTRLEN];
   float delta = 1.0/(count-1);
   grid tween;
   int i, j;
   float t;

   tween.x = start->x;
   tween.y = start->y;
   tween.data = new vector3f[tween.x*tween.y];

   sprintf(buffer, "%s.%05d.sp", filename, 0);
   start->write_grid(buffer);

   for (i=1, t = delta; i<count-1; i++, t += delta) {
      for (j=0; j<tween.x*tween.y; j++) {
         tween.data[j][0] = interpolate(start->data[j][0], end->data[j][0], t);
         tween.data[j][1] = interpolate(start->data[j][1], end->data[j][1], t);
         tween.data[j][2] = interpolate(start->data[j][2], end->data[j][2], t);
      }

      sprintf(buffer, "%s.%05d.sp", filename, i);
      tween.write_grid(buffer);
   }

   sprintf(buffer, "%s.%05d.sp", filename, count-1);
   end->write_grid(buffer);
}


void main(int argc, char **argv) {

   grid start, end;
   int count;

   if (argc < 5) {
      printf("usage: morph <start .sp> <end .sp> <count> <outname>\n");
      exit(1);
   }

   start.read_grid(argv[1]);
   end.read_grid(argv[2]);

   if (start.x != end.x || start.y != end.y)
      printf("ERROR: %s dimensions are not the same as %s... Aborting...\n", argv[1], argv[2]);

   count = atoi(argv[3]);

   if (count < 2) {
      printf("Warning: morphing needs to create at least 2 frames...\n");
      count = 2;
   }

   generate(&start, &end, count, argv[4]);
}