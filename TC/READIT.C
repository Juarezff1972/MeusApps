main ()
{
char *c;
fopen("readme.txt",1);
play:
fscanf("readme.txt","%c",*c);
if (c == 26) goto stop;
printf("%c",*c);
goto play;
stop:
printf("\n\n");
printf("Finis...");
printf("\n");
}