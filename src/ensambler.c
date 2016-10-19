/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : emsambler.c                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ensambler.h"

int ensamblerVacio(pEnsambler E)
{
	return E == NULL;
}


pEnsambler ultimoEnsambler(pEnsambler E)
{
	while (!ensamblerVacio(E->sgte))
		E = E->sgte;
	
	return E;
}


static pEnsambler nuevoNodoEmsambler(char *variable, char *funcion, int direccion, char tamano, char tipo)
{
	pEnsambler aux;
	
	aux = (pEnsambler) malloc(sizeof(PtrtEnsambler));
	
	aux->variable      = malloc(strlen(variable) * sizeof(char));
	memcpy(aux->variable,variable,strlen(variable));
	aux->funcion       = malloc(strlen(funcion) * sizeof(char));
	memcpy(aux->funcion,funcion,strlen(funcion));
	aux->direccion     = direccion;
	aux->tamano        = tamano;
	aux->tipo		    = tipo;
	aux->sgte          = NULL;
	
	return aux;
}


void insertarEnsambler(pEnsambler *E, char *variable, char *funcion, int direccion, char tamano, char tipo)
{
     pEnsambler nuevo = nuevoNodoEmsambler(variable,funcion,direccion,tamano,tipo);

     if (ultimoEnsambler(*E))
        *E = nuevo;
     else
         ultimoEnsambler(*E)->sgte = nuevo;
}


void eliminarEnsambler(pEnsambler *E)
{	pEnsambler aux;

	while (!ensamblerVacio(*E))
	{
		aux = *E;
		(*E) = (*E)->sgte;
		free(aux->funcion);
		free(aux->variable);
		free(aux);
	}
}
