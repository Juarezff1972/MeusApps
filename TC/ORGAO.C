#include <noise.h>
#include <stdio.h>

int c;
int duration=20000;
int note;

main()
{
printf("Dura‡„o da nota: %d\n",duration);
c=getch();
while ((c)!=27)
   {
     note=0;
     if (c=='`' || c=='~') note=190;
     if (c=='1' || c=='!') note=200;
     if (c=='2' || c=='@') note=210;
     if (c=='3' || c=='#') note=220;
     if (c=='4' || c=='$') note=230;
     if (c=='5' || c=='%') note=240;
     if (c=='6' || c=='^') note=250;
     if (c=='7' || c=='&') note=260;
     if (c=='8' || c=='*') note=270;
     if (c=='9' || c=='(') note=280;
     if (c=='0' || c==')') note=290;
     if (c=='q' || c=='Q') note=300;
     if (c=='w' || c=='W') note=310;
     if (c=='e' || c=='E') note=320;
     if (c=='r' || c=='R') note=330;
     if (c=='t' || c=='T') note=340;
     if (c=='y' || c=='Y') note=350;
     if (c=='u' || c=='U') note=360;
     if (c=='i' || c=='I') note=370;
     if (c=='o' || c=='O') note=380;
     if (c=='p' || c=='P') note=390;
     if (c=='a' || c=='A') note=400;
     if (c=='s' || c=='S') note=410;
     if (c=='d' || c=='D') note=420;
     if (c=='f' || c=='F') note=430;
     if (c=='g' || c=='G') note=440;
     if (c=='h' || c=='H') note=450;
     if (c=='j' || c=='J') note=460;
     if (c=='k' || c=='K') note=470;
     if (c=='l' || c=='L') note=480;
     if (c=='z' || c=='Z') note=490;
     if (c=='x' || c=='X') note=500;
     if (c=='c' || c=='C') note=510;
     if (c=='v' || c=='V') note=520;
     if (c=='b' || c=='B') note=530;
     if (c=='n' || c=='N') note=540;
     if (c=='m' || c=='M') note=550;
     noise(note,duration);
     c=getch();
     printf("%c",c);
   }
}