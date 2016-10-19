/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : lDimensiones.c                        *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lDimensiones.h"

/*******************************************************************************
Esta funcion nos indica si una lista esta vacia.
*******************************************************************************/
int listaVaciaDimensiones(lDimensiones l)
{
	return l == NULL;
}

/*******************************************************************************
Esta funcion devuelve el ltimo elemento de una lista de dimesiones.
*******************************************************************************/
lDimensiones ultimaDimension(lDimensiones l)
{
	while (!listaVaciaDimensiones(l->sgte))
		l = l->sgte;
	return l;
}

/*******************************************************************************
Este metodo almacena una nueva variable indicando el tipo de variable y la linea
en la que sha sido definida esa variable.
*******************************************************************************/
static lDimensiones nuevoNodoDimension(int dimension)
{
	lDimensiones aux;

	aux = (lDimensiones) malloc(sizeof(PtrlDimensiones));
	aux->dimension = dimension;
	aux->sgte = NULL;
	
	return aux;
}

void insertarDimension(lDimensiones *l, int dimension)
{
     lDimensiones nuevo = nuevoNodoDimension(dimension);

     if (listaVaciaDimensiones(*l))
        *l = nuevo;
     else
         ultimaDimension(*l)->sgte = nuevo;
}


int contarRestoDimensiones(lDimensiones l, int dimFinal)
{
	int contador;
	int resto;
	
	contador = 0;
	resto    = 1;
	
	while (contador < dimFinal)
	{
		resto *= l->dimension;
		contador++;
	}
	
	return resto;
}


int calcularDimensiones(lDimensiones l)
{
	int resultado = 1;

	while (!listaVaciaDimensiones(l))
	{
		resultado *= l->dimension;
		l = l->sgte;
	}

	return resultado;
}


/*******************************************************************************
Este método elimina todos los nodos de una lista de dimensiones.
*******************************************************************************/
void eliminarListaDimension(lDimensiones *l)
{
   lDimensiones aux;

	while (!listaVaciaDimensiones(*l))
	{
		aux = *l;
		(*l) = (*l)->sgte;
		free(aux);
	}
}


