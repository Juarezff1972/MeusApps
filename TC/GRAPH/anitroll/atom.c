



#include "atom.h"


/* *************************************************************
************************************************************* */
void atom::new_action(FILE *infile, float  timefactor, char *buffer) {
}


/* *************************************************************
   This is the destructor for this class
************************************************************* */
atom::~atom() {

   eventtype *ptr;

   for (ptr=event; ptr!=(eventtype *)NULL; ptr=event) {
      event = event->next;
      delete ptr;
   }

   if (ob != (quark *)NULL)
      delete ob;
   if (next != (atom *)NULL)
      delete next;
}


/* *************************************************************
   Read in an object's data
************************************************************* */
void atom::read_data(char *filename) {

   FILE       *infile;                          // input file

   infile = fopen(filename, "r");

   ob = read_quark(infile, (quark *)NULL);

   fclose(infile);
}


void atom::update(quark *parent) {

   firstflag = 0;
   copyarray4(oldcenter, center);
   ob->update(this);
}


/* *************************************************************
   This function implements new events
************************************************************* */
void atom::new_action(int frameno, atom **header) {

   FILE      *infile;                           // input file
   int       number;                            // # of changes during an event
   int       i, j;                              // loop vars
   char      parent[MAXSTRLEN], dest[MAXSTRLEN];// temp string
   eventtype *ptr;                              // event pointer
   quark     *qtr;                              // quark pointer
   float     temp[3];                           // temp array
   int       tempd[2];                          // temp integer array
   char      token[MAXSTRLEN];

   for (ptr=event; ptr != (eventtype *)NULL && frameno >= ptr->event; ptr=event) {

      if (!(infile = fopen(ptr->efilename, "r"))) {
         printf("Can't access %s... Aborting\n", ptr->efilename);
         exit(0);
      }

      get_token(infile, token);

      for (number=atoi(token); number>0; number--) {
         get_token(infile, token);
         strcpy(dest, token);			// backup in case of quark name
         lower_case(token);
         if (!strcmp(token, "composite")) {	// the following event affects
            get_token(infile, token);
            lower_case(token);
            if (!strcmp(token, "spline")) {      // xyz dof change

               get_token(infile, token);
               kcount = atoi(token);
               get_token(infile, token);
               kcatchup = atof(token);
               get_token(infile, token);
               kframesperspline = atof(token);
               get_token(infile, token);
               kauto = atoi(token);

               kcatchup *= ptr->timefactor;
               kframesperspline *= ptr->timefactor;

               if (not_zero(kcatchup)) {
                  kstatus  = 1;
                  kcounter = 0;
                  copyarray3(kold, kcurrent);
               }

               else
                  kstatus = 0;

               kkey = 0;
               kframe = 0;

               if (kknot != (vector4f *)NULL)
                  delete [] kknot;

               kknot = new vector4f[kcount];

               for (j=0; j<kcount; j++) {
                  get_token(infile, token);
                  kknot[j][0] = atof(token);
                  get_token(infile, token);
                  kknot[j][1] = atof(token);
                  get_token(infile, token);
                  kknot[j][2] = atof(token);
               }

               copyarray3(kcurrent, kknot[1]);

               if (!kstatus)
                  for (i=0; i<4; i++)
                     for (j=0; j<4; j++)
                        Q[i][j] = kknot[j][i];
            }

            else {                              // alpha/beta/gamma dof change
               get_token(infile, token);
               qcount = atoi(token);
               get_token(infile, token);
               qcatchup = atof(token);
               get_token(infile, token);
               qframesperspline = atof(token);
               get_token(infile, token);
               qauto = atoi(token);

               qcatchup *= ptr->timefactor;
               qframesperspline *= ptr->timefactor;

               // center about which the object is rotated
               get_token(infile, token);
               origin[0] = atof(token);
               get_token(infile, token);
               origin[1] = atof(token);
               get_token(infile, token);
               origin[2] = atof(token);

               if (not_zero(qcatchup)) {
                  qstatus  = 1;
                  qcounter = 0;
                  copyarray4(qold, qcurrent);
               }

               else
                  qstatus = 0;

               qkey = 0;
               qframe = 0;

               qcount--;

               if (qstart != (vector4f *)NULL)
                  delete [] qstart;

               qstart = new vector4f[qcount];

               if (qend != (vector4f *)NULL)
                  delete [] qend;

               qend = new vector4f[qcount];

               for (j=0; j<qcount; j++) {
                  get_token(infile, token);
                  temp[0] = deg2rad(atof(token));
                  get_token(infile, token);
                  temp[1] = deg2rad(atof(token));
                  get_token(infile, token);
                  temp[2] = deg2rad(atof(token));

                  euler2quat(temp, qstart[j]);

                  if (j)
                     copyarray4(qend[j-1], qstart[j]);
               }

               get_token(infile, token);
               temp[0] = deg2rad(atof(token));
               get_token(infile, token);
               temp[1] = deg2rad(atof(token));
               get_token(infile, token);
               temp[2] = deg2rad(atof(token));

               if (qcount)
                  euler2quat(temp, qend[qcount-1]);
               else {
                  euler2quat(temp, qstart[0]);
                  copyarray4(qend[0], qstart[0]);
               }

               copyarray4(qcurrent, qstart[0]);
               for (j=0; j<4; j++)
                  qdelta[j]   = (qend[0][j]-qstart[0][j]) / qframesperspline;
            }

         }

         else
            if (!strcmp(token, "move")) {        // change parent of a child
               get_token(infile, token);
               get_token(infile, parent);
               move_quark(token, parent);
            }

            else if (!strcmp(token, "drop")) {     // make a child indep. object
               get_token(infile, token);
               drop_quark(frameno, token, header, infile);
            }

            else if (!strcmp(token, "absorb")) {// absorb another object
               get_token(infile, token);
               get_token(infile, parent);
               if (!strcmp(token, name))
                  join_atom(token, parent, header);
            }

            else if (!strcmp(token, "take")) {// take child from object
               get_token(infile, token);
               get_token(infile, parent);
               get_token(infile, dest);

               take_quark(dest, parent, token, *header);
            }

            else if (!strcmp(token, "scale")) { // global scale of object
               get_token(infile, token);
               temp[0] = atof(token);
               get_token(infile, token);
               scale_quark(temp[0], token);
            }

            else {                     // child event
               qtr = ob->find((quark *)NULL, dest);

               if (qtr != (quark *)NULL)
                  qtr->new_action(infile, ptr->timefactor);
               else {
                  printf("Can't find %s\n", dest);

                  get_token(infile, token);
                  lower_case(token);

                  if (!strcmp(token, "spline") || !strcmp(token, "quaternion")) {
                     get_token(infile, token);
                     get_token(infile, token);
                     get_token(infile, token);
                     get_token(infile, token);

                     for (j=0; j<tempd[0]; j++) {
                        get_token(infile, token);
                        get_token(infile, token);
                        get_token(infile, token);
                     }

                  }

               }

            }

      }

      event = event->next;
      delete ptr;
   }

}


