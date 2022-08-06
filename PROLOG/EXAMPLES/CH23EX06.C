/*
   Turbo Prolog 2.0 Reference Guide Chapter 3, Example Program 6
   
   Copyright (c) 1986, 88 by Borland International, Inc
   
*/
   
void *alloc_gstack(unsigned);

typedef struct {
   char functor;  /* Type of functor */
   int ival;  /* Value of the functor */
   char cval;
   double rval;
   char *sval;
} mydom;

pack_0(int P1, char P2, double P3, char *P4, mydom **P5 )
{
   mydom *p=*P5=alloc_gstack(sizeof(mydom));
   p->functor=1;
   p->ival=P1;
   p->cval=P2;
   p->rval=P3;
   p->sval=P4;
}
