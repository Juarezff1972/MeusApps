#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <ctype.h>
#include "implode.h"
////////////////////////////////////////////////////////////////////////
unsigned input_code();
void *malloc();
////////////////////////////////////////////////////////////////////////
char bkpfile[81];
int InFile, OutFile;
char far *WorkBuff;
unsigned error;
unsigned type;
unsigned dsize;
FILE *temp_file,*in_file,*out_file;
////////////////////////////////////////////////////////////////////////
unsigned far pascal ReadBuff(char far *buff, unsigned short int far *size)
{
   unsigned read = 0;
   _dos_read(InFile,buff,*size,&read);
   putch('.');
   return(read);
}
////////////////////////////////////////////////////////////////////////
unsigned far pascal WriteBuff(char far *buff, unsigned short int far *size)
{
   unsigned wrote = 0;
   _dos_write(OutFile,buff,*size,&wrote);
   return(wrote);
}
////////////////////////////////////////////////////////////////////////
void eliminate(char nome[81])
{
   struct REGPACK regs;
   unsigned aux1,aux2,aux3;
   aux1=0x4100;
   aux2=FP_OFF(nome);
   aux3=FP_SEG(nome);
   asm {
      push ax
      push dx
      push ds
      mov ax,aux1
      mov dx,aux2
      push aux3
      pop ds
      int 0x21
      pop ds
      pop dx
      pop ax
   }
}
////////////////////////////////////////////////////////////////////////
void chngdir(char nome[81])
{
   struct REGPACK regs;
   unsigned aux1,aux2,aux3;
   unsigned z;
   _dos_setdrive(toupper(nome[1])-64,&z);
   aux1=0x3B00;
   aux2=FP_OFF(nome);
   aux3=FP_SEG(nome);
   asm {
      push ax
      push dx
      push ds
      mov ax,aux1
      mov dx,aux2
      push aux3
      pop ds
      int 0x21
      pop ds
      pop dx
      pop ax
   }
}
////////////////////////////////////////////////////////////////////////
int montaarq(char filename[81],char toname[81])
{
   int i;
   unsigned long y;
   long tam;
   struct find_t s;
   char filesize[11];
   char filedate[5];
   printf("\n%s",filename);
   temp_file=fopen(toname,"ab");
   if (temp_file==NULL) return 1; // Erro ao abrir arquivo temporário
   if (_dos_open(filename,O_RDONLY,&InFile) != 0)
   {
      printf("Erro ao abrir arquivo %s.\n",filename);
      return 3;
   }
   _dos_creat("trash.tmp",0,&OutFile);
   implode(ReadBuff,WriteBuff,WorkBuff,&type,&dsize);
   _dos_close(InFile);
   _dos_close(OutFile);
   i=_dos_findfirst("trash.tmp",_A_NORMAL,&s);
   in_file=fopen("trash.tmp","rb");
   tam=s.size;
   ltoa(s.size,filesize,10);
   if (in_file==NULL)  return 2; // Erro ao abrir arquivo
   i=_dos_findfirst(filename,_A_NORMAL,&s);
   if (i!=0) return 3;
   filename=s.name;
   ltoa(s.wr_date,filedate,10);
   while (strlen(filename)<12) strcat(filename," ");
   while (strlen(filesize)< 9) strcat(filesize," ");
   while (strlen(filedate)< 4) strcat(filedate," ");
   for (i=0;i<12;i++) putc(filename[i],temp_file);
   putc('/',temp_file);
   for (i=0;i<9;i++) putc(filesize[i],temp_file);
   putc('/',temp_file);
   for (i=0;i<4;i++) putc(filedate[i],temp_file);
   putc('/',temp_file);
   for(y=0;y<atol(filesize);y++) putc(getc(in_file),temp_file);
   putc('/',temp_file);   putc(0x0,temp_file);   putc('/',temp_file);
   putc('i',temp_file);   putc('N',temp_file);   putc('f',temp_file);
   putc('O',temp_file);   putc('c',temp_file);   putc('L',temp_file);
   putc('a',temp_file);   putc('S',temp_file);   putc('s',temp_file);
   putc('/',temp_file);
   fclose(temp_file);
   fclose(in_file);
   eliminate("trash.tmp");
   return 0;
}
///////////////////////////////////////////////////////////////////////
int desmontaarq(char filename[81],char directory[81])
{
   char newname[14],newsize[10],newdate[5],aux;
   struct find_t s;
   int i;
   unsigned long filesize,y;
   unsigned filedate;
   i=_dos_findfirst(filename,_A_NORMAL,&s);
   if (i!=0) return 3;
   in_file=fopen(filename,"rb");
   if (in_file==NULL) return 2; // Erro ao abrir arquivo
   while (!feof(in_file))
   {
      for (i=0;i<=13;i++) newname[i]=0;
      for (i=0;i<=9; i++) newsize[i]=0;
      for (i=0;i<=4; i++) newdate[i]=0;
      for (i=0;i<=11;i++) newname[i]=getc(in_file);
      printf("\n%s",newname);
      if (getc(in_file)=='/')
      {
         for (i=0;i<=8;i++) newsize[i]=getc(in_file);
         if (getc(in_file)=='/')
         {
            for (i=0;i<=3;i++) newdate[i]=getc(in_file);
            if (getc(in_file)=='/')
            {
               chngdir(directory);
               filesize=atol(newsize);
               filedate=atoi(newdate);
               out_file=fopen("trash.tmp","wb");
               if (out_file==NULL) return 1; // Erro ao abrir item
               for (y=0;y<filesize;y++) putc(getc(in_file),out_file);
               fclose(out_file);
               _dos_open("trash.tmp",O_RDONLY,&InFile);
               _dos_creat(newname,0,&OutFile);
               error=explode(ReadBuff,WriteBuff,WorkBuff);
               if (error) puts("Erro no arquivo compactado!\n");
               _dos_close(InFile);
               _dos_close(OutFile);
               _dos_setftime((int)out_file,filedate,0);
            }
         }
      }
      for(i=0;i<=12;i++) aux=getc(in_file);
   }
   fclose(in_file);
   eliminate("trash.tmp");
   return 0;
}
///////////////////////////////////////////////////////////////////////
int criabkp(char directory[81])
{
   char aux[81],dbfs[81],cdxs[81],fpts[81];
   struct find_t s;
   int i;
   FILE *arqin,*arqout;
   i=_dos_findfirst(bkpfile,_A_NORMAL,&s);
   if (i==0) eliminate(bkpfile);
   chngdir(directory);
   strcpy(aux,directory);
   strcpy(dbfs,aux); strcat(dbfs,"\\*.dbf");
   strcpy(cdxs,aux); strcat(cdxs,"\\*.cdx");
   strcpy(fpts,aux); strcat(fpts,"\\*.fpt");
   i=_dos_findfirst(dbfs,_A_NORMAL,&s);
   if (i!=0) return 3;
   while (montaarq(s.name,bkpfile)==0)
   {
      i=_dos_findnext(&s);
      if (i!=0) break; //return 3;
   }
   i=_dos_findfirst(cdxs,_A_NORMAL,&s);
   if (i!=0) return 3;
   while (montaarq(s.name,bkpfile)==0)
   {
      i=_dos_findnext(&s);
      if (i!=0) break; //return 3;
   }
   i=_dos_findfirst(fpts,_A_NORMAL,&s);
   if (i!=0) return 3;
   while (montaarq(s.name,bkpfile)==0)
   {
      i=_dos_findnext(&s);
      if (i!=0) break; //return 3;
   }
   chngdir(directory);
   fclose(arqin);
   fclose(arqout);
   eliminate("temp.tmp");
   return 0;
}
///////////////////////////////////////////////////////////////////////
void lebkp(char directory[81])
{
   char aux[81];
   FILE *arqin,*arqout;
   strcpy(aux,directory);
   strcat(aux,"\\");
   strcat(aux,bkpfile);
   chngdir(directory);
   desmontaarq(bkpfile,directory);
   eliminate("temp.tmp");
}
///////////////////////////////////////////////////////////////////////
void main(int argc, char *argv[])
{
   FILE *input_file, *output_file;
   char input_file_name[81];
   char opcao;
   type=CMP_BINARY;
   dsize=4096;
   WorkBuff=malloc(35256);
   if (WorkBuff == NULL)
   {
      puts("Erro ao alocar espaço para tabelas!\n");
      exit(1);
      return;
   }
   if (argc>3)
   {
      strcpy(input_file_name,argv[1]);
      strcpy(bkpfile,argv[2]);
      opcao=0;
      if (strcmpi(argv[3],"c")==0) opcao=1;
      if (strcmpi(argv[3],"e")==0) opcao=2;
   }
   else
   {
      printf("Uso: LZW2 <diretório> <arquivo> C/E\n");
      exit(0);
      return;
   }
   chngdir(input_file_name);
   if (opcao==1) criabkp(input_file_name);
   if (opcao==2) lebkp(input_file_name);
  _ffree(WorkBuff);
}


/*
filename.ext/999999999/ddmmaaaa/rrrrrrrr... ...rrrr/0/iNfOcLaSs/
*/



