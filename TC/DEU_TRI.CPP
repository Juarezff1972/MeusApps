#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <string.h>


char *current_directory(char *path)
{
   strcpy(path, "X:\\");      /* fill string with form of response: X:\ */
   path[0] = 'A' + getdisk();    /* replace X with current drive letter */
   getcurdir(0, path+3);  /* fill rest of string with current directory */
   return(path);
}

void vai(void)
{
   struct ffblk ffblk;
   int done;
   char curdir[MAXPATH];
   current_directory(curdir);
   printf("Directory listing of %s\n",curdir);
   done = findfirst("*.*",&ffblk,FA_RDONLY+FA_HIDDEN+FA_SYSTEM+FA_LABEL+FA_DIREC+FA_ARCH);
   while (!done)
   {
      printf("%s\\%s\n",curdir,xffblk.ff_name);
      if ( strcmp(ffblk.ff_name,".") && strcmp(ffblk.ff_name,"..") )
      {
         if (ffblk.ff_attrib==16)
        {
            chdir(ffblk.ff_name);
            vai();
         }
      }

      done = findnext(&ffblk);
   }
   return;
}

void main(void)
{
   vai();
}
/*

 Constant  ณ Description
อออออออออออุอออออออออออออออออออออ
 FA_RDONLY ณ Read-only attribute
 FA_HIDDEN ณ Hidden file
 FA_SYSTEM ณ System file
 FA_LABEL  ณ Volume label
 FA_DIREC  ณ Directory
 FA_ARCH   ณ Archive

*/