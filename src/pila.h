/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : pila.h                            	   *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
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
