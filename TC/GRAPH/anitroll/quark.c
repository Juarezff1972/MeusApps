


#include <stdlib.h>
#include "quark.h"
#include "base.h"

/* *************************************************************
   This is the constructor for this class
************************************************************* */
quark::quark() {

   qauto = qcount = qstatus = 0;
   qcurrent[0] = qcurrent[1] = qcurrent[2] = qcurrent[3] = 0;
   qdelta[0] = qdelta[1] = qdelta[2] = qdelta[3] = 0;
   qframesperspline = 1;
   qkey = 0;
   qframe = 0;
   qstart = qend = (vector4f *)NULL;

   kauto = kcount = kstatus = 0;
   kcurrent[0] = kcurrent[1] = kcurrent[2] = 0;
   kcurrent[3] = 1;
   kdelta[0] = kdelta[1] = kdelta[2] = kdelta[3] = 0;
   kframesperspline = 1;
   kkey = 0;
   kframe = 0;
   kknot = (vector4f *)NULL;

   firstflag = 1;

   init_mx(initxform);
   init_mx(localmx);
   origin[0] = origin[1] = origin[2] = 0;
   strcpy(obtype, "WFBW");
   colorname[0] = 0;
   texname[0] = 0;
   mcinfo = CINULL;
   shadow = 0;
   iscale = scale = 1.0;
   bflag = BNONE;
   active = 1;

   edge = (linktype *)NULL;
}


/* *************************************************************
   This is the destructor for this class
************************************************************* */
quark::~quark() {

   linktype *ptr;

   if (qstart != (vector4f *)NULL)
      delete qstart;

   if (qend != (vector4f *)NULL)
      delete qend;

   if (kknot != (vector4f *)NULL)
      delete kknot;

   for (ptr=edge; edge!=(linktype *)NULL; ptr=edge) {
      ptr->link->remove_link(this);
      edge=edge->next;
      delete ptr->link;
      delete ptr;
   }

}


