

ordena_enfesto()
{
    int n;
    int ap;
    int i;
    int ch;
    int alterou = 0;
    float aux;
    struct list_ord *p;
    struct list_ord *ord();

    n = lote_aux.comp_enf;
    aux = n;
    if(aux < lote_aux.comp_enf)
         n++;
    for(i = 0;i < n;i++)
    {
         ch = 0;
         p = NULL;
         for(ap = vetenv[i];ap != -1;ap = listenv[ap].prox)
              p = ord(ap,p,&ch);
         if(ch)
         {
              alterou = 1;
              devolve(p,vetenv[i]);
         }
         libera_p(p);
    }
    return(alterou);
}


libera_p(p)
    struct list_ord *p;
{
    struct list_ord *aux;

    while(p != NULL)
    {
         aux = p->prox;
         free(p);
         p = aux;
    }
}


struct list_ord *ord(i,ap,alterou)
    int i;
    int *alterou;
    atruct list_ord *ap;
{
    struct list_ord *p,*q,*ant;
    int k,j;

    p = (struct list_ord *) malloc(sizeof(struct list_ord));
    strcpy(p->(ap.identam),listenv[i].identam);
    for(k = 0;k < 3;k++)
         for(j = 0;j < 2;j++)
              p->(ap.(m[k][j])) = listenv[i].(m[k][j])
    ;
    p->(ap.apl) = listenv[i].apl;
    p->(ap.ch_tomba) = listenv[i].ch_tomba;
    p->(ap.cor) = listenv[i].cor;
    monta_reg(p->apl);
    if(p->(ap.ch_tomba))
         tomba_molde();
    recup_molde(i);
    for(k = 0;k < 2;k++)
    {
         p->(ap.(env[k].x)) = registro.envelope[k].x;
         p->(ap.(env[k].y)) = registro.envelope[k].y;
    }
    *alterou = 0;
    j = (p->(ap.(env[0].x)) + p->(ap.(env[1].x))) / 2;
    j += ((p->(ap.(env[0].x)) + p->(ap.(env[1].x))) / 2;
