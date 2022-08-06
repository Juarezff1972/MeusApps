

#include <string.h>
#include <stdio.h>
#include "texture.h"

int code_mask[13] = {
     0,
     0x0001, 0x0003,
     0x0007, 0x000F,
     0x001F, 0x003F,
     0x007F, 0x00FF,
     0x01FF, 0x03FF,
     0x07FF, 0x0FFF
};


/* ************************************************************************
************************************************************************ */
int gif::out_line(unsigned char *buffer, rgb *image, int width, int scany) {

   int i, j;

   j = gimage.left+width;

   if (gimage.transparentflag) {
      for (i=gimage.left; i<j; i++)
         if (buffer[i] != gimage.transparentindex)
            ((int *)image->pdata[scany])[i] = *((int *)(color_table[buffer[i]]));
   }

   else
      for (i=gimage.left; i<j; i++)
         ((int *)image->pdata[scany])[i] = *((int *)(color_table[buffer[i]]));

   return 1;
}


/* get_next_code()
 * - gets the next code from the GIF file.  Returns the code, or else
 * a negative number in case of file errors...
 */
int gif::get_next_code(FILE *infile, gifdecodetype *stuff) {

   int ret;

   if (!stuff->bits_left) {
      if (!stuff->bytes_left) {

         // Out of bytes in current block, so read next block

         stuff->nextcptr = stuff->buffer;
         do {
            fread(&stuff->bytes_left, 1, 1, infile);
         } while(!stuff->bytes_left);

         fread(stuff->buffer, stuff->bytes_left, 1, infile);
      }

      stuff->cptr = *stuff->nextcptr++;
      stuff->bits_left = 8;
      stuff->bytes_left--;
   }

   ret = stuff->cptr >> (8 - stuff->bits_left);

   while (stuff->codesize > stuff->bits_left) {
      if (!stuff->bytes_left) {

         // Out of bytes in current block, so read next block

         stuff->nextcptr = stuff->buffer;
         do {
            fread(&stuff->bytes_left, 1, 1, infile);
         } while(!stuff->bytes_left);

         fread(stuff->buffer, stuff->bytes_left, 1, infile);
      }

      stuff->cptr = *stuff->nextcptr++;
      ret |= stuff->cptr << stuff->bits_left;
      stuff->bits_left += 8;
      --stuff->bytes_left;
   }

   stuff->bits_left -= stuff->codesize;
   return ret &= code_mask[stuff->codesize];
}


/* unsigned short decoder(linewidth)
 *    unsigned short linewidth;               * Pixels per line of image *
 *
 * - This function decodes an LZW image, according to the method used
 * in the GIF spec.  Every *linewidth* "characters" (ie. pixels) decoded
 * will generate a call to out_line(), which is a user specific function
 * to display a line of pixels.  The function gets it's codes from
 * get_next_code() which is responsible for reading blocks of data and
 * seperating them into the proper size codes.  Finally, get_byte() is
 * the global routine to read the next byte from the GIF file.
 *
 * It is generally a good idea to have linewidth correspond to the actual
 * width of a line (as specified in the Image header) to make your own
 * code a bit simpler, but it isn't absolutely necessary.
 *
 * Returns: 1 if successful, else 0.  (See ERRS.H)
 *
 */

