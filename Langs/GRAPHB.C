/* ***********************************************************************
   *									 *
   *		      Video Graphics Array Driver                        *
   *									 *
   *				  By					 *
   *									 *
   *			      Larry Sharp				 *
   *                                                                     *
   *				  And					 *
   *									 *
   *			  Christopher D. Watkins			 *
   *									 *
   *									 *
   *  (c) 1991 Larry Sharp and Christopher D. Watkins			 *
   *									 *
   ***********************************************************************



   Plot		 - place pixel to screen
   Set_Palette	 - set palette register
   Init_Palette  - 64 levels of grey, red, green and blue
   Init_Palette2 - 7 colors with 35 intensities each - use with Pixel
   Cycle_Palette - cycle through palette
   Circle	 - circle draw routine
   Draw		 - line draw routine
   Init_Graphics - initialize graphics
   Wait_For_Key	 - wait for key press
   Exit_Graphics - sound and wait for keypress before exiting graphics
   Title	 - set up text screen colors
*/

#include "stdio.h"
#include "dos.h"
#include "conio.h"
#include "math.h"
#include "malloc.h"
#include "defs.h"
#include "mathb.h"
#include "graphb.h"

union  REGS reg;
struct SREGS inreg;

void Set_Mode(int Mode)

{
  reg.h.ah=0;
  reg.h.al=Mode;
  int86(0x10,&reg,&reg);
}

Word   X_Off, Y_Off;
static Word far *Pre_Calc_Y1;
static DWord far *Pre_Calc_Y2;
static Byte Res;

#define Low_Res    1
#define Medium_Res 2
#define High_Res   3

void Calc_Offsets()
{
  Word tmp, tmp2;

  if(XRes<MaxXRes)
  {
    tmp=MaxXRes>>1;
    tmp2=XRes>>1;
    X_Off=tmp-tmp2;
  }
  else
    X_Off=0;
  if(YRes<MaxYRes)
  {
    tmp=MaxYRes>>1;
    tmp2=YRes>>1;
    Y_Off=tmp-tmp2;
  }
  else
    Y_Off=0;
}

void Pre_Calc()
{
  Word j;

  for(j=0; j<=MaxYRes; j++)
  {
    if(Res==Medium_Res)
      Pre_Calc_Y2[j]=(DWord)MaxXRes*(DWord)j;
    else
      Pre_Calc_Y1[j]=MaxXRes*j;
  }
}

static Word Old_Page, Old_Page_2;

void Plot(Word x, Word y, Byte color)
{
  long L_Offset;
  Word Offset, Page;
  char far *address;

  if((x<XRes) && (y<YRes))
  {
    switch(Res)
    {
      case High_Res :   Offset=Pre_Calc_Y1[y+Y_Off]+x+X_Off;
			Page=y>>6;
			if(Page!=Old_Page)
			{
			  outpw(0x3CD, Page|64);
			  Old_Page=Page;
			}
			address=(char far *)(0xA0000000L+Offset);
			*address=color;
			break;
      case Medium_Res :	x+=X_Off;
			y+=Y_Off;
			L_Offset=Pre_Calc_Y2[y]+x;
			Page=(L_Offset>>16);
			Offset=L_Offset&65535;
			if(Page!=Old_Page)
			{
			  outpw(0x3CD, Page|64);
			  Old_Page=Page;
			}
			address=(char far *)(0xA0000000L+Offset);
			*address=color;
			break;
      case Low_Res    :	Offset=Pre_Calc_Y1[y+Y_Off]+x+X_Off;
			address=(char far *)(0xA0000000L+Offset);
			*address=color;
			break;
    }
  }
}

static Palette_Register Color;

void Set_Palette(Palette_Register Hue)
{
  reg.x.ax=0x1012;
  segread(&inreg);
  inreg.es=inreg.ds;
  reg.x.bx=0;
  reg.x.cx=256;
  reg.x.dx=(int)&Hue[0];
  int86x(0x10,&reg,&reg,&inreg);
}

