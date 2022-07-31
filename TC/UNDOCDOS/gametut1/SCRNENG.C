#include <stdio.h>
#include <stdlib.h>

#include "scrneng.h"

  // the screen display engine routine
int scrneng(unsigned curr_dat)
{
	FILE *fp;
					 int  i,j;
	unsigned char dat_file[] = "SCRN00.DAT";
	unsigned char scrn_grid[21][33];

	// open, read in the grid, and close the file
	if((fp = fopen(dat_file,"rb"))==NULL) {
		printf("\nError opening file");
		return(1);
	}
	for (i=1;i<=20;i++) {
		for (j=1;j<=32;j++)
			scrn_grid[i][j] = fgetc(fp) - 97;
		fgetc(fp);  // skip CR LF
		fgetc(fp);  //
	}
	fclose(fp);

	for(i=1;i<=20;i++)
		for(j=1;j<=32;j++)
			display_item(scrn_grid[i][j],j-1,i-1);

	return(0); // no error
}

