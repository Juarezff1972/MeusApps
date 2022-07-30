#ifndef vision_h
#define vision_h

#define WHITE 255
#define BLACK 0

typedef unsigned char pix_type;

typedef struct {
   int  width;   /* Horizontal width of image in pixels */
   int  height;  /* Vertical height of image in pixels (also number of rows) */
   pix_type	 *data;/* Image data, 1 byte per pixel, in large row-major block */
   pix_type	 **image; /* Array of pointers to beginning of each data row */
} image_type;

typedef struct blob_type {
   int x_loc, y_loc;     /* X and y location of the center of the blob image */
   int id;               /* ID of this blob, as determined by recognition */
   pix_type color;       /* Foreground color of blob */
   image_type *image;    /* The blob image */
   struct blob_type *next;  /*next in a list of blobs */
} blob_type;

typedef struct edge_type {
   int direction;
   struct edge_type *next;
} edge_type;

typedef struct {
   int  rows;    /* Number of rows in matrix. */
   int  cols;    /* Number of columns in matrix. */
   int  weight;  /* Weight factor by which each element should be divided*/
   int *data;    /* Matrix elements, stored in row-major order */
   int **matrix; /* Array of pointers to beginning of each matrix row */
} matrix_type;
#endif
