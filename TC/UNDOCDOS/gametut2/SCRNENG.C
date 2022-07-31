#include <stdio.h>
#include <stdlib.h>

#include "scrneng.h"

extern int  curr_x;
extern int  curr_y;
extern int  max_g;

struct {
		int curr_x;
		int curr_y;
} ghost[4];


extern unsigned char scrn_grid[21][33];

  // the screen display engine routine
int scrneng(unsigned curr_dat) {
	FILE *fp;
					 int  i,j;
	unsigned char dat_file[] = "SCRN00.DAT";

	// open, read in the grid, and close the file
	if((fp = fopen(dat_file,"rb"))==NULL) {
		printf("\nError opening file");
		return(1);
	}
	for (i=1;i<=20;i++) {
		for (j=1;j<=32;j++) {
			scrn_grid[i][j] = fgetc(fp) - 97;
			if (scrn_grid[i][j] == 25) {
				curr_y = i;
				curr_x = j;
			}
			if (scrn_grid[i][j] == 24) {
				ghost[max_g].curr_y = i;
				ghost[max_g].curr_x = j;
				max_g++;
			}
		}
		fgetc(fp);  // skip CR LF
		fgetc(fp);  //
	}
	fclose(fp);

	return(0); // no error
}

void dsplyscrn(void) {
	int i, j;

	for(i=1;i<=20;i++)
		for(j=1;j<=32;j++)
			display_item(scrn_grid[i][j],j-1,i-1);
}
