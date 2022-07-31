                  switch(campo_escolhido->codigo)
                  {
                     case ENTRADA_NT :
                          switch(tecla1)
                          {
                            case ENTER :
                                 if (campo_escolhido->dado != NULL)
                                 {
                                    insere_lhs_nt(campo_escolhido->dado[0]);
                                    aux_lhs = funcao_de_disponibilidade (campo_escolhido->dado[0]);
                                    campo_escolhido = campo_escolhido->prox;
                                 }
                                 break;
                            case BACKSPACE:
                                 if(campo_escolhido->dado!=NULL)
                                 {
                                    mouse_off();
                                    setfillstyle(SOLID_FILL,fundo_tela);
                                    bar(campo_escolhido->x1,campo_escolhido->y1,
                                        campo_escolhido->x1+campo_escolhido->x2,
                                        campo_escolhido->y1+campo_escolhido->y2);
                                    campo_escolhido->dado = NULL;
                                    mouse_on();
                                 }
                                 break;
                            case TAB :
                                 if (campo_escolhido->dado != NULL)
                                 {
                                    insere_lhs_nt(campo_escolhido->dado[0]);
                                    aux_lhs = funcao_de_disponibilidade (campo_escolhido->dado[0]);
                                    campo_escolhido = campo_escolhido->prox;
                                 }
                                 break;
                            default :
                                 if((tecla1 > 44 ) && (tecla1 < 122))
                                 {
                                    if (campo_escolhido->dado == NULL)
                                    {
                                       campo_escolhido->dado = malloc(2);
                                       campo_escolhido->dado[0] = tecla1;
                                       campo_escolhido->dado[1] = '\0';
                                       mouse_off();
                                       outtextxy(campo_escolhido->x1+10,campo_escolhido->y1+5,
                                                 campo_escolhido->dado);
                                       mouse_on();
                                    }
                                 }
                                 break;
                          }
                          break;
                     case ENTRADA_PRODUCAO :
                          switch(tecla1)
                          {
                            case ENTER :
                                 break;
                            case BACKSPACE:
                                 if(campo_escolhido->dado!=NULL)
                                 {
                                    mouse_off();
                                    setfillstyle(SOLID_FILL,fundo_tela);
                                    bar(campo_escolhido->x1+10*strlen(campo_escolhido->dado),
                                        campo_escolhido->y1,
                                        campo_escolhido->x1+10*strlen(campo_escolhido->dado)+10,
                                        campo_escolhido->y1+campo_escolhido->y2);
                                    campo_escolhido->dado[strlen(campo_escolhido->dado)-2] = '0';
                                    mouse_on();
                                 }
                                 break;
                            case TAB :
                                 break;
                            default :
                                 if((tecla1 > 44 ) && (tecla1 < 122))
                                 {
                                    if (campo_escolhido->dado == NULL)
                                    {
                                       campo_escolhido->dado = malloc(2);
                                       campo_escolhido->dado[0] = tecla1;
                                       campo_escolhido->dado[1] = '\0';
                                       mouse_off();
                                       outtextxy(campo_escolhido->x1+10*strlen(campo_escolhido->dado),
                                                 campo_escolhido->y1+5,
                                                 campo_escolhido->dado);
                                       mouse_on();
                                    }
                                    else
                                    {
                                       auxiliar = malloc(strlen(campo_escolhido->dado)+2);
                                       strcpy(auxiliar,campo_escolhido->dado);
                                       free(campo_escolhido->dado);
                                       auxiliar[strlen(auxiliar)] = tecla1;
                                       auxiliar[strlen(auxiliar)+1] = '\0';
                                       campo_escolhido->dado = malloc(strlen(auxiliar)+1);
                                       strcpy(campo_escolhido->dado,auxiliar);
                                       free(auxiliar);
                                       mouse_off();
                                       outtextxy(campo_escolhido->x1+10/*strlen(campo_escolhido->dado)*/,
                                                 campo_escolhido->y1+5,
                                                 campo_escolhido->dado/*[strlen(campo_escolhido->dado)-1]*/);
                                       mouse_on();
                                    }

                                 }
                          } /* end do switch(tecla1) */
                          break;
                  }  /* end do switch(campo_escolhido->codigo) */




/**************************************************************************/
char le_tecla ( int x, int y )
{   char tecla;
	do
	{
      while(!kbhit())
      {
        outtextxy(x,y,"_");
		  limpa(x,y,x+10,y+8,LIGHTGRAY);
		}
		 tecla = getch();
	 } while(((tecla < 44 ) || (tecla > 122))
		&&  (tecla != ESC)&&(tecla!=ENTER)&&(tecla!=BACKSPACE) );
	return(tecla);
}

char *le_caracter (int x, int y, int x1, int y1, int cor, int tel)
{
	 char car[1];
	 char palavra[80];
	 int i,u,max,ymax;

	 mouse_off();

	 if (tel) tela(x,y,x1,y1,1,cor);
	 i = 0;
	 u = 0;
	 ymax = y + y1;
	 settextstyle(DEFAULT_FONT,0,1);
	 do
	 {
		  setcolor(RED);
		  palavra[i] = le_tecla(x+10+10*u, y+ 10);
		  if (palavra[i] == ESC)
			  {
				palavra[0] = '\0';
				mouse_on();
				return(palavra);
			  }
		  if ((palavra[i]!= ENTER)&&(palavra[i] != BACKSPACE))
		  {
				sprintf(car, "%c",palavra[i]);
				outtextxy(x +10+10*u ,y+10, car);
		  }

		  if (palavra[i] == BACKSPACE)
        {   palavra[i] = '\0';
            if (i != 0)
				{
                i--;
                u--;
					 if (u == -1)
					 {
						  u = max;
						  y = y - 15 ;
                }
                limpa(x+10+ 10*u,y+10 ,x+20+10*u,y+18,LIGHTGRAY);
					 setcolor(LIGHTGRAY);
				}
            i--;
            u--;
		  }
		  i++;
        u++;
        if (x+10+10*u >= x-10 + x1)
        {
            max = u-1;
            y = y + 15;
				u = 0;

        }
		  if (y >= ymax - 20)
		  {
				y = y-15;
				u = max;
            while(le_tecla(x+10+10*u, y+ 10) != BACKSPACE);
				i--;
		  }

	 }while(palavra[i-1] != ENTER);
	 palavra[i-1] = '\0';

	 mouse_on();
	 return(palavra);
}
