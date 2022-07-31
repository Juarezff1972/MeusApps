/* Output to SB DAC using DMA mode */

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <io.h>
#include <alloc.h>
#include <dos.h>
#include <stdlib.h>
#include <alloc.h>


/* Defines for Soundblaster and Soundblaster Pro IO address */
#define LEFT_FM_STATUS	    0x00    /* Pro only */
#define LEFT_FM_ADDRESS     0x00    /* Pro only */
#define LEFT_FM_DATA	    0x01    /* Pro only */
#define RIGHT_FM_STATUS     0x02    /* Pro only */
#define RIGHT_FM_ADDRESS    0x02    /* Pro only */
#define RIGHT_FM_DATA	    0x03    /* Pro only */
#define MIXER_ADDRESS	    0x04    /* Pro only */
#define MIXER_DATA	    0x05    /* Pro only */
#define DSP_RESET	    0x06
#define FM_STATUS	    0x08
#define FM_ADDRESS	    0x08
#define FM_DATA 	    0x09
#define DSP_READ_DATA	    0x0A
#define DSP_WRITE_DATA	    0x0C
#define DSP_WRITE_STATUS    0x0C
#define DSP_DATA_AVAIL	    0x0E
#define CD_ROM_DATA	    0x10    /* Pro only */
#define CD_ROM_STATUS	    0x11    /* Pro only */
#define CD_ROM_RESET	    0x12    /* Pro only */
#define CD_ROM_ENABLE	    0x13    /* Pro only */

#define ADLIB_FM_STATUS     0x388
#define ADLIB_FM_ADDRESS    0x388
#define ADLIB_FM_DATA	    0x389

/* Defines for 8237 DMA Controller IO addresses */
#define DMA	    0x00
#define CH0_BASE    DMA+0
#define CH0_COUNT   DMA+1
#define CH1_BASE    DMA+2
#define CH1_COUNT   DMA+3
#define CH2_BASE    DMA+4
#define CH2_COUNT   DMA+5
#define CH3_BASE    DMA+6
#define CH3_COUNT   DMA+7
#define DMA_STATUS  DMA+8
#define DMA_CMD     DMA+8
#define DMA_REQUEST DMA+9
#define DMA_MASK    DMA+10
#define DMA_MODE    DMA+11
#define DMA_FF      DMA+12
#define DMA_TMP     DMA+13
#define DMA_CLEAR   DMA+13
#define DMA_CLRMSK  DMA+14
#define DMA_WRMSK   DMA+15
#define DMAPAGE     0x80

int InitSB(void);
extern unsigned IOaddr;
extern unsigned IRQ;

void far interrupt (*OldIRQ)();
volatile int DMA_complete;

/* Define a useful macro for writing data to the DAC */
#define writedac(x) { while(inportb(IOaddr+DSP_WRITE_STATUS) & 0x80); \
		      outportb(IOaddr+DSP_WRITE_DATA,(x)); }

/* Interrupt handler for DMA complete IRQ from Soundblaster */
void far interrupt SBHandler()
{
    enable();
    DMA_complete = 1;
    outportb(0x20,0x20);
}

/* Sets the sample rate to be used for digitising or playback */
void SetSampleRate(unsigned rate)
{
    unsigned char tc;

    tc = (unsigned char)(256 - (1000000/rate));

    writedac(0x40);  /* Command byte for sample rate */
    writedac(tc);    /* Sample rate time constant */
}

void SetVoice(int state)
{
    writedac((state) ? 0xd1 : 0xd3);   /* Command bytes for voice:  */
				       /* 0xD1	-    voice on	    */
				       /* 0xD3	-    voice off	    */
}




void OutVoice(char *data, unsigned dlen)
{
    int t;
    long addr;
    register int i;
    unsigned char im, tm;

    DMA_complete = 0;

    /* Enable interrupts on PIC */
    im = inportb(0x21);
    tm = ~(1 << IRQ);
    outportb(0x21,im & tm);
    enable();

    /* Set DMA mode */
    outportb(DMA_MASK,5);
    outportb(DMA_FF,0);
    outportb(DMA_MODE,0x49);

    /* Set transfer address */
    addr = ((long)FP_SEG(data) << 4) + (long)FP_OFF(data);
    t = (int)(addr >> 16);
    outportb(DMAPAGE+3,t);
    t = (int)(addr & 0xffff);
    outportb(CH1_BASE,t & 0xff);
    outportb(CH1_BASE,t >> 8);

    /* Set transfer length byte count */
    outportb(CH1_COUNT,dlen & 0xff);
    outportb(CH1_COUNT,dlen >> 8);

    /* Unmask DMA channel */
    outportb(DMA_MASK,1);

    /* Setup Soundblaster for transfer */
    writedac(0x48);   /* One command byte used for DMA DAC transfer */

    /* Write length */
    writedac(dlen & 0xff);
    writedac(dlen >> 8);

    writedac(0x14);   /* Second command byte for DMA DAC transfer */

    /* Write length */
    writedac(dlen & 0xff);
    writedac(dlen >> 8);
}

void main(int argc, char *argv[])
{
    FILE *f;
    char *raw;
    long sample_len;
    register int j, i;
    unsigned sl, tmp, nr, sr;
    unsigned char tm;

    if(argc < 2)
    {
	puts("Usage: dacdma sample [sample rate]");
	exit(1);
    }

    if(InitSB())
    {
	printf("Could not find Soundblaster!\n");
	exit(1);
    }
    printf("Found Soundblaster at %xh.\n",IOaddr);

    f = fopen(argv[1],"rb");
    if(f == NULL)
    {
	printf("Could not open sample file %s\n",argv[1]);
	exit(1);
    }
    sample_len = filelength(fileno(f));
    if(sample_len > 64000)
        sl = 64000;
    else
        sl = (int)sample_len;

    raw = (signed char *)farmalloc((unsigned long)sl);
    nr = fread(raw,1,sl,f);

    fclose(f);


    /* Insert our IRQ handler into interrupt chain */
    disable();
    OldIRQ = getvect(0x08 + IRQ);
    setvect(0x08 + IRQ,SBHandler);
    enable();

    sr = 11000;
    if(argc == 3)
	sr = atoi(argv[2]);

    SetSampleRate(sr);
    SetVoice(1);

    OutVoice(raw,sl);

    while(!kbhit() && !DMA_complete)
	;
    if(!DMA_complete)
	getch();

    SetVoice(0);

    /* Restore old IRQ vector */
    disable();
    setvect(0x08 + IRQ,OldIRQ);
    tm = inportb(0x21);
    outportb(0x21,tm | (1 << IRQ));
    enable();

    farfree(raw);

    exit(0);
}
