      {
         sbbuffer->pdata[i][j] = 2;

         if (lparm->query_whatami() == POINT) {
            pray[0] = j;
            pray[1] = i;
            pray[2] = -1;
            lparm->screen2map(pray);
            matvecmultv(iworld, pray);             // 1 pixel to the right ray
         }

         else {
            pzero[0] = j;
            pzero[1] = i;
            pzero[2] = 0;
            lparm->screen2map(pzero);
            matvecmulto(iworld, pzero);
         }

         if ((k=Intersect_Terrain(increment, pzero, pray, surface, normal)) == -1)
            break;

         if (k) { // hit
            matvecmulto(world, surface);

            if (surface[2] >lparm->lbuff->lbuff.pdata[i][j].zdata) {
               sbbuffer->pdata[i][j] = 1;
               lparm->lbuff->lbuff.pdata[i][j].zdata = surface[2];
               lparm->lbuff->lbuff.pdata[i][j].idbuff = id;
            }

         }

      }

