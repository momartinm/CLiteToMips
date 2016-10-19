/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : pilaArbol.h                       	   *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __PILAARBOL_H__
#define __PILAARBOL_H__

#include "arbol.h"

typedef struct nodoPilaArbol{
       
       arbol nodo;
       struct nodoPilaArbol *sgte ;

} PtrPilaArbol, *pilaArbol;

int pilaArbolVacia(pilaArbol P);
void apilarArbol(pilaArbol *P, arbol nodo);
arbol desapilarArbol(pilaArbol *P);

#endif
