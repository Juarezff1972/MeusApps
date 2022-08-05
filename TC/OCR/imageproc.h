#ifndef imageproc_h
#define imageproc_h

int split(int *, double);
int *get_histogram(image_type *);
image_type *crop_image(image_type *,int,int,int,int);
image_type *my_edge_enhance(image_type *);
image_type *edge_enhance(image_type *);
image_type *scale_image(image_type *, double);
image_type *binarize_image(image_type *, int);
image_type *convolve_image(image_type *, matrix_type *);
image_type *thin_image(image_type *, int);
image_type *hough_xform(image_type *source, int xdim,int ydim,int color);

#endif
