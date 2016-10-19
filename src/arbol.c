/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : arbol.c                               *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"


int arbolVacio(arbol a)
{
	return a == NULL;
}

arbol nuevoNodoArbol(char *elemento, int tipo, int tipoDato, int posicion, char* indice)
{
	arbol aux;
	
	aux = (arbol) malloc(sizeof(PtrArbol));
	
	if (tipoDato == 50)
	{
		aux->elemento = malloc(sizeof(char) * 5);
		bzero(aux->elemento,5);
	}
	else
	{
		aux->elemento = malloc(sizeof(char) * strlen(elemento) + 1);
		bzero(aux->elemento,(strlen(elemento) + 1));
	}
	
	strcpy(aux->elemento,elemento);
	
	if (indice != NULL)
	{	
		aux->indice = malloc(sizeof(char) * strlen(indice) + 1);
		bzero(aux->indice,(strlen(indice) + 1));
		strcpy(aux->indice,indice);
	}
	else
		aux->indice = NULL;

	
	aux->etiquetaOk   = NULL;
	aux->etiquetaNook = NULL;
	aux->etiqueta     = NULL;	aux->tipo         = tipo;
	aux->tipoDato     = tipoDato;
	aux->posicion     = posicion;
	aux->dcho         = NULL;
	aux->izdo         = NULL;
	
	return aux;
}

void insertarArbol(arbol *a, char *elemento, int tipo, int tipoDato, int posicion, char* indice)
{
	arbol nuevo = nuevoNodoArbol(elemento,tipo,tipoDato,posicion,indice);
	
	if (listaVaciaAuxiliar(*a))
		*a = nuevo;
	else
	{
		if (listaVaciaAuxiliar((*a)->dcho))
			(*a)->dcho = nuevo;
		else
			(*a)->izdo = nuevo;
	}
}

void insertarNodoArbol(arbol *a, arbol nodo)
{
	if (listaVaciaAuxiliar(*a))
		*a = nodo;
	else
	{
		if (listaVaciaAuxiliar((*a)->dcho))
			(*a)->dcho = nodo;
		else
			(*a)->izdo = nodo;
	}
}


int *obtenerEtiquetaOk(arbol a)
{
	return a->etiquetaOk;
}


int *obtenerEtiquetaNook(arbol a)
{
	return a->etiquetaNook;
}


int *obtenerEtiqueta(arbol a)
{
	return a->etiqueta;
}


void modificarEtiqueta(arbol a, int *valor)
{
	a->etiqueta = valor;
}

void modificarEtiquetaOk(arbol a, int *valor)
{
	a->etiquetaOk = valor;
}


void modificarEtiquetaNook(arbol a, int *valor)
{
	a->etiquetaNook = valor;
}

void crearEtiqueta(arbol a, int valor)
{
	int *auxiliar;

	auxiliar    = malloc(sizeof(int));
	*auxiliar   = valor;

	a->etiqueta = auxiliar;
}

void crearEtiquetaOk(arbol a, int valor)
{
	int *auxiliar;

	auxiliar    = malloc(sizeof(int));
	*auxiliar   = valor;

	a->etiquetaOk = auxiliar;
}

void crearEtiquetaNook(arbol a, int valor)
{
	int *auxiliar;

	auxiliar    = malloc(sizeof(int));
	*auxiliar   = valor;

	a->etiquetaNook = auxiliar;
}


void eliminarNodo(arbol *a)
{
	arbol aux;
	
	aux = *a;

	free(aux->elemento);
	free(aux->indice);
	free(aux);
	
	a = NULL;
}


