/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : pilaArbol.c                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilaArbol.h"

int pilaArbolVacia(pilaArbol P)
{
	return P == NULL;
}

pilaArbol nuevoNodoPilaArbol(arbol nodo)
{
	pilaArbol aux;

	aux = (pilaArbol) malloc(sizeof(PtrPilaArbol));
	aux->nodo = nodo;
	aux->sgte = NULL;
	return aux;
}

void apilarArbol(pilaArbol *P, arbol nodo)
{
	pilaArbol aux;
	
	aux = (pilaArbol) malloc(sizeof(PtrPilaArbol));
   aux->nodo = nodo;
   aux->sgte = *P;
   *P        = aux;
}

arbol desapilarArbol(pilaArbol *P)
{
	pilaArbol aux;
	arbol 	aAux;
	 
	if (!pilaArbolVacia(*P))
   {
   	aux  = *P;
   	aAux = aux->nodo;
   	*P = (*P)->sgte;
      free(aux);	}
   
   return aAux;
}