/* ********************************************************
******************************************************** */
int atom::scale_quark(float s, char *part) {

   quark *ptr, *rtr;

   if ((ptr = ob->find(&rtr, part)) == (quark *)NULL) {
      printf("Could not scale %s\n", part);
      return 0;
   }

   ptr->iscale = s;
   return 1;
}


/* ********************************************************
   This function changes the parent of a child
******************************************************** */
int atom::move_quark(char *child, char *parent) {

   quark *ptr, *qtr;                    // temp pointers
   quark *rtr = (quark *)NULL;          // temp pointer

   if ((ptr = ob->find(&rtr, child)) == (quark *)NULL ||
       ((qtr = ptr->find(ptr, parent)) == (quark *)NULL &&
        (qtr = ob->find((quark *)NULL, parent)) == (quark *)NULL)) {

      printf("Could not move %s to %s\n", child, parent);
      return 0;
   }

   if (rtr != (quark *)NULL) {
      ptr->split(rtr);
      rtr->split(ptr);
   }

   else
      printf("WARNING: Could not find parent %s of %s\n", parent, child);

   ptr->bond(qtr);
   qtr->bond(ptr);

   return 1;
}


/* ********************************************************
   This function makes a child an independent object
******************************************************** */
int atom::drop_quark(int frameno, char *child, atom **header, FILE *infile) {

   atom      *atr;                      // new object
   quark     *qtr;                      // child to drop
   quark     *rtr = (quark *)NULL;      // parent of child
   eventtype *ptr, *ttr, *fptr;         // event pointers
   char      token[MAXSTRLEN];		// temp string
   int       errorflag;

   if ((qtr = ob->find(&rtr, child)) == (quark *)NULL) {  // find child
      get_token(infile, token);
      get_token(infile, token);
      get_token(infile, token);

      do {
         if ((errorflag = !get_token(infile, token)) || token[0] == '}')
            break;
      } while (1);

      return 0;
   }

   if (rtr != (quark *)NULL) {  // separate child from parent
     rtr->split(qtr);
     qtr->split(rtr);
   }

   else
      printf("WARNING: Could not find parent of %s\n", child);

   atr = new atom;                                      // make new object
   atr->ob = qtr;

   atr->next = *header;
   *header   = atr;

   get_token(infile, atr->name);			// read object events
   get_token(infile, token);

   do {
      if ((errorflag = !get_token(infile, token)) || token[0] == '}')
         break;

      ptr = new eventtype;

      get_token(infile, token);
      ptr->event = atoi(token) + frameno;

      get_token(infile, token);
      get_token(infile, token);
      ptr->timefactor = atof(token);

      get_token(infile, token);
      get_token(infile, ptr->efilename);

      if (atr->event == (eventtype *)NULL || ptr->event <= atr->event->event) {

         if (atr->event == (eventtype *)NULL)
            fptr = ptr;

         ptr->next  = atr->event;
         atr->event = ptr;
      }

      else
         if (fptr->event <= ptr->event) {
            ptr->next  = (eventtype *)NULL;
            fptr->next = ptr;
            fptr       = ptr;
         }

         else {
            ttr = atr->event;

            while (ttr->next->event < ttr->event)
               ttr = ttr->next;

            ptr->next = ttr->next;
            ttr->next = ptr;
         }

   } while (1);

   atr->new_action(frameno, header);           // update object

   return 1;
}