int gif::decoder(FILE *infile, rgb *image) {

   unsigned char size;                  // # of bits of pixels
   int top_slot;                        // # of pixels to decode
   int clear, ending;                   // clear/end code
   int slot, newcodes;                  // first avail compression code value
   int oc, fc = 0;
   unsigned char *sp, *bufptr;

   unsigned char *buffer, *stack;
   int bufcnt;
   int c, ret;

   int code;
   gifdecodetype stuff;
   int scany = head.ysize - (1 + gimage.top);

   stuff.bytes_left = 0;                /* # bytes left in block */
   stuff.bits_left  = 0;                /* # bits left in current byte */

                // Initialize for decoding a new image...

   fread(&size, 1, 1, infile);
                // initializes the decoder for reading a new image.
   stuff.codesize = size + 1;

   top_slot = 1 << stuff.codesize;
   clear = 1 << size;
   ending = clear + 1;
   slot = newcodes = ending + 1;
   stuff.bytes_left = stuff.bits_left = 0;

   // Allocate space for the decode buffer

   buffer = new unsigned char[gimage.width + 1];
   stack  = new unsigned char[gimage.height*gimage.width+1];

   // Set up the stack pointer and decode buffer pointer
   sp     = stack;
   bufptr = buffer;
   bufcnt = 0;

   /* This is the main loop.  For each code we get we pass through the
    * linked list of prefix codes, pushing the corresponding "character" for
    * each code onto the stack.  When the list reaches a single "character"
    * we push that on the stack too, and then start unstacking each
    * character for output in the correct order.  Special handling is
    * included for the clear code, and the whole thing ends when we get
    * an ending code.
    */

   while ((c = get_next_code(infile, &stuff)) != ending) {

      // If the code is a clear code, reinitialize all necessary items.

      if (c == clear) {
         stuff.codesize = size + 1;
         slot = newcodes;
         top_slot = 1 << stuff.codesize;

         // Continue reading codes until we get a non-clear code
         // (Another unlikely, but possible case...)

         while ((c = get_next_code(infile, &stuff)) == clear);

         // If we get an ending code immediately after a clear code
         // (Yet another unlikely case), then break out of the loop.

         if (c == ending)
            break;

         /* Finally, if the code is beyond the range of already set codes,
          * (This one had better NOT happen...  I have no idea what will
          * result from this, but I doubt it will look good...) then set it
          * to color zero.
          */

         if (c >= slot)
            c = 0;

         oc = fc = c;

         /* And let us not forget to put the char into the buffer... And
          * if, on the off chance, we were exactly one pixel from the end
          * of the line, we have to send the buffer to the out_line()
          * routine...
          */

         *bufptr++ = c;
         if (++bufcnt == gimage.width) {

            if (image != (rgb *)NULL)
               ret = out_line(buffer, image, bufcnt, scany);
            else
               ret = 1;

            scany-=gimage.slinc;

            if (scany <= head.ysize - (gimage.top+gimage.height) - 1) {
               if (gimage.interlace_flag)
                  switch (gimage.pass) {

                     case 0:
                        gimage.pass = 1;
                        gimage.slinc = 8;
                        scany = head.ysize - (5 + gimage.top);
                        break;

                     case 1:
                        gimage.pass = 2;
                        gimage.slinc = 4;
                        scany = head.ysize - (3 + gimage.top);
                        break;

                     default:
                        gimage.pass = 3;
                        gimage.slinc = 2;
                        scany = head.ysize - (2 + gimage.top);
                        break;

                  }

            }

            bufptr = buffer;
            bufcnt = 0;
         }

      }

      else {

         /* In this case, it's not a clear code or an ending code, so
          * it must be a code code...  So we can now decode the code into
          * a stack of character codes. (Clear as mud, right?)
          */

         code = c;

         /* Here we go again with one of those off chances...  If, on the
          * off chance, the code we got is beyond the range of those already
          * set up (Another thing which had better NOT happen...) we trick
          * the decoder into thinking it actually got the last code read.
          * (Hmmn... I'm not sure why this works...  But it does...)
          */

         if (code >= slot) {
            code = oc;
            *sp++ = fc;
         }

         /* Here we scan back along the linked list of prefixes, pushing
          * helpless characters (ie. suffixes) onto the stack as we do so.
          */

         while (code >= newcodes) {
            *sp++ = suffix[code];
            code  = prefix[code];
         }

         /* Push the last character on the stack, and set up the new
          * prefix and suffix, and if the required slot number is greater
          * than that allowed by the current bit size, increase the bit
          * size.  (NOTE - If we are all full, we *don't* save the new
          * suffix and prefix...  I'm not certain if this is correct...
          * it might be more proper to overwrite the last code...
          */

         *sp++ = code;
         if (slot < top_slot) {
            suffix[slot] = fc = code;
            prefix[slot++] = oc;
            oc = c;
         }

         if (slot >= top_slot)
            if (stuff.codesize < 12) {
               top_slot <<= 1;
               stuff.codesize++;
            }

         /* Now that we've pushed the decoded string (in reverse order)
          * onto the stack, lets pop it off and put it into our decode
          * buffer...  And when the decode buffer is full, write another
          * line...
          */

         while (sp > stack) {
            *bufptr++ = *(--sp);
            if (++bufcnt == gimage.width) {
               if (image != (rgb *)NULL)
                  ret = out_line(buffer, image, bufcnt, scany);
               else
                  ret = 1;

               scany-=gimage.slinc;

               if (scany <= head.ysize - (gimage.top+gimage.height) - 1) {
                  if (gimage.interlace_flag)
                     switch (gimage.pass) {

                        case 0:
                           gimage.pass = 1;
                           gimage.slinc = 8;
                           scany = head.ysize - (5 + gimage.top);
                           break;

                        case 1:
                           gimage.pass = 2;
                           gimage.slinc = 4;
                           scany = head.ysize - (3 + gimage.top);
                           break;

                        default:
                           gimage.pass = 3;
                           gimage.slinc = 2;
                           scany = head.ysize - (2 + gimage.top);
                           break;

                     }

               }

               bufptr = buffer;
               bufcnt = 0;
            }

         }

      }

   }

   if (bufcnt && image != (rgb *)NULL)
      ret = out_line(buffer, image, bufcnt, scany);
   else
      ret = 1;

   delete [] buffer;
   delete [] stack;
   return ret;
}