int quark::parse(FILE *infile, char *token) {

   vector4f tmx[4];
   char     flag;
   int      count;
   quark    *ptr;
   linktype *qtr;

   switch (token[0]) {

      case '2':
         if (!strcmp(token, "2d_texture")) {
            get_token(infile, texname);
            mcinfo |= CITEXTURE;
            return 1;
         }

         return 0;

      case '3':
         if (!strcmp(token, "3d_texture")) {
            get_token(infile, texname);
            mcinfo |= CI3DTEX;
            return 1;
         }

         return 0;

      case 'b':
         if (!strcmp(token, "biplane")) {
            mcinfo |= CIBITMAP;
            return 1;
         }

         if (!strcmp(token, "bound")) {
            read_bound(infile);
            return 1;
         }

         return 0;

      case 'c':
         if (!strcmp(token, "children")) {
            get_token(infile, token);

            for (count = atoi(token); count > 0; count--) {	// read in quark's children
               ptr = read_quark(infile, this);

               qtr = new linktype;
               qtr->link = ptr;
               qtr->next = edge;
               edge = qtr;
            }

            return 1;
         }

         return 0;

      case 'l':
         if (!strcmp(token, "luminate")) {
//            mcinfo |= CILUM;		// obsolete
            return 1;
         }

         return 0;

      case 'n':
         if (!strcmp(token, "name")) {
            get_token(infile, name);
            return 1;
         }

         return 0;

      case 'r':

         if (!strcmp(token, "raycast")) {
            mcinfo |= CIRAYCAST;
            return 1;
         }

         if (strlen(token) < 8)
            return 0;

         if (token[7] == 'l') {

            if (strlen(token) < 14)
               return 0;

            switch(token[13]) {

               case 'x':
                  if (!strcmp(token, "rotate_local_x")) {
                     get_token(infile, token);
                     rotate_mx_x(localmx, deg2rad(atof(token)));
                     return 1;
                  }

                  return 0;

               case 'y':
                  if (!strcmp(token, "rotate_local_y")) {
                     get_token(infile, token);
                     rotate_mx_y(localmx, deg2rad(atof(token)));
                     return 1;
                  }

                  return 0;

               case 'z':
                  if (!strcmp(token, "rotate_local_z")) {
                     get_token(infile, token);
                     rotate_mx_z(localmx, deg2rad(atof(token)));
                     return 1;
                  }

                  return 0;

               default:
                  break;
            }

            return 0;
         }

         switch(token[7]) {

            case 'x':
               if (!strcmp(token, "rotate_x")) {
                  get_token(infile, token);
                  rotate_mx_x(initxform, deg2rad(atof(token)));
                  return 1;
               }

               return 0;

            case 'y':
               if (!strcmp(token, "rotate_y")) {
                  get_token(infile, token);
                  rotate_mx_y(initxform, deg2rad(atof(token)));
                  return 1;
               }

               return 0;

            case 'z':
               if (!strcmp(token, "rotate_z")) {
                  get_token(infile, token);
                  rotate_mx_z(initxform, deg2rad(atof(token)));
                  return 1;
               }

               return 0;

            default:
               break;
         }

         return 0;

      case 'o':
         if (!strcmp(token, "origin")) {
            get_token(infile, token);
            origin[0] = atof(token);
            get_token(infile, token);
            origin[1] = atof(token);
            get_token(infile, token);
            origin[2] = atof(token);
            return 1;
         }

         return 0;

      case 's':
         if (!strcmp(token, "scale")) {
            get_token(infile, token);
            scale = atof(token);
            return 1;
         }

         if (!strcmp(token, "shadow")) {
            shadow = 1;
            return 1;
         }

         if (!strcmp(token, "shade")) {
            get_token(infile, token);
            lower_case(token);

            switch (token[0]) {
               case 'a':
                  if (!strcmp(token, "alex")) {
                     strcpy(obtype, "ALEX");
                     get_token(infile, colorname);
                     return 1;
                  }

                  return 0;

               case 'b':
                  if (!strcmp(token, "bw")) {
                     strcpy(obtype, "BW");
                     return 1;
                  }

                  return 0;

               case 'c':
                  if (!strcmp(token, "constant")) {
                     strcpy(obtype, "CONSTANT");
                     get_token(infile, colorname);
                     return 1;
                  }

                  return 0;

               case 'd':
                  if (!strcmp(token, "dot")) {
                     strcpy(obtype, "DOT");
                     return 1;
                  }

                  return 0;

               case 'g':			// line/particle called before this
                  if (!strcmp(token, "gouraud")) {
                     strcpy(obtype, "GOURAUD");
                     get_token(infile, colorname);
                     return 1;
                  }

                  return 0;

               case 'i':
                  if (!strcmp(token, "irradiant")) {
                     strcpy(obtype, "IRRADIANT");
                     get_token(infile, colorname);
                     return 1;
                  }

                  return 0;

               case 'p':
                  if (!strcmp(token, "phong")) {
                     strcpy(obtype, "PHONG");
                     get_token(infile, colorname);
                     return 1;
                  }

                  if (strlen(token) < 2)
                     return 0;

                  switch (token[1]) {

                     case 'a':
                        if (!strcmp(token, "palex")) {
                           strcpy(obtype, "PALEX");
                           get_token(infile, colorname);
                           return 1;
                        }

                        return 0;

                     case 'b':
                        if (!strcmp(token, "pbw")) {
                           strcpy(obtype, "PBW");
                           return 1;
                        }

                        return 0;

                     case 'c':
                        if (!strcmp(token, "pconstant")) {
                           strcpy(obtype, "PCONSTANT");
                           get_token(infile, colorname);
                           return 1;
                        }

                        return 0;

                     case 'd':
                        if (!strcmp(token, "pdot")) {
                           strcpy(obtype, "PDOT");
                           return 1;
                        }

                        return 0;

                     case 'g':
                        if (!strcmp(token, "pgouraud")) {
                           strcpy(obtype, "PGOURAUD");
                           get_token(infile, colorname);
                           return 1;
                        }

                        return 0;

                     case 'p':
                        if (!strcmp(token, "pphong")) {
                           strcpy(obtype, "PPHONG");
                           get_token(infile, colorname);
                           return 1;
                        }

                        return 0;

                     case 'w':
                        if (!strcmp(token, "pwfbw")) {
                           strcpy(obtype, "PWFBW");
                           return 1;
                        }

                        return 0;

                     default:
                        break;
                  }

                  return 0;

               case 'r':
                  if (!strcmp(token, "rsolid")) {
                     strcpy(obtype, "RSOLID");
                     get_token(infile, colorname);
                     return 1;
                  }

                  return 0;

               case 'w':
                  if (!strcmp(token, "wfbw")) {
                     strcpy(obtype, "WFBW");
                     return 1;
                  }

                  return 0;

               default:
                  break;
            }

            return 0;
         }

         return 0;

      case 't':
         if (strlen(token) < 6)
            return 0;

         if ((flag = !strcmp(token, "tr_matrix")) || !strcmp(token, "tr_local_matrix")) {
            get_token(infile, token);
            tmx[0][0] = atof(token);
            get_token(infile, token);
            tmx[0][1] = atof(token);
            get_token(infile, token);
            tmx[0][2] = atof(token);
            get_token(infile, token);
            tmx[0][3] = atof(token);

            get_token(infile, token);
            tmx[1][0] = atof(token);
            get_token(infile, token);
            tmx[1][1] = atof(token);
            get_token(infile, token);
            tmx[1][2] = atof(token);
            get_token(infile, token);
            tmx[1][3] = atof(token);

            get_token(infile, token);
            tmx[2][0] = atof(token);
            get_token(infile, token);
            tmx[2][1] = atof(token);
            get_token(infile, token);
            tmx[2][2] = atof(token);
            get_token(infile, token);
            tmx[2][3] = atof(token);

            get_token(infile, token);
            tmx[3][0] = atof(token);
            get_token(infile, token);
            tmx[3][1] = atof(token);
            get_token(infile, token);
            tmx[3][2] = atof(token);
            get_token(infile, token);
            tmx[3][3] = atof(token);

            if (flag)
               matmatmulto(tmx, initxform);
            else
               matmatmulto(tmx, localmx);

            return 1;
         }

         switch (token[5]) {

            case 'l':
               if (!strcmp(token, "translate")) {
                  get_token(infile, token);
                  initxform[0][3] += atof(token);
                  get_token(infile, token);
                  initxform[1][3] += atof(token);
                  get_token(infile, token);
                  initxform[2][3] += atof(token);

                  return 1;
               }

               if (!strcmp(token, "translate_local")) {
                  get_token(infile, token);
                  localmx[0][3] += atof(token);
                  get_token(infile, token);
                  localmx[1][3] += atof(token);
                  get_token(infile, token);
                  localmx[2][3] += atof(token);

                  return 1;
               }

               return 0;

            case 'p':
               if (!strcmp(token, "transparent")) {
                  mcinfo |= CITRANSPARENT;
                  return 1;
               }

               return 0;

            default:
               break;
         }

         return 0;

      default:
         break;
   }

   return 0;
}


