#include <dos.h>
#include <process.h>

int hz=100;
void noise(int hertz,int duration);

main(){

for(hz=1;hz<=1600;hz+=50)
noise(hz,5000);

for (hz=1;hz<=20000;hz++);

for (hz=2000;hz>=250;hz-=50)
noise(hz,5000);

for (hz=1;hz<=6000;hz++);

noise(40,40000);

exit(0);}

void noise(int hertz,int duration){
int t1=1,high_byte=0,low_byte=0;
short count=0;unsigned char old_port=0,new_port=0;

if (hertz<40) return;
if (hertz>4600) return;

count=1193180l/hertz;
high_byte=count/256;low_byte=count-(high_byte*256);

outportb(0x43,0xb6);
outportb(0x42,low_byte);
outportb(0x42,high_byte);

old_port=inportb(0x61);
new_port=(old_port | 0x03);
outportb(0x61,new_port);
for (t1=1;t1<=duration;t1++);
outportb(0x61,old_port);

return;}