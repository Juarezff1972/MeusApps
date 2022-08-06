

#include <string.h>
#include "matrix.h"
#include "texture.h"
                                        // 1/9
#define ONETHIRD 0.3333333333


unsigned int texbase::query_texel(float x, float y, float *color, int frame, int mflag, float d) {

   color[0] = color[1] = color[2] = 0.0;
   return 0;
}


int texture::read_texture(char *fname, texbase **tptr, int flip) {

   return 0;
}


int texture::read_texture(char *fname, int flip) {

   strcpy(filename, fname);

   if (!tob.read_data(filename, flip)) {   // read rgb
      if (!tob.read_tiff(filename))
         if (!tob.read_gif(filename))
            if (!tob.read_ppm(filename))
               if (!tob.read_jpg(filename))
                  if (!tob.read_ilbm(filename))
                     return 0;

      if (flip)
         tob.flip_bytes();
   }

   maxx = tob.maxx;
   maxy = tob.maxy;

   return 1;
}


void texture::build_mipmaps() {

   int temp2;
   int tempx, tempy;
   int h, i, j, k, l, m, n;
   int r, g, b;
   unsigned char *p, *mm;

   if (tob.maxx < tob.maxy)
      i = tob.maxx;
   else
      i = tob.maxy;

   for (mapcount=0, temp2=1; temp2<i; temp2*=3, mapcount++);

   if (!mapcount)
      return;

   if (mipmap != (mipmaptype *)NULL)
      delete [] mipmap;

   mipmap = new mipmaptype[mapcount];

   tempx=(tob.maxx+2)/3;
   tempy=(tob.maxy+2)/3;
   mipmap[0].tob.init_map(tempx, tempy);
   mipmap[0].compression = 3;
   mipmap[0].icompression = ONETHIRD;
   mipmap[0].idiff=0.5;
   mm = (unsigned char *)mipmap[0].tob.data;

   for (i=0; i<tob.maxy; i+=3)
      for (j=0; j<tob.maxx; j+=3) {
         r = g = b  = 0;

         for (k=0, m=i; k<3; k++, m++) {
            if (m == tob.maxy)
               m = 0;

            for (l=0, n=j; l<3; l++, n++) {
               if (n == tob.maxx)
                  n = 0;

               p = (unsigned char *)(&tob.pdata[m][n]) + 1;
               b += *p; p++;
               g += *p; p++;
               r += *p;
            }

         }

         mm++;
         *mm = (unsigned char)(b/9);mm++;
         *mm = (unsigned char)(g/9);mm++;
         *mm = (unsigned char)(r/9);mm++;
      }

   for (h=0, temp2=1; temp2<mapcount; h++, temp2++) {
      tempx=(tempx+2)/3;
      tempy=(tempy+2)/3;

      mipmap[temp2].tob.init_map(tempx, tempy);
      mipmap[temp2].compression=mipmap[h].compression*3;
      mipmap[temp2].icompression=mipmap[h].icompression*ONETHIRD;
      mipmap[temp2].idiff=1.0/(mipmap[temp2].compression-mipmap[h].compression);
      mm = (unsigned char *)mipmap[temp2].tob.data;

      for (i=0; i<mipmap[h].tob.maxy; i+=3)
         for (j=0; j<mipmap[h].tob.maxx; j+=3) {
            r = g = b = 0;

            for (k=0, m=i; k<3; k++, m++) {
               if (m == mipmap[h].tob.maxy)
                  m = 0;

               for (l=0, n=j; l<3; l++, n++) {
                  if (n == mipmap[h].tob.maxx)
                     n = 0;

                  p = (unsigned char *)(&mipmap[h].tob.pdata[m][n]) + 1;
                  b += *p; p++;
                  g += *p; p++;
                  r += *p;
               }

            }

            mm++;
            *mm = (unsigned char)(b/9);mm++;
            *mm = (unsigned char)(g/9);mm++;
            *mm = (unsigned char)(r/9);mm++;
         }

   }

}


