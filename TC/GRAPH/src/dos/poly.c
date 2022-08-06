


#include "polygon.h"
#include "global.h"
#include "memman.h"

#define FP_ONE 1024
#define FP_DIV_ONE 0.001
#define FP_BOP 10

/* **************************************************
************************************************** */
void polygon::polyct(int i, zbuffer *zbuff) {

   unsigned long flatacolor, flatdcolor;
   int a0, a1, a2, d0, d1, d2;

   edgetype *ptr, *qtr;
   edgetype *aet;

   float *rcolor, *acolor;
   float mtemp;
   float mtempz, mdeltaz;

   int scany, scanxy;
   int start, end;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   unsigned int *pzdata = zbuff->data;

   scany = et[i]->starty;
   aet = (edgetype *)NULL;
   scanxy = toprow - scany*zbuff->maxx;

   acolor = ob->shade[i].ambient;
   rcolor = facedata[i].constcolor;

   _asm {
      push eax
      push ebx
      mov eax, acolor
      mov ebx, rcolor

      fld dword ptr [eax]
      fld st(0)
      fadd dword ptr [ebx]
      fxch st(1)
      fistp a0
      fistp d0

      fld dword ptr [eax+4]
      fld st(0)
      fadd dword ptr [ebx+4]
      fxch st(1)
      fistp a1
      fistp d1

      fld dword ptr [eax+8]
      fld st(0)
      fadd dword ptr [ebx+8]
      fxch st(1)
      fistp a2
      fistp d2

      pop ebx
      pop eax
   }

   WAIT();

   ((unsigned char *)&flatacolor)[2] = BYTE_LUT(a0);
   ((unsigned char *)&flatacolor)[1] = BYTE_LUT(a1);
   ((unsigned char *)&flatacolor)[0] = BYTE_LUT(a2);
   ((unsigned char *)&flatdcolor)[2] = BYTE_LUT(d0);
   ((unsigned char *)&flatdcolor)[1] = BYTE_LUT(d1);
   ((unsigned char *)&flatdcolor)[0] = BYTE_LUT(d2);

   while (scany < zbuff->maxy) {

      while (et[i] && et[i]->starty == scany) {
         ptr = et[i];
         et[i] = et[i]->next;
         sortbucket(&aet, ptr);
      }

      if (!INTERLACE || !(scany & 0x01)) {
         for (ptr=aet; ptr!=(edgetype *)NULL; ptr=qtr->next) {

            mtempz = ptr->start.point[2];
            qtr   = ptr->next;

            start = round(ptr->start.point[0]);
            end   = round(qtr->start.point[0]);

            if (start == end)
               continue;

            mtemp = 1.0/(qtr->start.point[0] - ptr->start.point[0]);
            mdeltaz = (qtr->start.point[2] - mtempz) * mtemp;

            start = (start + scanxy)*12;
            end   = (end + scanxy)*12;

            _asm {
               pushad

               fld mtempz
               mov edx, start
               mov ecx, pzdata
               mov esi, flatacolor
               mov edi, flatdcolor
               mov ebx, 12

loopzb:
               fcom dword ptr [ecx+edx]
               fnstsw         ax
               sahf
               jbe            endboolzb

               fst dword ptr  [ecx+edx]
               mov [ecx+edx+4], esi;
               mov [ecx+edx+8], edi;

endboolzb:

               fadd mdeltaz
               add edx, ebx
               cmp edx, end
               jl loopzb

               fstp mtempz
               popad
            }

         }

      }

      scany++; scanxy -= zbuff->maxx;

      if (scany >= zbuff->maxy)
         break;

      for (qtr = aet, aet = (edgetype *)NULL; qtr != (edgetype *)NULL;) {
         ptr = qtr;
         qtr = qtr->next;

         if (scany < round(ptr->end.point[1])) {
            ptr->start.point[0] += ptr->dx;
            ptr->start.point[2] += ptr->dz;

            sortbucket(&aet, ptr);
         }

         else
            control.push(EDGETYPE, ptr);
      }

      if (aet == (edgetype *)NULL) {
         if (!et[i])
            break;

         scany = et[i]->starty;
         scanxy = toprow - scany*zbuff->maxx;
      }

   }

   while (aet != (edgetype *)NULL) {
      ptr=aet;
      aet=aet->next;
      control.push(EDGETYPE, ptr);
   }

   while (et[i] != (edgetype *)NULL) {
      ptr=et[i];
      et[i]=et[i]->next;
      control.push(EDGETYPE, ptr);
   }

}