void Init_Palette(Palette_Register Color)
{
  Word i;

  for(i=0; i<64; i++)
  {
    Color[i].Red=i;
    Color[i].Grn=i;
    Color[i].Blu=i;
  }
  for(i=64; i<128; i++)
  {
    Color[i].Red=i-64;
    Color[i].Grn=0;
    Color[i].Blu=0;
  }
  for(i=128; i<192; i++)
  {
    Color[i].Red=0;
    Color[i].Grn=i-128;
    Color[i].Blu=0;
  }
  for(i=192; i<=255; i++)
  {
    Color[i].Red=0;
    Color[i].Grn=0;
    Color[i].Blu=i-192;
  }
}

void Init_Palette_2(Palette_Register Color)
{
  Word i;

  for(i=0; i<36; i++)
  {
    Color[i].Red=0;
    Color[i].Grn=0;
    Color[i].Blu=Round(1.8*i);
  }
  for(i=36; i<72; i++)
  {
    Color[i].Red=0;
    Color[i].Grn=Round(1.8*(i-36));
    Color[i].Blu=0;
  }
  for(i=72; i<108; i++)
  {
    Color[i].Red=0;
    Color[i].Grn=Round(1.8*(i-72));
    Color[i].Blu=Round(1.8*(i-72));
  }
  for(i=108; i<144; i++)
  {
    Color[i].Red=Round(1.8*(i-108));
    Color[i].Grn=0;
    Color[i].Blu=0;
  }
  for(i=144; i<180; i++)
  {
    Color[i].Red=Round(1.8*(i-144));
    Color[i].Grn=0;
    Color[i].Blu=Round(1.8*(i-144));
  }
  for(i=180; i<216; i++)
  {
    Color[i].Red=Round(1.8*(i-180));
    Color[i].Grn=Round(1.8*(i-180));
    Color[i].Blu=0;
  }
  for(i=216; i<252; i++)
  {
    Color[i].Red=Round(1.8*(i-216));
    Color[i].Grn=Round(1.8*(i-216));
    Color[i].Blu=Round(1.8*(i-216));
  }
}

void Cycle_Palette(Palette_Register Hue)
{
  Word i;
  RGB  tmp;

  tmp=Hue[0];
  memcpy(&Hue[0], &Hue[1], 765);
  Hue[255]=tmp;
  Set_Palette(Hue);
}

void Swap(int *first, int *second)
{
  int temp;

  temp=*first;
  *first=*second;
  *second=temp;
}

void Circle(Word x, Word y, Word radius, Byte color)
{
  int a, af, b, bf, target, r2, asp;

  if(Res==High_Res)
    asp=100;
  else
    asp=120;
  target=0;
  a=radius;
  b=0;
  r2=Sqr(radius);
  while(a>=b)
  {
    b=Round(sqrt(r2-Sqr(a)));
    Swap(&target,&b);
    while(b<target)
    {
      af=(asp*a)/100;
      bf=(asp*b)/100;
      Plot(x+af, y+b, color);
      Plot(x+bf, y+a, color);
      Plot(x-af, y+b, color);
      Plot(x-bf, y+a, color);
      Plot(x-af, y-b, color);
      Plot(x-bf, y-a, color);
      Plot(x+af, y-b, color);
      Plot(x+bf, y-a, color);
      ++b;
    }
    --a;
  }
}

void Draw(int xx1, int yy1, int xx2, int yy2, Byte color)
{
  int LgDelta, ShDelta, Cycle, LgStep, ShStep, dtotal;

  LgDelta=xx2-xx1;
  ShDelta=yy2-yy1;
  if(LgDelta<0)
  {
    LgDelta=-LgDelta;
    LgStep=-1;
  }
  else
    LgStep=1;
  if(ShDelta<0)
  {
    ShDelta=-ShDelta;
    ShStep=-1;
  }
  else
    ShStep=1;
  if(ShDelta<LgDelta)
  {
    Cycle=LgDelta>>1;
    while(xx1!=xx2)
    {
      Plot(xx1, yy1, color);
      Cycle+=ShDelta;
      if(Cycle>LgDelta)
      {
	Cycle-=LgDelta;
	yy1+=ShStep;
      }
      xx1+=LgStep;
    }
    Plot(xx1, yy1, color);
  }
  else
  {
    Cycle=ShDelta >> 1;
    Swap(&LgDelta, &ShDelta);
    Swap(&LgStep, &ShStep);
    while(yy1 != yy2)
    {
      Plot(xx1, yy1, color);
      Cycle+=ShDelta;
      if(Cycle>LgDelta)
      {
	Cycle-=LgDelta;
	xx1+=ShStep;
      }
      yy1+=LgStep;
    }
    Plot(xx1, yy1, color);
  }
}

