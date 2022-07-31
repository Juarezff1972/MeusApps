// CHKD.C
// MSQC2.5
// SMALL Model

// to do:
// - check both FATs for diffent entry values
// - move data from a bad sector and then mark that sector bad
// - defrag

// For my own safety, if this program in any way destroys your machine or
// anything attached to it, I in no way take responsibility.

#include "chkd.h"

unsigned int  i,x,y;
unsigned int  b_sect, s_clus, t_clus, a_clus;
unsigned long freespace;
unsigned int  block_loc = 1;
unsigned int  sect_num = 0;
unsigned char chk_char = 'F';
unsigned char fat[9216];        // 18 sectors * 512 bytes/sector (2847)
unsigned int  fatval1;
unsigned int  fatval2;
unsigned int  fatval3;
unsigned int  fatloc = 0;
unsigned char our_buff[1536];   // 512 * 3 at a time

void main(void) {

	makescr();
	getdiskinfo();
	gotoxy(19,20);
	printf("%8i", b_sect);
	gotoxy(20,20);
	printf("%8i", s_clus);
	gotoxy(21,20);
	printf("%8i", t_clus);
	gotoxy(22,20);
	printf("%8i", a_clus);
	gotoxy(23,20);
	freespace = (long) b_sect * (long) s_clus * (long) a_clus;
	printf("%8li", freespace);

	if (getfat()) {
		printf("\nError reading FAT");
		exit(-1);
	}

	x = 3;
	y = 4;
	for (i=1;i<=949;i++) {
		gotoxy(y,x);
		putch('?');
		chk_char = 178;
		if (chksector())
			chk_char = 'b';
		else {
			fatval1 = getnextfatval(fatloc);
			fatloc++;
			fatval2 = getnextfatval(fatloc);
			fatloc++;
			fatval3 = getnextfatval(fatloc);
			fatloc++;
			if ((fatval1 == 0xFF7) || (fatval2 == 0xFF7) || (fatval3 == 0xFF7))
				chk_char = 'B';
			else
				if (((fatval1>=0xFF0)&&(fatval1<=0xFF6))||((fatval2>=0xFF0)&&(fatval2<=0xFF6))||((fatval3>=0xFF0)&&(fatval3<=0xFF6)))
					chk_char = 'R';
				else
					if ((fatval1 == 0x000) && (fatval2 == 0x000) && (fatval3 == 0x000)) chk_char = 177;
		}

		// we know that Sector 0 is BOOT SECTOR and sector 1 - 18 is the FAT,
		// so lets denote this with an R and 5 f's
		if (sect_num == 3) chk_char = 'R';
		if ((sect_num > 3) && (sect_num <= 18)) chk_char = 'f';
		gotoxy(y,x);
		putch(chk_char);

		if (x++ == 77) {
			x = 3;
			y++;
		}
	}
	gotoxy(24,1);
	exit(0);
}

void getdiskinfo() {
  _asm {
    mov  ah,36h
		mov  dl,01h            ; change to 02h for disk drive b:
    int  21h
		mov  a_clus,bx
    mov  b_sect,cx
    mov  s_clus,ax
    mov  t_clus,dx
  }
}

int chksector() {
	int  error_code = 0;

	_asm {
		mov  al,00h            ; change to 01h for disk drive b:
		mov  cx,03
		mov  dx,sect_num
		mov  bx,offset our_buff
		int  25h
		pop  ax
		jnc  chks_done
		mov  error_code,ax
chks_done:
	}
	sect_num += 3;
	return(error_code);
}

int getfat() {
	int  error_code = 0;

	_asm {
		mov  al,00h              ; change to 01h for disk drive b:
		mov  bx,offset fat[0]    ; offset of fat[0]
		mov  cx,18               ; 18 sectors
		mov  dx,1                ; #1 - #18
		int  25h                 ; read it
		pop  ax
		jnc  chks_done
		mov  error_code,ax
chks_done:
	}
	return(error_code);
}

int getnextfatval(unsigned int fatloctn) {
	unsigned int tempval = 0;

	tempval = fatloctn;
	fatloctn *= 3;
	fatloctn /= 2;
	if ((tempval & 0x0001) == 1) {
		tempval = (unsigned char) fat[fatloctn];
		tempval <<= 8;
		tempval |= (unsigned char) fat[fatloctn+1];
		tempval &= 0x0FFF;
	} else {
		tempval = (unsigned char) fat[fatloctn];
		tempval <<= 8;
		tempval |= (unsigned char) fat[fatloctn+1];
		tempval &= 0xFFF0;
		tempval >>= 4;
	}
	return(tempval);
}

void makescr(void) {
	printf("ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ Check Floppy Disk  v1.00 ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸");
	printf("³     °  = 3 sectors                  Forever Young Software  16 Apr 1998      ³");
	printf("³                                                                              ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°  ³");
	printf("³ °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°                            ³");
	printf("³                                                                              ³");
	printf("ÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµ");
	printf("³    Bytes/Sector:                                   ³  R = reserved           ³");
	printf("³ Sectors/Cluster:                                   ³  B or b = bad           ³");
	printf("³  Total Clusters:                                   ³  f = FAT                ³");
	printf("³ Avlble Clusters:                                   ³  ± = unused space       ³");
	printf("³ Free Disk Space:                                   ³  ² = used space         ³");
	printf("ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾");
}

void gotoxy(int x,int y) {
	x--;
	y--;
	_asm {
		mov  ah,02
		mov  bh,00h
		mov  dh,x        // row
		mov  dl,y        // col
		int  10h
	}
}