/* **************************************************
************************************************** */
void polygon::polyct2(int i, int flagb, zbuffer *zbuff) {

   int a0, a1, a2, d0, d1, d2;

   edgetype *ptr, *qtr;
   edgetype *aet;

   vector3f rcolor, acolor;
   float mtemp;
   float mtempz, mdeltaz;

   unsigned int *pzdata = zbuff->data;

   int scany, scanxy;
   int start, end;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   unsigned int tempcolor;
   vector3f tcolor;
   float mtempt[2];
   float mdeltat[2];
   int tsizex, tsizey;

   scany = et[i]->starty;
   aet = (edgetype *)NULL;
   scanxy = toprow - scany*zbuff->maxx;

   copyarray3(acolor, ob->shade[i].ambient);
   rcolor[0] = acolor[0] + facedata[i].constcolor[0];
   rcolor[1] = acolor[1] + facedata[i].constcolor[1];
   rcolor[2] = acolor[2] + facedata[i].constcolor[2];

   tcolor[0] = tcolor[1] = tcolor[2] = 1.0;

   tsizex = tob->data[i].ob->maxx - 1;
   tsizey = tob->data[i].ob->maxy - 1;

   while (scany < zbuff->maxy) {

      while (et[i] && et[i]->starty == scany) {
         ptr = et[i];
         et[i] = et[i]->next;
         sortbucket(&aet, ptr);
      }

      if (!INTERLACE || !(scany & 0x01)) {
         for (ptr=aet; ptr!=(edgetype *)NULL; ptr=qtr->next) {

            mtempz = ptr->start.point[2];
            qtr   = ptr->next;

            start = round(ptr->start.point[0]);
            end   = round(qtr->start.point[0]);

            if (start == end)
               continue;

            mtemp = 1.0/(qtr->start.point[0] - ptr->start.point[0]);
            mdeltaz = (qtr->start.point[2] - mtempz) * mtemp;

            start += scanxy;
            end   += scanxy;

            copyarray2(mtempt, ptr->start.uv);
            mdeltat[0] = (qtr->start.uv[0] - mtempt[0]) * mtemp;
            mdeltat[1] = (qtr->start.uv[1] - mtempt[1]) * mtemp;

            do {
               if (mtempz>zbuff->zbuff[start].zdata) {

                  tempcolor = tob->query_texel(i, mtempt[0], mtempt[1], tcolor, frame, -mtempz*facedata[i].area);

                  if (flagb && (tempcolor & 0xffffff00) == inviso)
                     goto polyct_inviso;

                  zbuff->zbuff[start].zdata = mtempz;

                  _asm {
                     push eax
                     push ebx
                     push ecx

                     lea ecx, tcolor
                     lea eax, acolor
                     lea ebx, rcolor

                     fld dword ptr [ecx]
                     fld st(0)
                     fmul dword ptr [eax]
                     fxch st(1)
                     fmul dword ptr [ebx]
                     fxch st(1)
                     fistp a0
                     fistp d0

                     fld dword ptr [ecx+4]
                     fld st(0)
                     fmul dword ptr [eax+4]
                     fxch st(1)
                     fmul dword ptr [ebx+4]
                     fxch st(1)
                     fistp a1
                     fistp d1

                     fld dword ptr [ecx+8]
                     fld st(0)
                     fmul dword ptr [eax+8]
                     fxch st(1)
                     fmul dword ptr [ebx+8]
                     fxch st(1)
                     fistp a2
                     fistp d2

                     pop ecx
                     pop ebx
                     pop eax
                  }

                  zbuff->zbuff[start].ambient[2] = BYTE_LUT(a0);
                  zbuff->zbuff[start].ambient[1] = BYTE_LUT(a1);
                  zbuff->zbuff[start].ambient[0] = BYTE_LUT(a2);

                  zbuff->zbuff[start].pixel[2] = BYTE_LUT(d0);
                  zbuff->zbuff[start].pixel[1] = BYTE_LUT(d1);
                  zbuff->zbuff[start].pixel[0] = BYTE_LUT(d2);
               }

polyct_inviso:

               addarray2(mtempt, mdeltat);
               mtempz += mdeltaz;
               start++;
            } while (start < end);

         }

      }

      scany++; scanxy -= zbuff->maxx;

      if (scany >= zbuff->maxy)
         break;

      for (qtr = aet, aet = (edgetype *)NULL; qtr != (edgetype *)NULL;) {
         ptr = qtr;
         qtr = qtr->next;

         if (scany < round(ptr->end.point[1])) {
            ptr->start.point[0] += ptr->dx;
            ptr->start.point[2] += ptr->dz;
            addarray2(ptr->start.uv, ptr->dt);
            sortbucket(&aet, ptr);
         }

         else
            control.push(EDGETYPE, ptr);
      }

      if (aet == (edgetype *)NULL) {
         if (!et[i])
            break;

         scany = et[i]->starty;
         scanxy = toprow - scany*zbuff->maxx;
      }

   }

   while (aet != (edgetype *)NULL) {
      ptr=aet;
      aet=aet->next;
      control.push(EDGETYPE, ptr);
   }

   while (et[i] != (edgetype *)NULL) {
      ptr=et[i];
      et[i]=et[i]->next;
      control.push(EDGETYPE, ptr);
   }

}