/* *************************************************************
************************************************************* */
void quark::preprocess() {

   int i;

   i = strlen(obtype);

   if (mcinfo & CITEXTURE) {
      obtype[i] = '2';
      i++;
   }

   if (mcinfo & CIRAYCAST) {
      obtype[i] = 'R';
      i++;
   }

   if (mcinfo & CI3DTEX) {
      obtype[i] = '3';
      i++;
   }

   if (mcinfo & CITRANSPARENT) {
      obtype[i] = 'T';
      i++;
   }

   if (mcinfo & CIBITMAP) {
      obtype[i] = 'B';
      i++;
   }

   obtype[i] = 0;
}


/* *************************************************************
   Read animation data for hiearchical parts
************************************************************* */
quark *quark::read_quark(FILE *infile, quark *parent) {

   quark      *header;                    // object pointers
   char       token[MAXSTRLEN];                 // quark type
   char       errorflag;

   get_token(infile, token);
   lower_case(token);

   header = (quark *)NULL;

   switch (token[0]) {

      case 'b':
         if (!strcmp(token, "buckey")) {	// sphere
            header = new buckey;
            break;
         }

         if (!strcmp(token, "boid")) {		// boid
            header = new boid;
            break;
         }

         if (!strcmp(token, "boidbait")) {	// boidbait (leader)
            header = new boidbait;
            break;
         }

         break;

      case 'c':
         if (!strcmp(token, "camera")) {		// camera
            header = new electron;
            break;
         }

         if (!strcmp(token, "chain")) {		// chain
            header = new chain;
            break;
         }

         break;

      case 'e':
         if (!strcmp(token, "electron")) {
            header = new electron;
            break;
         }

         break;

      case 'f':
         if (!strcmp(token, "flight")) {
            header = new flight;
            break;
         }

         if (!strcmp(token, "funnel")) {
            header = new funnel;
            break;
         }

         break;

      case 'g':
         if (!strcmp(token, "geosphere")) {
            header = new buckey;
            break;
         }

         if (!strcmp(token, "geocone")) {
            header = new funnel;
            break;
         }

         if (!strcmp(token, "geocylinder")) {
            header = new tube;
            break;
         }

         break;

      case 'h':
         if (!strcmp(token, "helix")) {
            header = new helix;
            break;
         }

         break;

      case 'i':					// radiant
         if (!strcmp(token, "ion")) {
            header = new ion;
            break;
         }

         break;

      case 'l':					// light
         if (!strcmp(token, "light")) {
            header = new photon;
            break;
         }

         if (!strcmp(token, "line")) {
            header = new chain;
            break;
         }

         if (!strcmp(token, "lock")) {
            header = new lock;
            break;
         }

         break;

      case 'n':					// neutron
         if (!strcmp(token, "neutron")) {
            header = new neutron;
            break;
         }

         break;

      case 'p':
         if (!strcmp(token, "particle")) {
            header = new tachyon;
            break;
         }

         if (!strcmp(token, "photon")) {
            header = new photon;
            break;
         }

         if (!strcmp(token, "polygon")) {
            header = new neutron;
            break;
         }

         break;

      case 'q':
         if (!strcmp(token, "quark")) {
            header = new quark;
            break;
         }

         break;

      case 'r':
         if (!strcmp(token, "radiant")) {
            header = new ion;
            break;
         }

         break;

      case 's':
         if (!strcmp(token, "sbfast")) {
            header = new helix;
            break;
         }

         if (!strcmp(token, "spatch")) {
            header = new shell;
            break;
         }

         if (!strcmp(token, "shell")) {
            header = new shell;
            break;
         }

         break;

      case 't':					// particle
         if (!strcmp(token, "tachyon")) {
            header = new tachyon;
            break;
         }

         if (!strcmp(token, "tube")) {
            header = new tube;
            break;
         }

         break;

      default:
         break;
   }

   if (header == (quark *)NULL) {
      printf("ERROR: Invalid object type \"%s\"... Aborting...\n", token);
      fclose(infile);
      exit(1);
   }

   if (parent == (quark *)NULL)                 // insert quark in hiearchy
      header->edge = (linktype *)NULL;
   else {
      header->edge       = new linktype;
      header->edge->link = parent;
      header->edge->next = (linktype *)NULL;
   }

   get_token(infile, token);

   do {
      if ((errorflag = !get_token(infile, token)) || token[0] == '}')
         break;

      lower_case(token);

      if (!header->parse(infile, token)) {
         printf("ERROR: Invalid data format... Last read token %s... Aborting...\n", token);
         fclose(infile);
         exit(1);
      }

   } while (1);

   if (errorflag) {
      printf("ERROR: EOF reached before token \"}\"... Aborting...\n");
      fclose(infile);
      exit(1);
   }

   header->preprocess();

   return header;
}

