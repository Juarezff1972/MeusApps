#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

#define INIT_BITS 1                                   // 10     10
#define MAX_BITS 9                                     // 8      1
#define HASHING_SHIFT MAX_BITS-8

#if MAX_BITS == 14
#define TABLE_SIZE 18041
#elif MAX_BITS==13
#define TABLE_SIZE 9029
#else
#define TABLE_SIZE 5021
#endif

#define CLEAR_TABLE 256
#define TERMINATOR 257
#define FIRST_CODE 258
#define CHECK_TIME 100

#define MAXVAL(n) (( 1 << (n)) -1)

unsigned input_code();
void *malloc();

int *code_value;
unsigned *prefix_code;
unsigned *append_character;
unsigned decode_stack[4000];

int num_bits=INIT_BITS;
unsigned long bytes_in=0,bytes_out=0;
int max_code;
unsigned long checkpoint=CHECK_TIME;
char bkpfile[81];
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
void chdir(char nome[81])
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
int find_match(int hash_prefix,unsigned hash_character)
{
   int index, offset;

   index = (hash_character<<HASHING_SHIFT)^hash_prefix;
   if(index==0) offset=1;
   else offset = TABLE_SIZE-index;
   while(1)
   {
      if(code_value[index]==-1)
         return(index);
      if(prefix_code[index]==hash_prefix&&append_character[index]==hash_character)
         return(index);
      index -= offset;
      if(index<0)
         index += TABLE_SIZE;
   }
}
///////////////////////////////////////////////////////////////////////
void output_code(FILE *output,unsigned code)
{
   static int output_bit_count=0;
   static unsigned long output_bit_buffer=0L;
   output_bit_buffer|=(unsigned long)code<<(32-num_bits-output_bit_count);
   output_bit_count+=num_bits;
   while (output_bit_count>=8)
   {
      putc(output_bit_buffer>>24,output);
      output_bit_buffer<<=8;
      output_bit_count-=8;
      bytes_out++;
   }
}
////////////////////////////////////////////////////////////////////////
void compress(FILE *input, FILE *output)
{
   unsigned next_code=FIRST_CODE;
   unsigned character;
   unsigned string_code;
   unsigned index;
   int i,ratio_new,ratio_old=100;

   for(i=0;i<TABLE_SIZE;i++) code_value[i]=-1;
   printf("Comprimindo\n");
   string_code=getc(input);
   while ((character=getc(input)) != (unsigned)EOF)
   {
      if (!(++bytes_in % 1000)) putchar('.');
      index=find_match(string_code,character);
      if (code_value[index]!=-1)
         string_code=code_value[index];
      else
      {
         if (next_code <= max_code)
         {
            code_value[index]=next_code++;
            prefix_code[index]=string_code;
            append_character[index]=character;
         }
         output_code(output,string_code);
         string_code=character;
         if (next_code > max_code)
         {
            if (num_bits < MAX_BITS)
            {
               putchar('+');
               max_code=MAXVAL(++num_bits);
            }
            else if (bytes_in > checkpoint)
                 {
                    if (num_bits == MAX_BITS > max_code)
                    {
                       ratio_new=bytes_out*100/bytes_in;
                       if (ratio_new > ratio_old)
                       {
                          output_code(output,CLEAR_TABLE);
                          putchar('C');
                          num_bits=INIT_BITS;
                          next_code=FIRST_CODE;
                          max_code=MAXVAL(num_bits);
                          bytes_in=bytes_out=0;
                          ratio_old=100;
                          for(i=0;i<TABLE_SIZE;i++) code_value[i]=-1;
                       }
                       else ratio_old=ratio_new;
                    }
                    checkpoint=bytes_in+CHECK_TIME;
                 }
            }
      }
   }
   output_code(output,string_code);
   if(next_code == max_code)
   {
      ++num_bits;
      putchar('+');
   }
   output_code(output,TERMINATOR);
   output_code(output,0);
   output_code(output,0);
   output_code(output,0);
   putchar('\n');
   printf("%d\n",ratio_new);
}
////////////////////////////////////////////////////////////////////////
unsigned input_code(FILE *input)
{
   unsigned return_value;
   static int input_bit_count=0;
   static unsigned long input_bit_buffer=0L;
   while(input_bit_count<=24)
   {
      input_bit_buffer|=(unsigned long)getc(input)<<(24-input_bit_count);
      input_bit_count+=8;
   }
   return_value=input_bit_buffer>>(32-num_bits);
   input_bit_buffer<<=num_bits;
   input_bit_count-=num_bits;
   return (return_value);
}
///////////////////////////////////////////////////////////////////////
void expand(FILE *input,FILE *output)
{
   unsigned next_code=FIRST_CODE;
   unsigned new_code;
   unsigned old_code;
   int character,counter=0,clear_flag=1;
   unsigned char *string;
   char *decode_string(unsigned char *buffer, unsigned code);
   printf("Expandindo\n");
   while ((new_code=input_code(input))!=TERMINATOR)
   {
      if (clear_flag)
      {
         clear_flag=0;
         old_code=new_code;
         character=old_code;
         putc(old_code,output);
         continue;
      }
      if (new_code==CLEAR_TABLE)
      {
         clear_flag=1;
         num_bits=INIT_BITS;
         next_code=FIRST_CODE;
         putchar('C');
         max_code=MAXVAL(num_bits);
         continue;
      }
      if (++counter==1000)
      {
         counter=0;
         putchar('.');
      }
      if (new_code>=next_code)
      {
         *decode_stack=character;
         string=decode_string(((unsigned char *)decode_stack)+1,old_code);
      }
      else string=decode_string(((unsigned char *)decode_stack),new_code);
      character=*string;
      while(string>=(unsigned char*)decode_stack) putc(*string--,output);
      if (next_code<=max_code)
      {
         prefix_code[next_code]=old_code;
         append_character[next_code++]=character;
         if (next_code==max_code&&num_bits<MAX_BITS)
         {
            putchar('+');
            max_code=MAXVAL(++num_bits);
         }
      }
      old_code=new_code;
   }
   putchar('\n');
}
///////////////////////////////////////////////////////////////////////
char *decode_string(unsigned char *buffer, unsigned code)
{
   int i=0;
   while (code>255)
   {
      *buffer++=append_character[code];
      code=prefix_code[code];
      if (i++>=4000)
      {
         printf("Erro durante expansão.\n");
         exit(1);
      }
   }
   *buffer=code;
   return (buffer);
}
///////////////////////////////////////////////////////////////////////
int montaarq(char filename[81])
{
   FILE *temp_file,*in_file;
   int i;
   unsigned long y;
   struct find_t s;
   char filesize[11];
   char filedate[5];
   printf("%s\n",filename);
   temp_file=fopen("temp.tmp","ab");
   if (temp_file==NULL) return 1; // Erro ao abrir arquivo temporário
   in_file=fopen(filename,"rb");
   if (in_file==NULL)  return 2; // Erro ao abrir arquivo
   i=_dos_findfirst(filename,_A_NORMAL,&s);
   if (i!=0) return 3;
   filename=s.name;
   ltoa(s.size,filesize,10);
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
   for(y=0;y<=s.size;y++) putc(getc(in_file),temp_file);
   putc('/',temp_file);   putc(0x0,temp_file);   putc('/',temp_file);
   putc('i',temp_file);   putc('N',temp_file);   putc('f',temp_file);
   putc('O',temp_file);   putc('c',temp_file);   putc('L',temp_file);
   putc('a',temp_file);   putc('S',temp_file);   putc('s',temp_file);
   putc('/',temp_file);
   fclose(temp_file);
   fclose(in_file);
   return 0;
}
///////////////////////////////////////////////////////////////////////
int desmontaarq(char filename[81])
{
   FILE *temp_file,*in_file,*out_file;
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
      printf("%s\n",newname);
      if (getc(in_file)=='/')
      {
         for (i=0;i<=8;i++) newsize[i]=getc(in_file);
         if (getc(in_file)=='/')
         {
            for (i=0;i<=3;i++) newdate[i]=getc(in_file);
            if (getc(in_file)=='/')
            {
               filesize=atol(newsize);
               filedate=atoi(newdate);
               out_file=fopen(newname,"wb");
               if (out_file==NULL) return 1; // Erro ao abrir item
               for (y=0;y<filesize;y++) putc(getc(in_file),out_file);
               fclose(out_file);
               _dos_setftime((int)out_file,filedate,0);
            }
         }
      }
      for(i=0;i<=13;i++) aux=getc(in_file);
   }
   fclose(in_file);
   return 0;
}
///////////////////////////////////////////////////////////////////////
int criabkp(char directory[81])
{
   char aux[81],dbfs[81],cdxs[81],fpts[81];
   struct find_t s;
   int i;
   FILE *arqin,*arqout;
   chdir(directory);
   strcpy(aux,directory);
   strcpy(dbfs,aux); strcat(dbfs,"\\*.dbf");
   strcpy(cdxs,aux); strcat(cdxs,"\\*.cdx");
   strcpy(fpts,aux); strcat(fpts,"\\*.fpt");
   i=_dos_findfirst(dbfs,_A_NORMAL,&s);
   if (i!=0) return 3;
   while (montaarq(s.name)==0)
   {
      i=_dos_findnext(&s);
      if (i!=0) break; //return 3;
   }
   i=_dos_findfirst(cdxs,_A_NORMAL,&s);
   if (i!=0) return 3;
   while (montaarq(s.name)==0)
   {
      i=_dos_findnext(&s);
      if (i!=0) break; //return 3;
   }
   i=_dos_findfirst(fpts,_A_NORMAL,&s);
   if (i!=0) return 3;
   while (montaarq(s.name)==0)
   {
      i=_dos_findnext(&s);
      if (i!=0) break; //return 3;
   }
   arqin=fopen("temp.tmp","rb");
   arqout=fopen(bkpfile,"wb");
   chdir(directory);
   compress(arqin,arqout);
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
   strcat(aux,"\\temp.tmp");
   arqin=fopen(bkpfile,"rb");
   arqout=fopen(aux,"wb");
   chdir(directory);
   expand(arqin,arqout);
   fclose(arqin);
   fclose(arqout);
   chdir(directory);
   desmontaarq(aux);
   eliminate("temp.tmp");
}
///////////////////////////////////////////////////////////////////////
void main(int argc, char *argv[])
{
   FILE *input_file, *output_file;
   char input_file_name[81];
   char opcao;
   prefix_code=malloc(TABLE_SIZE*sizeof(unsigned));
   append_character=malloc(TABLE_SIZE*sizeof(unsigned char));

   if (prefix_code==NULL || append_character==NULL)
   {
      printf("Erro ao alocar espaço para tabelas!\n");
      exit(1);
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
      printf("Uso: LZW <diretório> <arquivo> C/E\n");
      exit(0);
   }
   chdir(input_file_name);
   if (opcao==1)
   {
      code_value=malloc(TABLE_SIZE*sizeof(unsigned));
      if (code_value==NULL)
      {
         printf("Erro ao alocar espaço para tabelas!\n");
         exit(1);
      }
      criabkp(input_file_name);
      free(code_value);
   }
   if (opcao==2) lebkp(input_file_name);
   free(prefix_code);
   free(append_character);
}


/*
filename.ext/999999999/ddmmaaaa/rrrrrrrr... ...rrrr/0/iNfOcLaSs/
*/