/* **************************************************
************************************************** */
void polygon::polyct2db(int i, zbuffer *zbuff) {

   edgetype *ptr, *qtr;
   edgetype *aet;

   int   rcolor[3], acolor[3];
   float mtemp, mtemp2, mtemp3;
   float mtempz, mdeltaz;

   int scany, scanxy;
   int start, end;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   unsigned int *pzdata = zbuff->data;

   int tmaxx;
   int tsize[2];
   int mtempt[2];
   int mdeltat[2];
   unsigned int *tdata = (unsigned int *)((texture *)tob->data[i].ob)->tob.data;
   unsigned int color0 = inviso;

   scany = et[i]->starty;
   aet = (edgetype *)NULL;
   scanxy = toprow - scany*zbuff->maxx;

   mtemp = 256.0/255.0;

   acolor[0] = (int)(ob->shade[i].ambient[0]*mtemp);
   acolor[1] = (int)(ob->shade[i].ambient[1]*mtemp);
   acolor[2] = (int)(ob->shade[i].ambient[2]*mtemp);
   rcolor[0] = (int)((ob->shade[i].ambient[0]+facedata[i].constcolor[0])*mtemp);
   rcolor[1] = (int)((ob->shade[i].ambient[1]+facedata[i].constcolor[1])*mtemp);
   rcolor[2] = (int)((ob->shade[i].ambient[2]+facedata[i].constcolor[2])*mtemp);

   acolor[0] = clamp(acolor[0], 0, 256);
   acolor[1] = clamp(acolor[1], 0, 256);
   acolor[2] = clamp(acolor[2], 0, 256);
   rcolor[0] = clamp(rcolor[0], 0, 256);
   rcolor[1] = clamp(rcolor[1], 0, 256);
   rcolor[2] = clamp(rcolor[2], 0, 256);

   tsize[0] = ((tmaxx = tob->data[i].ob->maxx) - 1)*FP_ONE;
   tsize[1] = (tob->data[i].ob->maxy - 1)*FP_ONE;

   while (scany < zbuff->maxy) {

     while (et[i] && et[i]->starty == scany) {
         ptr = et[i];
         et[i] = et[i]->next;
         sortbucket(&aet, ptr);
      }


      if (!INTERLACE || !(scany & 0x01)) {
         for (ptr=aet; ptr!=(edgetype *)NULL; ptr=qtr->next) {

            qtr   = ptr->next;

            start = round(ptr->start.point[0]);
            end   = round(qtr->start.point[0]);

            if (start == end)
               continue;

            mtempz = ptr->start.point[2];
            mtemp = 1.0/(qtr->start.point[0] - ptr->start.point[0]);
            mdeltaz = (qtr->start.point[2] - ptr->start.point[2]) * mtemp;

            start = (start + scanxy)*12;
            end   = (end + scanxy)*12;

            // init texture scan seg

            mtempt[0] = (ptr->start.uv[0] - (int)ptr->start.uv[0])*tsize[0];
            if (mtempt[0] < 0)
               mtempt[0] += tsize[0];

            mtempt[1] = (ptr->start.uv[1] - (int)ptr->start.uv[1])*tsize[1];
            if (mtempt[1] < 0)
               mtempt[1] += tsize[1];

            mtemp2 = (qtr->start.uv[0] - ptr->start.uv[0]) * mtemp;
            mtemp3 = fabs(mtemp2);
            if (mtemp3 >= 1.0 || mtemp3 < FP_DIV_ONE)
               mdeltat[0] = 0;
            else
               mdeltat[0] = mtemp2*tsize[0];

            mtemp2 = (qtr->start.uv[1] - ptr->start.uv[1]) * mtemp;
            mtemp3 = fabs(mtemp2);
            if (mtemp3 >= 1.0 || mtemp3 < FP_DIV_ONE)
               mdeltat[1] = 0;
            else
               mdeltat[1] = mtemp2*tsize[1];

            // done init texture scan seg

            _asm {
               pushad
               fld mtempz
               mov edi, start
               mov esi, pzdata

doloop1:
               fcom dword ptr [edi+esi]
               fnstsw         ax
               sahf
               jbe            polyct_inviso1

//                tempcolor = tdata[(mtempt[1]>>FP_BOP)*tmaxx + (mtempt[0]>>FP_BOP)];
               mov ecx, tdata
               mov edx, tmaxx
               mov ebx, mtempt
               mov eax, mtempt+4
               shr ebx, FP_BOP
               shr eax, FP_BOP
               lea ecx, [ecx+ebx*4]
               imul eax, edx
               sub ebx, ebx
               mov eax, [eax*4+ecx];           // eax gets tempcolor

//                if ((tempcolor & 0xffffff00) == inviso)
//                   goto polyct_inviso;
               mov edx, color0
               and eax, 0xffffff00
               cmp edx, eax
               jne endif1

polyct_inviso1:
               jmp polyct_inviso2

endif1:
               fst dword ptr  [edi+esi]

//                blue
//                zbuff->zbuff[start].ambient[0] = (acolor[2]*((unsigned char *)&tempcolor)[1]) >> 8;
//                zbuff->zbuff[start].pixel[0] = ((rcolor[2]*((unsigned char *)&tempcolor)[1]) >> 8);

              mov       bl,  ah
              mov       ecx, acolor+8
              mov       edx, rcolor+8
              shr       eax, 8
              imul      ecx, ebx
              imul      edx, ebx
              mov       [edi+esi+4], ch
              mov       [edi+esi+8], dh

//                green
//                zbuff->zbuff[start].ambient[1] = (acolor[1]*((unsigned char *)&tempcolor)[2]) >> 8;
//                zbuff->zbuff[start].pixel[1] = ((rcolor[1]*((unsigned char *)&tempcolor)[2]) >> 8);
              mov       bl,  ah
              mov       ecx, acolor+4
              mov       edx, rcolor+4
              shr       eax, 8
              imul      ecx, ebx
              imul      edx, ebx
              mov       [edi+esi+5], ch
              mov       [edi+esi+9], dh

//                  red
//                  zbuff->zbuff[start].ambient[2] = (acolor[0]*((unsigned char *)&tempcolor)[3]) >> 8;
//                  zbuff->zbuff[start].pixel[2] = ((rcolor[0]*((unsigned char *)&tempcolor)[3]) >> 8);
              mov       bl,  ah
              mov       ecx, acolor
              mov       edx, rcolor

              imul      ecx,ebx
              imul      edx,ebx
              mov       [edi+esi+6], ch
              mov       [edi+esi+10], dh

polyct_inviso2:

//               mtempz += mdeltaz;
               fadd mdeltaz;

//               addarray2(mtempt, mdeltat);
               mov eax, mtempt
               mov ecx, mdeltat
               mov ebx, mtempt+4
               mov edx, mdeltat+4
               add eax, ecx
               add ebx, edx

//               if (mtempt[0] < 0)
//                  mtempt[0] += tsize[0];
               mov ecx, tsize
               mov edx, tsize+4

               cmp eax, 0
               jge elseif1
               add eax, ecx
               jmp endelseif1

elseif1:
//               else if (mtempt[0] > tsize[0])
//                  mtempt[0] -= tsize[0];
               cmp eax, ecx
               jle endelseif1
               sub eax, ecx

endelseif1:

//               if (mtempt[1] < 0)
//                  mtempt[1] += tsize[1];
               cmp ebx, 0
               jge elseif2
               add ebx, edx
               jmp endelseif2

elseif2:
//               else if (mtempt[1] > tsize[1])
//                  mtempt[1] -= tsize[1];
               cmp ebx, edx
               jle endelseif2
               sub ebx, edx

endelseif2:

//               start++;
               add edi, 12
               mov edx, end

               mov mtempt,   eax
               mov mtempt+4, ebx

//            } while (start < end);
               cmp edi, edx
               jne doloop1

               fstp mtempz
               popad
           }

         }

      }

      scany++; scanxy -= zbuff->maxx;

      if (scany >= zbuff->maxy)
         break;

      for (qtr = aet, aet = (edgetype *)NULL; qtr != (edgetype *)NULL;) {
         ptr = qtr;
         qtr = qtr->next;

         if (scany < round(ptr->end.point[1])) {
            ptr->start.point[0] += ptr->dx;
            ptr->start.point[2] += ptr->dz;
            addarray2(ptr->start.uv, ptr->dt);
            sortbucket(&aet, ptr);
         }

         else
            control.push(EDGETYPE, ptr);
      }

      if (aet == (edgetype *)NULL) {
         if (!et[i])
            break;

         scany = et[i]->starty;
         scanxy = toprow - scany*zbuff->maxx;
      }

   }

   while (aet != (edgetype *)NULL) {
      ptr=aet;
      aet=aet->next;
      control.push(EDGETYPE, ptr);
   }

   while (et[i] != (edgetype *)NULL) {
      ptr=et[i];
      et[i]=et[i]->next;
      control.push(EDGETYPE, ptr);
   }

}


