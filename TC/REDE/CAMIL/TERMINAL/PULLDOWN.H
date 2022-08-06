#ifndef _MENU_H_
#define _MENU_H_

#define ESC -1
#define ESQ 75
#define DIR 77
#define DOWN 80
#define OPEN 50
#define CLOSE -50
#define POPUP 100
#define PULLDWN -100

typedef struct {
  int opc;
  int tipo;
  int estado;
  int col_in;
  int lin_in;
  int cor_frente;
  int cor_fundo;
} menu_info;

#endif