/* ********************************************************
   This function takes another object, and makes it
   a part of it.
******************************************************** */
int atom::join_atom(char *part, char *parent, atom **header) {

   atom     *atr, *btr;                 // atom pointers
   quark    *ptr;                       // quark pointers
   quark    *ntr;                       // link pointer
   char     token[MAXSTRLEN];

   strcmp(token, parent);
   lower_case(token);

   if (strcmp(token, "link"))
      if ((ptr = ob->find((quark *)NULL, parent)) == (quark *)NULL)
         return 0;

   btr = (atom *)NULL;

   if (!strcmp(part, (*header)->name)) {        // find new child
      btr = *header;
      *header = (*header)->next;
   }

   else {
      atr = *header;

      while (atr->next != (atom *)NULL) {
         btr = atr->next;
         if (!strcmp(part, btr->name)) {
            atr->next = btr->next;
            break;
         }

      }

   }

   if (btr == (atom *)NULL)
      return 0;

   if (!strcmp(token, "link")) {               // coexists w/ current children
      ntr = new quark;

      strcpy(ntr->name, "child");

      ntr->bond(btr->ob);
      btr->ob->bond(ntr);
      ntr->bond(ob);
      ob->bond(ntr);
      ob = ntr;
   }

   else {                                       // child of object's children
      ptr->bond(btr->ob);
      btr->ob->bond(ptr);
   }

   return 1;
}


/* ********************************************************
   This function takes part of another object, and makes it
   a part of itself
******************************************************** */
int atom::take_quark(char *dest, char *parent, char *part, atom *header) {

   atom  *atr;                          // target pointer
   quark *qtr, *str;                    // quark pointers
   quark *ptr = (quark *)NULL;          // quark pointer

   for (atr=header; atr!=(atom *)NULL && strcmp(parent, atr->name); atr=atr->next);

   if (atr == (atom*)NULL ||
       (qtr = atr->ob->find(&ptr, part)) == (quark *)NULL ||
       (str = ob->find((quark *)NULL, dest)) == (quark *)NULL) {

      printf("Could not take %s from %s to %s\n", part, parent, dest);
      return 0;
   }

   if (ptr != (quark *)NULL) {
      ptr->split(qtr);
      qtr->split(ptr);
   }

   else
      atr->ob = (quark *)NULL;

   str->bond(qtr);
   qtr->bond(str);

   return 1;
}


/* *************************************************************
   This function calculates the current 6 DOF for the current object
   and its children, then outputs to a file
************************************************************* */
void atom::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   vector4f r[4];                    // temp matrices

   init_mx(r);

   calc_initmx(r, xmx);

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];
   center[3] = 1;
                                                // send matrix to children
   if (ob != (quark *)NULL)
      ob->whereami(frameno, mptr, obcount, wflag, actors, (quark *)NULL, xmx);
}