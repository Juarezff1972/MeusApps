#include <stdio.h>
#define _INCLUDE_XOPEN_SOURCE
#include <math.h>
#include "mymacros.h"
#include "vision.h"
#include "utilities.h"
#include "imageproc.h"

static struct {
   int drow, dcol;
} *compass, offsets[] = {
   {-1, -1},
   {-1, 0},
   {-1, 1},
   {0, 1},
   {1, 1},
   {1, 0},
   {1, -1},
   {0, -1},
   {-1, -1},
   {-1, 0}
};

/* Integer mask for specifying removability of a center pixel with
   neigborhoods designated by 8-bit patterns, with bits having the 
   significance shown below.

   The byte derived from the neighborhood indexes the array.
   A '1' in the array indicates that the center pixel may be removed
   with that neighborhood.

   The neighborhood: 7 6 5          The byte: 7 6 5 4 3 2 1 0
                     0   4
                     1 2 3
*/

char thin_mask[256] = {
0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 
0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 
0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 
1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 
0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 
1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 
1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0
};

image_type *thin_image(image_type *image, int solid)
{
   int new_bit_mask, old_bit_mask, row, col;
   image_type *t_image;
   
   t_image = copy_image(image);
   for (row = 0; row < t_image->height; row++)
      for (col = 0; col < t_image->width; col++) {
            if (t_image->image[row][col] != solid) 
               continue;
            new_bit_mask = old_bit_mask = 0;
            for (compass = offsets; compass < offsets + 8; compass++) {
               new_bit_mask <<= 1;
               old_bit_mask <<= 1;
               if (t_image->image[row+compass->drow][col+compass->dcol] 
                == solid)
                  new_bit_mask |= 01;
               if (image->image[row+compass->drow][col+compass->dcol] == solid)
                  old_bit_mask |= 01;
            }
            if (thin_mask[new_bit_mask] && thin_mask[old_bit_mask])
               t_image->image[row][col] = WHITE-solid;
         } 
   return(t_image);
}

image_type *convolve_image(image_type *image, matrix_type *matrix)
{
   int row, col, m_row, m_col, product; 
   image_type *c_image;
   int row_offset = matrix->rows % 2, col_offset = matrix->cols % 2;

   c_image = new_image(image->height, image->width);
   for (row = 0; row < image->height-matrix->rows+1; row++)
      for (col = 0; col < image->width-matrix->cols+1; col++) {
         product = 0;
         for (m_row = 0; m_row < matrix->rows; m_row++)
            for (m_col = 0; m_col < matrix->cols; m_col++)
               product += image->image[row+m_row][col+m_col] 
                          * matrix->matrix[m_row][m_col];
         product /= matrix->weight;
         c_image->image[row+row_offset][col+col_offset] = 
          make_between(product, BLACK, WHITE);
      }
   return(c_image);
}

image_type *binarize_image(image_type *image, int threshold)
{
   int row, col; 
   image_type *b_image;

   b_image = new_image(image->height, image->width);
   for (row = 0; row < image->height; row++)
      for (col = 0; col < image->width; col++)
         if (image->image[row][col] >= threshold) 
            b_image->image[row][col] = WHITE;
         else
            b_image->image[row][col] = BLACK;
   return(b_image);
}

image_type *scale_image(image_type *image, double factor)
{
   int row, col, new_val, new_row, new_col, lo_row, hi_row, lo_col, hi_col;
   float flo_row, flo_col, fhi_row, fhi_col, rfactor, wfactor;
   image_type *s_image;

   s_image = new_image((int)(image->height*factor+.9999), 
                       (int)(image->width*factor+.9999));
   rfactor = (float) s_image->height / image->height;
   wfactor = (float) s_image->width / image->width;
   for (row = 0; row < image->height; row++)
      for (col = 0; col < image->width; col++) {
         lo_row = flo_row = row * rfactor;
         hi_row = fhi_row = (row+1) * rfactor;
         lo_col = flo_col = col * wfactor;
         hi_col = fhi_col = (col+1) * wfactor;
         for (new_row = lo_row; new_row <= hi_row; new_row++)
            for (new_col = lo_col; new_col <= hi_col; new_col++) {

               new_val = s_image->image[new_row][new_col] + 
               image->image[row][col] *
               (min(new_row+1, fhi_row) - max(new_row, flo_row)) *
               (min(new_col+1, fhi_col) - max(new_col, flo_col)) + .5;

               s_image->image[new_row][new_col] =
                make_between(new_val, BLACK, WHITE);
            }
      }
   return(s_image);
}
   