/*
        Assumption - x, y w/in parameters...
*/
unsigned int texture::query_texel(float x, float y, float *color, int frame, int mflag, float d) {

   unsigned char *p, *q, *r, *s;
   unsigned int temp, temp2;
   int i, j, k, l;
   float dx, dy;
   int indx;

   if (!mipmap || !mflag) {        // no antialiasing
      F2I(i, y);
      F2I(j, x);
      p = ((unsigned char *)tob.pdata[i])+(j<<2);
      color[0] = CONVERT_0_255_0_1(*(p+3));
      color[1] = CONVERT_0_255_0_1(*(p+2));
      color[2] = CONVERT_0_255_0_1(*(p+1));

      return *(unsigned int *)p;
   }

   if (d <= 1.0 + CORRECT) {                            // gouraud shade antialias texture

      i = (int)y;
      dy = y - i;
      k = i+1;
      if (k == tob.maxy)
         k = 0;

      j = (int)x;
      dx = x - j;
      l = j+1;
      if (l == tob.maxx)
         l = 0;

      p = (unsigned char *)(&temp);

      j = j<<2;
      r = (unsigned char *)(tob.pdata[i]) + j;
      s = (unsigned char *)(tob.pdata[k]) + j;
      p++; s++; r++;
      *p = (unsigned char)(*r + (*s - *r)*dy); p++; s++; r++;
      *p = (unsigned char)(*r + (*s - *r)*dy); p++; s++; r++;
      *p = (unsigned char)(*r + (*s - *r)*dy);

      q = (unsigned char *)(&temp2);

      l = l<<2;
      r = (unsigned char *)(tob.pdata[i]) + l;
      s = (unsigned char *)(tob.pdata[k]) + l;
      q++; s++; r++;
      *q = (unsigned char)(*r + (*s - *r)*dy); q++; s++; r++;
      *q = (unsigned char)(*r + (*s - *r)*dy); q++; s++; r++;
      *q = (unsigned char)(*r + (*s - *r)*dy);

      p = (unsigned char *)(&temp);
      q = (unsigned char *)(&temp2);
      p++; q++;
      color[2] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx) ); p++; q++;
      color[1] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx) ); p++; q++;
      color[0] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx) );

      return temp;
   }
                                                        // last mip map
   if (d > mipmap[mapcount-1].compression-CORRECT) {
      p = (unsigned char *)(*mipmap[mapcount-1].tob.pdata);

      color[0] = CONVERT_0_255_0_1(*(p+3));
      color[1] = CONVERT_0_255_0_1(*(p+2));
      color[2] = CONVERT_0_255_0_1(*(p+1));

      return *(unsigned int *)p;
   }

   if (d <= mipmap[0].compression) {            // between texture and first mip map
      temp = tob.pdata[(int)y][(int)x];
      p = ((unsigned char *)(&temp)) + 3;

      i = (int)(y*mipmap[0].icompression);
      j = (int)(x*mipmap[0].icompression);
      q = ((unsigned char *)mipmap[0].tob.pdata[i])+(j<<2) + 3;

      dx = (d - 1.0)*mipmap[0].idiff;

      color[0] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx)); p--; q--;
      color[1] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx)); p--; q--;
      color[2] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx));

      return temp;
   }

   for (indx=0; indx<mapcount-2 && d>mipmap[indx+1].compression; indx++);

   i = (int)(y*mipmap[indx].icompression);
   j = (int)(x*mipmap[indx].icompression);
   temp = mipmap[indx].tob.pdata[i][j];
   p = ((unsigned char *)(&temp)) + 3;

   i = (int)(y*mipmap[indx+1].icompression);
   j = (int)(x*mipmap[indx+1].icompression);
   q = ((unsigned char *)mipmap[indx+1].tob.pdata[i])+(j<<2) + 3;

   dx = (d - mipmap[indx].compression)*mipmap[indx+1].idiff;

   color[0] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx)); p--; q--;
   color[1] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx)); p--; q--;
   color[2] = CONVERT_0_255_0_1(*p += (int)((*q - *p)*dx));

   return temp;
}


