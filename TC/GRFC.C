

inicializa_gr(w_no,letra)
    int *w_no;
    int *letra;
{
    int vs_no;
    int x,y;
 
    Set_logo("SPGR");
    Set_message_mode(2);
    Display_message(1,1,"RISCAGEM");
    Inq_displayed_vs(&vs_no);
    Create_win_no_border(vs_no,0,0,CANTO_X_WIN,CANTO_Y_WIN,w_no);
    desenha_bordas(*w_no,SOBREPOE_MENUBAR);
    hidecursor(*w_no);
    fgcolor(*w_no,AZUL);
    Load_symbol_file(TEXT_FONT,letra);   
    drawsymbols(*w_no,*letra,100,300,"Ordem de Producao : ",20);
    fgcolor(*w_no,BRANCO);
    Load_symbol_file("/usr/arqsp/icons",&font_descr_s);
    Inq_symbol_size(font_descr_s,"t",sizeof("t"),&x,&y,&largura,&altura);   
    drawsymbols(*w_no,font_descr_s,CANTO_X_WIN - largura,CANTO_Y_WIN - altura,
                "t",sizeof("t"));
    showcursor(*w_no);
    flushbuffer(*w_no);
    Enable_events(BUTTON_EVENT|KEYBOARD_EVENT);
    cria_mix();
    fd_enc = open("/usr/arqsp/en.c",O_RDWR);
    fd_ltc = open("/usr/arqsp/lt.c",O_RDWR);
}
