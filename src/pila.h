/******************************************************
* Programa    : MIPS R3000                            *
* M�dulo      : pila.h                            	   *
* Autor       : Moises Mart�nez Mu�oz                 *
* Autor       : Ignacio �lvarez Santiago              *
******************************************************/

#ifndef __PILA_H__
#define __PILA_H__

#include "lAuxiliar.h"

typedef struct nodoPila{
       
       lAuxiliar nodo;
       struct nodoPila *sgte;

} PtrPila, *Pila;

int pilaVacia(Pila P);
void apilar(Pila *P, lAuxiliar nodo);
lAuxiliar desapilar(Pila *P);
char *cabeceraPila(Pila *P);


#endif
