le_string(w_no,fonte,x,y,tam,cor,i,xfb,yfb,cor_f,xfc,yfc,cadeia)
                     /* Funcao que le um string e plota-o na tela	      */
                     /* /spp/.../spsup/spsupgt/...			      */
                     /* /spp/spdl/...					      */
    int w_no;        /* Numero da window a se plotar			      */
    int fonte;       /* Tipo de letra a se utilizar			      */
    int x,y;         /* Ponto onde se plotara o string			      */
    int tam;         /* Tamanho do string a ser lido			      */
    int cor;         /* Cor da letra a ser plotada			      */
    int i;           /* Indice de inicio do string			      */
    int cor_f;       /* Cor de fundo					      */
    int xfb,yfb;     /* Coordenada (x,y) inferior do retangulo a se plotar    */
    int xfc,yfc;     /* Coordenada (x,y) superior do retangulo a se plotar    */
    char cadeia[];   /* Retornara a cadeia lida				      */
{
    int cur_event_mask; /* Mascara de teste de eventos			      */
    int cont,px,py;     /* Variaveis auxiliares				      */
    char car[1];        /* Buffer utilizado na leitura dos caracteres do tec. */

    hidecursor(w_no);
    fgcolor(w_no,cor_f);
    rectf(w_no,xfb,yfb,xfc,yfc);
    car[0] = cadeia[0];
    fgcolor(w_no,cor);
    cadeia[i] = '_';
    cadeia[i+1] = '\0';
    drawsymbols(w_no,fonte,x,y,cadeia,strlen(cadeia));
    showcursor(w_no);
    flushbuffer(w_no);
    Set_keyboard_buffer(car,1);
    if(car[0] != '\r')
    {
         do {
              Wait_for_events(KEYBOARD_EVENT|BUTTON_EVENT,&cur_event_mask);
              if(KEYBOARD_EVENT & cur_event_mask)
              {
                   Get_keyboard_data(&cont);
                   Clear_keyboard_data();
                   if(car[0] != '\r')
                   {
                       if(((car[0] == '\b')||(car[0] == -128)||(car[0] == 127))
                            && (i>0))
                       {
                            fgcolor(w_no,cor_f);
                            hidecursor(w_no);
              