/* ************************************************************************
************************************************************************ */
int gif::read_header(FILE *infile) {

   unsigned char temp[3];

   fread(head.signature, 1, 3, infile);      // signature
   head.signature[3] = 0;

   if (strcmp(head.signature, "GIF"))
      return 0;

   fread(head.version, 1, 3, infile);        // version
   head.version[3] = 0;

//   head.xsize = getushort(infile);           // logical screen descriptor
//   head.ysize = getushort(infile);
   head.xsize = getrushort(infile);           // logical screen descriptor
   head.ysize = getrushort(infile);

   fread(temp, 3, 1, infile);

   head.gctable_flag = (temp[0]>>7);                 // global color table flag
   head.color_resolution = ((temp[0]>>4) & 0x07);    // color resolution - # of bits per color
   head.sort_flag = (temp[0]>>3) & 0x01;             // sort flag
   head.gctable_size = 1 << ((temp[0] & 0x07)+1);    // global color table size 2^(x+1)

   head.bc_index = temp[1];
   head.pixel_aspect_ratio = temp[2];

   return 1;
}


/* ************************************************************************
************************************************************************ */
int gif::read_data(char *fname, rgb *image) {

   FILE *infile;
   int i, j, k, m; // , l;
   unsigned char temp;
   unsigned char temp2[16];
   unsigned char is, it;

   if (!(infile = fopen(fname,"rb"))) {
      printf("Could not open %s for input as GIF file\n", fname);
      return 0;
   }

   if (!read_header(infile)) {                  // read in header & logical screen descriptor
      printf("%s is not in GIF format\n", fname);
      fclose(infile);
      return 0;
   }

   image->head.xsize = head.xsize;
   image->head.ysize = head.ysize;
   image->head.zsize = 3;
   image->init_map(head.xsize, head.ysize);

//   l = 7-head.color_resolution;

   if (head.gctable_flag) {                     // global color table
      i = head.gctable_size<<2;
      pgctable = new puchar[head.gctable_size];
      gctable = new unsigned char[i];

      for (j=k=0; k<i; j++, k+=4) {
         pgctable[j] = &gctable[k];

         fread(&pgctable[j][3], 1, 1, infile);
         fread(&pgctable[j][2], 1, 1, infile);
         fread(&pgctable[j][1], 1, 1, infile);
         pgctable[j][0] = 0;
      }

/* apparently not needed
      if (l)
         for (k=0; k<head.gctable_size; k++) {
            pgctable[k][3] = (unsigned char)(pgctable[k][3] << l);
            pgctable[k][2] = (unsigned char)(pgctable[k][2] << l);
            pgctable[k][1] = (unsigned char)(pgctable[k][1] << l);
         }
*/

      for (i=0; i<head.ysize; i++)              // init background color
         for (j=0; j<head.xsize; j++)
            ((int *)image->pdata[i])[j] = *((int *)pgctable[head.bc_index]);
   }

   while (1) {                                  // image descriptors
      if (fread(&is, 1, 1, infile) != 1)
         break;

      switch (is) {

         case 0x3b:                           // trailer/eof
            if (pgctable != (unsigned char **)NULL) {
               delete [] pgctable;
               delete [] gctable;
               pgctable = (unsigned char **)NULL;
            }

            if (gimage.plctable != (unsigned char **)NULL) {
               delete [] gimage.plctable;
               delete [] gimage.lctable;
               gimage.plctable = (unsigned char **)NULL;
            }

            fclose(infile);
            return 1;

         case 0x21:                             // extension
            fread(&it, 1, 1, infile);

            switch (it) {

               case 0xf9:                     // graphic control extension
                  fread(&temp, 1, 1, infile); // graphic control label = 0xf9
                  fread(&temp, 1, 1, infile); // blocksize = 4
                  fread(&temp, 1, 1, infile); // packed byte
                                              // bit 5-7 reserved
                                              // bit 2-4 disposal method
                                              // bit 1 user input flag
                                              // bit 0 transparent flag

                  gimage.transparentflag = temp & 0x01;
                  gimage.renderflag = (temp>>2) && 0x07;

                  getrushort(infile);           // delay time

                  fread(&gimage.transparentindex, 1, 1, infile); // transparent color index
                  fread(&temp, 1, 1, infile); // block terminator = 0x00

                  break;

               case 0x01:                       // plain text extension
                  fread(temp2, 1, 1, infile);

               case 0xff:                       // application extention
                  fread(temp2, 12, 1, infile);

               case 0xfe:                       // comment extension

               default:
                  do {
                     fread(&temp, 1, 1, infile);
                     for (i=0; i<temp;i++)
                        fread(temp2, 1, 1, infile);
                  } while(temp);

                  break;
            }

            break;

         case 0x2c:                     // image separator
            gimage.image_separator = is;

            gimage.left   = getrushort(infile);
            gimage.top    = getrushort(infile);
            gimage.width  = getrushort(infile);
            gimage.height = getrushort(infile);

            fread(&temp, 1, 1, infile);

            gimage.lctable_flag = (temp>>7) & 0x01;
            gimage.interlace_flag = (temp>>6) & 0x01;
            gimage.sort_flag = (temp>>5) & 0x01;
            gimage.reserved = (temp>>3) & 0x03;
            gimage.lctable_size = 1<<((temp & 0x07)+1);

            if (gimage.interlace_flag) {
               gimage.pass = 0;
               gimage.slinc = 8;
            }

            else
               gimage.slinc = 1;

            if (gimage.lctable_flag) {          // local color table if not exist use gct
               if (gimage.plctable != (unsigned char **)NULL) {
                  delete [] gimage.plctable;
                  delete [] gimage.lctable;
               }

               i = gimage.lctable_size<<2;

               gimage.lctable = new unsigned char[i];
               gimage.plctable = new puchar[gimage.lctable_size];

               for (j=k=0; k<i; j++, k+=4) {
                  gimage.plctable[j] = &gimage.lctable[k];
                  fread(&gimage.plctable[j][3], 1, 1, infile);
                  fread(&gimage.plctable[j][2], 1, 1, infile);
                  fread(&gimage.plctable[j][1], 1, 1, infile);
                  gimage.plctable[j][0] = 0;
               }

/* apparently not needed
               if (l)
                  for (k=0; k<gimage.lctable_size; k++) {
                     gimage.plctable[k][3] = (unsigned char)(gimage.plctable[k][3] << l);
                     gimage.plctable[k][2] = (unsigned char)(gimage.plctable[k][2] << l);
                     gimage.plctable[k][1] = (unsigned char)(gimage.plctable[k][1] << l);
                  }
*/

               color_table = gimage.plctable;
            }

            else
               color_table = pgctable;

            switch (gimage.renderflag) {

               case 0:
               case 1:
                  if (!decoder(infile, image)) {
                     if (pgctable != (unsigned char **)NULL) {
                        delete [] pgctable;
                        delete [] gctable;
                        pgctable = (unsigned char **)NULL;
                     }

                     if (gimage.plctable != (unsigned char **)NULL) {
                        delete [] gimage.plctable;
                        delete [] gimage.lctable;
                        gimage.plctable = (unsigned char **)NULL;
                     }

                     fclose(infile);
                     return 0;
                  }

                  break;

               case 2:                          // goes through default
                  j = gimage.top-gimage.height;
                  m = gimage.left+gimage.width;

                  for (i=gimage.top; i>j; i--)
                     for (k=gimage.left; k<m; k++)
                        ((int *)image->pdata[i])[k] = *((int *)(pgctable[head.bc_index]));

               default:
                  if (!decoder(infile, (rgb *)NULL)) {
                     if (pgctable != (unsigned char **)NULL) {
                        delete [] pgctable;
                        delete [] gctable;
                        pgctable = (unsigned char **)NULL;
                     }

                     if (gimage.plctable != (unsigned char **)NULL) {
                        delete [] gimage.plctable;
                        delete [] gimage.lctable;
                        gimage.plctable = (unsigned char **)NULL;
                     }

                     fclose(infile);
                     return 0;
                  }

                  break;
            }

            gimage.transparentflag = 0;
            gimage.renderflag = 0;

            break;
      }

   }

   if (pgctable != (unsigned char **)NULL) {
      delete [] pgctable;
      delete [] gctable;
      pgctable = (unsigned char **)NULL;
   }

   if (gimage.plctable != (unsigned char **)NULL) {
      delete [] gimage.plctable;
      delete [] gimage.lctable;
      gimage.plctable = (unsigned char **)NULL;
   }

   fclose(infile);
   return 1;
}

