/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : pila.c                                *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"

/*******************************************************************************
Esta función nos indica si la pila esta vacia.
*******************************************************************************/
int pilaVacia(Pila P)
{
    return P == NULL;
}

/*******************************************************************************
Esta función crea una nuevo nodo para la pila y lo devuelve como retorno de la 
función.
*******************************************************************************/
void apilar(Pila *P, lAuxiliar nodo)
{
	Pila aux;
	
	aux = (Pila) malloc(sizeof(PtrPila));
   aux->nodo = nodo;
   aux->sgte = *P;
   *P        = aux;
}


lAuxiliar desapilar(Pila *P)
{
	Pila aux;
	lAuxiliar aAux;
	 
	if (!pilaVacia(*P))
   {
   	aux  = *P;
   	aAux = aux->nodo;
   	*P = (*P)->sgte;
      free(aux);	}
   
   return aAux;
}


char *cabeceraPila(Pila *P)
{
	if (!pilaVacia(*P))
	{
		return (*P)->nodo->elemento;
	}
	return NULL;
}