/* **************************************************
************************************************** */
void polygon::polyct2d(int i, zbuffer *zbuff) {

   edgetype *ptr, *qtr;
   edgetype *aet;

   int   rcolor[3], acolor[3];
   float mtemp, mtemp2, mtemp3;
   float mtempz, mdeltaz;

   int scany, scanxy;
   int start, end;
   int toprow = zbuff->maxx*(zbuff->maxy-1);

   unsigned int *pzdata = zbuff->data;

   int tmaxx;
   int tsize[2];
   int mtempt[2];
   int mdeltat[2];
   unsigned int *tdata = (unsigned int *)((texture *)tob->data[i].ob)->tob.data;

   scany = et[i]->starty;
   aet = (edgetype *)NULL;
   scanxy = toprow - scany*zbuff->maxx;

   mtemp = 256.0/255.0;

   acolor[0] = (int)(ob->shade[i].ambient[0]*mtemp);
   acolor[1] = (int)(ob->shade[i].ambient[1]*mtemp);
   acolor[2] = (int)(ob->shade[i].ambient[2]*mtemp);
   rcolor[0] = (int)((ob->shade[i].ambient[0]+facedata[i].constcolor[0])*mtemp);
   rcolor[1] = (int)((ob->shade[i].ambient[1]+facedata[i].constcolor[1])*mtemp);
   rcolor[2] = (int)((ob->shade[i].ambient[2]+facedata[i].constcolor[2])*mtemp);

   acolor[0] = clamp(acolor[0], 0, 256);
   acolor[1] = clamp(acolor[1], 0, 256);
   acolor[2] = clamp(acolor[2], 0, 256);
   rcolor[0] = clamp(rcolor[0], 0, 256);
   rcolor[1] = clamp(rcolor[1], 0, 256);
   rcolor[2] = clamp(rcolor[2], 0, 256);

   tsize[0] = ((tmaxx = tob->data[i].ob->maxx) - 1)*FP_ONE;
   tsize[1] = (tob->data[i].ob->maxy - 1)*FP_ONE;

   while (scany < zbuff->maxy) {

     while (et[i] && et[i]->starty == scany) {
         ptr = et[i];
         et[i] = et[i]->next;
         sortbucket(&aet, ptr);
      }


      if (!INTERLACE || !(scany & 0x01)) {
         for (ptr=aet; ptr!=(edgetype *)NULL; ptr=qtr->next) {

            qtr   = ptr->next;

            start = round(ptr->start.point[0]);
            end   = round(qtr->start.point[0]);

            if (start == end)
               continue;

            mtempz = ptr->start.point[2];
            mtemp = 1.0/(qtr->start.point[0] - ptr->start.point[0]);
            mdeltaz = (qtr->start.point[2] - ptr->start.point[2]) * mtemp;

            start = (start + scanxy)*12;
            end   = (end + scanxy)*12;

            // init texture scan seg

            mtempt[0] = (ptr->start.uv[0] - (int)ptr->start.uv[0])*tsize[0];
            if (mtempt[0] < 0)
               mtempt[0] += tsize[0];

            mtempt[1] = (ptr->start.uv[1] - (int)ptr->start.uv[1])*tsize[1];
            if (mtempt[1] < 0)
               mtempt[1] += tsize[1];

            mtemp2 = (qtr->start.uv[0] - ptr->start.uv[0]) * mtemp;
            mtemp3 = fabs(mtemp2);
            if (mtemp3 >= 1.0 || mtemp3 < FP_DIV_ONE)
               mdeltat[0] = 0;
            else
               mdeltat[0] = mtemp2*tsize[0];

            mtemp2 = (qtr->start.uv[1] - ptr->start.uv[1]) * mtemp;
            mtemp3 = fabs(mtemp2);
            if (mtemp3 >= 1.0 || mtemp3 < FP_DIV_ONE)
               mdeltat[1] = 0;
            else
               mdeltat[1] = mtemp2*tsize[1];

            // done init texture scan seg

            _asm {
               pushad
               fld mtempz
               mov edi, start
               mov esi, pzdata

doloop1:
               fcom dword ptr [edi+esi]
               fnstsw         ax
               sahf
               jbe            polyct_inviso1

//                tempcolor = tdata[(mtempt[1]>>FP_BOP)*tmaxx + (mtempt[0]>>FP_BOP)];
               mov edx, tmaxx
               mov ecx, tdata
               mov ebx, mtempt
               mov eax, mtempt+4
               shr ebx, FP_BOP
               shr eax, FP_BOP
               lea ecx, [ecx + ebx*4]
               imul eax, edx
               sub ebx, ebx
               mov eax, [eax*4+ecx];           // eax gets tempcolor

               jmp endif1

polyct_inviso1:
               jmp polyct_inviso2

endif1:
               fst dword ptr  [edi+esi]

//                zbuff->zbuff[start].ambient[0] = (acolor[2]*((unsigned char *)&tempcolor)[1]) >> 8;
//                zbuff->zbuff[start].pixel[0] = ((rcolor[2]*((unsigned char *)&tempcolor)[1]) >> 8);
              mov       bl,  ah
              mov       ecx, acolor+8
              mov       edx, rcolor+8
              shr       eax, 8
              imul      ecx, ebx
              imul      edx, ebx
              mov       [edi+esi+4], ch
              mov       [edi+esi+8], dh

//                zbuff->zbuff[start].ambient[1] = (acolor[1]*((unsigned char *)&tempcolor)[2]) >> 8;
//                zbuff->zbuff[start].pixel[1] = ((rcolor[1]*((unsigned char *)&tempcolor)[2]) >> 8);
              mov       bl,  ah
              mov       ecx, acolor+4
              mov       edx, rcolor+4
              shr       eax, 8
              imul      ecx, ebx
              imul      edx, ebx
              mov       [edi+esi+5], ch
              mov       [edi+esi+9], dh

//                  zbuff->zbuff[start].ambient[2] = (acolor[0]*((unsigned char *)&tempcolor)[3]) >> 8;
//                  zbuff->zbuff[start].pixel[2] = ((rcolor[0]*((unsigned char *)&tempcolor)[3]) >> 8);
              mov       bl,  ah
              mov       ecx, acolor
              mov       edx, rcolor

              imul      ecx,ebx
              imul      edx,ebx
              mov       [edi+esi+6], ch
              mov       [edi+esi+10], dh

polyct_inviso2:

//               mtempz += mdeltaz;
               fadd mdeltaz;

//               addarray2(mtempt, mdeltat);
               mov eax, mtempt
               mov ecx, mdeltat
               mov ebx, mtempt+4
               mov edx, mdeltat+4
               add eax, ecx
               add ebx, edx

//               if (mtempt[0] < 0)
//                  mtempt[0] += tsize[0];
               mov ecx, tsize
               mov edx, tsize+4

               cmp eax, 0
               jge elseif1
               add eax, ecx
               jmp endelseif1

elseif1:
//               else if (mtempt[0] > tsize[0])
//                  mtempt[0] -= tsize[0];
               cmp eax, ecx
               jle endelseif1
               sub eax, ecx

endelseif1:

//               if (mtempt[1] < 0)
//                  mtempt[1] += tsize[1];
               cmp ebx, 0
               jge elseif2
               add ebx, edx
               jmp endelseif2

elseif2:
//               else if (mtempt[1] > tsize[1])
//                  mtempt[1] -= tsize[1];
               cmp ebx, edx
               jle endelseif2
               sub ebx, edx

endelseif2:

//               start++;
               add edi, 12
               mov edx, end

               mov mtempt,   eax
               mov mtempt+4, ebx

//            } while (start < end);
               cmp edi, edx
               jne doloop1

               fstp mtempz
               popad
           }

         }

      }

      scany++; scanxy -= zbuff->maxx;

      if (scany >= zbuff->maxy)
         break;

      for (qtr = aet, aet = (edgetype *)NULL; qtr != (edgetype *)NULL;) {
         ptr = qtr;
         qtr = qtr->next;

         if (scany < round(ptr->end.point[1])) {
            ptr->start.point[0] += ptr->dx;
            ptr->start.point[2] += ptr->dz;
            addarray2(ptr->start.uv, ptr->dt);
            sortbucket(&aet, ptr);
         }

         else
            control.push(EDGETYPE, ptr);
      }

      if (aet == (edgetype *)NULL) {
         if (!et[i])
            break;

         scany = et[i]->starty;
         scanxy = toprow - scany*zbuff->maxx;
      }

   }

   while (aet != (edgetype *)NULL) {
      ptr=aet;
      aet=aet->next;
      control.push(EDGETYPE, ptr);
   }

   while (et[i] != (edgetype *)NULL) {
      ptr=et[i];
      et[i]=et[i]->next;
      control.push(EDGETYPE, ptr);
   }

}






