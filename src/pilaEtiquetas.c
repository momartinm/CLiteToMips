/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : pilaEtiquetas.c                       *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilaEtiquetas.h"

/*******************************************************************************
Esta función nos indica si la pila esta vacia.
*******************************************************************************/
int pilaEtiquetasVacia(PilaE P)
{
    return P == NULL;
}

/*******************************************************************************
Esta función crea una nuevo nodo para la pila y lo devuelve como retorno de la 
función.
*******************************************************************************/
PilaE nuevoNodoPilaEtiquetas(char *cadena)
{
	PilaE aux;
    
	aux = (PilaE) malloc(sizeof(PtrPilaE));
	aux->cadena = malloc(sizeof(char)*(strlen(cadena)+1));
	bzero(aux->cadena,(strlen(cadena)+1));
	strcpy(aux->cadena,cadena);
	aux->sgte   = NULL;
	return aux;
}

/*******************************************************************************
Esta método inserta una nuevo nodo en la cabeza de la pila
*******************************************************************************/
void apilarEtiqueta(PilaE *P, char *cadena)
{
    PilaE aux = nuevoNodoPilaEtiquetas(cadena);
    aux->sgte = *P;
    *P = aux;
}

/*******************************************************************************
Mediante este método se elimina el elemento de la cima de la pila.
*******************************************************************************/
char *desapilarEtiqueta(PilaE *P)
{
	PilaE aux;
	static char resultado[255];
	 
	if (!pilaEtiquetasVacia(*P))
  	{
   		aux = *P;
   		bzero(resultado,255);
	   	memcpy(resultado,aux->cadena,strlen(aux->cadena));
      		*P = (*P)->sgte;
     		free(aux->cadena);
      		free(aux);	}
   
	return resultado;
}

/*******************************************************************************
Mediante este método se obtiene el valor del nodo que se encuentra en la cabecera
de la pila.
*******************************************************************************/
char *cabeceraPilaEtiquetas(PilaE *P)
{
	PilaE aux;
	
	if (!pilaEtiquetasVacia(*P))
		aux = *P;
	
	if (aux != NULL)
		return aux->cadena;
	else
		return NULL;
}
