

le_ordem_de_producao(w_no,letra,ch,string,exi)
    int w_no;
    int letra;
    int *ch;
    int *exi;
    char string[];
{
    char car[1];
    int cem,cont,i;

    hidecursor(w_no);
    fgcolor(w_no,PRETO);
    rectf(w_no,395,300,700,330);
    fgcolor(w_no,VERMELHO);
    drawsymbols(w_no,letra,400,300,"_",1);
    showcursor(w_no);
    flushbuffer(w_no);
    Clear_button_data();
    Clear_keyboard_data();
    Set_keyboard_buffer(car,1);
    Wait_for_events(KEYBOARD_EVENT|BUTTON_EVENT,&cem);
    if(BUTTON_EVENT & cem)
    {
         int area,x,y,bt,tr,tt;

         *exi = -2;
         Get_button_data(&area,&x,&y,&bt,&tr,&tt);
         if((x >= 1070) && (y >= 766))
               *ch = 0;
    }
    else
    {
         Get_keyboard_data(&cont);
         if((car[0] != '\r') && (car[0] != '\n'))
         {
              string[0] = car[0];
              i = 1;
         }
         else
              i = 0;
         do 
         {
              le_string(w_no,letra,400,300,8,VERMELHO,i,395,300,PRETO,700,330,
                        string);
              i = 0;
         } while( ! strcmp(string,""));
         strcpy(lote.ident,string);
         {
              long f,g;

              pesq_lote(lote.ident,&f,&g,exi);
         }
    }
}
