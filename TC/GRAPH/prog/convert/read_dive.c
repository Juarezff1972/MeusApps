



#include <time.h>

#include "convert.h"
#include "global.h"
#include "texture.h"




int dive::read_data(char *filename) {

   FILE *infile;
   char token[MAXSTRLEN], buffer[3][MAXSTRLEN];
   int errorflag;
   time_t x;
   int i, j, k, l;
   vector4f mx[4], lx[4], xmx[4];
   vector4f point;
   objectlist *otr;

   int base_cid, curr_cid;

   base_shade *mtr;

   char texname[MAXSTRLEN];
   float texdim[2];
   texture tob;

   int nbfflag, nflag, tflag;

   point[3] = 1;

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open %s.  Aborting...\n", filename);
      exit(1);
   }

   fclose(infile);

   time(&x);
   sprintf(token, "cp %s %d.c", filename, x);
   system(token);

   while (!(infile=fopen("dive.vh", "w")));
   fprintf(infile, " \n");
   fclose(infile);

#ifdef SGI
   sprintf(token, "cc -E %d.c > %d", x, x);
#else
   sprintf(token, "cc -E %d.c -o %d", x, x);
#endif

   system(token);
   system("rm dive.vh");
   sprintf(token, "rm %d.c", x);
   system(token);

   if (!(infile=fopen(filename, "r"))) {
      printf("Cannot open preprocessed file...  Aborting...\n");
      exit(1);
   }

   printf("Reading...\n");

   vertexcount = edgecount = polycount = 0;
   curr_cid = 1;

   do {
      if ((errorflag = !get_tokena(infile, token))) 
         break;

      if (!strcmp(token, "object")) {
         get_tokena(infile, token);

         if (token[0] != '{') {
            printf("Error... '{' not after 'object'... Aborting...\n");
            errorflag = 1;
            break;
         }

         init_mx(mx);
         init_mx(lx);
         nbfflag = 0;
         base_cid = curr_cid;
         texname[0] = 0;

         while(1) {
            if ((errorflag = !get_tokena(infile, token)) || token[0] == '}') 
               break;

            switch (token[0]) {
               case '{':
                  i = 1;

                  while (i>0) {
                     if ((errorflag = !get_tokena(infile, token))) 
                        break;

                     if (token[0] == '{')
                        i++;
                     else if (token[0] == '}')
                        i--;
                  }

                  break;

               case '}':
                  break;

               case 'b':
                  if (!strcmp(token, "behavior")) {
                     get_ca(infile, '{');

                     while (1) {
                        if ((errorflag = !get_tokena(infile, token)))
                           break;

                        if (token[0] == '}')
                           break;

                        if (!strcmp(token, "arc"))
                           do {
                              if ((errorflag = !get_tokena(infile, token)))
                                 break;
                           } while (token[0] != '}');
                     }

                     break;
                  }

                  if (!strcmp(token, "begin.tcl")) {
                     do {
                        if ((errorflag = !get_tokena(infile, token)))
                           break;

                     } while (strcmp(token, "end.tcl"));

                     break;
                  }

                  break;

               case 'c':
                  if (!strcmp(token, "collision"))
                     get_tokena(infile, token);

                  break;

               case 'e':
                  if (!strcmp(token, "eulerxyz")) {
                     get_tokena(infile, token);

                     get_tokena(infile, token);
                     point[0] = atof(token);
                     get_tokena(infile, token);
                     point[1] = atof(token);
                     get_tokena(infile, token);
                     point[2] = atof(token);

                     rotate_mx_z(mx, point[2]);
                     rotate_mx_y(mx, point[1]);
                     rotate_mx_x(mx, point[0]);

                     break;
                  }

                  break;

               case 'f':
                  if (!strcmp(token, "fixedxyz")) {
                     get_tokena(infile, token);

                     get_tokena(infile, token);
                     point[0] = atof(token);
                     get_tokena(infile, token);
                     point[1] = atof(token);
                     get_tokena(infile, token);
                     point[2] = atof(token);

                     rotate_mx_x(mx, point[0]);
                     rotate_mx_y(mx, point[1]);
                     rotate_mx_z(mx, point[2]);

                     break;
                  }

                  break;

               case 'g':
                  if (!strcmp(token, "gateway")) {
                     get_ca(infile, '\"');
                     get_ca(infile, '\"');
                     get_tokena(infile, token);
                     get_tokena(infile, token);
                     get_tokena(infile, token);
                     get_tokena(infile, token);

                     break;
                  }

                  if (!strcmp(token, "gouraud")) {
                     get_tokena(infile, token);
                     break;
                  }

                  break;

               case 'i':
                  if (!strcmp(token, "id")) { 		// id
                     get_tokena(infile, token);
                     break;
                  }

                  if (!strcmp(token, "inline.tcl")) {
                     get_ca(infile, '\"');
                     get_ca(infile, '\"');
                     break;
                  }

                  break;

               case 'l':
                  if (!strcmp(token, "locallight")) {
                     get_tokena(infile, token);
                     break;
                  }

                  if (!strcmp(token, "light")) {
                     get_ca(infile, '}');
                     break;
                  }

                  break;

               case 'm':
                  if (!strcmp(token, "material")) {
                     if ((errorflag = !get_tokena(infile, token))) 
                        break;

                     if (token[0] != '{') {
                        printf("Bad material type... Aborting...\n");
                        errorflag = 1; 
                        break;
                     }

                     mtr = new base_shade;
                     mtr->next = surface_list;
                     surface_list = mtr;
                     mtr->id = curr_cid; 
                     curr_cid++;

                     do {
                        if ((errorflag = !get_tokena(infile, token))) 
                           break;

			if (!strcmp(token, "ambient")) {
                           get_tokena(infile, token);
                           mtr->ambient[0] = atof(token);
                           get_tokena(infile, token);
                           mtr->ambient[1] = atof(token);
                           get_tokena(infile, token);
                           mtr->ambient[2] = atof(token);
                        }

			else if (!strcmp(token, "diffuse")) {
                           get_tokena(infile, token);
                           mtr->diffuse[0] = atof(token);
                           get_tokena(infile, token);
                           mtr->diffuse[1] = atof(token);
                           get_tokena(infile, token);
                           mtr->diffuse[2] = atof(token);
                        }

			else if (!strcmp(token, "emission")) {
                           get_tokena(infile, token);
                           mtr->luminate[0] = atof(token);
                           get_tokena(infile, token);
                           mtr->luminate[1] = atof(token);
                           get_tokena(infile, token);
                           mtr->luminate[2] = atof(token);
                        }

                        else if (!strcmp(token, "specular")) {
                           get_tokena(infile, token);
                           mtr->specular[0] = atof(token);
                           get_tokena(infile, token);
                           mtr->specular[1] = atof(token);
                           get_tokena(infile, token);
                           mtr->specular[2] = atof(token);
                        }

                        else if (!strcmp(token, "spec_power")) {
                           get_tokena(infile, token);
                           mtr->specn = atof(token);
                        }

                        else if (!strcmp(token, "transparency")) {
                           get_tokena(infile, token);
                           mtr->trans = atof(token);
                        }

                     } while (token[0] != '}');

                     break;
                  }

                  break;

               case 'n':
                  if (!strcmp(token, "name")) {		// name
                     get_ca(infile, '\"');
                     get_ca(infile, '\"');
                     break;
                  }

                  if (!strcmp(token, "nograsp")) {
                     get_tokena(infile, token);
                     break;
                  }

                  if (!strcmp(token, "nobackface")) {
                     get_tokena(infile, token);
                     nbfflag = !strcmp(token, "on");

                     break;
                  }

                  break;

               case 'r':
                  if (!strcmp(token, "realobj")) {
                     get_tokena(infile, token);
                     break;
                  }

                  if (!strcmp(token, "rotation")) {

                     get_tokena(infile, token);

                     get_tokena(infile, token);
                     lx[0][0] = atof(token);
                     get_tokena(infile, token);
                     lx[0][1] = atof(token);
                     get_tokena(infile, token);
                     lx[0][2] = atof(token);
                     normalize3(lx[0]);

                     get_tokena(infile, token);

                     get_tokena(infile, token);
                     lx[1][0] = atof(token);
                     get_tokena(infile, token);
                     lx[1][1] = atof(token);
                     get_tokena(infile, token);
                     lx[1][2] = atof(token);
                     normalize3(lx[1]);

                     get_tokena(infile, token);

                     get_tokena(infile, token);
                     lx[2][0] = atof(token);
                     get_tokena(infile, token);
                     lx[2][1] = atof(token);
                     get_tokena(infile, token);
                     lx[2][2] = atof(token);
                     normalize3(lx[2]);

                     break;
                  }

                  break;

               case 's':
                  if (!strcmp(token, "solid")) {
                     get_tokena(infile, token);
                     break;
                  }

                  if (!strcmp(token, "set")) {
                     get_ca(infile, ')');
                     break;
                  }

                  if (!strcmp(token, "sub")) {
                     get_ca(infile, '{');
                     i = 1;
                     while (i > 0) {
                        if ((errorflag = !get_tokena(infile, token)))
                           break;

                        if (token[0] == '{')
                           i++;
                        else if (token[0] == '}')
                           i--;
                     }

                     break;
                  }

                  break;

               case 't':
                  if (!strcmp(token, "translation")) {

                     get_tokena(infile, token);

                     get_tokena(infile, token);
                     mx[0][3] += atof(token);
                     get_tokena(infile, token);
                     mx[1][3] += atof(token);
                     get_tokena(infile, token);
                     mx[2][3] += atof(token);
                     break;
                  }

                  if (!strcmp(token, "texture")) {
                     if (!get_ca(infile, '\"'))
                        break;

                     for (i=0; i<MAXSTRLEN; i++) {
                        texname[i] = (errorflag = fgetc(infile));
                        if (errorflag == EOF || texname[i] == '\"')
                           break;
                     }

                     if (i==MAXSTRLEN) {
                        printf("ERROR: Texture name exceeds %d bytes... Aborting...\n", MAXSTRLEN);
                        fclose(infile);
                        sprintf(token, "rm %d", x);
                        system(token);
                        exit(1);
                     }

                     if (errorflag == EOF) {
                        errorflag = 1;
                        break;
                     }

                     texname[i] = 0;

                     if (!tob.read_texture(texname,1)) {
                        printf("Warning: Cannot access texture %s -> therefore cannot scale properly...\n", texname);
                        texdim[0] = texdim[1] = 1.0;
                     }

                     else {
                        texdim[0] = tob.maxx - 1;
                        texdim[1] = tob.maxy - 1;
                     }

                     break;
                  }

                  break;

               case 'v':
                  if (!strcmp(token, "visibility")) {
                     get_tokena(infile, token);
                     break;
                  }

                  if (!strcmp(token, "view")) {
                     copymx4x4(xmx, lx);
                     matmatmulto(mx, xmx);

                     get_ca(infile, '{');
                     if ((errorflag = !get_tokena(infile, token)))
                        break;

                     if (!strcmp(token, "name")) {
                        get_tokena(infile, token);
                        get_tokena(infile, token);
                     }

                     if (!strcmp(token, "material_index")) {
                        get_tokena(infile, token);
                        mtr = (base_shade *)surface_list->find(atoi(token)+base_cid);
                        get_tokena(infile, token);
                     }

                     switch (token[0]) {

                        case 'B':
                           if (!strcmp(token, "BOX")) {
                              get_ca(infile, '}');
                              break;
                           }

                           if (!strcmp(token, "BOXVECTOR")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'C':
                           if (!strcmp(token, "CYLINDER")) {
                              get_ca(infile, '}');
                              break;
                           }

                           if (!strcmp(token, "CTEXT")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'E':
                           if (!strcmp(token, "ELLIPSE")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'L':
                           if (!strcmp(token, "LINE")) {
                              get_ca(infile, '}');
                              break;
                           }

                           if (!strcmp(token, "LTEXT")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'N':
                           if (!strcmp(token, "N_POLY")) {
                              get_ca(infile, '}');
                              break;
                           }

                           if (!strcmp(token, "N_LINE")) {
                              get_ca(infile, '}');
                              break;
                           }

                           if (!strcmp(token, "N_M_POLY")) {
                              nflag = tflag = 0;

                              if ((errorflag = !get_tokena(infile, token)))
                                 break;

                              l = atoi(token);

                              if ((errorflag = !get_tokena(infile, token)))
                                 break;

                              if ((errorflag = !get_tokena(infile, token)))
                                 break;

                              replace(token, '(', ' ');
                              replace(token, ')', ' ');
                              replace(token, '+', ' ');

                              j = sscanf(token, "%s %s %s", buffer[0], buffer[1], buffer[2]);
                              for (i=0; i<j; i++)
                                 if (!strcmp(buffer[i], "N_PER_VERTEX"))
                                    nflag = 1;
                                 else if (!strcmp(buffer[i], "T_PER_VERTEX"))
                                    tflag = 1;
//                                 else if (!strcmp(buffer[i], "C_PER_VERTEX")) ||
//					!strcmp(buffer[i], "C_PER_PRIM"))
//                                    cflag = 1;

                              for (i=0; i<l; i++) {
                                 if ((errorflag = !get_tokena(infile, token))) {
                                    printf("Error... Invalid Polygon format... Aborting...\n");
                                    fclose(infile);
                                    sprintf(token, "rm %d", x);
                                    system(token);
                                    exit(1);
                                 }

                                 get_tokena(infile, token);
                                 j = atoi(token);

                                 otr = new objectlist;
                                 polycount++;
                                 otr->id = polycount;
                                 otr->rnflag = reverse_normal;
                                 otr->vptr = new vertexdata[j];

                                 otr->lw = mtr;
                                 otr->lw->count++;

                                 otr->polynum = j;
                                 otr->flag_dbl_side = nbfflag;

                                 edgecount += j;

                                 if (tflag)
                                    strcpy(otr->texname, texname);

                                 if (ob == (objectlist *)NULL)
                                    ob = otr;
                                 else
                                    ob->insert_id(otr);

                                 for (k=j-1; k>-1; k--) {
                                    get_tokena(infile, token);
                                    get_tokena(infile, token);
                                    point[0] = atof(token);
                                    get_tokena(infile, token);
                                    point[1] = atof(token);
                                    get_tokena(infile, token);
                                    point[2] = atof(token);

                                    matvecmulto(xmx, point);

                                    if (head == (vertexlist *)NULL) {
                                       head = new vertexlist;
                                       copyarray3(head->pt, point);
                                       head->id = vertexcount = ob->vptr[k].index = 1;
                                    }

                                    else
                                       otr->vptr[k].index = head->insert(point, &vertexcount);

                                    if (nflag) {
                                       get_tokena(infile, token);
                                       get_tokena(infile, token);
                                       get_tokena(infile, token);
                                       get_tokena(infile, token);
                                    }                                     

                                    if (tflag) {
                                       get_tokena(infile, token);
                                       get_tokena(infile, token);
                                       otr->vptr[k].t[0] = atof(token)*texdim[0];
                                       get_tokena(infile, token);
                                       otr->vptr[k].t[1] = atof(token)*texdim[1];
                                    }                                     

                                 }

                              }
                                 
                              get_ca(infile, '}');

                              break;
                           }

                           break;

                        case 'Q':
                           if (!strcmp(token, "QUAD_GRID")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'R':
                           if (!strcmp(token, "RBOX")) {
                              get_ca(infile, '}');
                              break;
                           }

                           if (!strcmp(token, "RTEXT")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'S':
                           if (!strcmp(token, "SPHERE")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        case 'T':
                           if (!strcmp(token, "TEXT")) {
                              get_ca(infile, '}');
                              break;
                           }

                           break;

                        default:
                           break;
                     }

                     break;
                  }

                  break;

               case 'w':
                  if (!strcmp(token, "wireframe"))
                     get_tokena(infile, token);

                  break;

               default:
                  break;
            }

         }

      }

   } while (1);


   fclose(infile);
   sprintf(token, "rm %d", x);
   system(token);
   return 1;
}