/* *************************************************************
	1) origin   (translate)
	2) quat     (rotate around origin)
	3) init_mx  (translate/rotate)
        4) scale    (scale all)
	5) spline   (translate)
        6) xmx      (parent transform/rotate)
************************************************************* */
void quark::calc_initmx(vector4f *in, vector4f *out) {

   vector4f r[4];

   init_mx(out);

   if (whatami() != ATOM) {
      out[0][3] += origin[0];
      out[1][3] += origin[1];
      out[2][3] += origin[2];
   }

   if (qstatus)                                 // calc rotation
      preparequat(r);
   else
      calc_localquat(r);

   matmatmulto(r, out);

   matmatmulto(initxform, out);                  // combine w/ global xform

   init_mx(r);					 // scale global
   r[0][0] = r[1][1] = r[2][2] = iscale;

   matmatmulto(r, out);

   if (kstatus)                                 // calc spline
      preparesp(r);
   else
      calc_localsp(r);

   matmatmulto(r, out);                  // combine w/ global xform

   matmatmulto(in, out);
}


/* *************************************************************
************************************************************* */
int quark::remove_link(quark *x) {

   linktype *ptr, *qtr;

   if (edge == (linktype *)NULL)
      return 0;

   if (edge->link == x) {
      ptr = edge;
      edge = edge->next;
      delete ptr;
      return 1;
   }

   for (qtr=edge, ptr=edge->next; ptr!=(linktype *)NULL; qtr=ptr, ptr=ptr->next)
      if (ptr->link == x) {
         qtr->next = ptr->next;
         delete ptr;
         return 1;
      }

   return 0;
}


