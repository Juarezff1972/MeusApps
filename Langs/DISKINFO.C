#include <dos.h>
#include <dir.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <process.h>

unsigned char drive,nivel;
struct dfree *BytesLivres;
unsigned char *DirTotal;
unsigned char *DiretorioAtivo;
unsigned char *NumeroDeDiretorios[10];
unsigned char *NumeroDeArquivos;
unsigned char NomeDoDisco[13];

void LerArvore(void);

void LerArvore(void)
{
  char *path;
  struct ffblk *lista;
  char *attr;
  getcurdir(drive,path);
  nivel=nivel+1;
  if (strlen(path)>3)  path=*path+47;
  findfirst(*path+"*.*",lista,FA_RDONLY+FA_HIDDEN+FA_SYSTEM+FA_LABEL+FA_DIREC+FA_ARCH);
  attr = lista.ff_attrib;
  while (((attr == FA_DIREC)|(attr == FA_LABEL)))
  {
     if (attr == FA_LABEL)  NomeDoDisco=lista.ff_name;
     findnext(lista);
     attr=lista.ff_attrib;
     NumeroDeArquivos=NumeroDeArquivos+1;
  };
  while ((errno == 0) & (attr == FA_DIREC))
  {
    if ((lista.ff_name <> ".") & (lista.ff_name <> ".."))
    {
       chdir(lista.name);
       getcurdir(drive,path);
       NumeroDeDiretorios[nivel]=NumeroDeDiretorios[nivel]+1;
       LerArvore();
       nivel=nivel-1;
       chdir("..");
    };
    findnext(lista);
    attr=lista.ff_attrib;
    while (((attr<>directory)and(doserror==0)))
    {
       if (attr==FA_LABEL)  NomeDoDisco=lista.ff_name;
       findnext(lista);
       attr=lista.ff_attrib;
       NumeroDeArquivos=NumeroDeArquivos+1;
    };
  };
};
/*-------------------------------------------------------------*/
main()
{
   for (nivel=0;nivel<10;nivel++) NumeroDeDiretorios[nivel]=0;
   nivel=0;
   DirTotal=0;
   NumeroDeArquivos=0;
   if (_argc==0)
   {
      printf("\n");
      printf("Escreva: DISKINFO X: onde X e' o drive a ser lido.");
      printf("\n");
      exit(2);
   };
   drive=255;
   if ((_argv[1]=="a:") | (_argv[1]=="A:"))  drive=0;
   if ((_argv[1]=="b:") | (_argv[1]=="B:"))  drive=1;
   if ((_argv[1]=="c:") | (_argv[1]=="C:"))  drive=2;
   if ((_argv[1]=="d:") | (_argv[1]=="D:"))  drive=3;
   if ((_argv[1]=="e:") | (_argv[1]=="E:"))  drive=4;
   if ((_argv[1]=="f:") | (_argv[1]=="F:"))  drive=5;
   if ((_argv[1]=="g:") | (_argv[1]=="G:"))  drive=6;
   if ((_argv[1]=="h:") | (_argv[1]=="H:"))  drive=7;
   if (drive==255)
   {
      printf("\n");
      printf("Drive %c n„o existe...\n",_argv[1]);
      printf("\n");
      exit(1);
   };
   getdfree(drive,BytesLivres);
   if (((drive>0) & (drive<9) & (BytesLivres.df_sclus == -1)))
   {
      printf("\n");
      printf("Drive %c n„o est  pronto...",_argv[1]);
      printf("\n");
      exit(3);
   };
   getcurdir(drive,DiretorioAtivo);
   printf("Drive: %d\n",_argv[1]);
   printf("Dos Vers„o %d.%d\n",_osminor,_osmajor);
   printf("\n");
   printf("Diret¢rio Ativo: %c\n",DiretorioAtivo);
   printf("Existem %d bytes livres. (%d Kb ou %d Mb)\n"
	  ,(BytesLivres.df_bsec*BytesLivres.df_sclus*BytesLivres.df_avail)
	  ,((BytesLivres.df_bsec*BytesLivres.df_sclus*BytesLivres.df_avail)/1024)
	  ,((BytesLivres.df_bsec*BytesLivres.df_sclus*BytesLivres.df_avail)/1024)/1024);
   printf("Existem %d bytes no total. (%d Kb ou %d Mb)\n"
	  ,(BytesLivres.df_bsec*BytesLivres.df_sclus*BytesLivres.df_total)
	  ,((BytesLivres.df_bsec*BytesLivres.df_sclus*BytesLivres.df_total)/1024)
	  ,((BytesLivres.df_bsec*BytesLivres.df_sclus*BytesLivres.df_total)/1024)/1024);
   printf("\n");
   chdir(_argv[1]+"\\");
   LerArvore();
   printf("Existem %d arquivos no disco.",NumeroDeArquivos);
   for (nivel=1;nivel<10;nivel++)
   {
      if (NumeroDeDiretorios[nivel]>0)
      {
         DirTotal=DirTotal+NumeroDeDiretorios[nivel];
	 printf("Existem %d diret¢rios de nivel %d\n",NumeroDeDiretorios[nivel],nivel);
      };
   };
   printf("Existem no total %d diret¢rios.\n",DirTotal);
   printf("\n");
   printf("Volume: %c",NomeDoDisco);
   printf("\n");
}