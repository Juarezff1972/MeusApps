#include "burfman.h"

void main(void)
{
  unsigned int  curr_dat = 0; // The current dat file to use
	unsigned char ch = 0;       // Key entered at keyboard
	unsigned char lastkey = 0;  // Last key entered (movement command)

	if (scrninit()) {
		printf("\nThere was an error initializing the screen.");
		exit(-1);
	}

	if (scrneng(curr_dat)) {
		printf("\nThere was an error creating the screen.");
		scrnnrml();
		exit(-2);
  }

	//to leave it on there for us to see
	getch();
	scrnnrml();
}
