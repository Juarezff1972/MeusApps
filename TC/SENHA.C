main()
{
char s[80], *strcmp();
printf("QUAL O SUA SENHA? ");
gets(s);
if(strcmp(s,"0123456"));
{
printf("ACESSO PERMITIDO.\n");
return 0;
}
printf("ACESSO NEGADO.\n");
}

