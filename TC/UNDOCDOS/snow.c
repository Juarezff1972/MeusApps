#include <stdlib.h>
#include <conio.h>
#include <graph.h>
#include <time.h>

// Macro to get a random integer within a specified range
#define getrandom(min, max) ((rand() % (int)(((max)+1) - (min))) + (min))
#define numflakes 300

void main(void)
{
  unsigned int r;
	unsigned int snow [numflakes+1] [2];

  if (_setvideomode(_VRES16COLOR) == 0) {
	  printf("\nCan't set video mode");
		exit(-1);
	}

	_settextposition(12,10);
	_settextcolor(5);
	_outtext("This is a demo for a snow-fall effect in QC25");

	_clearscreen;
	_setcolor(5);
	_moveto(10,300);
	_lineto(30,330);
	_moveto(10,301);
	_lineto(30,331);
	_moveto(35,340);
	_lineto(15,370);
	_moveto(35,341);
	_lineto(15,371);

  _setcolor(1);
  _ellipse(_GBORDER, 175, 301, 225, 327);
  _ellipse(_GBORDER, 175, 302, 225, 328);

	_setcolor(12);
  _rectangle(_GBORDER, 350, 250, 240, 300);
  _rectangle(_GBORDER, 351, 251, 241, 301);

	_setcolor(13);
	_moveto(450,310);
	_lineto(480,350);
	_lineto(420,350);
  _lineto(450,310);
  _moveto(419,350);
  _lineto(449,310);

  /* Seed the random number generator with current time. */
  srand((unsigned)time(NULL));
  for (r=1; r<numflakes; r++) {
    snow[r] [1] = getrandom(1, 639); // newone x
    snow[r] [2] = getrandom(1, 479); // newone y
  }

	do {
    for (r=1; r<numflakes; r++) {
			(snow [r] [2])++;
			if (_getpixel(snow [r] [1], snow [r] [2]) != 0) {
        if ((_getpixel(snow [r] [1] + 1, snow [r] [2]) == 0) && (snow [r] [1] < 640)) {
          (snow [r] [1])++;
          _setcolor(0);
          _setpixel(snow [r] [1] - 1, snow [r] [2] - 1);
          _setcolor(15);
          _setpixel(snow [r] [1], snow [r] [2]);
					goto nextloop;
				} else
        if ((_getpixel(snow [r] [1] - 1, snow [r] [2]) == 0) && (snow [r] [1] > 0)) {
          (snow [r] [1])--;
          _setcolor(0);
          _setpixel(snow [r] [1] + 1, snow [r] [2] - 1);
          _setcolor(15);
          _setpixel(snow [r] [1], snow [r] [2]);
          goto nextloop;
        }
        snow [r] [2] = getrandom(1, 10);
        snow [r] [1] = getrandom(1, 639); // newone x
        goto nextloop;
      }
      if (snow [r] [2] > 479) {
        snow [r] [2] = getrandom(1, 10);
        snow [r] [1] = getrandom(1, 639); // newone x
			} else {
			  _setcolor(0);
        _setpixel(snow [r] [1], snow [r] [2] - 1);
				_setcolor(15);
        _setpixel(snow [r] [1], snow [r] [2]);
      }
nextloop:;
		}
  } while (!(kbhit()));

  _setvideomode(_TEXTC80);
	exit(0);
}
