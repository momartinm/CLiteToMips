/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : lAuxiliar.c                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lAuxiliar.h"

/*******************************************************************************
Esta funcion nos indica si una lista esta vacia.
*******************************************************************************/
int listaVaciaAuxiliar(lAuxiliar l)
{
	return l == NULL;
}

/*******************************************************************************
Esta funcion devuelve el ltimo elemento de una lista de variables.
*******************************************************************************/
lAuxiliar ultimoAuxiliar(lAuxiliar l)
{
	while (!listaVaciaAuxiliar(l->sgte))
		l = l->sgte;
	return l;
}


lAuxiliar nuevoNodoAuxiliar(char *elemento, int tipoDato, int tipo, int posicion, char *indice)
{
	lAuxiliar aux;
	
	aux = (lAuxiliar) malloc(sizeof(PtrlAuxiliar));
	aux->tipoDato = tipoDato;
	aux->tipo	  = tipo;
	aux->posicion = posicion;
	
	aux->elemento = malloc(sizeof(char)*strlen(elemento)+1);
	bzero(aux->elemento,(strlen(elemento)+1));
	strcpy(aux->elemento,elemento);
	
	if (indice != NULL)
	{
		aux->indice = malloc(sizeof(char)*strlen(indice)+1);
		bzero(aux->indice,(strlen(indice)+1));
		strcpy(aux->indice,indice);
	}
	
	aux->sgte = NULL;
	
	return aux;
}

/*******************************************************************************
Este metodo almacena una nueva variable indicando el tipo de variable y la linea
en la que sha sido definida esa variable.
*******************************************************************************/
void insertarAuxiliar(lAuxiliar *l, char *elemento, int tipoDato, int tipo, int posicion, char *indice)
{

     lAuxiliar nuevo = nuevoNodoAuxiliar(elemento,tipoDato,tipo,posicion,indice);

     if (listaVaciaAuxiliar(*l))
        *l = nuevo;
     else
         ultimoAuxiliar(*l)->sgte = nuevo;
}


void insertarNodoAuxiliar(lAuxiliar *l, lAuxiliar nodo)
{

     if (listaVaciaAuxiliar(*l))
        *l = nodo;
     else
         ultimoAuxiliar(*l)->sgte = nodo;
}

/*******************************************************************************
Esta funcin devuelve el nombre de la variable almacenada en L.
*******************************************************************************/
char *obtenerAuxiliar(lAuxiliar l)
{
     return l->elemento;
}

/*******************************************************************************
Esta funcin devuelve el tipo de la variable almacenada en L.
*******************************************************************************/
int obtenerDatoAuxiliar(lAuxiliar l)
{
     return l->tipoDato;
}

/*******************************************************************************
Esta funcin devuelve el tipo de la variable almacenada en L.
*******************************************************************************/
int obtenerTipoAuxiliar(lAuxiliar l)
{
     return l->tipo;
}


/*******************************************************************************
Esta funcin devuelve el tipo de la variable almacenada en L.
*******************************************************************************/
int obtenerPosicionAuxiliar(lAuxiliar l)
{
     return l->posicion;
}

/*******************************************************************************
Esta funcin devuelve el nombre de la variable almacenada en L.
*******************************************************************************/
char *obtenerIndiceAuxiliar(lAuxiliar l)
{
     return l->indice;
}


/*******************************************************************************
Este método elimina todos los nodos de una lista de eventos.
*******************************************************************************/
void eliminarListaAuxiliar(lAuxiliar *l)
{	lAuxiliar aux;

	while (!listaVaciaAuxiliar(*l))
	{
		aux = *l;
		(*l) = (*l)->sgte;
		free(aux->elemento);
		free(aux);
	}
}
