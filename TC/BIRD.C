#include <stdio.h>
#include <graphics.h>
#include <process.h>
#include <mem.h>
#include <bios.h>

void keyboard(void);
void quit_pgm(void);
void coords(void);
void graphics_setup(void);
void labels(void);
void draw_bird(void);

void animCGA(void);
void pagemove(unsigned source,unsigned target);

int frame=0,t1=1,t2=2;
float sx,sy,sx1,sy1,sx2,sy2; float x_res,y_res;
int c0=0,c1=1,c2=2,c3=3,c4=4,c5=5,c6=6,c7=7,c8=8,c9=9,c10=10,
c11=11,c12=12,c13=13,c14=14,c15=15,mode_flag=0;
char fill_75[]={187,238,187,238,187,238,187,138};

int pl=0;
 int array1[][2]={205,219,208,218,212,214,218,212,221,208,229,197,
                  232,192,241,187,250,183,258,183,265,185,273,189,
                  282,198,377,180,376,182,310,200,297,207,289,211,
                  286,217,282,221,278,230,269,238,256,248,249,253,
                  240,261,235,269,231,278,228,280,214,294,199,304,
                  188,310,181,317,177,324,176,321,169,328,168,325,
                  162,330,162,326,135,340,126,345,120,345,111,346,
                  106,349, 58,390, 49,399, 42,405, 42,402, 51,391,
                   74,364, 87,353, 99,339,107,324,114,309,127,289,
                  138,274,146,259};
 int array2[][2]={146,259,143,246,136,234,128,221,110,199, 98,184,
                   88,169, 79,144, 76,136, 74,118, 73, 98, 69, 72,
                   74, 69, 80, 71, 86, 74,107,101,119,112,124,118,
                  143,139,154,154,161,166,166,171,181,188,187,195,
                  193,200,197,209,205,219,179,259};
 int array3[][2]={119,112,118, 99,118, 87,121, 59,121, 52,126, 49,
                  134, 47,139, 62,146, 73,154, 88,174,129,187,170,
                  194,184,199,197,207,209,212,214,139, 99};
 int array4[][2]={248,199,250,196,258,195,261,197,263,200,261,204,
                  258,207,252,205,250,203,248,199,257,200};
 int array5[][2]={212,214,218,212,224,212,222,217,239,217,237,219,
                  245,222,259,219,258,222,269,220,268,224,278,220,
                  278,224,282,221,278,230,269,238,256,248,249,253,
                  240,261,240,258,234,261,234,257,232,258,232,253,
                  226,252,227,249,219,248,222,244,219,242,220,240,
                  211,238,214,230,211,228,212,214,239,239};
 int array6[][2]={146,259,140,250,129,245,109,234, 92,219, 82,209,
                   77,193, 73,179, 74,159, 78,146, 84,136, 96,138,
                  119,147,129,157,140,165,152,178,159,184,175,199,
                  189,210,194,213,205,219, 85,179};
 int array7[][2]={152,178,151,156,151,137,154,121,156,113,164, 98,
                  166, 89,170, 84,178, 93,180,100,185,107,188,127,
                  191,148,193,169,197,179,202,199,212,214,177,119};
 int array8[][2]={146,259,119,258,104,253, 98,249, 93,241, 90,237,
                   90,228, 94,204, 97,198,106,193,124,190,137,189,
                  151,195,163,199,175,208,205,219,109,242};
 int array9[][2]={163,199,163,184,169,171,169,152,173,144,178,138,
                  187,127,194,119,199,118,202,125,203,134,204,162,
                  207,179,209,195,210,207,212,214,199,139};
int array10[][2]={146,259,137,270,124,277,118,277,114,276,111,269,
                  112,260,114,258,125,246,127,237,139,228,163,219,
                  174,218,179,218,191,219,205,219,119,269};
int array11[][2]={174,218,178,203,185,185,192,177,199,169,211,160,
                  224,157,227,161,227,169,226,177,224,188,217,205,
		  212,214,219,179};

main(){
   graphics_setup();
   setcolor(c7);

   cleardevice();
   frame=0;
   labels();
   draw_bird();
   pagemove(0xb800,0x8800);
   cleardevice();
   frame=1;
   labels();
   draw_bird();
   pagemove(0xb800,0x8c00);
   cleardevice();
   frame=2;
   labels();
   draw_bird();
   pagemove(0xb800,0x9000);
   cleardevice();
   frame=3;
   labels();
   draw_bird();
   pagemove(0xb800,0x9400);
   pagemove(0x8800,0xb800);
   for (t1=1;t1<=30000;t1++);
   animCGA();
   quit_pgm();
}

void animCGA(void){
   for (t1=1;t1!=2; ){
      pagemove(0x8c00,0xb800);  for (t2=1;t2<=1000;t2++);
      pagemove(0x9000,0xb800);  for (t2=1;t2<=1000;t2++);
      pagemove(0x9400,0xb800);  for (t2=1;t2<=1000;t2++); keyboard();
      pagemove(0x9000,0xb800);  for (t2=1;t2<=1000;t2++);
      pagemove(0x8c00,0xb800);  for (t2=1;t2<=1000;t2++);
      pagemove(0x8800,0xb800);  for (t2=1;t2<=1000;t2++); keyboard();
   };
   return;
}