/* *************************************************************
   This function finds a specific child in the hiearchy

   returns child
************************************************************* */
quark *quark::find(quark *parent, char *part) {

   linktype *ptr;
   quark    *qtr;

   if (!strcmp(name, part))
      return this;

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)
      if (parent != ptr->link) {
         qtr = ptr->link->find(this, part);
         if (qtr != (quark *)NULL)
            return qtr;
      }

   return (quark *)NULL;
}


/* *************************************************************
   This function finds a specific child in the hiearchy
   NOTE: child must have a parent...

   returns parent and child
************************************************************* */
quark *quark::find(quark **parent, char *part) {

   linktype *ptr;
   quark    *qtr;
   quark    *ttr;

   if (!strcmp(name, part))
      return this;

   ttr = *parent;

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)
      if (ttr != ptr->link) {
         *parent = this;
         qtr = ptr->link->find(parent, part);
         if (qtr != (quark *)NULL)
            return qtr;
      }

   *parent = (quark *)NULL;
   return (quark *)NULL;
}


/* *************************************************************
   This function removes a child from the hiearchy
   and deletes the child
************************************************************* */
int quark::split(quark *ob) {

   linktype *ptr, *qtr;

   if (ob == (quark *)NULL)
      return -1;

   qtr = edge;

   if (edge->link == ob) {
      edge = edge->next;
      delete qtr;
      return 1;
   }

   while (qtr->next != (linktype *)NULL) {
      ptr = qtr->next;
      if (ptr->link == ob) {
         qtr->next = ptr->next;
         delete ptr;
         return 1;
      }

      qtr = ptr;
   }

   return 0;
}


/* *************************************************************
   this function connects a child to the hiearchy
************************************************************* */
void quark::bond(quark *ob) {

   linktype *ptr;

   ptr = new linktype;
   ptr->link = ob;
   ptr->next = edge;
   edge = ptr;
}


/* ********************************************************
   This procedure creates a combination xform mx by combining
   the parent mx, with the local mx, which is calculated w/ the
   following order :

  1. rotation
  2. move to local coord
  3. attach to parent
  4. concatinate parent xform - done by caller

  **** assume framesperspline >= 1
******************************************************** */
void quark::calc_localquat(float  mx[][4]) {

   int    i, j;                         // looping var
   float  r[4][4];                      // temp mx
   float  temp[4];

   calc_quat(r, qcurrent);              // calc rotation

   init_mx(mx);

   for (i=0; i<3; i++)                  // move joint to local origin
      mx[i][3] -= origin[i];

   matmatmulto(r, mx);

   for (i=0; i<3; i++)
      mx[i][3] += origin[i];

   qframe++;                             // calc next parameters

   if (qframe < qframesperspline) {
      addarray4(qcurrent, qdelta);     // increment rotation
   }

   else
      if (qkey < qcount-1) {
         qframe -= qframesperspline;
         qkey++;

         for (j=0; j<4; j++)            // calc initial cond.
            qdelta[j]   = (qend[qkey][j]-qstart[qkey][j]) / qframesperspline;

         copyarray4(qcurrent, qstart[qkey]);
         copyarray4(temp, qdelta);
         smultarray4(temp, qframe);
         addarray4(qcurrent, temp);
      }

      else
         if (qauto) {
            qframe -= qframesperspline;
            qkey = 0;

            for (j=0; j<4; j++)            // calc initial cond.
               qdelta[j]   = (qend[0][j]-qstart[0][j]) / qframesperspline;

            copyarray4(qcurrent, qstart[0]);
            copyarray4(temp, qdelta);
            smultarray4(temp, qframe);
            addarray4(qcurrent, temp);
         }

         else
            qframe = qframesperspline;
}


