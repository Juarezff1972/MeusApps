/////////////////////////////////////////////////////////////////////////////
//   PROJETO: Disassemblador
/////////////////////////////////////////////////////////////////////////////
//   FONTE  : DISASTRE
/////////////////////////////////////////////////////////////////////////////
/*   EQUIPE : Aderson Cleber Pifer      Nง  1
//            Anderson F. Martins       Nง  3
// 	        Gustavo H. Busnardo       Nง  9
//			     Juarez Fioravanti Filho   Nง 13
//            Ricardo Ramos Leito      Nง xx
/*///////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
///////////////////////////////////////////////////////////////////////////
FILE *fp;
struct ffblk s;
int reg,r_m;
int post,achar;
char *TabReg16[]={"AX","CX","DX","BX","SP","BP","SI","DI"};
char *TabReg8[]={"AL","CL","DL","BL","AH","CH","DH","BH"};
char *TabSeg[]={"ES","CS","SS","DS"};
char *Mem[]={"[BX+SI+DISP]","[BX+DI+DISP]","[BP+SI+DISP]","[BP+DI+DISP]",
             "[SI+DISP]","[DI+DISP]","[BP+DISP]","[BX+DISP]"};
unsigned char opcode=0;
int tamanho;
void LeByte();
void LeOpc();
///////////////////////////////////////////////////////////////////////////
void LeOpc()
{
	switch(opcode)
	{
     //================================mov===============================
	  case 0x88: //mov reg/mem p/ reg (byte,d=0)
	  case 0x89: //mov reg/mem p/ reg (word,d=0)
	  case 0x8A: //mov reg/mem p/ reg (byte,d=1)
	  case 0x8B: //mov reg/mem p/ reg (word,d=1)
	  {

         r_m=post&0x0007;
         reg=post&0x0007;
			printf("mov %s,%s\n",TabReg16[r_m],TabReg8[reg]);

			break;
	  }
	  case 0xC6: //mov imedi p/ reg|mem (byte)
	  case 0xC7: //mov imedi p/ reg|mem (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mov %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0xB8: //mov imedi p/ reg (ax,word)
	  case 0xB9: //mov imedi p/ reg (cx,word)
	  case 0xBA: //mov imedi p/ reg (dx,word)
	  case 0xBB: //mov imedi p/ reg (bx,word)
	  case 0xBC: //mov imedi p/ reg (sp,word)
	  case 0xBD: //mov imedi p/ reg (bp,word)
	  case 0xBE: //mov imedi p/ reg (si,word)
	  case 0xBF: //mov imedi p/ reg (di,word)
	  {

         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mov %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0xB0: //mov imedi p/ reg (al,byte)
	  case 0xB1: //mov imedi p/ reg (cl,byte)
	  case 0xB2: //mov imedi p/ reg (dl,byte)
	  case 0xB3: //mov imedi p/ reg (bl,byte)
	  case 0xB4: //mov imedi p/ reg (ah,byte)
	  case 0xB5: //mov imedi p/ reg (ch,byte)
	  case 0xB6: //mov imedi p/ reg (dh,byte)
	  case 0xB7: //mov imedi p/ reg (bh,byte)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mov %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
	  case 0xA0: //mov mem p/ acum (byte)
	  case 0xA1: //mov mem p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("mov %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0xA2: //mov acum p/ mem (byte)
	  case 0xA3: //mov acum p/ mem (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mov %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0x8E: //mov reg|mem p/ seg reg
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mov %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
	  case 0x8C: //mov seg reg p/ reg|mem
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("mov %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //================================fim mov=============================

    //================================push================================
	  case 0xFF: //push reg p/ mem
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("push %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0x50: //push p/ reg (ax)
	  case 0x51: //push p/ reg (cx)
	  case 0x52: //push p/ reg (dx)
	  case 0x53: //push p/ reg (bx)
	  case 0x54: //push p/ reg (sp)
	  case 0x55: //push p/ reg (bp)
	  case 0x56: //push p/ reg (si)
	  case 0x57: //push p/ reg (di)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("push %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0x06: //push p/ seg reg (al)
	  case 0x0E: //push p/ seg reg (cl)
	  case 0x16: //push p/ seg reg (dl)
	  case 0x1E: //push p/ seg reg (bl)
	  case 0x26: //push p/ seg reg (ah)
	  case 0x2E: //push p/ seg reg (ch)
	  case 0x36: //push p/ seg reg (dh)
	  case 0x3E: //push p/ seg reg (bh)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("push %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
    //==============================fim push===============================

    //================================pop==================================
	  case 0x8F: //pop reg p/ mem
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("pop %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0x58: //pop p/ reg (ax)
	  case 0x59: //pop p/ reg (cx)
	  case 0x5A: //pop p/ reg (dx)
	  case 0x5B: //pop p/ reg (bx)
	  case 0x5C: //pop p/ reg (sp)
	  case 0x5D: //pop p/ reg (bp)
	  case 0x5E: //pop p/ reg (si)
	  case 0x5F: //pop p/ reg (di)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("pop %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0x07: //pop p/ seg reg (al)
	  case 0x0F: //pop p/ seg reg (cl)
	  case 0x17: //pop p/ seg reg (dl)
	  case 0x1F: //pop p/ seg reg (bl)
	  case 0x27: //pop p/ seg reg (ah)
	  case 0x2F: //pop p/ seg reg (ch)
	  case 0x37: //pop p/ seg reg (dh)
	  case 0x3F: //pop p/ seg reg (bh)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("pop %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
     //=============================fim pop=================================

    //=============================xchg====================================
	  case 0x86: //xchg reg/mem p/ reg (byte)
	  case 0x87: //xchg reg/mem p/ reg (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("xchg %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0x90: //xchg reg c/ acum
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("xchg %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
    //================================fim xchg===============================

    //================================in=====================================
	  case 0xE4: //in fred port (w=0)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("in %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
	  case 0xE5: //in fred port (w=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("in %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0xEC: //in variable port (w=0)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("in %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
	  case 0xED: //in variable port (w=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("in %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //================================fim in=================================

     //==================================out==================================
	  case 0xE6: //out fred port (w=0)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("out %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
	  case 0xE7: //out fred port (w=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("out %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0xEE: //out variable port (w=0)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("out %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
	  case 0xEF: //out variable port (w=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("out %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==================================fim out==============================

     //==================================xlat=================================
     case 0xD7: //traduz um byte no reg AL
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("xlat %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
    //=================================fim xlat==============================

    //=================================lea===================================
	  case 0x8D: //transfere uma palavra p/ reg(16 bits)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("lea %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim lea===============================

     //=================================lds===================================
	  case 0xC4: //transfere quatro bytes p/ reg(16 bits,DS)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("lds %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim lds===============================

     //=================================lahf===================================
	  case 0x9F: //carrega os bits 7,6,4,2,0 do reg AH
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("lahf %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
     //=================================fim lahf===============================

     //=================================sahf===================================
	  case 0x9E: //altera o conteudo dos flags S,Z,A e P
		     //usando os bits transferidos do reg AH
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("sahf %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
     //=================================fim sahf===============================

     //=================================pushf==================================
	  case 0x9C: //decrementa o ponteiro de pilha de 2 unidades e
		     //transfere o conteudo do reg de flags p/ o topo da pilha
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("pushf %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim pushf=============================

     //=================================popf==================================
	  case 0x9D: //transfere a palavra no topo da pilha
		     //p/ reg de flags.Sp e incrementado de 2 unidades.
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("popf %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim popf===============================

     //================================add===============================
	  case 0x00: //add reg/mem p/ reg (byte,d=0)
	  case 0x01: //add reg/mem p/ reg (word,d=0)
	  case 0x02: //add reg/mem p/ reg (byte,d=1)
	  case 0x03: //add reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("add %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0x80: //add imedi p/ reg|mem (byte)
	  case 0x81: //add imedi p/ reg|mem (word)
	  {

       //
         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("add %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0x04: //add ime p/ acum (byte)
	  case 0x05: //add ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("add %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim add================================

     //=================================adc====================================
	  case 0x10: //adc reg/mem p/ reg (byte,d=0)
	  case 0x11: //adc reg/mem p/ reg (word,d=0)
	  case 0x12: //adc reg/mem p/ reg (byte,d=1)
	  case 0x13: //adc reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("adc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0x14: //adc ime p/ acum (byte)
	  case 0x15: //adc ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("adc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim adc================================

//===================================inc==================================
	  case 0xFE: //inc reg p/ mem
	  //case 0xFF: //inc reg p/ mem
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("inc %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0x40: //inc imedi p/ reg (ax,word)
	  case 0x41: //inc imedi p/ reg (cx,word)
	  case 0x42: //inc imedi p/ reg (dx,word)
	  case 0x43: //inc imedi p/ reg (bx,word)
	  case 0x44: //inc imedi p/ reg (sp,word)
	  case 0x45: //inc imedi p/ reg (bp,word)
	  case 0x46: //inc imedi p/ reg (si,word)
	  case 0x47: //inc imedi p/ reg (di,word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("inc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim inc================================

     //===================================aaa==================================
	 /* case 0x37: //aaa reg p/ mem
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("aaa\t%s\n",TabReg8[reg]);

			break;
	  }*/
    // =================================fim aaa=================================

     //===================================baa==================================
	  /*case 0x27: //aaa reg p/ mem
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("baa\t%s\n",TabReg8[reg]);

			break;
	  } */
     //=================================fim baa================================*/

     //===================================sub==================================
	  case 0x28: //sub reg/mem p/ reg (byte,d=0)
	  case 0x29: //sub reg/mem p/ reg (word,d=0)
	  case 0x30: //sub reg/mem p/ reg (byte,d=1)
	  case 0x31: //sub reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("sub %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  case 0x82: //sub imedi p/ reg|mem (byte,s=1)
	  case 0x83: //sub imedi p/ reg|mem (word,s=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("sub %s,%s\n",Mem[reg],TabReg8[reg]);

			break;
	  }
	  case 0x2C: //sub ime p/ acum (byte)
	  case 0x2D: //sub ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("sub %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim sub=================================

     //===================================ssb==================================
	  case 0x18: //ssb reg/mem p/ reg (byte,d=0)
	  case 0x19: //ssb reg/mem p/ reg (word,d=0)
	  case 0x20: //ssb reg/mem p/ reg (byte,d=1)
	  case 0x21: //ssb reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ssb %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     /*case 0x82: //ssb imedi p/ reg|mem (byte,s=1)
	  case 0x83: //ssb imedi p/ reg|mem (word,s=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ssb\t%s\n",Mem[reg]);

			break;
	  }*/
	 /* case 0x0E: //ssb ime p/ acum (byte)
	  case 0x0F: //ssb ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("ssb\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //=================================fim ssb=================================

     //===================================dec==================================
     /*case 0xFE: //dec reg p/ mem(w=0)
	  case 0xFF: //dec reg p/ mem(w=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("dec\t%s\n",Mem[reg]);

			break;
	  } */
	  case 0x48: //dec p/ reg (ax)
	  case 0x49: //dec p/ reg (cx)
	  case 0x4A: //dec p/ reg (dx)
	  case 0x4B: //dec p/ reg (bx)
	  case 0x4C: //dec p/ reg (sp)
	  case 0x4D: //dec p/ reg (bp)
	  case 0x4E: //dec p/ reg (si)
	  case 0x4F: //dec p/ reg (di)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("dec %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim dec================================

     //===================================neg==================================
	  case 0xF6: //neg ,subtrai operando destino de 0, armazena o
		     //resultado em um complemento de dois no destino.(w=0)
	  case 0xF7:   //(w=1).
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("neg %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
//=================================fim neg================================

//===================================cmp==================================
	  case 0x38: //cmp reg/mem p/ reg (byte,d=0)
	  case 0x39: //cmp reg/mem p/ reg (word,d=0)
	  case 0x3A: //cmp reg/mem p/ reg (byte,d=1)
	  case 0x3B: //cmp reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cmp %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	  /*case 0x80: //cmp imedi c/ reg|mem (byte,s=0)
	  case 0x81: //cmp imedi c/ reg|mem (word,s=0)
	  case 0x82: //cmp imedi c/ reg|mem (byte,s=1)
	  case 0x83: //cmp imedi c/ reg|mem (word,s=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cmp\t%s\n",Mem[reg]);

			break;
	  }*/
	  case 0x3C: //cmp imedi c/ acum (byte)
	  case 0x3D: //cmp imedi c/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cmp %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim cmp===============================

//===================================aas==================================
	  /*case 0x3F: //aas reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("aas\t%s\n",TabReg8[reg]);

			break;
	  } */
//=================================fim aas=================================

//===================================das==================================
	  /*case 0x2F: //das reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("das\t%s\n",TabReg8[reg]);

			break;
	  }*/
//=================================fim das=================================

//===================================mul==================================
     /*case 0xF6: //das reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mul\t%s\n",TabReg8[reg]);

			break;
	  }
     case 0xF7: //das reg (AH)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("mul\t%s\n",TabReg8[reg]);

			break;
	  }
     //=================================fim mul=================================

     //===================================imul==================================
     case 0xF6: //imul reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("imul\t%s\n",TabReg8[reg]);

			break;
	  }
     case 0xF7: //imul reg (AH)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("imul\t%s\n",TabReg8[reg]);

			break;
	  } */
     //=================================fim imul================================*/

     //===================================aam==================================
	  case 0xD4: //aam reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("aam %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
     //=================================fim aam=================================

     //===================================div===================================
	 /* case 0xF6: //div reg (AL,AH) se (byte)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("div\t%s\n",TabReg8[reg]);

			break;
	  }
	  case 0xF7: //div reg (AX,DX) se (word)
	  {


         r_m=post&0x0007;reg=opcode&0x0007;
			printf("div\t%s\n",TabReg16[reg]);

			break;
	  }
     //=================================fim div=================================

     //===================================idiv===================================
	  case 0xF6: //div reg (AL,AH) se (byte)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("idiv\t%s\n",TabReg8[reg]);

			break;
	  }
	  case 0xF7: //idiv reg (AX,DX) se (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("idiv\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //=================================fim idiv=================================*/

     //===================================aad==================================
	  case 0xD5: //aad reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("aad %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
//=================================fim aad=================================

//===================================cbw==================================
	  case 0x98: //aad reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cbw %s,%s\n",TabReg8[reg],Mem[reg]);

			break;
	  }
//=================================fim cbw=================================

//===================================cwd==================================
	  case 0x99: //cwd reg (AL)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cwd %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim cwd=================================

     //===================================not===================================
     /*case 0xF6: //not inverte os bits
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("not\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //=================================fim not=================================

     //=================================shl/sal=================================
	  case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("shl/sal %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //===============================fimshl/sal===============================

     //=================================shr====================================
   /*case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0005;
			printf("shr\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //===============================fim shr==============================

     //=================================sar================================
   /*case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0005;
			printf("sar\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //===============================fim sar==============================

     //=================================rol================================
   /*case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;reg=opcode&0x0005;
			printf("rol\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //===============================fim rol==============================

     //=================================ror================================
   /*case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0005;
			printf("ror\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //===============================fim ror==============================

     //=================================rcl================================
   /*case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0005;
			printf("rcl\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //===============================fim rcl==============================

     //=================================rcr================================
	/*case 0xD0:
	  case 0xD1:
	  case 0xD2:
	  case 0xD3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0005;
			printf("rcr\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //===============================fim rcr==============================

     //================================and===============================
  /* case 0x20: //and reg/mem p/ reg (byte,d=0)
	  case 0x21: //and reg/mem p/ reg (word,d=0)
	  case 0x22: //and reg/mem p/ reg (byte,d=1)
	  case 0x23: //and reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("and\t%s\n",TabReg16[reg]);

			break;
	  }
	  case 0x80: //and imedi p/ reg|mem (byte)
	  case 0x81: //and imedi p/ reg|mem (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("and\t%s\n",Mem[reg]);

			break;
	  }*/
	  case 0x24: //and ime p/ acum (byte)
	  case 0x25: //and ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("and %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim and================================

     //================================test==============================
	  case 0x84: //test reg/mem p/ reg (byte,d=0)
	  case 0x85: //test reg/mem p/ reg (word,d=0)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("test %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
	/*case 0xF6: //test imedi p/ reg|mem (byte)
	  case 0xF7: //test imedi p/ reg|mem (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("test\t%s\n",Mem[reg]);

			break;
	  }*/
	  case 0xA8: //test ime p/ acum (byte)
	  case 0xA9: //test ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("test %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
    //=================================fim test================================

    //=================================or===============================
	  case 0x08: //or reg/mem p/ reg (byte,d=0)
	  case 0x09: //or reg/mem p/ reg (word,d=0)
	  case 0x0A: //or reg/mem p/ reg (byte,d=1)
	  case 0x0B: //or reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("or %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
   /*case 0x80: //or imedi p/ reg|mem (byte)
	  case 0x81: //or imedi p/ reg|mem (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("or\t%s\n",Mem[reg]);

			break;
	  }*/
	  case 0x34: //or ime p/ acum (byte)
	  case 0x35: //or ime p/ acum (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("or %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim or================================

//================================xor===============================
	/*case 0x30: //xor reg/mem p/ reg (byte,d=0)
	  case 0x31: //xor reg/mem p/ reg (word,d=0)
	  case 0x32: //xor reg/mem p/ reg (byte,d=1)
	  case 0x33: //xor reg/mem p/ reg (word,d=1)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("xor\t%s\n",TabReg16[reg]);

			break;
	  }
	  case 0x80: //xor imedi p/ reg|mem (byte)
	  case 0x81: //xor imedi p/ reg|mem (word)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("xor\t%s\n",Mem[reg]);

			break;
	  }
	  case 0x34: //xor ime p/ acum (byte)
	  case 0x35: //xor ime p/ acum (word)3
	  {


         r_m=post&0x0007;
         reg=opcode&0x000;
			printf("xor\t%s\n",TabReg16[reg]);

			break;
	  }*/
     //=================================fim xor================================

     //===================================rep==================================
	  case 0xF2:
	  case 0xF3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("rep %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim rep=================================

//===================================movs=================================
	  case 0xA4:
	  case 0xA5:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("movs %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim movs================================

//===================================cmps=================================
	  case 0xA6:
	  case 0xA7:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cmps %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim cmps================================

//===================================scas==================================
	  case 0xAE:
	  case 0xAF:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("scas %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//=================================fim scas================================

//===================================lods=================================
	  case 0xAC:
	  case 0xAD:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("lods %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim lods================================

     //===================================stos=================================
	  case 0xAA:
	  case 0xAB:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("stos %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim stos================================

     //===================================call=================================
	  case 0xE8:  //chamada dentro do segmento e direta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("call %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
  /* case 0xFF:  //chamada dentro do segmento e indireta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("call\t%s\n",TabSeg
			[reg]);

			break;
	  }*/
	  case 0x9A:  //chamada fora do segmento e direta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("call %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
   /*case 0xFF:  //chamada fora do segmento e indireta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("call\t%s\n",TabSeg[reg]);

			break;
	  }*/
     //=================================fim call================================

     //====================================jmp=================================
	  case 0xE9:  //chamada dentro do segmento e direta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jmp %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
   /*case 0xE9:  //chamada dentro do segmento e direta(short)
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jmp\t%s\n",TabSeg[reg]);

			break;
	  }*/
  /* case 0xFF:  //chamada dentro do segmento e indireta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jmp\t%s\n",TabSeg
			[reg]);

			break;
		}*/
	  case 0xEA:  //chamada fora do segmento e direta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jmp %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
   /*case 0xFF:  //chamada fora do segmento e indireta
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jmp\t%s\n",TabSeg[reg]);

			break;
	  }*/
     //=================================fim =jmp================================

     //====================================ret=================================
	  case 0xC3:  //chamada dentro do segmento
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ret %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
	  case 0xC2:  //chamada dentro do segmento mais immed. p/ SP
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ret %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
	  case 0xCB:  //chamada fora do segmento
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ret %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
	  case 0xCA:  //chamada fora do segmento mais immed. p/ SP
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ret %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim ret================================

     //===================================je|jz================================
	  case 0x74:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("je|jz %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim je|jz===============================

     //===================================jl|jnge===============================
	  case 0x7C:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jl|jnge %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jl|jnge=============================

     //===================================jle|jng===============================
	  case 0x7E:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jl|jng %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jl|jng=============================

     //==================================jb|jnae===============================
	  case 0x72:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jb|jnae %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jb|jnae=============================

     //=================================jbe|/jna================================
	  case 0x76:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jbe|jna %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jbe|jna=============================

     //==================================jp|jpe=================================
	  case 0x7A:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jp|jpe %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jp|jpe==============================

     //===================================jo====================================
	  case 0x70:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jo %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jo==================================

     //===================================js====================================
	  case 0x78:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("js %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim js=================================

     //=================================jne|jnz================================
	  case 0x75:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jne|jnz %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jne|jnz============================

     //===================================jnl|jge==============================
	  case 0x7D:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jnl|jge %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }

     //=================================fim jnl|jge=============================

     //==================================jnle|jg=================================
	  case 0x7F:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jnle|jg %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jnle|jg=============================

     //==================================jnb|jae=================================
	  case 0x73:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jnb|jae %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jnb|jae===============================

     //===================================ja|jnbe=================================
	  case 0x77:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("ja|jnbe %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim ja|jnbe==============================

     //==================================jnp|jpo=================================
	  case 0x7B:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jnp|jpo %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jnp|jpo==============================

     //===================================jno===================================
	  case 0x71:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jno %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jno=================================

     //==================================jns====================================
	  case 0x79:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jns %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim jns================================


     //==================================loop=================================
	  case 0xE2:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("loop %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //=================================fim loop================================

     //==============================loopne|loopnz==============================
	  case 0xE0:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("loopne|loopnz %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim loopne|loopnz==========================

     //=================================jcxz===================================
	  case 0xE3:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("jcxz %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim jcxz===============================

     //=============================loope|loopz=============================
	  case 0xE1:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("loope|loopz %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim loope|loopz==========================

     //=================================type speched==========================
	  case 0xCD:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("type %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim type speched==========================

     //=================================type 3=================================
	  case 0xCC:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("type %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim type 3================================

     //=================================iret===================================
	  case 0xCF:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("iret %s,%s\n",TabSeg[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim iret==================================

     //=================================clc====================================
	  case 0xF8:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("clc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim clc===================================

     //=================================cmc===================================
	  case 0xF5:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cmc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim cmc==================================

     //=================================stc===================================
     case 0xF9:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("stc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim stc==================================

     //=================================cld===================================
	  case 0xFC:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cld %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim cld==================================

     //=================================std===================================
	  case 0xFD:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("std %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim std==================================

     //=================================cli===================================
	  case 0xFA:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("cli %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim cli==================================

     //=================================sti===================================
     case 0xFB:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("sti %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim sti==================================

     //=================================hlt===================================
	  case 0xF4:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("hlt %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim hlt=================================

     //=================================wait===================================
	  case 0x9B:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("wait %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim wait================================

     //=================================esc===================================
	  case 0x1B:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("esc %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
     //==============================fim esc=================================

     //=================================lock===================================
	  case 0xF0:
	  {


         r_m=post&0x0007;
         reg=opcode&0x0007;
			printf("lock %s,%s\n",TabReg16[reg],TabReg8[reg]);

			break;
	  }
//==============================fim lock=================================
	  default:  printf("\nEste cขdigo gera opcode no implementado!!!\n\n");

               break;
   }
}
/////////////////////////////////////////////////////////////////////////////
void main(int argc,char *argv[])
{
  char *nome_arq;

  if(argc!=2)
  {
	  printf("\nFormato inv lido. Use o formato: DISASTRE <Nome-do-arquivo>\n");
	  exit(1);
  }
  achar=findfirst(argv[1],&s,0);
  if (achar)
  {
     printf("Arquivo no encontrado\n");
	  exit(3);
  }
  if (!(fp=fopen(argv[1],"rb")))
  {
     printf("O arquivo no pode ser aberto\n");
     exit(2);
  }
  tamanho=s.ff_fsize;
  clrscr();
  cprintf("\rษอออออออออออออออออออออออออออออออออออออออออออออออออออป \r\n");
  cprintf("\rบ             Programa DISASSEMBLADOR               บ \r\n");
  cprintf("\rฬอออออออออออออออออออออออออออออออออออออออออออออออออออน \r\n");
  cprintf("\rบ                   Equipe Coyote                   บ \r\n");
  cprintf("\rศอออออออออออออออออออออออออออออออออออออออออออออออออออผ \r\n\n");
  LeByte();
  fclose(fp);
  printf("\n\n****************  FIM ******************\n");
}
/////////////////////////////////////////////////////////////////////////////
void LeByte()
{
   while(tamanho>0)
   {
      opcode=fgetc(fp);
      delay(100);
      LeOpc();
      tamanho--;
   }
}
/////////////////////////////////////////////////////////////////////////////