int   CentreX, CentreY;

void Init_Graphics(Byte Mode)
{
  switch(Mode)
  {
    case 19  : Res=Low_Res;
	       MaxXRes=320;
	       MaxYRes=200;
	       break;
    case 45  : Res=Medium_Res;
	       MaxXRes=640;
	       MaxYRes=350;
	       break;
    case 120 : Res=Medium_Res;
	       MaxXRes=640;
	       MaxYRes=400;
	       break;
    case 46  : Res=Medium_Res;
	       MaxXRes=640;
	       MaxYRes=480;
	       break;
    case 48  : Res=Medium_Res;
	       MaxXRes=800;
	       MaxYRes=600;
	       break;
    case 56  : Res=High_Res;
	       MaxXRes=1024;
	       MaxYRes=768;
	       break;
    default  : printf("Mode %d is not a valid graphics mode\n\n",Mode);
	       puts("Hit any key to exit");
	       getch();
	       exit(1);
	       break;
  }
  MaxX=MaxXRes-1;
  MaxY=MaxYRes-1;
  if(XRes==0)
    XRes=MaxXRes;
  if(YRes==0)
    YRes=MaxYRes;
  Calc_Offsets();
  if(Res==Medium_Res)
    Pre_Calc_Y2=farcalloc(768, sizeof(DWord));
  else
    Pre_Calc_Y1=farcalloc(768, sizeof(Word));
  CentreX=XRes/2;
  CentreY=YRes/2;
  Asp=(1024.0/768.0)*((float)YRes/(float)XRes);
  Pre_Calc();
  Set_Mode(Mode);
  Init_Palette_2(Color);
  Set_Palette(Color);
}

void Set_Graphics_Mode(Word xRes, Word yRes)
{
  XRes=xRes;
  YRes=yRes;
  if((XRes<321) && (YRes<201))
    Init_Graphics(19);
  else if((XRes<641) && (YRes<351))
    Init_Graphics(45);
  else if((XRes<641) && (YRes<401))
    Init_Graphics(120);
  else if((XRes<641) && (YRes<481))
    Init_Graphics(46);
  else if((XRes<801) && (YRes<601))
    Init_Graphics(48);
  else
    Init_Graphics(56);
}

void Wait_For_Key()
{
  char k;

  while(!(k=getch()));
}