/*
            do {
               if (mtempz>zbuff->zbuff[start].zdata) {
                  zbuff->zbuff[start].zdata = mtempz;

                  tempcolor = tdata[(mtempt[1]>>FP_BOP)*tmaxx + (mtempt[0]>>FP_BOP)];

                  if (flagb & ((tempcolor & 0xffffff00) == inviso))
                     goto polyct_inviso;

                  zbuff->zbuff[start].ambient[2] = (acolor[0]*((unsigned char *)&tempcolor)[3]) >> 8;
                  zbuff->zbuff[start].ambient[1] = (acolor[1]*((unsigned char *)&tempcolor)[2]) >> 8;
                  zbuff->zbuff[start].ambient[0] = (acolor[2]*((unsigned char *)&tempcolor)[1]) >> 8;

                  zbuff->zbuff[start].pixel[2] = ((rcolor[0]*((unsigned char *)&tempcolor)[3]) >> 8);
                  zbuff->zbuff[start].pixel[1] = ((rcolor[1]*((unsigned char *)&tempcolor)[2]) >> 8);
                  zbuff->zbuff[start].pixel[0] = ((rcolor[2]*((unsigned char *)&tempcolor)[1]) >> 8);
               }

polyct_inviso:

               addarray2(mtempt, mdeltat);
               if (mtempt[0] < 0)
                  mtempt[0] += tsize[0];
               else if (mtempt[0] > tsize[0])
                  mtempt[0] -= tsize[0];

               if (mtempt[1] < 0)
                  mtempt[1] += tsize[1];
               else if (mtempt[1] > tsize[1])
                  mtempt[1] -= tsize[1];

               mtempz += mdeltaz;
               start++;
            } while (start < end);
*/

