#include <a:int.h>


int main ( void )
{
   char *texto,
	*texto_mensagem;

   init_grafico();
   init_mouse();
   posiciona_mouse(640,480);

   inicializa();

   texto = malloc(30);
   texto_mensagem = malloc(80);

	strcpy(texto_mensagem,"Opcoes de Gramatica");
	strcpy(texto,"Gramatica");
	insere_menu_principal (texto,MENU_GRAMATICA,texto_mensagem);
	   strcpy(texto_mensagem,"Criar nova Gramatica");
	   strcpy(texto,"Criar");
	   insere_sub_menu (MENU_GRAMATICA ,texto ,SUB_GRAMATICA_CRIAR ,texto_mensagem);
          strcpy(texto,"Criar Gramatica");
          seta_janela(SUB_GRAMATICA_CRIAR,texto,JANELAX,JANELAY,400,300);
          strcpy(texto,"Entrada do Nao Terminal :");
          insere_opcoes( SUB_GRAMATICA_CRIAR, NAO_BOTAO, ENTRADA_NT, texto, 30, 15,1);
          strcpy(texto,"Entrada da Producao :");
          insere_opcoes( SUB_GRAMATICA_CRIAR, NAO_BOTAO, ENTRADA_PRODUCAO, texto, 50, 15,4);
          strcpy(texto,"+ NT");
          insere_opcoes( SUB_GRAMATICA_CRIAR, BOTAO, MAIS_NT,texto, 70, 40,0);
          strcpy(texto,"+ Prod.");
          insere_opcoes( SUB_GRAMATICA_CRIAR, BOTAO, MAIS_PRODUCAO,texto, 70, 40,0);
          strcpy(texto,"OK");
          insere_opcoes( SUB_GRAMATICA_CRIAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_GRAMATICA_CRIAR, BOTAO, CANCEL, texto, 70, 40,0);

	   strcpy(texto_mensagem,"Carregar Gramatica");
	   strcpy(texto,"Carregar");
	   insere_sub_menu (MENU_GRAMATICA ,texto ,SUB_GRAMATICA_CARREGAR ,texto_mensagem);
          strcpy(texto,"Carregar Gramatica");
          seta_janela(SUB_GRAMATICA_CARREGAR,texto,JANELAX,JANELAY,250,150);
          strcpy(texto,"OK");
          insere_opcoes( SUB_GRAMATICA_CARREGAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_GRAMATICA_CARREGAR, BOTAO, CANCEL,texto, 70, 40,0);

      strcpy(texto_mensagem,"Salvar Gramatica");
	   strcpy(texto,"Salvar");
	   insere_sub_menu (MENU_GRAMATICA ,texto ,SUB_GRAMATICA_SALVAR ,texto_mensagem);
          strcpy(texto,"Salvar Gramatica");
          seta_janela(SUB_GRAMATICA_SALVAR,texto,JANELAX,JANELAY,250,150);
          strcpy(texto,"OK");
          insere_opcoes( SUB_GRAMATICA_SALVAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_GRAMATICA_SALVAR, BOTAO, CANCEL,texto, 70, 40,0);


	strcpy(texto_mensagem,"Opcoes do texto de entrada");
	strcpy(texto,"Texto");
	insere_menu_principal (texto,MENU_TEXTO,texto_mensagem);
	   strcpy(texto_mensagem,"Criar novo texto de entrada");
	   strcpy(texto,"Criar");
	   insere_sub_menu (MENU_TEXTO ,texto ,SUB_TEXTO_CRIAR ,texto_mensagem);

          strcpy(texto,"Criar Texto");
          seta_janela(SUB_TEXTO_CRIAR,texto,150/*JANELAX*/,200/*JANELAY*/,400,150);
          strcpy(texto,"Entrada :");
          insere_opcoes( SUB_TEXTO_CRIAR, NAO_BOTAO, ENTRADA_TEXTO, texto, 180, 15,20);
          strcpy(texto,"OK");
          insere_opcoes( SUB_TEXTO_CRIAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_TEXTO_CRIAR, BOTAO, CANCEL, texto, 70, 40,0);

	   strcpy(texto_mensagem,"Carregar texto");
	   strcpy(texto,"Carregar");
	   insere_sub_menu (MENU_TEXTO ,texto ,SUB_TEXTO_CARREGAR ,texto_mensagem);
          strcpy(texto,"Carregar Texto");
          seta_janela(SUB_TEXTO_CARREGAR,texto,JANELAX,JANELAY,250,150);
          strcpy(texto,"OK");
          insere_opcoes( SUB_TEXTO_CARREGAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_TEXTO_CARREGAR, BOTAO, CANCEL,texto, 70, 40,0);

      strcpy(texto_mensagem,"Salvar texto");
	   strcpy(texto,"Salvar");
	   insere_sub_menu (MENU_TEXTO ,texto ,SUB_TEXTO_SALVAR ,texto_mensagem);
          strcpy(texto,"Salvar Texto");
          seta_janela(SUB_TEXTO_SALVAR,texto,JANELAX,JANELAY,250,150);
          strcpy(texto,"OK");
          insere_opcoes( SUB_TEXTO_SALVAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_TEXTO_SALVAR, BOTAO, CANCEL,texto, 70, 40,0);


	strcpy(texto_mensagem,"Executar o parsing do texto de entrada");
	strcpy(texto,"Executar");
	insere_menu_principal (texto,MENU_EXECUTAR,texto_mensagem);
	   strcpy(texto_mensagem,"Executar o parsing");
	   strcpy(texto,"Rodar");
	   insere_sub_menu (MENU_EXECUTAR ,texto ,SUB_EXECUTAR_RODAR ,texto_mensagem);
          strcpy(texto,"Rodar o Parsing");
          seta_janela(SUB_EXECUTAR_RODAR,texto,JANELAX+160,JANELAY+216,250,135);
          strcpy(texto,"OK");
          insere_opcoes( SUB_EXECUTAR_RODAR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_EXECUTAR_RODAR, BOTAO, CANCEL,texto, 70, 40,0);

	   strcpy(texto_mensagem,"Executar o parsing passo a passo");
	   strcpy(texto,"Passo a Passo");
	   insere_sub_menu (MENU_EXECUTAR ,texto ,SUB_EXECUTAR_PASSO ,texto_mensagem);
          strcpy(texto,"Rodar Passo a Passo");
          seta_janela(SUB_EXECUTAR_PASSO,texto,JANELAX+160,JANELAY+216,250,135);
          strcpy(texto,"OK");
          insere_opcoes( SUB_EXECUTAR_PASSO, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_EXECUTAR_PASSO, BOTAO, CANCEL,texto, 70, 40,0);


	strcpy(texto_mensagem,"Opcoes do sistema");
	strcpy(texto,"Opcoes");
	insere_menu_principal (texto,MENU_OPCOES,texto_mensagem);
	   strcpy(texto_mensagem,"Opcoes de cores do programa");
	   strcpy(texto,"Cor");
	   insere_sub_menu (MENU_OPCOES ,texto ,SUB_OPCOES_COR ,texto_mensagem);
          strcpy(texto,"Opcoes de cores do sistema");
          seta_janela(SUB_OPCOES_COR,texto,JANELAX,JANELAY,250,150);
          strcpy(texto,"OK");
          insere_opcoes( SUB_OPCOES_COR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_OPCOES_COR, BOTAO, CANCEL,texto, 70, 40,0);

	   strcpy(texto_mensagem,"Sair do programa");
	   strcpy(texto,"Sair");
	   insere_sub_menu (MENU_OPCOES ,texto ,SUB_OPCOES_SAIR ,texto_mensagem);
          strcpy(texto,"Abandonar Programa");
          seta_janela(SUB_OPCOES_SAIR,texto,JANELAX,JANELAY,250,150);
          strcpy(texto,"OK");
          insere_opcoes( SUB_OPCOES_SAIR, BOTAO_DEFAULT, OK,texto, 70, 40,0);
          strcpy(texto,"CANCEL");
          insere_opcoes( SUB_OPCOES_SAIR, BOTAO, CANCEL,texto, 70, 40,0);


   strcpy(texto,"Parsing Full Backup");
   tela_fundo(1,texto);
   inicializa_tela_gramatica();
   inicializa_tela_parsing();
   inicializa_tela_texto();
   atualiza_tela_gramatica();

    do
    {  evento = pega_proximo_evento();  /* pega o proximo evento
                                           (mouse/teclado) */
       if (opcao_ativa!=NULL)
          executa_opcao();
       else
          trata_evento(evento);
   }while(fim!=TRUE);
   finaliza_mouse();
   return 0;
}