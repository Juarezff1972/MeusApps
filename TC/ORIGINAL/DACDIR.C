/* Output to SB DAC in single sample mode */

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <io.h>
#include <alloc.h>
#include <dos.h>

int InitSB(void);
extern unsigned IOaddr;
extern unsigned IRQ;

/* Set voice on or off */
void SetVoice(int state)
{
    while(inportb(0x22c) & 0x80)
        ;
    outportb(0x22c,(state) ? 0xd1 : 0xd3);
}

/* Read first 64000 bytes of a raw sample file and play the sample */
void main(int argc, char *argv[])
{
    FILE *f;
    signed char *raw;
    unsigned sample_len;
    register int j, i;
    int tmp;

    if(argc != 2)
    {
	puts("Usage: dacdir sample_file");
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
    }
    sample_len = (unsigned)filelength(fileno(f));

    raw = (signed char *)malloc(sample_len);
    fread(raw,1,sample_len,f);

    fclose(f);

    SetVoice(1);

    for(i = 0; i < sample_len; i++)
    {
	while(inportb(0x22c) & 0x80)
	    ;
	outportb(0x22c,0x10);
	while(inportb(0x22c) & 0x80)
            ;
	outportb(0x22c,raw[i]);

	while(inportb(0x22e) & 0x80)
	    ;
    }

    SetVoice(0);

    free(raw);

    InitSB();

    exit(0);
}
