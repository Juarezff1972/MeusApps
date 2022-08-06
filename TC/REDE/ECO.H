/*
 *     Arquivo eco.h
 *     Manoel Camillo
 */
/******************* Comandos ao pre-processador **************/
         //define
#define BUFFER_SIZE 128
#define ECO_SOCKET 0x5454
#define DELAY 100
#define TIME_OUT 10
#define MAX_RETRY 10

#ifndef SOCKET_ALREADY_OPEN
  #define SOCKET_ALREADY_OPEN 0xff
#endif
#ifndef SOCKET_TABLE_FULL
  #define SOCKET_TABLE_FULL 0xfe
#endif
#ifndef IPX_NOT_INSTALLED
  #define IPX_NOT_INSTALLED 0xf0
#endif
#ifndef ECB_CANNOT_BE_CANCELLED
  #define ECB_CANNOT_BE_CANCELLED 0xf0
#endif

        //include
#include <stdio.h>
#include <nxt.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>