// *******************************************************************************

int texvid::read_texture(char *fname, int flip) {

   return 0;
}


int texvid::read_texture(char *fname, texbase **tptr, int flip) {

   FILE *infile;
   char buffer[MAXSTRLEN], buffer2[MAXSTRLEN], buffer3[MAXSTRLEN];
   int i, j, k;

   if (!(infile = fopen(fname, "r"))) {
      printf("Video script file %s could not be accessed...\n", filename);
      return 0;
   }

   strcpy(filename, fname);
   fscanf(infile, "%s", buffer);
   lower_case(buffer);

   if (strcmp(buffer, "asdf")) {
      printf("Invalid Video Magic Number for script %s...\n", filename);
      fclose(infile);

      return 0;
   }

   fscanf(infile, "%s", buffer);
   lower_case(buffer);

   if (!strcmp(buffer, "repeat"))
      vpflag = VIDREPEAT;
   else
      vpflag = VIDONE;

   fscanf(infile, "%d", &count);

   if (!count) {
      fclose(infile);
      return 1;
   }

   if (tob != (texbase **)NULL)
      delete [] tob;

   tob = new ptexbase[count];
   for (i=0; i<count; ) {
      fscanf(infile, "%s", buffer);
      lower_case(buffer);

      if (!strcmp(buffer, "series")) {
         fscanf(infile, "%s %d %d %s", buffer2, &j, &k, buffer3);
         if (j <= k)
            for (; j<=k; j++) {
               sprintf(buffer, "%s.%05d.%s", buffer2, j, buffer3);

               for (tob[i]=*tptr; tob[i]!=(texbase *)NULL; tob[i]=tob[i]->next)
                  if (!strcmp(buffer, tob[i]->filename))
                     break;

               if (tob[i] == (texbase *)NULL) {
                  tob[i] = new texture;
                  tob[i]->read_texture(buffer, flip);
                  tob[i]->next = *tptr;
                  *tptr = tob[i];
               }

               i++;
            }

         else
            for (; j>=k; j--) {
               sprintf(buffer, "%s.%05d.%s", buffer2, j, buffer3);

               for (tob[i]=*tptr; tob[i]!=(texbase *)NULL; tob[i]=tob[i]->next)
                  if (!strcmp(buffer, tob[i]->filename))
                     break;

               if (tob[i] == (texbase *)NULL) {
                  tob[i] = new texture;
                  tob[i]->read_texture(buffer, flip);
                  tob[i]->next = *tptr;
                  *tptr = tob[i];
               }

               i++;
            }

      }

      else {

         for (tob[i]=*tptr; tob[i]!=(texbase *)NULL; tob[i]=tob[i]->next)
            if (!strcmp(buffer, tob[i]->filename))
               break;

         if (tob[i] == (texbase *)NULL) {
            tob[i] = new texture;
            tob[i]->read_texture(buffer, flip);
            tob[i]->next = *tptr;
            *tptr = tob[i];
         }

         i++;
      }

   }

   fclose(infile);

   maxx = tob[0]->maxx;
   maxy = tob[0]->maxy;

   return 1;
}


/*
        Assumption - x, y w/in parameters...
*/
unsigned int texvid::query_texel(float x, float y, float *color, int frame, int mflag, float d) {

   int i;

   if (tob == (texbase **)NULL)
      return 0;

   if (frame >= count) {
      if (vpflag == VIDONE)
         return tob[count-1]->query_texel(x,y,color,count-1, mflag, d);

      i = frame % count;
      return tob[i]->query_texel(x,y,color,i,mflag,d);
   }

   return tob[frame]->query_texel(x,y,color,frame,mflag,d);
}


int query_video(char *filename) {

   FILE *infile;
   char buffer[MAXSTRLEN];

   if (!(infile = fopen(filename, "r")))
      return 0;

   fscanf(infile, "%255s", buffer);
   lower_case(buffer);

   fclose(infile);
   return (!strcmp(buffer, "asdf"));
}


