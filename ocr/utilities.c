#include "vision.h"
#include "mymacros.h"
#include "utilities.h"
#include <sys/file.h>
#include "fcntl.h"
#include <stdio.h>
#include <string.h>

#define COLS 16

image_type *new_image(int height, int width)
{
   int row;
   image_type *image;

   image = get_new(image_type);
   image->height = height;
   image->width = width;
   height += 2;
   width += 2;
   if ((image->data = (pix_type *) malloc(height*width*sizeof(char))) == NULL
    || (image->image = (pix_type **) malloc(height*sizeof(char *))) == NULL) {
      fprintf(stderr, "Choke! Malloc has insufficient space.\n");
      exit(1);
   }
   bzero(image->data, height*width*sizeof(char));
   for (row = 0; row < height; row++) 
      image->image[row] = image->data + row*width + 1;
   image->image++;
   return image;
}

blob_type *new_blob(int height, int width)
{
   blob_type *blob;

   blob = get_new(blob_type);
   blob->x_loc = blob->y_loc = 0;
   blob->color = BLACK;
   blob->next = NULL;
   blob->image = new_image(height, width);
   return blob;
}

void burn_blob(blob_type *blob)
{
   burn_image(blob->image);
   free(blob);
}

void burn_image(image_type *image)
{
   free(image->data);
   free(image->image-1);
}

image_type *read_bin_image(char *file_name)
{
   int file, height, width;
   image_type *image;

   if ((file = open(file_name, O_RDONLY, 0600)) == -1) {
      fprintf(stderr, "Error: cannot open image file %s\n", file_name);    
      exit(1);
   }
   read(file, &height, sizeof(int));
   read(file, &width, sizeof(int));
   image = new_image(height, width);
   read(file, image->data, (image->height+2)*(image->width+2)*sizeof(char));
   close(file);
   return(image);
}

void print_bin_image(image_type *image, char *file_name)
{
   int file;

   if ((file = open(file_name, O_CREAT|O_RDWR|O_TRUNC, 0600)) == -1) {
      fprintf(stderr, "Error: cannot open image file %s\n", file_name);    
      exit(1);
   }
   write(file, &image->height, sizeof(int));
   write(file, &image->width, sizeof(int));
   write(file, image->data, (image->height+2)*(image->width+2)*sizeof(char));
   close(file);
}

image_type *read_num_image(char *file_name)
{
   FILE *file;
   int row, col, height, width, pixel;
   image_type *image;

   if ((file = fopen(file_name, "r")) == NULL) {
      fprintf(stderr, "Error: cannot open image file %s\n", file_name);    
      exit(1);
   }
   fscanf(file, "%d %d", &height, &width);
   image = new_image(height, width);
   for (row = 0; row < image->height; row++) 
      for (col = 0; col < image->width; col++) {
         fscanf(file, "%d", &pixel);
         image->image[row][col] = pixel;
      }
   fclose(file);
   return image;
}

void print_num_image(image_type *image, char *file_name)
{
   FILE *file;
   int row, col;

   if ((file = fopen(file_name, "w")) == NULL) {
      fprintf(stderr, "Error: cannot open output file %s\n", file_name);    
      exit(1);
   }
   fprintf(file, "%d %d\n", image->height, image->width);
   for (row = 0; row < image->height; row++) {
      for (col = 0; col < image->width; col++) {
         fprintf(file, "%3d ", image->image[row][col]);
         if (col % COLS == COLS-1)
            fprintf(file, "\n");
      }
      fprintf(file, "\n");
   }
   fclose(file);
}

image_type *copy_image(image_type *source)
{
   image_type *image;

   image = new_image(source->height, source->width);
   memcpy(image->data, source->data, 
    (image->height+2)*(image->width+2)*sizeof(char));
   return image;
}

matrix_type *new_matrix(int rows, int cols)
{
   int row;
   matrix_type *matrix;

   matrix = get_new(matrix_type);
   matrix->rows = rows;
   matrix->cols = cols;
   if ((matrix->data = (int *) malloc(rows*cols*sizeof(int))) == NULL
    || (matrix->matrix = (int **) malloc(rows*sizeof(int *))) == NULL) {
      fprintf(stderr, "Malloc bombed on matrix allocation.\n");
      exit(1);
   }
   for (row = 0; row < rows; row++)
      matrix->matrix[row] = matrix->data + row * matrix->cols;
   return matrix;
}

matrix_type *read_matrix(char *filename)
{
   FILE *file;
   int rows, cols, weight, row, col;
   matrix_type *matrix;

   if ((file = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Error: cannot open image file %s\n", filename);    
      exit(1);
   }
   fscanf(file, "%d %d", &rows, &cols);
   matrix = new_matrix(rows, cols);
   fscanf(file, "%d", &matrix->weight);
   for (row = 0; row < rows; row++)
      for (col = 0; col < cols; col++)
         fscanf(file, "%d", &matrix->matrix[row][col]);

   return matrix;
}


#define LINE_LEN 100
 
image_type *read_pgm_image(char *filename)
{
   FILE *fpFile;
   int iWidth, iHeight, row;
   char szTemp[LINE_LEN];
   image_type *psImage;
 
   if ((fpFile = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Error: cannot open image file %s\n", filename);    
      exit(1);
   }
 
   fgets(szTemp, LINE_LEN-1, fpFile);
   if (strncmp(szTemp, "P5", 2)) {
      fprintf(stderr, "Error: image file is in wrong format.\n");    
      exit(1);
   }
 
   while (fgets(szTemp, LINE_LEN-1, fpFile) != NULL && *szTemp == '#')
		;

   sscanf(szTemp, "%d %d", &iWidth, &iHeight);
   fgets(szTemp, LINE_LEN-1, fpFile); 

   psImage = new_image(iHeight, iWidth);
	for (row = 0; row < iHeight; row++)
      fread(psImage->image[row], sizeof(char), iWidth, fpFile);

   fclose(fpFile);
   return psImage;
}
 
void print_pgm_image(image_type *psImage, char *szFilename)
{
   FILE *fpFile;
   int   row;
 
   if ((fpFile = fopen(szFilename, "w")) == NULL) {
      fprintf(stderr, "Error: cannot open image file %s\n", szFilename);    
      exit(1);
   }
   fprintf(fpFile, "P5\n%d %d\n255\n", psImage->width, psImage->height);
	for (row = 0; row < psImage->height; row++)
      fwrite(psImage->image[row], sizeof(char), psImage->width, fpFile);

   fclose(fpFile);
}
