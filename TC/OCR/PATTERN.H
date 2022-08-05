
#ifndef PATTERN_H
#define PATTERN_H

typedef struct {
		   pattern_t *dapattern;
		   int letterval;
                 } myoutput;

pattern_t *make_pattern(blob_type *dablob,int x,int y);
void make_train_file(blob_type *bloblist);

#endif