void pagemove(unsigned source,unsigned target){
   movedata(source,0x0000,target,0x0000,16000);
   return;
}

void labels(void){
   setcolor(c7);moveto(0,192);
   outtext("640x200 2-color CGA mode");
   moveto(168,0);
   outtext("USING C FOR HIGH SPEED FRAME ANIMATION");
   moveto(408,192);outtext("Press any key to quit.");
   setcolor(c14);moveto(424,40);
   outtext("Ruby-throated Hummingbird");
   setcolor(c7);
   moveto(408,88);
   outtext("Range: Central & eastern U.S.");
   moveto(408,104);outtext("Breeds from southern");
   moveto(408,112);outtext("Canada to the Gulf Coast.");
   moveto(408,128);outtext("Depicted here in its");
   moveto(408,136);outtext("characteristic hovering");
   moveto(408,144);outtext("flight pattern.");
   setcolor(c12);
   moveto(408,160);
   outtext("Animation rate:");
   setcolor(c7);
   moveto(408,168);
   outtext("    10 frames per second.");
   return;
}
void draw_bird(void){
   setcolor(c7);setfillstyle(SOLID_FILL,c7);
   sx=0;sy=30;coords();sx1=sx;sy1=sy;
   sx=385;sy=458;coords();sx2=sx;sy2=sy;
   bar(sx1,sy1,sx2,sy2);
   setcolor(c0);

   pl=0;
   sx=array1[pl][0];sy=array1[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=55;t1++){
      sx=array1[pl][0];sy=array1[pl][1];pl++;
      coords();lineto(sx,sy);
   };

   if (frame==0) {
   pl=0;
   sx=array2[pl][0];sy=array2[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=26;t1++){
      sx=array2[pl][0];sy=array2[pl][1];pl++;
      coords();lineto(sx,sy);
   };
   sx=array2[pl][0];sy=array2[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   pl=0;
   sx=array3[pl][0];sy=array3[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=15;t1++){
      sx=array3[pl][0];sy=array3[pl][1];pl++;
      coords();lineto(sx,sy);};
   sx=array3[pl][0];sy=array3[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   }
   if (frame==1) {
   pl=0;
   sx=array6[pl][0];sy=array6[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=20;t1++){
      sx=array6[pl][0];sy=array6[pl][1];pl++;
      coords();lineto(sx,sy);
   };
   sx=array6[pl][0];sy=array6[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   pl=0;
   sx=array7[pl][0];sy=array7[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=16;t1++){
      sx=array7[pl][0];sy=array7[pl][1];pl++;
      coords();lineto(sx,sy);};
   sx=array7[pl][0];sy=array7[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   }

   if (frame==2) {
   pl=0;
   sx=array8[pl][0];sy=array8[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=15;t1++){
      sx=array8[pl][0];sy=array8[pl][1];pl++;
      coords();lineto(sx,sy);
   };
   sx=array8[pl][0];sy=array8[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   pl=0;
   sx=array9[pl][0];sy=array9[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=15;t1++){
      sx=array9[pl][0];sy=array9[pl][1];pl++;
      coords();lineto(sx,sy);};
   sx=array9[pl][0];sy=array9[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   }

   if (frame==3) {
   pl=0;
   sx=array10[pl][0];sy=array10[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=15;t1++){
      sx=array10[pl][0];sy=array10[pl][1];pl++;
      coords();lineto(sx,sy);
   };
   sx=array10[pl][0];sy=array10[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   pl=0;
   sx=array11[pl][0];sy=array11[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=12;t1++){
      sx=array11[pl][0];sy=array11[pl][1];pl++;
      coords();lineto(sx,sy);};
   sx=array11[pl][0];sy=array11[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   }
   setcolor(c0);
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);
   pl=0;
   sx=array4[pl][0];sy=array4[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=9;t1++){
      sx=array4[pl][0];sy=array4[pl][1];pl++;
      coords();
      lineto(sx,sy);
   };
   sx=array4[pl][0];sy=array4[pl][1];coords();
   setfillstyle(SOLID_FILL,c0);floodfill(sx,sy,c0);

   setcolor(c4);setfillstyle(SOLID_FILL,c4);
   pl=0;
   sx=array5[pl][0];sy=array5[pl][1];pl++;coords();moveto(sx,sy);
   for (t1=1;t1<=33;t1++){
      sx=array5[pl][0];sy=array5[pl][1];pl++;
      coords();
      lineto(sx,sy);
   };
   sx=array5[pl][0];sy=array5[pl][1];coords();floodfill(sx,sy,c4);

   setcolor(c7);return;
}

void keyboard(void){
   if (bioskey(1)==0) return; else quit_pgm();
}

void quit_pgm(void){
   cleardevice();restorecrtmode();exit(0);
}

void graphics_setup(void){
   int graphics_adapter,graphics_mode;

   graphics_adapter=CGA;graphics_mode=CGAHI;
   initgraph(&graphics_adapter,&graphics_mode,"");
   x_res=640;y_res=200;mode_flag=4;
   c0=0;c1=1;c2=1;c3=1;c4=1;c5=1;c6=1;c7=1;
   c8=1;c9=1;c10=1;c11=1;c12=1;c13=1;c14=1;c15=1;
   return;
}

void coords(void){
   sx=sx*(x_res/640);sy=sy*(y_res/480);return;
}