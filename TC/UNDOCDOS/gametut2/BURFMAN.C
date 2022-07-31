#include <bios.h>
#include <time.h>

#include "burfman.h"

/// You can change this to speed up or slow down the game
#define GAMESPEED 250L

struct {
		int curr_x;
		int curr_y;
} ghost[0x000F];

				 int  stall = 0;                // stall ghosts
				 int  g = 0;
				 int  max_g = 0;
				 int  curr_x, curr_y;           // 'burfmans' current location
				 int  dir_x = 0, dir_y = 0;     // 'burfmans' current direction
unsigned char scrn_grid[21][33];

void sleep(clock_t wait);

void main(void)
{
	unsigned  int curr_dat = 0; // The current dat file to use
	unsigned  int ch = 0;       // Key entered at keyboard (AX)

	if (scrninit()) {
		printf("\nThere was an error initializing the screen.");
		exit(-1);
	}

	if (scrneng(curr_dat)) {
		printf("\nThere was an error creating the screen.");
		scrnnrml();
		exit(-2);
  }

	// make sure there are not more than 15 ghosts
	max_g &= 0x000F;

	//this is the main loop
	do {

		dsplyscrn();  // display the screen
		sleep(GAMESPEED);

		if(_bios_keybrd(_KEYBRD_READY)) {
			ch = _bios_keybrd(_KEYBRD_READ);
			switch(ch) {
				case 0x011B:         // the esc key
					break;
				case 0x4D00:         // the right arrow key
				case 0x0736:         // numeral 6 (numlock off)
					dir_x = 1;
					dir_y = 0;
					break;
				case 0x4B00:         // the left arrow key
				case 0x0534:         // numeral 4 (numlock off)
					dir_x = -1;
					dir_y = 0;
					break;
				case 0x4800:         // the up arrow key
				case 0x0938:         // numeral 8 (numlock off)
					dir_y = -1;
					dir_x = 0;
					break;
				case 0x5000:         // the down arrow key
				case 0x0332:         // numeral 2 (numlock off)
					dir_y = 1;
					dir_x = 0;
					break;
			}
		}

		// if a direction has been chosen, then see if it is applicable
		//  and do accordingly.
		if ((scrn_grid[curr_y+dir_y][curr_x+dir_x] == 00) ||
				(scrn_grid[curr_y+dir_y][curr_x+dir_x] == 17)) {
			scrn_grid[curr_y][curr_x] = 00;
			curr_x += dir_x;
			curr_y += dir_y;
			scrn_grid[curr_y][curr_x] = 25;
		} else
			dir_x = dir_y = 0;


		// do ghost stuff
		if (stall++ == 2) {
			stall = 0;
			for (g=0; g<max_g+1; g++) {
				if (ghost[g].curr_x < curr_x) {
					if ((scrn_grid[ghost[g].curr_y][ghost[g].curr_x+1] == 00) ||
							(scrn_grid[ghost[g].curr_y][ghost[g].curr_x+1] == 17)) {
						scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 00;
						ghost[g].curr_x++;
						scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 24;
					}
				} else if (ghost[g].curr_x > curr_x) {
						if ((scrn_grid[ghost[g].curr_y][ghost[g].curr_x-1] == 00) ||
								(scrn_grid[ghost[g].curr_y][ghost[g].curr_x-1] == 17)) {
							scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 00;
							ghost[g].curr_x--;
							scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 24;
						}
				}
				if (ghost[g].curr_y < curr_y) {
					if ((scrn_grid[ghost[g].curr_y+1][ghost[g].curr_x] == 00) ||
							(scrn_grid[ghost[g].curr_y+1][ghost[g].curr_x] == 17)) {
						scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 00;
						ghost[g].curr_y++;
						scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 24;
					}
				} else if (ghost[g].curr_y > curr_y) {
						if ((scrn_grid[ghost[g].curr_y-1][ghost[g].curr_x] == 00) ||
								(scrn_grid[ghost[g].curr_y-1][ghost[g].curr_x] == 17)) {
							scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 00;
							ghost[g].curr_y--;
							scrn_grid[ghost[g].curr_y][ghost[g].curr_x] = 24;
						}
				}
			}
		}

	} while (ch!=0x011B);  // loop until the <esc> key was pressed

	scrnnrml();  // clear the screen and go to the text screen
}

/* Pauses for a specified number of microseconds. */
void sleep(clock_t wait)
{
		clock_t goal;

		goal = wait + clock();
		while(goal > clock());
}

