      {
         if (INTERLACE && (i & 0x01))
            continue;

#ifdef DOS
         index = toprow - i*zbuff->maxx + j;
#else
         index = i*zbuff->maxx + j;
#endif

         if (ditherflag && i && j && i<WINY-1 && j<WINX-1 && ((i+j) & 0x01))
            sbbuffer->data[index] = 4;  // miss
         else {
            sbbuffer->data[index] = 2;  // miss

            pray[0] = j;
            pray[1] = i;
            pray[2] = -1;

            parm->screen2map(pray);
            rcolor[0] = pray[0];
            rcolor[1] = pray[1];

            matvecmultv(iworld, pray);

            if ((k=Intersect_Terrain(increment, pzero, pray, surface, normal)) == -1)
               break;

            if (k) { // hit
               if (flag2t)
                  copyarray2(acolor, surface);

                 surface[2] = dotproduct4(world[2], surface);

               if (surface[2] > zbuff->zbuff[index].zdata) {
                  sbbuffer->data[index] = 1;    // hit

                  surface[0] = -surface[2]*rcolor[0];
                  surface[1] = -surface[2]*rcolor[1];

                  matvecmultv(isw, normal);

                  if (flag2t)
                     tob->query_texel(acolor[0], acolor[1], tcolor, frame, -surface[2]*parm->imscale);

                  if (!flagtr && spot) {
                     if (flag3t)
                        calc_color3b(&sparam, 0, surface, normal, lmain, spot, parm, &texcolor, rcolor, acolor);
                     else if (flag2t)
                        calc_color2b(surface, normal, lmain, spot, parm, &texcolor, tcolor, rcolor, acolor);
                     else
                        calc_colorb(surface, normal, lmain, spot, parm, &texcolor, rcolor, acolor);
                  }

                  else {
                     if (flag3t)
                        calc_color3(&sparam, 0, surface, normal, lmain, parm, &texcolor, rcolor, acolor);
                     else if (flag2t)
                        calc_color2(surface, normal, lmain, parm, &texcolor, tcolor, rcolor, acolor);
                     else
                        calc_color(surface, normal, lmain, parm, &texcolor, rcolor, acolor);
                  }

                  if (flagtr) {
                     set_datatr(zbuff->zbuff, index, surface[2], acolor, rcolor, a0, a1, a2, d0, d1, d2);
                  }

                  else {
                     set_data(zbuff->zbuff, index, surface[2], acolor, rcolor, a0, a1, a2, d0, d1, d2);
                  }

               }

            }

         }

      }

