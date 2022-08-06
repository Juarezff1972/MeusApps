/* unlzexe.c
* unlzexe ver 0.2 (PC-VAN UTJ44266 Kou )
*   UNLZEXE converts the compressed file by lzexe(ver.0.90,0.91) to the
*   UNcompressed executable one.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define FAILURE	1
#define SUCCESS 0

typedef unsigned int WORD;
typedef unsigned char BYTE;



void main(int argc,char **argv){
    int rdhead(FILE *,int *);
    void mkreltbl(FILE *,FILE *,int);
    void unpack(FILE *,FILE *);
    void wrhead(FILE *);

    FILE *ifile,*ofile;
    int ver;

    printf("UNLZEXE Ver. 0.2\n");
	if(argc!=3){
	    printf("uso:UNLZEXE packedfile unpackedfile");
	    exit(1);
	}
	if((ifile=fopen(argv[1],"rb"))==NULL){
	    printf("'%s' :n„o encontrado.",argv[1]);
	    exit(1);
	}

	if(rdhead(ifile,&ver)!=SUCCESS){
	    printf("'%s' n„o e' arquivo LZEXE.",argv[1]);
	}else {
	    if((ofile=fopen(argv[2],"w+b"))==NULL){
		printf("'%s' :erro ao abrir.",argv[2]);
		fclose(ifile);
		exit(1);
	    }
	    printf("arquivo '%s' e' compactado por LZEXE Ver. ",argv[1]);
	    switch(ver){
	    case 90: printf("0.90\n"); break;
	    case 91: printf("0.91\n"); break;
	}
	    mkreltbl(ifile,ofile,ver);
	    unpack(ifile,ofile);
	    wrhead(ofile);
	    fclose(ofile);
	    printf("arquivo descompactado '%s' foi gerado.\n",argv[2]);
    }
    fclose(ifile);
}

/*-------------------------------------------*/
static WORD ihead[0x10],ohead[0x10],inf[8];
static WORD allocsize;
static long fpos,loadsize;

/* EXE header test (is it LZEXE file?) */
int rdhead(FILE *ifile ,int *ver){
    if(fread(ihead,sizeof ihead[0],0x10,ifile)!=0x10)
	return FAILURE;
    memcpy(ohead,ihead,sizeof(ihead[0]) * 0x10);
    if(ihead[0]!=0x5a4d || ihead[4]!=2 || ihead[0x0d]!=0)
	return FAILURE;
    if(ihead[0x0c]==0x1c && memcmp(&ihead[0x0e],"LZ09",4)==0){
	*ver=90; return SUCCESS ;
    }
    if(ihead[0x0c]==0x1c && memcmp(&ihead[0x0e],"LZ91",4)==0){
	*ver=91; return SUCCESS ;
    }
    return FAILURE;
}

/* make relocation table */
void mkreltbl(FILE *ifile,FILE *ofile,int ver) {
    void reloc90();
    void reloc91();
    int i;

    allocsize=((ihead[1]+16-1)>>4) + ((ihead[2]-1)<<5) - ihead[4] + ihead[5];
    fpos=(long)(ihead[0x0b]+ihead[4])<<4;		/* goto CS:0000 */
    fseek(ifile,fpos,SEEK_SET);
    fread(inf, sizeof inf[0], 0x08, ifile);
    ohead[0x0a]=inf[0];		/* IP */
    ohead[0x0b]=inf[1];		/* CS */
    ohead[0x08]=inf[2];		/* SP */
    ohead[0x07]=inf[3];		/* SS */
    /* inf[4]:size of compressed load module (PARAGRAPH)*/
    /* inf[5]:increase of load module size (PARAGRAPH)*/
    /* inf[6]:size of decompressor with  compressed relocation table (BYTE) */
    /* inf[7]:check sum of decompresser with compressd relocation table(Ver.0.90) */
    ohead[0x0c]=0x1c;		/* start position of relocation table */
    fseek(ofile,0x1cL,SEEK_SET);
    switch(ver){
    case 90: reloc90(ifile,ofile,fpos);
	     break;
    case 91: reloc91(ifile,ofile,fpos);
	     break;
    }
    i=ohead[3]*4+ohead[0x0c];
    ohead[4]=((i+0x1ff) & ~0x1ff)>>4;

    for(i=0x200-(i & 0x1ff); i>0; i--)
       putc(0, ofile);
}
/* for LZEXE ver 0.90 */
void reloc90(FILE *ifile,FILE *ofile,long fpos) {
    unsigned int c;
    WORD rel_count=0;
    WORD rel_item[2];		/* [0]:offset, [1]:segment */

    fseek(ifile,fpos+0x19d,SEEK_SET);
				/* 0x19d=compressed relocation table address */
    for(rel_item[1]=0;rel_item[1]<0x10;rel_item[1]++) {
	if((c=getw(ifile))==0)
	    continue;
	else {
	    for(;c>0;c--) {
		rel_item[0]=getw(ifile);
		putw(rel_item[0],ofile);
		putw(rel_item[1],ofile);
		rel_count++;
	    }
	}
    }
    ohead[3]=rel_count;
}
/* for LZEXE ver 0.91*/
void reloc91(FILE *ifile,FILE *ofile,long fpos) {
    WORD span;
    WORD rel_count=0;
    WORD rel_item[2];		/* [0]:offset, [1]:segment */

    fseek(ifile,fpos+0x158,SEEK_SET);
				/* 0x158=compressed relocation table address */
    rel_item[0]=0; rel_item[1]=0;
    for(;;) {
	if((span=getc(ifile))==0) {
	    span=getw(ifile);
	    if(span==0){
		rel_item[1] += 0x0fff;
		continue;
	    } else if(span==1){
		break;
	    }
	}
	rel_item[0] += span;
	rel_item[1] += (rel_item[0] & ~0x0f)>>4;
	rel_item[0] &= 0x0f;
	putw(rel_item[0],ofile);
	putw(rel_item[1],ofile);
	rel_count++;
    }
    ohead[3]=rel_count;
}

