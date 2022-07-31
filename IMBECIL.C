typedef struct tags1
{
   int i;int j;
} s1;

typedef s1 s1tab[5];
typedef struct tags2 { s1tab t; s1tab m[4]; } s2;

void inics1 (s1 *s)
{
   s->i=s->j=0;
}

void inics1tab(s1tab s[], int i)
{
   int n;
   for (n=0;n<i;n++) inics1(& s[n]);
}

void inics2(s2 *s)
{
   int i;
   inics1tab(s->t,sizeof(s1tab)/sizeof(s1));
   for(i=0;i<4;i++) inics1tab(& s->m[i], sizeof(s1tab)/sizeof(s1));
}

void main()
{
   s2 s;
   inics2(&s);
}