#define MAXSOBEL 1141.0

image_type *edge_enhance(image_type *image)
{
   int row, col;
   image_type *e_image;

   e_image = new_image(image->height, image->width);
   for (row = 1; row < image->height-1; row++)
      for (col = 1; col < image->width-1; col++) {
         e_image->image[row][col] = 
          sqrt
          ((double) 
             sqr(image->image[row+1][col+1] - image->image[row-1][col+1] +
                 2*(image->image[row+1][col] - image->image[row-1][col]) +
                 image->image[row+1][col-1] - image->image[row-1][col-1])
             +
             sqr(image->image[row+1][col+1] - image->image[row+1][col-1] +
                 2*(image->image[row][col+1] - image->image[row][col-1]) +
                 image->image[row-1][col+1] - image->image[row-1][col-1])
          )/MAXSOBEL*(WHITE-BLACK) + BLACK;
      }
   return e_image;
}

#define MAXLAPLACE 5150

image_type *my_edge_enhance(image_type *image)
{
   int row, col;
   image_type *e_image, *l_image;

   l_image = new_image(image->height, image->width);
   for (row = 1; row < l_image->height-1; row++)
      for (col = 1; col < l_image->width-1; col++) 
         l_image->image[row][col] = (-20 * image->image[row][col] +
          image->image[row-1][col-1] + image->image[row+1][col-1] +
          image->image[row-1][col+1] + image->image[row+1][col+1] + 
          4*(
           image->image[row-1][col] + image->image[row+1][col] + 
           image->image[row][col-1] + image->image[row][col+1]
          )) / (MAXLAPLACE*2.0) * (WHITE-BLACK) + (WHITE-BLACK)/2;
/*
   e_image = edge_enhance(l_image);
   burn_image(l_image);
   return(e_image);
*/
   return(l_image);
}
   
int *get_histogram(image_type *image)
{
   int *histogram, row, col;

   if ((histogram = (int *) malloc(sizeof(int) * (WHITE - BLACK + 1))) == NULL){
      fprintf(stderr, "Out of space.\n");
      exit(1);
   }
   for (row = 0; row <= WHITE; row++)
      histogram[row] = 0;
   for (row = 0; row < image->height; row++)
      for (col = 0; col < image->width; col++) 
         histogram[image->image[row][col]]++;
   return histogram;
}

int split(int *histogram, double median)
   /* This one is yours to write */
{
}

image_type *hough_xform(image_type *source, int xdim, int ydim, int color)
{
   /* This one is yours to write */
}

image_type *crop_image(image_type *source, int lo_col, int hi_col,
							  int lo_row, int hi_row)
{
   int row, col;
   image_type *cropped;

	cropped = new_image(hi_row - lo_row + 1, hi_col - lo_col + 1);
	for (row = lo_row; row <= hi_row; row++)
		for (col = lo_col; col <= hi_col; col++)
			cropped->image[row-lo_row][col-lo_col] = source->image[row][col];
	return cropped;
}

image_type *autocrop_image(image_type *source, int background)
{
	int min_row, min_col, max_row, max_col, row, col;

   max_row = max_col = 0;
	min_row = source->height;
	min_col = source->width;

	for (row = 0; row < source->height; row++)
		for (col = 0; col < source->width; col++)
			if (source->image[row][col] != background) {
				min_row = min(min_row, row);
				min_col = min(min_col, col);
				max_row = max(max_row, row);
				max_col = max(max_col, col);
			}
   max_row = max(max_row, min_row);
   max_col = max(max_col, min_col);

   return crop_image(source, min_col, max_col, min_row, max_row);
}
