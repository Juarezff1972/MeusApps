#ifndef utilities_h
#define utilities_h

image_type *new_image(int, int);
image_type *copy_image(image_type *);
void burn_image(image_type *);

blob_type *new_blob(int , int);
void burn_blob(blob_type *);

matrix_type *read_matrix(char *);
matrix_type *new_matrix(int , int);

image_type *read_num_image(char *);
void print_num_image(image_type *, char *);

image_type *read_bin_image(char *);
void print_bin_image(image_type *, char *);

image_type *read_pgm_image(char *filename);
void print_pgm_image(image_type *psImage, char *szFilename);

#endif