/* ********************************************************
   This procedure creates a combination xform mx by combining
   the parent mx, with the local mx, which is calculated w/ the
   following order :

  1. rotation
  2. move to local coord
  3. attach to parent
  4. concatinate parent xform - done by caller

   this is used when between events
******************************************************** */
void quark::preparequat(float  mx[][4]) {

   int    i;                            // looping variable
   float  r[4][4];                      // temp mx
   float  qquat[4];                     // temp quaternion
   float  temp[4];

   for (i=0; i<4; i++)                  // calc rotation
      qquat[i] = (qcounter*qcurrent[i] + (qcatchup-qcounter)*qold[i])/qcatchup;

   normalize4(qquat);

   calc_quat(r, qquat);

   init_mx(mx);

   for (i=0; i<3; i++)                  // move joint to local origin
      mx[i][3] -= origin[i];

   matmatmulto(r, mx);

   for (i=0; i<3; i++)                  // move joint back to start pos
      mx[i][3] += origin[i];

   qcounter++;

   if (qcounter >= qcatchup) {          // calc next rotation
      qstatus = 0;
      qframe  = qcounter-qcatchup;

      copyarray4(temp, qdelta);
      smultarray4(temp, qframe);
      addarray4(qcurrent, temp);
   }

}


/* ********************************************************
   This function calculates the current xyz DOF

  **** assume framesperspline >= 1
******************************************************** */
void quark::calc_localsp(float  mx[][4]) {

   int i, j;                            // looping var

   init_mx(mx);

   for (i=0; i<3; i++)                  // move to world origin
      mx[i][3] = kcurrent[i];

   if (kknot != (vector4f *)NULL) {
      kframe++;

      if (kframe >= kframesperspline)      // calc next parameters
         if (kkey < kcount-4) {
            kframe -= kframesperspline;
            kkey++;

            for (i=0; i<4; i++)
               for (j=0; j<4; j++)
                  Q[i][j] = kknot[kkey+j][i];
         }

         else if (kauto) {
            kframe -= kframesperspline;
            kkey    = 0;

            for (i=0; i<4; i++)
               for (j=0; j<4; j++)
                  Q[i][j] = kknot[j][i];

/*
            for (i=0; i<4; i++)
               for (j=0; j<4; j++)
                  Q[i][j] = kknot[j][i];
*/
         }

         else
            kframe = kframesperspline;

      spline(kframe/kframesperspline, Q, kcurrent, M_CR);
   }

}


/* ********************************************************

   This function calculates the current xyz DOF between
   events
******************************************************** */
void quark::preparesp(float  mx[][4]) {

   int    i, j;                                 // looping var

   init_mx(mx);

   for (i=0; i<3; i++)
      mx[i][3] = kold[i] + (kcounter/kcatchup)*(kcurrent[i] - kold[i]);

   kcounter++;

   if (kcounter >= kcatchup) {
      kstatus = 0;
      kframe  = kcounter-kcatchup;

      for (i=0; i<4; i++)
         for (j=0; j<4; j++)
            Q[i][j] = kknot[j][i];

      spline(kframe/kframesperspline, Q, kcurrent, M_CR);
   }

}