/*---------------------*/
typedef struct {
	FILE  *fp;
	WORD  buf;
	BYTE  count;
    } bitstream;

void initbits(bitstream *,FILE *);
int getbit(bitstream *);

/*---------------------*/
/* decompressor routine */
void unpack(FILE *ifile,FILE *ofile){
    long len;
    long span;
    long fpos;
    unsigned i;
    bitstream bits;
    BYTE data[0x4500], *p=data;

    fpos=(long)(ihead[0x0b]-inf[4]+ihead[4])<<4;
    fseek(ifile,fpos,SEEK_SET);
    fpos=(long)ohead[4]<<4;
    fseek(ofile,fpos,SEEK_SET);
    initbits(&bits,ifile);
    printf(" descompactando. ");
    for (i=0;i<0x4500;i++) data[i]=0;
    for(;;){
	if(p-data>0x4000){
	    fwrite(&data,sizeof data[0],0x2000,ofile);
	    p-=0x2000;
	    memcpy(data,data+0x2000,p-data);
	    putchar('.');
	}
   if(feof(ifile)!=0) break;
	if(getbit(&bits)) {
	    *p++=getc(ifile);
	    continue;
	}
	if(!getbit(&bits)) {
	    len=getbit(&bits)<<1;
	    len |= getbit(&bits);
	    len += 2;
	    span=getc(ifile) | 0xff00;
	} else {
	    span=(BYTE)getc(ifile);
	    len=getc(ifile);
	    span |= ((len & ~0x07)<<5) | 0xe000;
	    len = (len & 0x07)+2;
	    if (len==2) {
		len=getc(ifile);

//		if(len==0) break;
		if(feof(ifile)!=0) break;

		if(len==1)
		    continue; /* segment change */
		else
		    len++;
	    }
	}
 	for( ;len>0;len--,p++){
	    *p=*(p+span);
	}
    }
    if(p!=data)
	fwrite(&data,sizeof data[0],p-data,ofile);
    loadsize=ftell(ofile)-fpos;
    printf("fim\n");
}

/* write EXE header*/
void wrhead(FILE *ofile) {
    if(ihead[6]!=0) {
	ohead[5]=allocsize-((loadsize+15)>>4);
	if(ihead[6]!=0xffff)
	    ohead[6]-=(ihead[5]-ohead[5]);
    }
    ohead[1]=(loadsize+(ohead[4]<<4))&0x1ff;
    ohead[2]=(loadsize+(ohead[4]<<4)+0x1ff) >>9;
    fseek(ofile,0L,SEEK_SET);
    fwrite(ohead,sizeof ohead[0],0x0e,ofile);
}
/*-------------------------------------------*/

/* get compress information bit by bit */
void initbits(bitstream *p,FILE *filep){
    p->fp=filep;
    p->count=0x10;
    p->buf=getw(filep);
//    printf("%04x ",p->buf);
}

int getbit(bitstream *p) {
    int b;
    b = p->buf & 1;
    if(--p->count == 0){
	(p->buf)=getw(p->fp);
//	printf("%04x ",p->buf);
	p->count= 0x10;
    }else
	p->buf >>= 1;

    return b;
}

