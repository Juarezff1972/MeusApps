#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define MASTER_VOL  0x22
#define VOC_VOL     0x04
#define LINE_VOL    0x2e
#define FM_VOL	    0x26
#define CD_VOL	    0x28
#define RECORD_SRC  0x0C

#define MIC	    0x11
#define LINE	    0x17
#define CD	    0x13

void main()
{
    unsigned char vol;
    int left, right;
    int i;
    unsigned char old[256];

    outportb(0x224, MASTER_VOL);
    vol = inportb(0x225);

    left = vol >> 4;
    right = vol & 0x0f;

    printf("Master volume is %d,%d\n", left, right);

    outportb(0x224, VOC_VOL);
    vol = inportb(0x225);

    left = vol >> 4;
    right = vol & 0x0f;

    printf("VOC volume is %d,%d\n", left, right);

    outportb(0x224, LINE_VOL);
    vol = inportb(0x225);

    left = vol >> 4;
    right = vol & 0x0f;

    printf("Line volume is %d,%d\n", left, right);

    outportb(0x224, FM_VOL);
    vol = inportb(0x225);

    left = vol >> 4;
    right = vol & 0x0f;

    printf("FM volume is %d,%d\n", left, right);

    outportb(0x224, CD_VOL);
    vol = inportb(0x225);

    left = vol & 0x0f;
    printf("CD volume is %d\n", left);

    outportb(0x224, RECORD_SRC);
    left = inportb(0x225);

    printf("Recording source is %d\n", left);
}