/* ********************************************************
******************************************************** */
int quark::read_bound(FILE *infile) {

   char token[MAXSTRLEN];
   int i;

   get_token(infile, token);
   lower_case(token);

   if (!strcmp(token, "brect")) {
      bflag = BRECT;

      for (i=0; i<8; i++) {
         get_token(infile, token);
         bdata[i][0] = atof(token);
         get_token(infile, token);
         bdata[i][1] = atof(token);
         get_token(infile, token);
         bdata[i][2] = atof(token);
         bdata[i][3] = 1;
      }

      return 1;
   }

   if (!strcmp(token, "bsphere")) {
      bflag = BSPHERE;
      get_token(infile, token);
      bsphere = atof(token);
      return 1;
   }

   if (!strcmp(token, "bplane")) {
      bflag = BPLANE;
      get_token(infile, token);
      bdata[0][0] = atof(token);
      get_token(infile, token);
      bdata[0][1] = atof(token);
      get_token(infile, token);
      bdata[0][2] = atof(token);
      normalize3(bdata[0]);

      get_token(infile, token);
      bdata[1][0] = atof(token);
      get_token(infile, token);
      bdata[1][1] = atof(token);
      get_token(infile, token);
      bdata[1][2] = atof(token);
      bdata[1][3] = 1;

      return 1;
   }

   if (!strcmp(token, "bpoint")) {
      bflag = BPOINT;
      return 1;
   }

   if (!strcmp(token, "bnone")) {
      bflag = BNONE;
      return 1;
   }

   return 0;
}


/* ********************************************************
   update data with new event
******************************************************** */
void quark::new_action(FILE *infile, float  timefactor, char *buffer) {

   int    i, j;                                 // looping var
   float  temp[3];                              // temp array
   char   token[MAXSTRLEN];                      // type of event

   if (buffer != (char *)NULL)
      strcpy(token, buffer);
   else {
      get_token(infile, token);
      lower_case(token);
   }

   if (!strcmp(token, "off")) {
      active = 0;
      return;
   }

   if (!strcmp(token, "on")) {
      active = 1;
      return;
   }

   if (!strcmp(token, "spline")) {               // spline event
      get_token(infile, token);
      kcount = atoi(token);
      get_token(infile, token);
      kcatchup = atof(token);
      get_token(infile, token);
      kframesperspline = atof(token);
      get_token(infile, token);
      kauto = atoi(token);

      kcatchup *= timefactor;
      kframesperspline *= timefactor;

      if (not_zero(kcatchup)) {
         kstatus = 1;
         kcounter = 0;
         copyarray3(kold, kcurrent);
      }

      else
         kstatus = 0;

      kkey   = 0;
      kframe = 0;

      if (kknot != (vector4f *)NULL)
         delete kknot;

      kknot = new vector4f[kcount];

      for (i=0; i<kcount; i++) {
         get_token(infile, token);
         kknot[i][0] = atof(token);
         get_token(infile, token);
         kknot[i][1] = atof(token);
         get_token(infile, token);
         kknot[i][2] = atof(token);
      }

      copyarray3(kcurrent, kknot[1]);

      if (!kstatus) 
         for (i=0; i<4; i++)
            for (j=0; j<4; j++)
               Q[i][j] = kknot[j][i];

      return;
   }

   get_token(infile, token);
   qcount = atoi(token);
   get_token(infile, token);
   qcatchup = atof(token);
   get_token(infile, token);
   qframesperspline = atof(token);
   get_token(infile, token);
   qauto = atoi(token);

   qcatchup *= timefactor;
   qframesperspline *= timefactor;

   if (not_zero(qcatchup)) {
      qstatus = 1;
      qcounter = 0;
      copyarray4(qold, qcurrent);
   }

   else
      qstatus = 0;

   qkey   = 0;
   qframe = 0;

   qcount--;

   if (qstart != (vector4f *)NULL)
      delete qstart;

   qstart = new vector4f[qcount];

   if (qend != (vector4f *)NULL)
      delete qend;

   qend = new vector4f[qcount];

   for (i=0; i<qcount; i++) {
      get_token(infile, token);
      temp[0] = deg2rad(atof(token));
      get_token(infile, token);
      temp[1] = deg2rad(atof(token));
      get_token(infile, token);
      temp[2] = deg2rad(atof(token));

       euler2quat(temp, qstart[i]);

      if (i)
         copyarray4(qend[i-1], qstart[i]);
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
   for (i=0; i<4; i++)
      qdelta[i]   = (qend[0][i]-qstart[0][i]) / qframesperspline;

}


/* *************************************************************
   place object based on local and parent xforms
************************************************************* */
void quark::whereami(int frameno, molecule **mptr, int *obcount, int wflag,
			quark *actors, quark *parent, vector4f *mx) {

   vector4f r[4];			// local xform mx
   linktype *ptr;				// pointer

   calc_initmx(mx, r);

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)  // apply xform to
      if (ptr->link != parent)                           //    children
         ptr->link->whereami(frameno, mptr, obcount, wflag, actors, this, r);

   copymx4x4(xmx, localmx)
   matmatmulto(r, xmx);

   center[0] = xmx[0][3];
   center[1] = xmx[1][3];
   center[2] = xmx[2][3];
}