void Exit_Graphics()
{
  sound(1000);
  delay(500);
  nosound(•–   ‡™˜€†”—”Ÿ   •"”›˜…˜‚›€¢‚¨…««”¨•¡•¢›   ‚ „€„ˆ‚ˆ‚€€€€ˆ‚ˆ‚„ƒƒ€€‰   ‚ €€€„„„„€‚€‚„€ƒƒ€€€ÿ‰   &««€€›£   €€«€"‚¨…««“¨”¢”˜™   '„ª†««©¥£Š¢‡¡€™™•   € €““€€€€‚‚€ƒƒ€„„€……€† †“‡‡€ˆˆ€‰‰€ŠŠ€‹‹€Œ Œ“€€   ®µ•!  MAKEFILESVG           PTp•ù  NOTES16 SVG           &tC•”  NOTES256SVG           PFt	•9  NOTES32KSVG           d‹†
•|	  GRVGA256TGI           ¼¤•‡  TPX     TP            2¬±•ñ  TURBO   TP            Ù«±•Í  TURBO   TPH            8c•r±
 $ÃÜmXîİŞÿ·Âh)ğy:n÷Ñré`’.~ˆ¸ñmÒ¾¼— Ï¶±À¢®ß^I à¤wl‘m K°>ˆ´ÚC7×’pışKj;á4ø
N”¶W`~BÓ]ãàÉ"ıSBÛuNø¤3}y.ƒí§mÑÀ2Fú‰ °c$D‚äÆŸ\NØáxÑPx‚ãS”’,)b‚O2ÑfŠ¥tO´‚ä)ü¸6Ø=
Ê–Ó¶®=„Te	q- Jé,DŒ±âYîÅ|âú"Jçòø¤ m;lår!4ûD7l·åt9tU)‡ ¹
Nˆ;n‚*whwmOaöÓ¶£’D#ÿãÄ´Ó°òšruQEòLôÓ€FM°
Ç`:­ƒñôò­}!%şÓC¾ı¢#’–)I/RàN¶iNçÀ‡8ûiÛ.®™Ô2”oôÆš6vÙÇ%+(‰e:‹]4êJ7Eì_'ÛQH¾ú"pæB0¥Hfúò\
N§Üò9(,¶ ÑÊt$€utJÍ4êJ7zy ¢.‹j)Fè½‚2N$)iº'ÕÓM§	ğ|Éuµfª=ÑÏÓx@(‹MÑ:QÓ…HcMBRàÛgÆ‰Ü)DZ¹şÑüàD‹pDPHZm8Oƒæ}´íÓ¨e(İ°FZÿ@ˆ´V1€tÓ’‚Ë`,§BÑ:C¶¬€óRr±¢ñğ"B¤sCÛ!”)mtİ¤9À)zÈI†…)mƒ¸Ë–L¨‚¾ÅTÃÉB“4WG˜¾Z(ß$¶YHæ
T†o¯%À0¤àJ}Ï*ß–‹º,9)YDK(Ğ­–h£MpæB0¥Hfúò\
Nwº^¢	ƒm½şÙĞèiS„ø>eù|$<8);3MMb^8¾\x¶ëŸ'œ‚ª[NÙG†«Ñ ¦„ÔViß^m§l­ùno9)YDK(Ğ‚–«Zjk%ˆ´×* ñÔ¶·G("y¨¬¿GHfúóm;eÂ9dóÆ2ÑTö±|´×o¢#’•”D²	À<y”Û+@ìèt4©Â|2ü„ğãí§l´§ Ë`Óèê’ÛeÊ0åleJ¾M·HuHÂßCuHÙ†µÍ‘îmw6ï\üX†*ÀpÊ!†7rÉÛ¨ Û``9#³3ê™ÛCrã”£Æ'×’L$«A›ÛŞrÎQçİ:”€Œq	Cy’°Íõäµß¶¿@8ò­ùh£Gˆñ‰õä„µZàÛlŠk%ˆ´×* ñÔ¶·G(côt†o¯$cˆJÎÚvËJr¶GxTaÍleJ¾M·GuHÙ†"ê‘¹]'k›w®mw6\üZ;+ÃúA²a”B8nå“¶äÔ]FÈìË¬Ñçméú¤¸ÆNFÅòe8Z 1äpæw<I+jaÜx¶ÎòyÈ&Á’aÚk•xO%§MÁ’àËE€²¿cmYµmğÊpµHz{üI+jaÛàl‰çWP„y
…mĞEÂ–¢ ~e´í½3‡3•v¨œ.%£ıËÎ¦¾àsÏ¶°alÆš<í€*pæT@<y†* \óÏ*ò{ / <êò / 1N€¼€ÖTA}Ğ[t2†yÑ¨‚ÙAm;`ñ—7Ìí• aŠˆƒì/ 0ÅDÁ«yåAmX¡‡újàËŠ¼±>ƒÄ¾ˆ´Ô€CƒN®yS;j*€x>ÂHdı=¶šËküe¦†y’Ù~¡šhÙÎ®¹WEš)t×. pc)ÂÖÊØ=Cm;eú†:gg*§¿Hl±n2Ç$	³Ê·ä€mX¡
Êpµpd€ø´]–BpP0Áo-–İ(¡‰AÁ’àÏ¶·pİ¶r†è³Me8Z °q)CC~b:Q|´P•Ú¥+e¥¤S
Q|¶½èí/'ÛNÛ¸:Ùb†è²ˆ–S¡mSWƒƒN¼fØË)£¥…,•ÁöÕÅCüÊ—ø)QÊt$£§«ÙÒÉ\pc)Â×ŒÛöÓ¶î¶PÃƒ8Ärƒ‚O2ú"e8Z¤oÈTD²¿ëKàÉÁ¬ş%ó÷×˜í¡õáæ3í§mÜ)ÛW9ğpæ‹ä™¥sRÅq@myEÀ¥>ÚvÏ5•_vÙÊpO¦¤=+î/“–EòÓRe8Z y ¢.m¨¨bWDĞºõR‡2œ-P
TD²	Ïçe8Z 5²Ò†Fûƒ*Ãî2DNÒ€¥>ÚvÏ:5[-‚¨aÁ>š•³è‹Giy>ÚvËJr¶
èj’9²å!leJ¾M·Nê‘¹]EÕ#}'k›®mw6G¹ø·ƒdÃ3]pİÊ!†7rÉÛeS"0æÀÀ Í‘œ?;e§L81”ájàÉ ğdÿAm1¦NØg‰CdO:º˜IïÒ)êø2:ªå] ­¶À481”ájàÉ ğe¢ÀY_±-«o€àÆS…ªç“¦Ê0µ¶{@<I9U/Õ)†* 4:‘åI–Ï„†NJHƒ$ĞëNÃ8}”\Uå‰ô9…ËƒmºHpc)ÂÕÁ’àÏ¶±Ãpærª{ô†Ê@aŠˆƒMËE‰Ümœ¨€x4Ğé¿!i¡ÁŒ§WHƒ	#'* ñCkeş‹]41Q ği¡ÒEò€w"2Ó]ß‡2œ-i­«y:hƒ$ĞğF[JI¡÷Ú‹åDH80¼¼¼y=Ê
# àÆQ…­¶}Á’àÉİ÷…¢„÷é]%'Ù Vq$Á’9öÓ¶F¬ú‹òVÏ¢$„ÂÓ'lrå+gÑBa!é®ÿ!o‡sH€ğm°+gÑpc)ÂÕ#h«¹ ºNšVÏ¢$„ÂF‚;`Dt°y–šsÆåX$-ğ6Dó«¨B<–üËMã!lpæA[B–ˆÑšáM!é©lKGş2@y¶Â¤)h¾–2”’)CO¶·I9â¸aÌcO§NÙ~ˆ´WE<P¤×@M8s9UËw.œlrú¹V	|ù<ä„yñ]Ï‡ı¶ÎŞú}y-ğÊpµH~2ã.WDy-ù•/ğRÚğg‹mX‚¶…("'©Y¦¼ƒÆ<dÄ¾ˆ†ŞÂ9 ŒAã2DƒƒF¶ÀFHO#SB¡RÔZhàeõ„TªŸĞÌÊ÷6\ü==Œû²Òœƒ-€ëŠ’9²å•leJ¾M·HuHÂßCuHÙ†µÍ‘îmw6ï\üX»!Š‡C2¼2ˆGaÜ²vÀj/ƒl¹HìËäúvÙ'=C1’\Ø$ó)KGì81”ájàÉ ğf4ÕçlrûN]tĞx†ãT–Hˆl‰çW\3äórÚ•’Û‚Õ’‡2œ-P>ÚvŞÇLl‰çWS	Ô.ššÃ½TªÔ¶±êHdQY¤\« £ şü:¨~× 1ÚÑ‚¯//[NÙ~ãª•mÓ\ ƒƒPÀôS<jèRÕF+ö%µn¶Îe8Z y:c>O9Âu¦¦°áÍJ­KiÛ#T0¢²€Y~Œwô€Çj?F
:¼¼¼@aµ<í§löãª´×Hƒ-&¾ÓP\Uå‰ô6Ë(‰——'Lì·—¢ÿ¢Í5 ğeƒ‰Jğ#Ò‹å¢„vÏ„!JEH¦y:`k*/Ëyni©Jû_”_.¶Í2œ-P>ÚvÕS¡À˜É.ly’h]z©CƒN®cOÕ;c—Úrèû¦ƒÄ7¤²DCdO:ºàAŸ'œƒ–Ô¬–Øà¢Ä×ÛC‡2œ-R2aÛC¸'CÆ}´í‡˜ÙÎ®¦¨]55‡z©[j[NÙpPÇ@„QY–7˜™cy€tı¯@cµ£^^^¶
¡¢²
:ïÊÌQ†‚ƒ?kĞíGèÁGW—»-§mœ÷((Œœª•mÓ\ ´X+ö%µm2œ-P<©¶gÉç ˜N¡tÔÖ9©U©m;lªQY@
¬¿F
;ˆú@cµ£^^^ 1ÚÑ‚¯/vmJà¶µcŒrªzpJàÉğòtÒ@<&¾ÓPlÅ^XŸAâTDƒƒËËŞÂÓvT×ÚjàËŠ¼±>ƒÄ¨‰—»'LeJËlíåè¿è³MH81”áj€xR¢$^^^
Z.pq$‡Ö –ÙšnÊÊpµH~‹œI!ùÇÛNÙiNA–ÀaG© ÙºØÊ•|›nê‘²EÕ#rºN×6ï\Úîl"¹ø´vÏCúA—pÊ!†7rÉÛ-)‡›‘Ù°é÷NØõHqŒ‡qŠXq-¡”Æá3|R¶°alÆš<í°Õ¿-›¢ù8ZJ\-Â–Š4ÔÖq‡ê[NØ9CÍŞ@`4äò@>¥’ 
ıyÃ|©Aä€ C?Al&(g—’õ(< THgè(-§l÷[òĞÏM;7hÀ˜²¿A&%´2˜ô¡íÛW(?àRÑFššÎ0ıKiÛt‚†›¼€ÖôäAm;`ñ—7Ìíºß$êPy  ¨ÏÛ,PÃ‚}5 ğ¥Nàù…''?ËUŠèóË~¶èeÓx[Ÿm;g€ÁcLE;nà0lå8'ÑaÙ¹8X$‚(%æN¯TG“³Zh	o.VË3E\#–©e§jû;7|@ˆÍĞFIÂèåÁm;n‚ `$êPy  ¨ÏÛ/Ô2ïÏ1FZ*•ˆPáHïÏ4çÂ•9B•+èDiP[• –À*KÄ…-Z?R“³dìİ«x¾Zjah¯éöÓ¶è"ò@>¥’ 
ı²ÅÁÁ1”¤ìÙ;7jŞ/–š˜Z+Àº¸«'Öá|¶è"¡š*•ˆF}´í„àb^HÔ ò@Py!Ÿ ¶ÎPÀÌcJQ¾¿?J™›µoËML-à]
\U“ëp¾[V(fŠ¥b@(‹EG~IÂèºìÿÑš…	
CfëjåC4~´İ[ÅòÓxTGÛNÛ¸omEC¯²FGfè$'y¸_ú_÷èŠ—ŒòvkM[–òö¼àVÛÑCºJI -%8WÊfvn‚B¥ã>ÚvËJr¶
‡êH6nË”l…±•*ù6İ;ªFåt1TŒ-ô0®l"¹µÜÙçâŞ»†fºáÊ!†7rÉÛPL7``Gf&ù²¤D(Áùpìd6n1¦'°f#Æ! ËEqe~‚LKMLz ÆÍÁ$d—üø2Ğ-°p¥¦›´Nîzºã°â:[C)‘2ÛªwÅ! $¥´íºÓ¨üwÒp'iÛ>L"M­§mÕ&
pãò¤$¶·N0ˆSÄp~Bú"¢%”hVÓ¶xEGü
-§mÒbâ©Z¶°øaÀ(m;h|Â%ÚÒá­§m˜Dr§†¶¶hÂ èš.ÚvÇ˜E[ˆ7è$–,rÒ	–8¥ç—@V«D‡¹…Â¦Úvİ aÀËiÛuÁÙ¹nû_†À1§Ò¶9ÿSYe)9ùÃÏÑ›H8ËEôü„6n	#¶‡ß¥‚BáQT¶³ÀlËlå9c‚|)hG…h)8ö2áZöÂÕn!¶[¨b|)hA¸‘G°’õøV½„£èTö€ÜAÛ/(e#›ØN~–!;’p 'ÛNØ
0›[,PÃ–JOÒ¼—Ñ›«bW.¬Z‡QôÌ,ÓR¹ÕÚën¸PÅÛÑ{8¢ùv/+½3¶²8§ î?¶X¡‚B?–ø×©”é!‡Â’4)8…kØZõ+p‘pvÜ…p%"a8İ¾½JûiÛŸØñCƒ	m°ÊrÄ#.„7d#>ÚvİR!@'–($ó!…m¨¨aË4Ô„İí¸0¤‘}ôEDK)Ğ \….¶ÎPÄ‡é®ëhç¡
Zk»t£xˆ-¶b†8MHL;UÙ»}n3í§lGü
-—ê$#øi´D²	  ğeÀA…$€> ¤‘bëÌVÏja÷â$¼k@#Ñ5˜H-IÀ‰S„8¾\l§yÃ›è‹M@Ÿm;ağCeè;+ÌXƒÛ9CY¦ùÊı‰Êt´@R±õä¢ùmç+ö$ásğe\çP„±Ÿm;e@àÀ8*˜Çmœ¡‡,ÓwİÀ‰[>ˆk!üPÒp <_¿$€ÇÛNÙò²ZPßƒŠ;gµ9fš‘WôDåt ŒGI/ÿ†
T‚[+¨`ï9%~€PĞŞd“4İ¢$ €©öÓ¶|œ©†üQÛQPÃ–i©qîwn÷ççÙ –(?$dšôEJ½²İC1-ÿœ“4İ¢$ €©öÓ¶‰¢ŠXŸc¶Ï(aË4×o¢-5*ˆz&£>ÚvĞıhô¡ ÿæWèí³”0åšj8RH¾‚BArœ	¾ÿDICDè-¶Á"†A!%s,E¦î@ìİ¾·™öÓ¶ †ÁTÄvÙÊrÍ5/ôDåwI1)b-7rfî=kg‚†pŸm;e¥9[ *èj’›²å•leJ¾M·HuHÂßCuHÙ†µÍ‘îmw6ï\üXÅC¡›=¢ÀXcw,¶U2#fì¹HìÙÃó¶è"% â‚O2ÓNÍÜ	Ìiô¤í€ (‹Exq(¾X²¿A&%¢ôE¦í¦7	œx¶Xà€åĞ•x$ó#‚4Ç_Ab!'×”Š‰b© .1…-ÜØã7ÛgôÓ³v‰ğy–¯¦ššÉb-5Ê¼5-§l"PÇèÇm§ltÎÍÚ'M},„—üÑbypDXË2JÍ‹åJ6Ùè;ŒRÀˆém¦7	›â€}´íº@à9K…¹>©¶rßb@´«Á'™¡ ş:ú	>¼¤TKuHqŒ)h«•§IIVÀ.C„4a°ê×±éîm:…”‡qŠXq-¡[¸LãÅ°Âéå[òùp<-rµªÖššÉb-5Ê¼5-§mÒŠıé¼•! ø‡Ø­§lsş‡hÃaÕ¯cÓÜÉ…¹°ápÓS„ÍñH@>ÚvÏ€E¢º<›¢ù8ZJC¸Å,¸–­Ü&o‡p+l÷Àóˆéiº·‹äİÉXË|!C… _¿<ÓŸl¿§(Rp#È J`RÑ]ÍV´ÔÖKi´ªÛ¸j[NÛ¥1ú1ÒDT„âb¶¶yÓ;7hŸEÎˆ:*•šjŸm;e¥9[ ¢|ÔaâØÊ•|›n¸ê‘€CuHÜ®†µÍŒ¹µÜØ