void quark::update(quark *parent) {

   linktype *ptr;
   float v1[3], v2[3];
   int i;
   firstflag = 0;
   copyarray4(oldcenter, center);
   copymx4x4(old_xmx, xmx);

   switch (bflag) {

      case BRECT:
         for (i=0; i<8; i++) {
            copyarray4(bold_data[i], bdata[i]);
            matvecmulto(xmx, bold_data[i]);
         }

         copyarray3(v1, bold_data[1]); // 0 1 2 3
         subarray3(v1, bold_data[0]);
         copyarray3(v2, bold_data[2]);
         subarray3(v2, bold_data[1]);
         xproduct(bold_normal[0], v2, v1);
         normalize3(bold_normal[0]);
         bold_normal[0][3] = -dotproduct3(bold_normal[0], bold_data[0]);

         bold_normal[1][0] = -bold_normal[0][0];
         bold_normal[1][1] = -bold_normal[0][1];
         bold_normal[1][2] = -bold_normal[0][2];
         bold_normal[1][3] = -dotproduct3(bold_normal[1], bold_data[4]);

         copyarray3(v1, bold_data[5]);
         subarray3(v1, bold_data[4]);
         copyarray3(v2, bold_data[1]);
         subarray3(v2, bold_data[5]);
         xproduct(bold_normal[2], v2, v1);
         normalize3(bold_normal[2]);
         bold_normal[2][3] = -dotproduct3(bold_normal[2], bold_data[0]);

         bold_normal[4][0] = -bold_normal[2][0];
         bold_normal[4][1] = -bold_normal[2][1];
         bold_normal[4][2] = -bold_normal[2][2];
         bold_normal[4][3] = -dotproduct3(bold_normal[4], bold_data[2]);

         copyarray3(v1, bold_data[5]);
         subarray3(v1, bold_data[1]);
         copyarray3(v2, bold_data[6]);
         subarray3(v2, bold_data[5]);
         xproduct(bold_normal[3], v2, v1);
         normalize3(bold_normal[3]);
         bold_normal[3][3] = -dotproduct3(bold_normal[3], bold_data[1]);

         bold_normal[5][0] = -bold_normal[3][0];
         bold_normal[5][1] = -bold_normal[3][1];
         bold_normal[5][2] = -bold_normal[3][2];
         bold_normal[5][3] = -dotproduct3(bold_normal[5], bold_data[0]);

         break;

      case BPLANE:
         copyarray3(bold_normal[0], bdata[0]);
         matvecmultv(xmx, bold_normal[0]);
         normalize3(bold_normal[0]);
         bold_normal[0][3] = -dotproduct3(bold_normal[0], bold_data[0]);

         copyarray3(bold_data[0], bdata[1]);
         bold_data[0][3] = 1;
         matvecmulto(xmx, bold_data[0]);

         break;

      default:
         break;
   }

   for (ptr=edge; ptr!=(linktype *)NULL; ptr=ptr->next)
      if (ptr->link != parent)
         ptr->link->update(this);
}


/* *************************************************************
************************************************************* */
int quark::query_texture(char *instring) {

   int  i, j;

   for (i=0, j=strlen(instring); i<j; i++)
      switch (instring[i]) {
         case '2':
            return 1;

         case '3':
            return 1;

         case 'V':
            return 1;

         default:
            break;
      }

   return 0;
}
